/*
  Order_Status_ui.cpp

  User Interface Definition and Implementation 
  for Order_Status.
  
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
#include "Order_Status.hpp"
#include "openLogin.h" // open-login has own connectstring

using namespace std;
openLogin oLogin;

class Order_Status_List:  public list_base
{
public: 
  // Constructor
  Order_Status_List(cgiScript&sc):list_base(sc){;}  
  ~Order_Status_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;

    hotCol=-2;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","Order_Status_ui.cgi");
    
    string heading =
                     "<a class='sortcol' href='Order_Status_ui.cgi?sort=Id'>Id</a>|"                     
                     "<a class='sortcol' href='Order_Status_ui.cgi?sort=Name'>Name</a>|"                     
                    ;
    emitHeadings(heading);
    getData("select Id, Name from  Order_Status" );   
    emitData();
    emitNavigation("Order_Status_ui.cgi");         
    emitEnd();
    return breturn;
  }      
}; 

class Order_Status_form:  public Order_Status_Obj, public forms_base
{
public: 
  Order_Status_form(cgiScript & script):Order_Status_Obj(),forms_base(script){setKey(*this);} 
  virtual ~Order_Status_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    stringFXfer( "Name", Name);
 
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
    
    script << makeTop("Order_Status_ui.cgi", "Order_Status")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("Id", "Id", Id ,"8");
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");
    
    script << makeTextBox("Name", "Name", Name ,"25");
    script << "<br class='clearall'>" << endl; 
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
  Order_Status_form myFrm(script); 
  Order_Status_List mylist(script);  
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
                     .replaceAll("$heading$","Order_Status");

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
