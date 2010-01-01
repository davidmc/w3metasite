/*
  Copyright 2002 by D.K. McCombs.
  ===============================
  
*/

#include <iostream>
#include <iomanip>
#include "cgiExtend.h"
#include "cgiTemplates.h"
#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "list_base.hpp"
#include "openLogin.h"
#include "siteLimit.h"
#include "templates.hpp"

using namespace std;
openLogin oLogin;

class template_form:  public Template, public forms_base
{
public: 
  template_form(cgiScript & script):Template(),forms_base(script){setKey(*this);} 
  virtual ~template_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    // Make data in controls back to object
    llongFXfer( "site_id", site_id );
    stringFXfer( "name", name);    
    stringFXfer( "path",path );  
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    string siteLimitation = siteFocus(oLogin);
    string sql = "select id, name from metasite.sites";
    if( siteLimitation.length() != 0 )
    {
      sql += " where id ";
      sql += siteLimitation;
    }
    sql += " order by name";
    script << "<!--" << sql << "-->" << endl;
    script << makeTop("template_ui.cgi", "TEMPLATES")
           << makeStaticBox("id", "id", id, "12")
           << makeComboBox( "Site", "site_id", site_id, 
                            sql ) 
           << makeTextBox("name","name",name,"50") 
           << makeFileBox( "path","path",path,"50") 
           << makeButtons( key() );
       
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  template_form myFrm(script); 

  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
            .replaceAll("$heading$","SITES");

    myFrm.loadControlTemplates("Templates/form.htmp");  
    myFrm.form_action();  
    myFrm.form_display();
    
    ocString end = pgTemplate.getParagraph("bottom");
    if( myFrm.getMode() != "s" )
    {
      script << end.replace("/*_extra_js_*/","  remenu();");
    }
    else
    {
      script << end;
    }  
  }
  else
  {
    script.Redirect("signIn.html"); 
  } 
}



// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"
