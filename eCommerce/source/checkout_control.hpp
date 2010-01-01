#include "page_control.hpp"
#include "ocTypes.h"
#include "cgiCookie.h"
#include "ocXML.h"
#include "Customer_ui.h"
#include "pay_form.hpp"
#include "Order.hpp"
#include "shoppingCart.hpp"
#include "lookup.hpp"
#include "ocMail.h"
#include "proofs.hpp"
#include <sstream> // for proof emission

/*
  checkout_countrol.hpp  
  Checkout Control   
  
*/
static char * mailPath = "/usr/sbin/sendmail";

typedef map <string, int> pnodecounts;
class virtualCheckout : public read_base
{
protected:
  ocString productSQL, keySQL;
  string sql;  
  string m_result;
  
  Ordered_Obj odr; // for ship amount tracking
  
public:   
  // templates
  ocString prodHead;      
  ocString prodList;
  ocString prodFoot;
  ocString prodBuy;
  ocString cfrmHead;
  ocString cfrmList;
  ocString cfrmFoot;
  
  ocString cfrmHeading;
  ocString cfrmListItem;
  ocString cfrmSubmit; 
  
  // the thanks page from the template
  ocString finiThanks; 
   
  // the pay error page from the template
  ocString finiError;
  
  // the mail body to the customer, from the template
  ocString mailCustomer;
  
  // the mail body to the customer, from the template
  ocString mailManufacturer;
  
  // Manufacturer address
  string mailMfrAddr;
  
  // the template control holds the 'from' email address 
  string mailFrom;
  // the template control holds the responsible party to send mail about new orders
  string mailStoreClerk;  
  
  cgiScript & script;
  uiShoppingCart cart;
  
  /* Constructor 
  */
  virtualCheckout(cgiScript & script):read_base(),odr(),script(script),cart(script)
  {      

    productSQL  = "select Id,External_Id, Name, Image, Lines_Custom_Text, SKU, "
                  "Mfr_Price,Rtl_Price,Shipping_Cost, Designer from Product Where Id = $Id";
    keySQL =  "select Id from Customer "
              "where Email = '$Email' and Password='$Password'";                       
  }
  ~virtualCheckout()
  {
    ;
  }

  /*
    Show the current step into the checkout process
  */  
  void show( page & pg,              
             string & acctTmpltName,  
             string contentTag )
  {

    bool gotProducts=false;
    // do we have a 'cart'?
    if( cart.XML.length() )
    {
      cgiInput & args = script.ClientArguments();
      
      // first show the products, buy step link at bottom
      if( args.count("step") == 0)                // 0) Begin of Checkout - Show Products in Cart
      {
        showProducts( script, prodHead, prodList, prodFoot );
        gotProducts=true;
      } 
      else      
      {        
        if( args["step"] == "buy" )               // 1) First step, (step=buy) agree to continue to buy 
        {
          // show sign on / new account forms (to sign in or register)
          script << prodBuy.replace("$result$", m_result ); 
          gotProducts=true;
        }
        else if ( args["step"] == "sign-in" )     // 2) Next step, sign in or ... 
        {
          // show user's existing account
          if( !accountForm(script,acctTmpltName,"ChangeUser") )
          { // re-show sign on / new account forms with error result
            script << prodBuy.replace("$result$",m_result);
          }            
          gotProducts=true;
        }
        else if ( args["step"] == "new-user" )    //  2) register
        {
          // show user new account form
          if( !accountForm(script,acctTmpltName,"AddUser") )
          { // re-show sign on / new account forms with error result
            script << prodBuy.replace("$result$",m_result);
          } 
          gotProducts=true;
        }
        else if( args["step"] == "payinfo" )      // 3) Setup or show payment options 
        {
          // show user payment setup form 
          paymentForm(script,acctTmpltName,""); 
          gotProducts=true;
        }
        else if( args["step"] == "verify" )       // 4) Show products and payment for verification
        {
          // show verification pane
          confirmOrderPanel(script,acctTmpltName,"VerifyPaymentSetup");            
          gotProducts=true;
        }           
        else if( args["step"] == "confirmed" )    // 5) Complete transaction, show reciept, Send mail
        {
          // create the order and show order confirmation
          createOrderPanel(script,acctTmpltName,"ShowtSetup");
          gotProducts=true;
        }
      }     
    }
    if( !gotProducts ) // emit the default page
    { 
      pg.emitContent( contentTag, "$content" );
    }
  }
    
  bool accountForm( cgiScript & script, string & formTemplate, string Action  )
  {   
    bool bRet = false;    
    Customer_form customer(script); 
    customer.loadControlTemplates(formTemplate);
    
    if( Action == "ChangeUser" )    
    {
      if( !setKey(script,customer) )
      {
        m_result = "We could not find your login in our system!";
        return bRet;
      }
    }
    
    // move form information - get data 
    customer.form_action(); 
    
    if( Action == "AddUser" && !customer.ivalidate() )
    {
      m_result = customer.last_result();
      return bRet;
    }
    
    // If we made it here, build the form
    bRet = true;
        
    if( Action == "AddUser" )    
    {  
      script << customer.makeTop( "?step=payinfo", "Customer Registration" ) ; 
    }
    else
    {  
      script << customer.makeTop( "?step=payinfo", "Account Verification" ) ; 
    }
    customer.form_display();
            
    script << customer.makeButtonedBottom(  Action ) ; 
    
    return bRet;
  }
  
  bool setKey(cgiScript & script, read_write_base & obj )
  {
    bool bret=false;
    cgiInput & args = script.ClientArguments();      
    sql = keySQL.replace("$Email",args["EMail"].c_str()).replace("$Password",args["Password"].c_str());
    if( rs.open(sql) )
    {
      obj.key(atoll(rs.getField(0).format().c_str()));
      rs.close();
      bret = true;
    }     
    return bret;
  }
  
  void paymentForm( cgiScript & script, string & formTemplate, string Action  )
  {   
    Payment_Form pay(script); 
    pay.loadControlTemplates(formTemplate);
    
    
    // To set any changes, 
    //  or insert a new customer, but don't show it
    Customer_form customer(script);
    
    bool isInserting = customer.getMode()=="i";    
    Action = isInserting?"Insert":"Update";
        
    cgiInput & args = script.ClientArguments();
    ocString customer_Id = args["customer_Id"].c_str();
    string Login = args["EMail"].c_str();    
      
    bool isCustomerOK = customer.form_action();
    
    if( isCustomerOK == false )    
    { 
      // This failure may be because
      // a user refreshed the page on insert! 
      // Just try to get just get the key
      if( customer.getMode() == "i" && customer_Id == "0"  )
      customer.getMode() = "s";
      string sql = "select Id from Customer where EMail = '";
      sql += Login;      
      sql += "'"; 

      if( rs.open(sql) )
      {
        customer.key(atoll(rs.getField(0).format().c_str()));        
        rs.close();
        isCustomerOK = customer.form_action();
      }
      else
      {
        script << "<h4 style=\"color:red;\">Error! Couldn't get a user key value!</h4>";
      }     
    }
    if( isCustomerOK )
    {     
      script << pay.makeTop( "?step=verify", "Payment Setup" ) ;
      
      // show the form
      if( ( customer_Id.length() == 0 || customer_Id == "0" )
          && customer.Id )
      {
        // this happens on an insert
        customer_Id.append(customer.Id);
        args["customer_Id"] = customer_Id.c_str();
      }
      pay.getMode() = "s";
      llong Payment_Service_Id;
      if( args.count("Payment_Service_Id") > 0 )
      {
        Payment_Service_Id = atoll(args["Payment_Service_Id"].c_str());
      }
      else 
      {  // look for an existing customer payment service
         ocString qry = "select Id from Customer_Payment_Service where Customer_Id = ";
         qry.append(customer.Id);
         string psid =  tableLookup ( qry );
         if( psid.length() ) Payment_Service_Id = atoll(psid.c_str());
      }
      pay.key(Payment_Service_Id);
      pay.Customer_Id = customer.Id;
      pay.form_action();
      pay.form_display();
      script << pay.makeButtonedBottom(  Action );     
    }
    else
    {
      script << "Error! unable to create customer account!<br> Result: <p><pre>"
             << customer.last_result() << "</pre><p>" << endl;
    }
  } 
  
  /*
     Final confirmation display, the payment form is acted on here.
  */ 
  void confirmOrderPanel( cgiScript & script, string & formTemplate, string Action  )
  { 
    // FIRST -  Save / Set the customer Payment Setup
    Payment_Form_Complete payment_setup(script);
    
    cgiInput & args = script.ClientArguments(); 
    // Get the key if we can  
    string qry = "select Id from Customer_Payment_Service where Customer_Id = ";
    qry += args["Customer_Id"].c_str();
    qry += " and Payment_Service_Id = ";
    qry += args["Payment_Service_Id"].c_str();

    if( rs.open(qry) )
    {
      string key = rs.getField(0).format();

      payment_setup.Id = atoll(key.c_str());
      payment_setup.key(payment_setup.Id);
      rs.close();
    }
    // perform the action, either insert or update
    bool isPaymentOK = payment_setup.form_action(); 
    
    if( !isPaymentOK )
    { script << "<h1 color='red'>!</h1><p>An Error occured setting your payment options - " 
             << endl
             << "<br> Result:  </p><pre>"
             << payment_setup.last_result()
             << "</pre>" 
             << endl;
      return;
    }
    
    // get the product ids and the counts
    if( cart.XML.length() )
    { 
      showProducts( script, cfrmHead, cfrmList, cfrmFoot);
    }
    
    // Get this customer, and the payment info
    string customer_Id = args["Customer_Id"].c_str();
    Customer_Obj customer;

    customer.key( atoll(customer_Id.c_str()) );
    if( customer.get_data() ) 
    {
      // show it all
      script << cfrmHeading.replace("$label","&nbsp;") << endl;
      script << cfrmHeading.replace("$label","Customer Information:") << endl;
      
      script << cfrmListItem.replace("$label","First Name")
                            .replace("$value",customer.First_Name.c_str()) << endl;
      script << cfrmListItem.replace("$label","Last Name")
                            .replace("$value",customer.Last_Name.c_str()) << endl;
      
      ocString pSrvSQL = "select Name from Payment_Service where Id = ";
      pSrvSQL.append(customer.Payment_Service_Id);
      script << cfrmListItem.replace("$label","Payment Type")
                            .replace("$value",tableLookup (pSrvSQL).c_str() ) << endl;      
      
      
      if( isPaymentOK )
      {
        // show it all
        script << cfrmHeading.replace("$label","Customer Payment Setup:") << endl;
        Customer_Payment_Parameters & uparms = payment_setup.uparms;
        for(size_t i=0;i<uparms.size();i++)
        { 
          if( uparms[i].Machine_Name != "cc" ) // don't show the cc again!
          {
            script << cfrmListItem.replace("$label",uparms[i].Name.c_str())
                            .replace("$value",uparms[i].Value.c_str()) << endl; 
          }                
        }  
      } //  ... href="javascript:confirmingOrder($ID)"><img title="Con ...
      script << cfrmSubmit.replace("$ID",customer_Id.c_str()) << endl;
    }     
  }
  
  void createOrderPanel( cgiScript & script, string & formTemplate, string Action  )
  {    
    
    // For who
    cgiInput & args = script.ClientArguments();
    string customer_Id = args["customer_Id"].c_str();
    Customer_Obj customer;    
    
    // Gather all the info...
    customer.key( atoll(customer_Id.c_str()) );
    
    if( customer_Id.length() > 0 && customer.get_data() )
    {
      // For UI output
      ocString ORDER_ID, ORDER_AMOUNT, BACK_URL;      

      // Get customer payment data & payment_service data
      Ordered_Obj order;
      order.Customer_Id = customer.Id;
      
      // How much and which and all
      order.setOrderedItems(cart.XML);      
  
      // This code is hard to reach! the cart will be new before they get this if there is a cart iframe
      if( cart.Checked_Out )
      {
        script << finiError.replace("$Error","You have already checked out").replace("$url",baseUrl.c_str()) << endl;  
        return;
      }
      // Do PAY pre-authorization to pay, Creates order, set order to 'waiting to ship' status.
      // 

      if( order.pay(customer.Payment_Service_Id) ) // changed to pay from preauth at rolands request
      {        
        cart.CheckOut();
        
        ORDER_ID.append(order.Id);
        ORDER_AMOUNT = order.Order_Total.format("%n");
        
        // Give customer an order id, so he can come back and view the status, along with a thanks!
        script << finiThanks.replace("$Order_Total",ORDER_AMOUNT.c_str()).replace("$Order_Id",ORDER_ID.c_str()); 
        
        // Email all and customer.
        emailManufacturer( order, customer ); //
        emailClerk(order);
        emailcustomer(order,customer);
      }
      else
      {
        // show transaction error
        script << finiError.replace("$Error",order.last_result().c_str())
                           .replace("$url",baseUrl.c_str()) << endl;         
      }  
    } // end if customer length and got a customer
    else
    {
      // show transaction error
      script << finiError.replace("$Error","I couldn't find you in our database!")
                         .replace("$url",baseUrl.c_str()) << endl; 
    }       
  }
  

  
  void emailManufacturer(  Ordered_Obj & order, Customer_Obj & customer )
  {    
    stringstream items;
    proofs proof;
    proof.m="1";
    proof.o="";
    proof.o.append(order.Id);
    proof.showOrderedItems( items );    
    // script << items.str();
    string custName = customer.First_Name + " " + customer.Last_Name;
    // get the designer for the ordered item, use that DESIGN NO (has A or B)     
    ocSendMail sendMail(mailPath);
    string sendTo = mailMfrAddr;
    ocString strOrder;
    strOrder.append(order.Id);
    ocString body=mailManufacturer
                            .replace("$items", items.str().c_str() )
                            .replace("$name$",custName.c_str() )
                            .replace("$address$",customer.Address.c_str())
                            .replace("$city$",customer.City.c_str())
                            .replace("$state$",customer.State.c_str())
                            .replace("$zip$",customer.Zip.c_str());
    sendMail.openRoute( mailFrom.c_str(), sendTo.c_str(), "You have an Order!" );
    sendMail.setMimeType("text/html");
    sendMail.write( body.c_str() );
    sendMail.send();     
  }
  
  void emailcustomer( Ordered_Obj & order, Customer_Obj & customer )
  {
    ocSendMail sendMail(mailPath);
    string sendTo = customer.EMail;
    ocString strOrder;
    strOrder.append(order.Id);
    ocString body=mailCustomer.replace("$orderid", strOrder.c_str() )
                            .replace("$amount", order.Order_Total.format("%n").c_str() );
    sendMail.openRoute( mailFrom.c_str(), sendTo.c_str(), "Thank You for your order" );
    sendMail.setMimeType("text/html");
    sendMail.write( body.c_str() );
    sendMail.send();
    // DBG script << "emailcustomer" << " to " << sendTo<< " from " << mailFrom << endl; // DBG
    // DBG script << body << endl;
  }
 
  void emailClerk( Ordered_Obj & order )
  {
    ocSendMail sendMail(mailPath);

    ocString body="<html><body><h1>An order has been placed and needs your attention!</h1>\n"
                  "The details of your order are given below.</p>\n"
                  "Order ID: ";
    body.append(order.Id);
    body+="<br>\nTotal Amount: ";
    body+=order.Order_Total.format("%n");  
    body+="<br>\n"
          "<p>You will find this order on the 'Orders' control panel in 'paid' status.<br>\n"
          "You should enter this order into quickbooks as soon as possible then set the status to 'entered'.<br>\n"
          "Thanks!</p>";
          
    sendMail.openRoute( mailFrom.c_str(), mailStoreClerk.c_str(), "You have an Order!" );
    sendMail.setMimeType("text/html");
    sendMail.write( body.c_str() );
    sendMail.send();
  }  
  
  // product node tracking in the cart. 
  pnodecounts pncs;
  void trackProductNodes(string & productId)
  {
    pnodecounts::iterator it = pncs.find(productId);
    if( it == pncs.end() )
    { // add it
      pncs.insert(make_pair(productId,0));
    }
    else
    {
      it->second++;
    }
  }
  // for confirmation and all
  void showProducts( cgiScript & script, ocString & prodHead, ocString & listTemplate, ocString & prodFoot)
  {  
    script << prodHead;    
    xmlParser parser( cart.XML );
    parser.parse();
    node_vector & xnodes = parser.nodeList();
    int i;
    money tot = 0.0;
    
    if( xnodes.size() == 0 )
    {
      // show transaction error
      script << finiError.replace("$Error","Nothing was found in the cart!")
                         .replace("$url",baseUrl.c_str()) << endl;
    }
    
    // for each  node in the XML
    for(i=0;i<xnodes.size();i++)
    {
      xmlNode & node = xnodes[i];
      string productId = node.attr["id"];
      string count = node.attr["count"];        
      if( count.length() == 0 ) count = "1";      
      if( productId.length() != 0 ) 
      {
        trackProductNodes( productId ); // count of nodes of a particular product id
        sql =  productSQL.replace( "$Id", productId );
        if( rs.open(sql) )
        {
          ocString seq;
          seq.append(pncs[productId]);
          money mon = atof( rs.getField("Rtl_Price").format().c_str() );
          
          double cnt = atof( count.c_str());
          double ship =  cnt*atof( rs.getField("Shipping_Cost").format().c_str() );
          odr.shipInc(ship);
          tot.amount() += cnt*mon.amount();
          script << listTemplate.replace( "$image", rs.getField("Image").format() )
                                .replaceAll("$title", rs.getField("Name").format() )
                                .replace( "$price", mon.format("%n") )
                                .replaceAll( "$count", count.c_str() )
                                .replaceAll( "$id", productId )
                                .replaceAll("$pnode",seq);
          rs.close();
        }
      } 
    }    
    ocString tmp = prodFoot.replace("$shipping", odr.netShipping() );
    tot.amount() += odr.Shipping_Total.amount();
    script << tmp.replace("$price", tot.format("%n"));     
  }   
};

class chkout_functor: public base_functor
{
protected:
  cgiScript & script;
  virtualCheckout checkout;
public:
  chkout_functor(page & ipg,cgiScript & scriptIn):base_functor(ipg),script(scriptIn),checkout(scriptIn){;}
  virtual ~chkout_functor(){;}
  
  virtual  bool operator()( xmlNode & node )
  {
    bool bRet = false;
    string headPName = getXmlAttribute(node,"uiListHead");
    string listPName = getXmlAttribute(node,"uiListitem");
    string footPName = getXmlAttribute(node,"uiListFoot");
    string buyPName =  getXmlAttribute(node,"uiBuyForm");      
    string defPName =  getXmlAttribute(node,"uiDefault");
    string uiCfrmHead =  getXmlAttribute(node,"uiCfrmHead");
    string uiCfrmFoot =  getXmlAttribute(node,"uiCfrmFoot");
    string uiCfrmList =  getXmlAttribute(node,"uiCfrmList");
        
    string acctTmpltName = getXmlAttribute(node,"uiAccountTemplate");
          
    if( listPName.length() )
    {       
      checkout.prodHead = pg.paragraph_string(headPName);      
      checkout.prodList = pg.paragraph_string(listPName);
      checkout.prodFoot = pg.paragraph_string(footPName);
      checkout.prodBuy = pg.paragraph_string(buyPName);
      checkout.cfrmHead = pg.paragraph_string(uiCfrmHead);
      checkout.cfrmList = pg.paragraph_string(uiCfrmList);
      checkout.cfrmFoot = pg.paragraph_string(uiCfrmFoot);
      checkout.cfrmHeading = pg.paragraph_string(getXmlAttribute(node,"uiCfrmGenHead"));
      checkout.cfrmListItem = pg.paragraph_string(getXmlAttribute(node,"uiCfrmGenList"));
      checkout.cfrmSubmit = pg.paragraph_string(getXmlAttribute(node,"uiCfrmSubmit"));
      checkout.finiThanks =  pg.paragraph_string(getXmlAttribute(node,"uiThanks"));  
      checkout.finiError =  pg.paragraph_string(getXmlAttribute(node,"uiError"));
      checkout.mailFrom = getXmlAttribute(node,"mailFrom");
      checkout.mailStoreClerk = getXmlAttribute(node,"mailStoreClerk");
      checkout.mailMfrAddr = getXmlAttribute(node,"mfrEmail");
                 
      checkout.mailCustomer = pg.paragraph_string(getXmlAttribute(node,"uiMailCustomer"));
      checkout.mailManufacturer = pg.paragraph_string(getXmlAttribute(node,"uiMailManufacturer"));
      checkout.show( pg, acctTmpltName, defPName  );                     
      bRet = true;        
    }      
    else
    {
      script << "<h1>No XML control items!</h1>" << endl;
    }
    return bRet;    
  }
    
};


 

