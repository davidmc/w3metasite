/*
  Config Form
  ===================================
  CGI User Interface for Pay Drop Config
            
*/
#include "connectstring"
#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "list_base.hpp"
#include "cgiTemplates.h"
#include "Config.hpp"
#include "nav_menu.h"

/*
  Config Form - Glues the business object to the form object through 
                  multiple inheritance.
                  
*/                 
class Config_Form:  public Config, public forms_base
{
public: 
  // Constructor
  Config_Form(cgiScript & script):Config(),forms_base(script){setKey(*this);}  
  ~Config_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "ConfigID", ConfigID );
  }
  void form_data_transfer( void )
  {
    stringFXfer( "ConfigVariable",  ConfigVariable);
    stringFXfer( "ConfigValue", ConfigValue );    
  } 
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script << makeTop( "config_ui.cgi", "Configuration" )
           << makeTextBox("Config ID","Id",ConfigID,"12")
           << makeTextBox("ConfigVariable","ConfigVariable",ConfigVariable,"56")  
           << makeTextBox("ConfigValue","ConfigValue",ConfigValue,"56")                            
           << makeButtons( key() )
           << makeBottom( m_result ) << endl; 
    return breturn;
  }      
};

class Config_List:  public list_base
{
public: 
  // Constructor
  Config_List(cgiScript&sc):list_base(sc)
  {
    ;                   
  }  
  ~Config_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","config_ui.cgi");
    hotCol=1;    
    
    emitFilter( "config_ui.cgi",  // program name
                "Config" );       // Label
    
    emitHeadings("Id|Configuration Parameter"); 
            
    getFilteredData( "ConfigID, ConfigVariable", "Config", 
                     "ConfigVariable like '$filter$%'", "ConfigVariable" );     
    emitData();
    
    emitNavigation("config_ui.cgi");
    
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
  Config_Form  cat(script); 
  Config_List mylist(script); // so you can set cookies 
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << pgTemplate.getParagraph("top");

    cat.loadControlTemplates("Templates/form.htmp");  
    cat.form_action();  
    cat.form_display();
    
    
    mylist.loadListTemplates("Templates/list.htmp");  
    mylist.list_display();
    
    script << pgTemplate.getParagraph("bottom");
  }
  else
  {
    script.Redirect("signIn.html"); 
  } 
};

// as an alternative to building more objects
#include "read_write_base.cpp"
#include "forms_base.cpp"


 
