/*
  group_services_ui.cpp

  User Interface Definition and Implementation 
  for group_services.
  
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
#include "group_services.hpp"
#include "list_base.hpp"
#include "openLogin.h" // open-login has own connectstring
using namespace std;
openLogin oLogin;

class group_services_form:  public group_services_Obj, public forms_base
{
public: 
  group_services_form(cgiScript & script):group_services_Obj(),forms_base(script)
  {
    setKey(*this);
  } 
  virtual ~group_services_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "service_id", service_id);
    llongFXfer( "group_id", group_id);
    shortFXfer( "sequence", sequence);
    stringFXfer( "xml_param1", xml_param1);
    stringFXfer( "xml_param2", xml_param2);
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  { 
    bool breturn = true;    
    ocString sql = "select id, menu_name from services order by sequence" ;
    script << makeTop("group_services_ui.cgi", "group_services")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("id", "id", id ,"8");
    script << "<br class='clearall'>" << endl;
    script << makeTextBox("sequence", "sequence", sequence ,"4","4");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("xml_param1", "xml_param1", xml_param1 ,"25");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("xml_param2", "xml_param2", xml_param2 ,"25");
    script << "<br class='clearall'>" << endl; 
    
    script << formTemplate.getParagraph("advanced_end");
    
    script << makeComboBox("service_id", "service_id", service_id ,sql);
    script << "<br class='clearall'>" << endl; 
    
    sql = "select id, name from groups ";
    if(group_id)
    {
      sql += "where id =";
      sql.append(group_id);
    }
    sql += "  order by name";
    
    script << makeComboBox("group_id", "group_id", group_id ,sql);
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
  group_services_form myFrm(script); 
   
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/childPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
                     .replaceAll("$heading$","group_services");

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

/*
  group_service_ui.cpp
  ---------------------
  Group Service UI  

  Copyright (c) 2004 by D.K. McCombs
  davidmc@w3sys.com

#include "connectstring"
#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "cgiTemplates.h"
#include "group_services.hpp"
#include "openLogin.h"
openLogin oLogin;

class group_services_Form: public group_services, public forms_base
{
public:
  
  // Constructor
  group_services_Form(cgiScript & script):group_services(),forms_base(script){setKey(*this);} 
  ~group_services_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  } 
   void form_data_transfer( void )
  {
    llongFXfer( "service_id", service_id ); 
    llongFXfer( "group_id", group_id );    
    intFXfer( "sequence", sequence );
    stringFXfer( "xml_param1", xml_param1 );
    stringFXfer( "xml_param2", xml_param2 );    
  } 
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    ocString gsql = "select id, name from groups ";
    if(group_id)
    {
      gsql += "where id =";
      gsql.append(group_id);
    }
    gsql += "  order by name";
    script << makeTop("group_services_ui.cgi", "GROUP SERVICES")
           << makeStaticBox("Id", "id", id, "12")
           << makeComboBox("Service", "service_id", service_id, 
                           "select id, name from services order by name" )   
           << makeComboBox("Group", "group_id", group_id, 
                           gsql.c_str() )                        
           << makeTextBox("sequence","sequence",sequence,"8")
           << makeTextArea("XML Parameters 1","xml_param1", xml_param1, "2", "50" )
           << makeTextArea("XML Parameters 2","xml_param2", xml_param2, "2", "50" )           
           << makeButtons( key() )
           << makeBottom( m_result ) << endl; 
    return breturn;
  } 

};



int main( int argcount, char ** args )
{
  bool isSignedOn = false;

  // An instance of the script
  // must be created before any other output.
  cgiScript script("text/html",false);
  group_services_Form myFrm(script); 

  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
            .replaceAll("$heading$","services");

    myFrm.loadControlTemplates("Templates/child_form.htmp");  
    myFrm.form_action();  
    myFrm.form_display();    

    script << pgTemplate.getParagraph("bottom");
  }
  else
  {
    script.Redirect("signIn.html"); 
  }  
}
// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"
*/

