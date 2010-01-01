/*
  Patron_ui.cpp

  User Interface Definition and Implementation 
  for Patron.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  
  
*/
#include "connectstring"
#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "list_base.hpp"
#include "cgiTemplates.h"
#include "openLogin.h"
#include "Patron.hpp"
#include "list_base.hpp"

using namespace std;
openLogin oLogin;

class Patron_List:  public list_base
{
public: 
  // Constructor
  Patron_List(cgiScript&sc):list_base(sc){;}  
  ~Patron_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;

    hotCol=1;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","Patron_ui.cgi");
    emitFilter( "Patron_ui.cgi",
                "    <B>FILTER (by ???)</B>" );  
    string heading =
                     "<a class='sortcol' href='Patron_ui.cgi?sort=Person_Id'>Person_Id</a>|"                     
                     "<a class='sortcol' href='Patron_ui.cgi?sort=Payment_Type'>Payment_Type</a>|"                     
                     "<a class='sortcol' href='Patron_ui.cgi?sort=Send_Newsletter'>Send_Newsletter</a>|"                     
                     "<a class='sortcol' href='Patron_ui.cgi?sort=Give_Discount'>Give_Discount</a>|"                     
                     "<a class='sortcol' href='Patron_ui.cgi?sort=Do_Cache_Data'>Do_Cache_Data</a>|"                     
                    ;
    emitHeadings(heading);
    getFilteredData(
                     "Person_Id, " 
                     "Payment_Type, " 
                     "Send_Newsletter, " 
                     "Give_Discount, " 
                     "Do_Cache_Data, " 
                     ,"Patron",                     
                     "??? like '$filter$%'" );   
    emitData();
    emitNavigation("Patron_ui.cgi");         
    emitEnd();
    return breturn;
  }      
}; 

class Patron_form:  public Patron_Obj, public forms_base
{
public: 
  Patron_form(cgiScript & script):Patron_Obj(),forms_base(script){setKey(*this);} 
  virtual ~Patron_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Person_Id", Person_Id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "Payment_Type", Payment_Type);
    boolFXfer( "Send_Newsletter", Send_Newsletter);
    boolFXfer( "Give_Discount", Give_Discount);
    boolFXfer( "Do_Cache_Data", Do_Cache_Data);
 
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script << makeTop("Patron_ui.cgi", "Patron");
    script << makeComboBox("Person_Id", "Person_Id", Person_Id ,sql);
    script << makeComboBox("Payment_Type", "Payment_Type", Payment_Type ,sql);
    script << makeBoolBox("Send_Newsletter", "Send_Newsletter", Send_Newsletter );
    script << makeBoolBox("Give_Discount", "Give_Discount", Give_Discount );
    script << makeBoolBox("Do_Cache_Data", "Do_Cache_Data", Do_Cache_Data );
    /*    
      Special Purpose alternative field controls:
      string makeManualComboBox( string label, string name, valType value, 
                        ocString labelValuePair = "", string defaulted = "", 
                        string attributes = "");
      string makeHiddenBox( string name, valType value );
      string makeStaticBox( string label, string name, valType value, 
                          string size, string length = "");
      template<class valType> 
      string makeDateBox( string label, string name, valType value, string size);
      string makeEmailBox( string label, string name, string value, 
                        string size, string length = "");
      string makePasswordBox( string label, string name, 
                            string value, string size); 
      string makeZipBox( string label, string name, string value, string size);
      string makeImageBox( string label, string name, string value, string size); 
      string makeFileBox( string label, string name, string value, string size);                                         
      string makeTextArea( string label, string name, 
                        string value, string rows="12", string cols="80"); 
      string makeStateBox( string label, string name, string value);
    */   
    script << makeButtons( key() );        
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  Patron_form myFrm(script); 
  Patron_List mylist(script);  
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
                     .replaceAll("$heading$","Patron");

    myFrm.loadControlTemplates("Templates/form.htmp");  
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
/*
  Person_ui.cpp

  User Interface Definition and Implementation 
  for Person.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  
  
*/

#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "list_base.hpp"
#include "cgiTemplates.h"
#include "openLogin.h"
#include "Person.hpp"
#include "list_base.hpp"

using namespace std;
openLogin oLogin;

class Person_List:  public list_base
{
public: 
  // Constructor
  Person_List(cgiScript&sc):list_base(sc){;}  
  ~Person_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;

    hotCol=1;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","Person_ui.cgi");
    emitFilter( "Person_ui.cgi",
                "    <B>FILTER (by ???)</B>" );  
    string heading =
                     "<a class='sortcol' href='Person_ui.cgi?sort=Id'>Id</a>|"                     
                     "<a class='sortcol' href='Person_ui.cgi?sort=First_Name'>First_Name</a>|"                     
                     "<a class='sortcol' href='Person_ui.cgi?sort=Last_Name'>Last_Name</a>|"                     
                     "<a class='sortcol' href='Person_ui.cgi?sort=Login'>Login</a>|"                     
                     "<a class='sortcol' href='Person_ui.cgi?sort=Password'>Password</a>|"                     
                     "<a class='sortcol' href='Person_ui.cgi?sort=Address'>Address</a>|"                     
                     "<a class='sortcol' href='Person_ui.cgi?sort=City'>City</a>|"                     
                     "<a class='sortcol' href='Person_ui.cgi?sort=State'>State</a>|"                     
                     "<a class='sortcol' href='Person_ui.cgi?sort=Zip'>Zip</a>|"                     
                     "<a class='sortcol' href='Person_ui.cgi?sort=Phone'>Phone</a>|"                     
                     "<a class='sortcol' href='Person_ui.cgi?sort=EMail'>EMail</a>|"                     
                    ;
    emitHeadings(heading);
    getFilteredData(
                     "Id, " 
                     "First_Name, " 
                     "Last_Name, " 
                     "Login, " 
                     "Password, " 
                     "Address, " 
                     "City, " 
                     "State, " 
                     "Zip, " 
                     "Phone, " 
                     "EMail, " 
                     ,"Person",                     
                     "??? like '$filter$%'" );   
    emitData();
    emitNavigation("Person_ui.cgi");         
    emitEnd();
    return breturn;
  }      
}; 

class Person_form:  public Person_Obj, public forms_base
{
public: 
  Person_form(cgiScript & script):Person_Obj(),forms_base(script){setKey(*this);} 
  virtual ~Person_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    stringFXfer( "First_Name", First_Name);
    stringFXfer( "Last_Name", Last_Name);
    stringFXfer( "Login", Login);
    stringFXfer( "Password", Password);
    stringFXfer( "Address", Address);
    stringFXfer( "City", City);
    stringFXfer( "State", State);
    stringFXfer( "Zip", Zip);
    stringFXfer( "Phone", Phone);
    stringFXfer( "EMail", EMail);
 
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script << makeTop("Person_ui.cgi", "Person");
    script << makeComboBox("Id", "Id", Id ,sql);
    script << makeTextBox("First_Name", "First_Name", First_Name ,"25");
    script << makeTextBox("Last_Name", "Last_Name", Last_Name ,"25");
    script << makeTextBox("Login", "Login", Login ,"25");
    script << makeTextBox("Password", "Password", Password ,"25");
    script << makeTextBox("Address", "Address", Address ,"25");
    script << makeTextBox("City", "City", City ,"25");
    script << makeTextBox("State", "State", State ,"25");
    script << makeTextBox("Zip", "Zip", Zip ,"25");
    script << makeTextBox("Phone", "Phone", Phone ,"25");
    script << makeTextBox("EMail", "EMail", EMail ,"25");
    /*    
      Special Purpose alternative field controls:
      string makeManualComboBox( string label, string name, valType value, 
                        ocString labelValuePair = "", string defaulted = "", 
                        string attributes = "");
      string makeHiddenBox( string name, valType value );
      string makeStaticBox( string label, string name, valType value, 
                          string size, string length = "");
      template<class valType> 
      string makeDateBox( string label, string name, valType value, string size);
      string makeEmailBox( string label, string name, string value, 
                        string size, string length = "");
      string makePasswordBox( string label, string name, 
                            string value, string size); 
      string makeZipBox( string label, string name, string value, string size);
      string makeImageBox( string label, string name, string value, string size); 
      string makeFileBox( string label, string name, string value, string size);                                         
      string makeTextArea( string label, string name, 
                        string value, string rows="12", string cols="80"); 
      string makeStateBox( string label, string name, string value);
    */   
    script << makeButtons( key() );        
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  Person_form myFrm(script); 
  Person_List mylist(script);  
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
                     .replaceAll("$heading$","Person");

    myFrm.loadControlTemplates("Templates/form.htmp");  
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



