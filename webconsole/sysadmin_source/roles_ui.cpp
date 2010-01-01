/*
  roles Form
  ===================================
  CGI User Interface for roles
            
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
#include "roles.hpp"
#include "openLogin.h"
openLogin oLogin;

/*
  roles Form 
    - Glues the business object to the form object through 
      multiple inheritance.      
                  
*/                 
class roles_Form:  public roles, public forms_base
{
public: 
  // Constructor
  roles_Form(cgiScript & script):roles(),forms_base(script){setKey(*this);}  
  ~roles_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    stringFXfer( "name", name );
    stringFXfer( "comment", comment );
    stringFXfer( "service_id",  role_services);
  } 
  
  
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script << makeTop("roles_ui.cgi", "ROLES")
           << makeStaticBox("Id","id",id,"12" )             
           << makeTextBox("Name","name",name,"24")
           << makeTextArea("Comments","comment",comment,  "2", "50")
           << makeComboBox( "Services","service_id",role_services,
                            "select id,  name from services", 
                            "", " multiple='multiple' size='6'" );
    script << makeButtons( key() )
           << makeBottom( m_result ) << endl; 
    return breturn;
  }


  // Done after any action - a false return sets a warning
  virtual bool isupplemental( void )
  {
    setServices();
    return true;
  }
  virtual bool usupplemental( changeMap & changes )
  {
    deleteServices();
    setServices();
    return true;
  }
  virtual bool dsupplemental( void )
  {
    deleteServices();
    return true;
  }
  virtual bool ssupplemental( void )
  {
    retrieveServices();
    return true;
  }
  

};

class roles_List:  public list_base
{

public:
  // Constructor
  roles_List(cgiScript&sc):list_base(sc)
  {
    ;                   
  }  
  ~roles_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","roles_ui.cgi");
    hotCol=1;    
    
    emitFilter( "roles_ui.cgi",  // program name
                "&nbsp;&nbsp;&nbsp;&nbsp;<B>FILTER</B>" ); // Label
    
    emitHeadings("Id|Role Name"); 
            
    getFilteredData( "id, name", 
                     "roles", 
                     "name like '$filter$%'", 
                     "name" );     
    emitData();
    
    emitNavigation("roles_ui.cgi");
    
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
  roles_Form myFrm(script); 
  roles_List mylist(script); // so you can set cookies 
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
            .replaceAll("$heading$","roles");

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


 
