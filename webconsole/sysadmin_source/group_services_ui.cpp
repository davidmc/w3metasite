/*
  group_service_ui.cpp
  ---------------------
  Group Service UI  

  Copyright (c) 2004 by D.K. McCombs
  davidmc@w3sys.com
*/
#include "metaconnectstring"
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

/*

 Group Services Form
 
 -- web glue for group services
 
 CREATE TABLE group_services (
  id bigint not null primary key auto_increment,
  service_id     bigint NOT NULL,    -- 1,n relationship to services
  group_id       bigint NOT NULL,    -- 1,n relationship to groups
  sequence       smallint NOT NULL,  -- order of appearance in setup
  xml_param1     text,               -- two xml params for anything else
  xml_param2     text
  ); 
 group_services_Form
*/  

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

