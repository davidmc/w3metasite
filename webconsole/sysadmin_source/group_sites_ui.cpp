/*
  group_sites Form
  ===================================
  CGI User Interface for group_sites
            
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
#include "group_sites.hpp"
#include "openLogin.h"

openLogin oLogin;
/*
  group_sites Form 
    - Glues the business object to the form object through 
      multiple inheritance.    
        
  CREATE TABLE group_sites (
    id bigint not null primary key auto_increment,
    group_id bigint NOT NULL,  -- group 
    site_id bigint NOT NULL    -- site   

*/                 
class group_sites_Form:  public group_sites, public forms_base
{
public: 
  // Constructor
  group_sites_Form(cgiScript & script):group_sites(),forms_base(script){setKey(*this);}  
  ~group_sites_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "group_id", group_id );
    llongFXfer( "site_id", site_id );    
  } 
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script << makeTop("group_sites_ui.cgi", "GROUP SITES")
           << makeStaticBox("Id", "id", id, "12")           
           << makeComboBox( "Group", "group_id", group_id, 
                            "select id, name from groups order by name" )  
           << makeComboBox( "Site", "site_id", site_id, 
                            "select id, name from metasite.sites order by name" )                            
           << makeButtons( key() )
           << makeBottom( m_result ) << endl; 
    return breturn;
  }      
};

class group_sites_List:  public list_base
{
public: 
  // Constructor
  group_sites_List(cgiScript&sc):list_base(sc)
  {
    ;                   
  }  
  ~group_sites_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","group_sites_ui.cgi");
    hotCol=1;    
   
    emitFilter( "group_sites_ui.cgi",  // program name
                "&nbsp;&nbsp;&nbsp;&nbsp;<B>FILTER</B>" );  // Label

    emitHeadings("Id|Group|Site"); 
            
    getFilteredData( "gs.id, g.name, s.name", 
                     "group_sites gs inner join groups g on gs.group_id=g.id "
                     "inner join metasite.sites s on gs.site_id = s.id ",
                     "g.name like '$filter$%'", 
                     "g.name,s.name" );     
    emitData();
    
    emitNavigation("group_sites_ui.cgi");
    
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
  group_sites_Form myFrm(script); 
  group_sites_List mylist(script); // so you can set cookies 
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
            .replaceAll("$heading$","group_sites");

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


 
