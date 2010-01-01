/*
  role_services_Popup.cpp

  User Interface Definition and Implementation 
  for role_services.
  
  Copyright (C) 2006 by D.K. McCombs.
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
#include "cgiTemplates.h"
#include "role_services.hpp"
#include "openLogin.h" 

using namespace std;
openLogin oLogin;
#include "InfoPoints.hpp"

class role_services_form:  public role_services_Obj, public forms_base
{
public: 
  role_services_form(cgiScript & script):role_services_Obj(),forms_base(script)
  {
    setKey(*this);
  } 
  virtual ~role_services_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "role_id", role_id);
    llongFXfer( "service_id", service_id);
    boolFXfer( "enabled", enabled);
    stringFXfer( "xml_params", xml_params);
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
    script << makeTop("role_services_Popup.cgi", "role_services")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("id", "id", id ,"8");
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");
    sql = "select id, name from roles where id = ";
    sql.append(role_id);
    script << makeComboBox("role_id", "role_id", role_id ,sql);
    script << "<br class='clearall'>" << endl;
    sql = "select id, concat(cat_name, ' - ', menu_name) from services";
    script << makeComboBox("service_id", "service_id", service_id ,sql);
    script << "<br class='clearall'>" << endl; 
    script << makeBoolBox("enabled", "enabled", enabled );
    script << "<br class='clearall'>" << endl; 
    script << makeTextArea("xml_params", "xml_params", xml_params ,"3","50");
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
  role_services_form myFrm(script); 
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
    pgTemplate.load("Templates/childPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
                     .replaceAll("$heading$","role_services");

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

