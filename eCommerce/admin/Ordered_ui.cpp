/*
  Ordered_ui.cpp

  User Interface Definition and Implementation 
  for Ordered.
  
  Copyright (c) 2005 by D.K. McCombs.
  
  davidmc@w3sys.com
  
  W3 Systems Design Inc.  
  
*/
#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "cgiTemplates.h"

#include "read_write_base.hpp"
#include "list_base.hpp"
#include "forms_base.hpp"
#include "Ordered.hpp"
#include "openLogin.h" // open-login has own connectstring

// #include "../source/proofs.hpp"
// #include "Customer.hpp"

using namespace std;
openLogin oLogin;

class Ordered_List:  public list_base
{
public: 
  // Constructor
  Ordered_List(cgiScript&sc):list_base(sc){;}  
  ~Ordered_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;

    hotCol=-2;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","Ordered_ui.cgi");
    emitFilter( "Ordered_ui.cgi",
                "    <B>Find (by Order Id)</B>" );  
    string heading =
                     "<a class='sortcol' href='Ordered_ui.cgi?sort=Id'>Id</a>|"                     
                     "<a class='sortcol' href='Ordered_ui.cgi?sort=s.Name'>Order Status</a>|"                     
                     "<a class='sortcol' href='Ordered_ui.cgi?sort=Product_Total'>Product Total</a>|"                     
                     "<a class='sortcol' href='Ordered_ui.cgi?sort=Shipping_Total'>Shipping Total</a>|"                     
                     "<a class='sortcol' href='Ordered_ui.cgi?sort=Tax_Total'>Tax Total</a>|"                     
                     "<a class='sortcol' href='Ordered_ui.cgi?sort=Order_Total'>Order Total</a>|"                     
                     "<a class='sortcol' href='Ordered_ui.cgi?sort=Order_Date'>Order Date</a>|"
                     "<a class='sortcol' href='Ordered_ui.cgi?sort=Transaction_Id'>Transaction Id</a>";
    emitHeadings(heading);
    getFilteredData(
                     "o.Id, " 
                     "s.Name, " 
                     "o.Product_Total, " 
                     "o.Shipping_Total, " 
                     "o.Tax_Total, " 
                     "o.Order_Total, " 
                     "o.Order_Date, " 
                     "o.Transaction_Id "
                     ,"Ordered o inner join Order_Status s on o.Order_Status_Id = s.Id",                     
                     "o.Id = $filter$" );   
    emitData();
    emitNavigation("Ordered_ui.cgi");         
    emitEnd();
    return breturn;
  }      
}; 

class Ordered_form:  public Ordered_Obj, public forms_base
{
public: 
  Ordered_form(cgiScript & script):Ordered_Obj(),forms_base(script){setKey(*this);} 
  virtual ~Ordered_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "Customer_Id", Customer_Id);
    llongFXfer( "Order_Status_Id", Order_Status_Id);
    moneyFXfer( "Product_Total", Product_Total);
    moneyFXfer( "Shipping_Total", Shipping_Total);
    moneyFXfer( "Tax_Total", Tax_Total);
    moneyFXfer( "Order_Total", Order_Total);
    dateFXfer( "Order_Date", Order_Date);
    stringFXfer( "Transaction_Id", Transaction_Id);
    stringFXfer( "Notes", Notes);
    llongFXfer( "Customer_Pmt_Svc_Id", Customer_Pmt_Svc_Id);
 
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    ocString sql; // for combo boxes
    
    script << makeTop("Ordered_ui.cgi", "Ordered")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("Id", "Id", Id ,"8");
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");    
    sql = "select Id, concat( First_Name, ' ', Last_Name ) as Name from Customer";
    script << makeComboBox("Customer", "Customer_Id", Customer_Id ,sql);
    script << "<br class='clearall'>" << endl; 
    sql = "select Id, Name from Order_Status";
    script << makeComboBox("Order Status", "Order_Status_Id", Order_Status_Id ,sql);
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Product_Total", "Product_Total", Product_Total ,"16","16");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Shipping_Total", "Shipping_Total", Shipping_Total ,"16","16");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Tax_Total", "Tax_Total", Tax_Total ,"16","16");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Order_Total", "Order_Total", Order_Total ,"16","16");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Order_Date", "Order_Date", Order_Date ,"24","24");
    script << "<br class='clearall'>" << endl;
    script << makeTextBox("Transaction_Id", "Transaction_Id", Transaction_Id ,"25");
    script << "<br class='clearall'>" << endl;      
    script << makeTextArea("Notes", "Notes", Notes ,"3","40");
    script << "<br class='clearall'>" << endl; 
    // select Id, Customer_Id, Payment_Service_Id from Customer_Payment_Service;
    // select  Id, Type, Name, Label, Instructions from Payment_Service;
    sql = "select cps.Id, ps.Name from Customer_Payment_Service cps "
          "inner join Payment_Service ps on cps.Payment_Service_Id = ps.Id where cps.Customer_Id =";
    sql.append(Customer_Id);          
    script << makeComboBox("Customer_Pmt_Svc_Id", "Customer_Pmt_Svc_Id", Customer_Pmt_Svc_Id ,sql);
    script << "<br class='clearall'>" << endl; 
    
    script << makeButtons( key() );  
 /*      
    if( Id )
    {  
      showCustomer();
      showProof();
    }
 */          
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
/*  
  void showProof(void)
  {
    proofs proof;
    proof.m="1";
    proof.o="";
    proof.o.append(Id);
    proof.showOrderedItems( script );     
  
  }
  void showCustomer()
  {
    Customer_Obj cstmr;
    cstmr.key(Customer_Id);
    if( cstmr.get_data() )
    {
      ocString grp = formTemplate.getParagraph("control_group");
      script << "<h2>Customer Information:</h2>" << endl;
      script << grp.replace("$label$","Customer")
                   .replace("$control$", (cstmr.First_Name+" "+cstmr.Last_Name).c_str());
      script << "<br class='clearall'>" << endl;             
      script << grp.replace("$label$","Phone")
                .replace("$control$",cstmr.Phone.c_str());
                
      script << "<br class='clearall'>" << endl;             
      script << grp.replace("$label$","E-Mail")
                .replace("$control$",cstmr.EMail.c_str());
      script << "<br class='clearall'>" << endl;             
      script << grp.replace("$label$","Address")
                .replace("$control$",cstmr.Address.c_str());
                                    
      script << "<br class='clearall'>" << endl;             
      script << grp.replace("$label$","City")
                .replace("$control$",cstmr.City.c_str());
      script << "<br class='clearall'>" << endl;             
      script << grp.replace("$label$","State")
                .replace("$control$",cstmr.State.c_str());
      script << "<br class='clearall'>" << endl;             
      script << grp.replace("$label$","Zip")
                .replace("$control$",cstmr.Zip.c_str());
      script << "<br class='clearall'>" << endl;                                                        
    }
  }
*/  
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  Ordered_form myFrm(script); 
  Ordered_List mylist(script);  
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
                     .replaceAll("$heading$","Ordered");

    myFrm.loadControlTemplates("Templates/divform.htmp");  
    myFrm.form_action();  
    myFrm.form_display();
    
    mylist.loadListTemplates("Templates/list.htmp");  
    mylist.list_display();
        
    ocString end = pgTemplate.getParagraph("bottom");
    script << end;          
  }
  else
  {
    script.Redirect("signIn.html"); 
  } 
}
// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"

