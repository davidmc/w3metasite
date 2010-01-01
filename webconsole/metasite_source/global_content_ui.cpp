/*

  Copyright 2004 by D.K. McCombs.
  ===============================
    
*/

#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "global_content.hpp"
#include "richForm.hpp"
#include "list_base.hpp"
#include "cgiTemplates.h"
#include "openLogin.h"

using namespace std;
openLogin oLogin;



class global_content_form:  public global_content, public richForm
{
public: 
  global_content_form(cgiScript & script):global_content(),richForm(script){setKey(*this);} 
  virtual ~global_content_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  
  void form_data_transfer( void )
  {
    // Make data in controls back to object
    llongFXfer( "site_id", site_id );        
    stringFXfer( "name", name);    
    stringFXfer( "content", content  ); 
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // Implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    ocString ssql = "select id, name from metasite.sites ";
    if(site_id)
    {
      ssql += "where id =";
      ssql.append(site_id);
    }
    script << makeTop("global_content_ui.cgi", "Global Content")
           << makeStaticBox("id", "id", id, "12")
           << makeComboBox("Site", "site_id", site_id, ssql )
        << makeTextBox( "Enter '<b>top</b>', '<b>end</b>', or spacer came", "name", name, "25" ) << "(Use the template name of the spacer.)"
           << makeRichEntry( "Content","content", content )
           << makeButtons( key() );
    script << makeBottom( m_result ) << endl;
    return breturn;
  }
  
  bool ivalidate( void  ) 
  { 
    name = ocString(name).remove("|");
    content = htmlDecode( content );
    return true; 
  }
  bool uvalidate( changeMap & changes  ) 
  { 
    name = ocString(name).remove("|");
    content = htmlDecode( content );
    return true; 
  }
  
  bool ssupplemental( void )
  { 
    name = ocString(name).remove("|");
    content = htmlFixup( content );
    return true; 
  }  
};

int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  global_content_form myFrm(script); 

  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/editorPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
            .replaceAll("$heading$","SITES");

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
