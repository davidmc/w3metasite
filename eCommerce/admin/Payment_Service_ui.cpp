/*
  Payment_Service_ui.cpp

  User Interface Definition and Implementation 
  for Payment_Service.
  
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
#include "Payment_Service.hpp"
#include "openLogin.h"


using namespace std;
openLogin oLogin;

class Payment_Service_List:  public list_base
{
public: 
  // Constructor
  Payment_Service_List(cgiScript&sc):list_base(sc){;}  
  ~Payment_Service_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;

    hotCol=-2;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","Payment_Service_ui.cgi");
    emitFilter( "Payment_Service_ui.cgi",
                "    <B>FILTER (by Name)</B>" );
    string heading =
                     "<a class='sortcol' href='Payment_Service_ui.cgi?sort=Id'>Id</a>|"                     
                     "<a class='sortcol' href='Payment_Service_ui.cgi?sort=Type'>Type</a>|"                     
                     "<a class='sortcol' href='Payment_Service_ui.cgi?sort=Name'>Name</a>|"                     
                     "<a class='sortcol' href='Payment_Service_ui.cgi?sort=Label'>Label</a>|"                     
                     "<a class='sortcol' href='Payment_Service_ui.cgi?sort=Instructions'>Instructions</a>|"                     
                    ;
    emitHeadings(heading);
    getFilteredData(
                     "Id, " 
                     "Type, " 
                     "Name, " 
                     "Label, " 
                     "Instructions " 
                     ,"Payment_Service",                     
                     "Name like '$filter$%'" );
    emitData();
    emitNavigation("Payment_Service_ui.cgi");         
    emitEnd();
    return breturn;
  }      
}; 

class Payment_Service_form:  public Payment_Service_Obj, public forms_base
{
public: 
  Payment_Service_form(cgiScript & script):Payment_Service_Obj(),forms_base(script){setKey(*this);} 
  virtual ~Payment_Service_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "Type", Type);
    stringFXfer( "Name", Name);
    stringFXfer( "Label", Label);
    stringFXfer( "Instructions", Instructions);
 
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
    
    script << makeTop("Payment_Service_ui.cgi", "Payment_Service")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("Id", "Id", Id ,"8");
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");
    
    script << makeTextBox("Type", "Type", Type ,"5");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Name", "Name", Name ,"45","25");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Label", "Label", Label ,"45","25");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Instructions", "Instructions", Instructions ,"225","25");
    script << "<br class='clearall'>" << endl; 

    script << makeButtons( key() );        
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  Payment_Service_form myFrm(script); 
  Payment_Service_List mylist(script);  
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
                     .replaceAll("$heading$","Payment_Service");

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

