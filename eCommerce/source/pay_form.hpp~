/*

  pay_form.hpp
  
  Payment Form
  =============
  CGI Form for ECommerce Payment

*/

#ifndef PAY_FORM_HPP
#define PAY_FORM_HPP

#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "PaymentServices.hpp"
#include "Customer.hpp"
#include "lookup.hpp"

/*  Payment Form - Add form that shows customer what
    parameters to set.
*/ 
class Payment_Form
 :public forms_base
 ,public Payment_Service
{
  bool hasBeenInAddressVerification;
public:    
  long long Customer_Id;
    
  // Constructor
  Payment_Form(cgiScript & in)
  :forms_base(in),Payment_Service(),Customer_Id(0LL),hasBeenInAddressVerification(false)
  {setKey(*this);}  
  ~Payment_Form(){;}
  
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "Payment_Service_Id", Id );    
  }
  
  // This get the data from the browser form to the class
  void form_data_transfer( void )
  { 
    llongFXfer( "Customer_Id", Customer_Id );    
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    bool bret = false;
    bret = db_action( mode, changes );    
    return bret;
  }
   
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;    

    script  << makeHiddenBox("Customer_Id",Customer_Id);
    
    // give an indication of the select payment, will only have one item in the combo box.
    ocString sql = "select Id, Name from Payment_Service where Id = ";
    sql.append(Id);   
     
    script << makeComboBox( "Payment Type", "Payment_Service_Id", Id, 
                             sql );
    // Build form items based on the select payment type
    return paymentSetupForm();                        
                
    return breturn;
  }
  
  bool paymentSetupForm( void )
  {
    bool breturn = true;
    getPaymentServiceParameters();  

    
    if( usr_params.size() )
    {
      Customer_Obj customer;
      customer.key(Customer_Id);
      customer.get_data();

      const avs_map & avs = customer.getAvsMap();
      
      // Get all user payment setup parameters for the payment type    
      for(  Payment_Type_Parameters::iterator pos = usr_params.begin();
            pos != usr_params.end();
            ++pos )
      {
        paymentSetupElement( pos->second, const_cast<avs_map &>(avs) );
      }
      
    }
    return breturn;
  }
   
  // COOL!  This is where we do all the payment form element magic!
  void paymentSetupElement(  Payment_Type_Parameter & parm,  avs_map & avs )
  {
    string size, length;
    string value;

    if( parm.Interface_Type == ui_cc_number )
    {
      size="16";
      length="16";
    }
    else if( parm.Interface_Type ==  ui_cc_exp )
    {
      size="4";
      length="4";
    }
    else if(parm.Interface_Type ==  ui_cvs )
    {
      size="4";
      length="4";
    }
    else if( parm.Interface_Type ==  ui_addr_verification )
    {
      if( hasBeenInAddressVerification == false )
      {
        script << titledSep("Billing Address:", 
                   "You may leave these as-is if they are the same as your shipping address.");

      }
      value=avs[parm.Machine_Name];
      hasBeenInAddressVerification= true;
    }
    else
    {
      length="30";
      size="80";
    }
    
    string label = parm.Name;    
    
    ocString ret = formTemplate.getParagraph("control_group");

    ocString parmName = "parm";
    parmName.append(parm.Id);
    ocTextbox * pText = textEntry( parmName, size, length );    
    
    if( pText )
    {   
      // first just set the avs value if any
      pText->setContent(value);  
      getUserParm( pText, parm );
      string ctrl = "<div class='instructions'>";
      ctrl += parm.Instructions;
      ctrl += "</div>"; 
      ctrl += pText->getHtml();
      ret = ret.replace("$label$",label.c_str())
               .replace("$control$", ctrl.c_str());

      delete pText;  
    }
    script << ret << endl;    
  }
  
  /*
    If the Customer's already in the system, 
    This will attempt to fill in the values
  */  
  bool getUserParm( ocBase * oc, Payment_Type_Parameter & parm )  
  {
    bool bret = false;
    if( oc && Customer_Id )
    {
      ocString sql = "select Id from Customer_Payment_Service where Customer_Id = "; 
      sql.append(Customer_Id);

      string val = tableLookup ( sql );
      if( val.length() )
      {        
        // Keys: Id  | Customer_Pmt_Service_Id | Pmt_Type_Param_Id
        // Get: Value from: Customer_Payment_Parameters
        sql = "select Value from Customer_Payment_Parameter "
              "where Customer_Pmt_Service_Id = ";
        sql += val;
        sql +=" and Pmt_Type_Param_Id = ";
        sql.append( parm.Id  );

        val = tableLookup ( sql );
        if( val.length() )
        {
          oc->setContent(val);
          bret=true;
        }
      }
    }
    return bret;
  }
  
  string makeButtonedBottom( string action  )
  {    
    ocString ret = formTemplate.getParagraph("form_end");    
    return ret.replaceAll("$ACTION",action);
  }
   
  string makeTop(string uri, string name )
  {
    ocString ret = formTemplate.getParagraph("form_begin");  
    return ret.replace("$step_title$","Payment Setup")
              .replace("$action$",uri.c_str())
              .replace("$name$",name.c_str())
              .replace("$instructions$","Please fill out all of the payment information below.");  
  }
  string titledSep(string title, string instruction )
  {
    ocString ret = formTemplate.getParagraph("form_titler");  
    return ret.replace("$name$",title.c_str())
              .replace("$instructions$",instruction.c_str());  
  }        
};

/*  Payment Form Completion-
    Actually sets the paremeters.
*/ 
class Payment_Form_Complete: public forms_base, public Customer_Payment_Service
{

public:
  Customer_Payment_Parameters uparms;
    
  // Constructor
  Payment_Form_Complete(cgiScript & in):forms_base(in),Customer_Payment_Service(){setKey(*this);}
  ~Payment_Form_Complete(){;}
  
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );    
  }
  
  // This get the data from the browser form to the class
  void form_data_transfer( void )
  {
    // Customer_Payment_Service part
    llongFXfer( "Customer_Id", Customer_Id );
    llongFXfer( "Payment_Service_Id", Payment_Service_Id );   
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {         
    bool bret = false;
    bret = db_action( mode, changes );  // do the base stuff
    
    if ( bret ) 
    {
      if( mode == "i" || mode == "u" )
      { deleteOldUserParameters();
        bret = createUserParameters();
      }  
    }
    else
    {
      if( mode =="u" && changes.size()==0 )
      { bret = true; // trivial case of update with no changes, all is ok
      }
      else    
      {  m_result += "Payment Form Completion object, failure on transaction request - mode: " + mode;

      }
    }    
    return bret;
  }
  // On update, do a wholesale replacement
  bool deleteOldUserParameters( void )
  {
    ocString sqlcmd = "delete from Customer_Payment_Parameter where Customer_Pmt_Service_Id = ";
    sqlcmd.append(Id);

    return cmd.execute(sqlcmd);
    
    
  }
  bool createUserParameters( void )
  {
    bool bret = false;
    Payment_Service pmt_svc;
    ocString where = " Id = ";
    where.append(Payment_Service_Id);
    
    if(pmt_svc.get_data(where))
    {
      if( pmt_svc.getPaymentServiceParameters() && 
          pmt_svc.usr_params.size() )
      {        
        // Get all user payment setup parameters for the payment type    
        for(  Payment_Type_Parameters::iterator pos = pmt_svc.usr_params.begin();
              pos != pmt_svc.usr_params.end();
              ++pos )
        {
          addUserElement( pos->second );
        }
        bret=true; 
      }
    }
    else
    {
      m_result = "Couldn't get payment service data for: " + where;

    }
    return bret;    
  }
  void addUserElement( Payment_Type_Parameter & parm )
  {
    cgiInput & args = script.ClientArguments();
    ocString parmKey = "parm";
    parmKey.append(parm.Id);
    if( args.count(parmKey.c_str()) )
    {

      Customer_Payment_Parameter CustomerPayParm;
      CustomerPayParm.Customer_Pmt_Service_Id=Id;
      CustomerPayParm.Pmt_Type_Param_Id=parm.Id;
      CustomerPayParm.Name=parm.Name;
      CustomerPayParm.Machine_Name=parm.Machine_Name;
      CustomerPayParm.Value=args[parmKey.c_str()].c_str();
      if( CustomerPayParm.db_insert() == false)
      {
        m_result += "Couldn't Insert ";
        m_result +=  CustomerPayParm.last_result();
      }
      else
      {
        uparms.push_back(CustomerPayParm);
      }
    }
  }
   
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;    
                     
                
    return breturn;
  }
  
       
};

#endif


