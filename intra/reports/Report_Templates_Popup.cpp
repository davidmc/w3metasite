/*
  Report_Templates_Popup.cpp

  User Interface Definition and Implementation 
  for Report_Templates.
  
  Copyright (C) 2005 by D.K. McCombs.
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
#include "Report_Templates.hpp"
#include "list_base.hpp"
#include "openLogin.h" // open-login has own connectstring
using namespace std;
openLogin oLogin;
#include "InfoPoints.hpp"

class Report_Templates_form:  public Report_Templates_Obj, public forms_base
{
public: 
  Report_Templates_form(cgiScript & script):Report_Templates_Obj(),forms_base(script)
  {
    setKey(*this);
  } 
  virtual ~Report_Templates_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    stringFXfer( "Name", Name);
    stringFXfer( "Path", Path);
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  { 
    bool breturn = true;    
    ocString sql;
    script << makeTop("Report_Templates_Popup.cgi", "Report_Templates")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("Id", "Id", Id ,"8");
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");
    
    script << makeTextBox("Name", "Name", Name ,"225","35");
    script << "<br class='clearall'>" << endl; 
    script << makeFileBox("Path", "Path", Path ,"55");
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
  Report_Templates_form myFrm(script); 
  // New: need to be able to override where to get the data and what cookie to set for id
  infoPoints iPoints;
  if( iPoints.idToken.length() )
  {
    oLogin.token = iPoints.idToken;
  }
  // end of new 
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/childPane.htmp"); // to get the right path to file picker
    
    script << ocString(pgTemplate.getParagraph("top"))
                     .replaceAll("$heading$","Report_Templates")
        .replace("<!--Added_Header_Content-->", // make sure filePick goes to reportTemplates
                              "<SCRIPT type='text/javascript'>\n"
                              "function placeFile(id)\n"
                              "{\n"
                               " ctrlId = id;\n"
                               " window.open(\"filePick.cgi/reportTemplates\",\n"
                               "             \"_blank\",\n"
                               "             \"scrollbars=yes,toolbar=no,menubar=no,location=no\");\n"
                              "}\n"
                              "</script>");

    myFrm.loadControlTemplates("Templates/childdivform.htmp");  
    myFrm.form_action();  
    myFrm.form_display();
        
    ocString end = pgTemplate.getParagraph("bottom");
    script << end;          
  }
  else
  {
    script.Redirect("/"); 
  } 
}
// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"

