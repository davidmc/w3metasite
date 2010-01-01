/*
  Ordered_Item_Option_ui.cpp

  User Interface Definition and Implementation 
  for Ordered_Item_Option.
  
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
#include "Ordered_Item_Option.hpp"
#include "openLogin.h" // open-login has own connectstring

using namespace std;
openLogin oLogin;

class Ordered_Item_Option_List:  public list_base
{
public: 
  // Constructor
  Ordered_Item_Option_List(cgiScript&sc):list_base(sc){;}  
  ~Ordered_Item_Option_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;

    hotCol=-2;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","Ordered_Item_Option_ui.cgi");
    emitFilter( "Ordered_Item_Option_ui.cgi",
                "    <B>FILTER (by Name)</B>" );  
    string heading =
                     "<a class='sortcol' href='Ordered_Item_Option_ui.cgi?sort=Id'>Id</a>|"                     
                     "<a class='sortcol' href='Ordered_Item_Option_ui.cgi?sort=Ordered_Item_Id'>Ordered_Item_Id</a>|"                     
                     "<a class='sortcol' href='Ordered_Item_Option_ui.cgi?sort=Name'>Name</a>|"                     
                     "<a class='sortcol' href='Ordered_Item_Option_ui.cgi?sort=Value'>Value</a>|"                     
                    ;
    emitHeadings(heading);
    getFilteredData(
                     "Id, " 
                     "Ordered_Item_Id, " 
                     "Name, " 
                     "Value " 
                     ,"Ordered_Item_Option",                     
                     "Name like '$filter$%'" );   
    emitData();
    emitNavigation("Ordered_Item_Option_ui.cgi");         
    emitEnd();
    return breturn;
  }      
}; 

class Ordered_Item_Option_form:  public Ordered_Item_Option_Obj, public forms_base
{
public: 
  Ordered_Item_Option_form(cgiScript & script):Ordered_Item_Option_Obj(),forms_base(script){setKey(*this);} 
  virtual ~Ordered_Item_Option_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "Ordered_Item_Id", Ordered_Item_Id);
    stringFXfer( "Name", Name);
    stringFXfer( "Value", Value);
 
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
    
    script << makeTop("Ordered_Item_Option_ui.cgi", "Ordered_Item_Option")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("Id", "Id", Id ,"8");
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");
    
    script << makeComboBox("Ordered_Item_Id", "Ordered_Item_Id", Ordered_Item_Id ,sql);
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Name", "Name", Name ,"25");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Value", "Value", Value ,"25");
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
  Ordered_Item_Option_form myFrm(script); 
  Ordered_Item_Option_List mylist(script);  
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
                     .replaceAll("$heading$","Ordered_Item_Option");

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

