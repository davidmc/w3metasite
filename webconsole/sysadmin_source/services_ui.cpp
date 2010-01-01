/*
  services Form
  ===================================
  CGI User Interface for services
            
*/
#include "metaconnectstring"
#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "list_base.hpp"
#include "cgiTemplates.h"
#include "services.hpp"
#include "openLogin.h"

openLogin oLogin;
/*
  services Form 
    - Glues the business object to the form object through 
      multiple inheritance.      
CREATE TABLE services (
  id bigint not null primary key auto_increment,
  sequence integer NOT NULL,         -- order of apearance in a list
  tree_depth smallint NOT NULL,      -- order and tree depth determine tree position without recursion
  enabled int NOT NULL,              -- permits turning off of service for web site maintenance
  name varchar(32) NOT NULL,         -- name (for header)
  menu_name varchar(32) NOT NULL,    -- name (for menu item)
  uri varchar(64) NOT NULL,          -- location of resource
  local_uri int NOT NULL,            -- local service flag
  secure int NOT NULL,               -- determines if url should use https protocol
  xml_params text NOT NULL           -- generic parameters for service
);                  
*/                 
class services_Form:  public services, public forms_base
{
public: 
  // Constructor
  services_Form(cgiScript & script):services(),forms_base(script){setKey(*this);}  
  ~services_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    intFXfer( "sequence", sequence );
    shortFXfer( "tree_depth", tree_depth );
    boolFXfer( "enabled", enabled );              
    stringFXfer( "name", name );
    stringFXfer( "menu_name", menu_name );
    stringFXfer( "uri", uri );
    boolFXfer( "local_uri", local_uri );
    boolFXfer( "secure", secure );
    stringFXfer( "xml_params", xml_params );
  } 
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script << makeTop("services_ui.cgi", "SERVICES")
           << makeStaticBox("Id", "id", id, "12")
           << makeTextBox("sequence","sequence",sequence,"8")
           << makeTextBox("Tree Depth", "tree_depth", tree_depth,"4")
           << makeBoolBox("Service Enabled?","enabled", enabled)
           << makeTextBox("Name","name",name,"32")
           << makeTextBox("Menu Name","menu_name",menu_name,"32") 
           << makeTextBox("Location","uri",uri,"64")
           << makeBoolBox("Local?","local_uri", local_uri)
           << makeBoolBox("secure?","secure", secure)
           << makeTextArea("XML Parameters","xml_params", xml_params, "2", "50" )
           << makeButtons( key() )
           << makeBottom( m_result ) << endl; 
    return breturn;
  }      
};

class services_List:  public list_base
{
public: 
  // Constructor
  services_List(cgiScript&sc):list_base(sc)
  {
    ;                   
  }  
  ~services_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","services_ui.cgi");
    hotCol=1;    
   
    emitFilter( "services_ui.cgi",  // program name
                "&nbsp;&nbsp;&nbsp;&nbsp;<B>FILTER</B>" );  // Label

    emitHeadings("Id|Service Name"); 
            
    getFilteredData( "id, name", 
                     "services", 
                     "name like '$filter$%'", 
                     "tree_depth,sequence" );     
    emitData();
    
    emitNavigation("services_ui.cgi");
    
    emitEnd();
    return breturn;
  }
  void derived_data_event( void )
  {  
      
  }
        
};

int main( int argc, char ** argv )
{  
  cgiScript script( "text/html", false );
  services_Form myFrm(script); 
  services_List mylist(script); // so you can set cookies 
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
            .replaceAll("$heading$","services");

    myFrm.loadControlTemplates("Templates/form.htmp");  
    myFrm.form_action();  
    myFrm.form_display();    
    
    mylist.loadListTemplates("Templates/list.htmp");  
    mylist.list_display();
    
    script << pgTemplate.getParagraph("bottom");
  }
  else
  {
    script.Redirect("signIn.html"); 
  }  
};

// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"


 
