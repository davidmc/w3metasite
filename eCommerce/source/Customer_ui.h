/*
  Customer_ui.h

  User Interface Definition and Implementation 
  for Customer.
  
  Copyright (c) 2005 by D.K. McCombs.
  davidmc@w3sys.com
  W3 Systems Design Inc : http://www.w3sys.com
    
*/
#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "cgiTemplates.h"
#include "Customer.hpp"
#include "list_base.hpp"
#include "forms_base.hpp"

using namespace std;

class Customer_form:  public Customer_Obj, public forms_base
{
public: 
  Customer_form(cgiScript & script):Customer_Obj(),forms_base(script){setKey(*this);} 
  virtual ~Customer_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    stringFXfer( "First_Name", First_Name);
    stringFXfer( "Last_Name", Last_Name);
    stringFXfer( "EMail", EMail);
    stringFXfer( "Password", Password);
    stringFXfer( "Address", Address);
    stringFXfer( "City", City);
    stringFXfer( "State", State);
    stringFXfer( "Zip", Zip);
    stringFXfer( "Phone", Phone);
    llongFXfer( "Payment_Service_Id", Payment_Service_Id);
 
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;

    script << makeHiddenBox("Id", Id );
    script << makeTextBox("First Name", "First_Name", First_Name ,"35","25");
    script << makeTextBox("Last Name", "Last_Name", Last_Name ,"35","25");
    script << makeTextBox("E-Mail", "EMail", EMail ,"45","25");
    script << makePasswordBox("Password", "Password", Password ,"10");
    script << makeTextBox("Address", "Address", Address ,"45","25");
    script << makeTextBox("City", "City", City ,"35","25");
    script << makeStateBox("State", "State", State );
    script << makeTextBox("Zip", "Zip", Zip ,"10");
    script << makeTextBox("Phone", "Phone", Phone ,"25");
    script << makeComboBox("Payment Type", "Payment_Service_Id", Payment_Service_Id ,
                           "select Id, Name from Payment_Service ");      
    return breturn;
  }
  // override for extra info
  string makeTop(string uri, string name )
  {
    ocString ret = formTemplate.getParagraph("form_begin");  
    return ret.replace("$step_title$","Check Out Step 3 : Account Information")
              .replace("$action$",uri.c_str())
              .replace("$name$",name.c_str())
              .replace("$instructions$","Please fill out all of the information below, including the shipping address.");  
  }  
  string makeButtonedBottom( string action  )
  {    
    ocString ret = formTemplate.getParagraph("form_end");    
    return ret.replaceAll("$ACTION",action);
  } 
};

/*
  How to Use the class:
  
  // need one of these
  cgiScript script;
  // instanceof the class
  Customer_form myFrm(script);   
 
  // Emit the top of page
  --needs onChange event to update xml changes info
    just add the following to the head element:
  +-----------------------------------------------------------------  
  |<SCRIPT type="text/javascript" src="openDom.js"></SCRIPT>
  |<SCRIPT type="text/javascript">
  |var myDom = new openDom();
  |var updates = new Array();
  |<SCRIPT type="text/javascript" src="forms.js"></SCRIPT>
  |<SCRIPT type="text/javascript" src="htmlFormEvents.js"></SCRIPT>
  +------------------------------------------------------------------
  
  // Load the forms control template, make sure its valid!
  myFrm.loadControlTemplates("Templates/form.htmp");  
  
  // this performs any inserts/updates/deletes
  myFrm.form_action();  
  // this shows the form
  myFrm.form_display(); 
  
  // compile in base object implementations here (or link them, but this saves a little exe size.)
  #include "read_write_base.cpp"
  #include "forms_base.cpp"
  
*/







