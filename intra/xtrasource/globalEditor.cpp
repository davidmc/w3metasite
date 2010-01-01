/*

  globalEditor.cpp

  User Interface Definition and Implementation 
  for global_content.
  
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
#include "global_content.hpp"
#include "openLogin.h" 
#include "global_content_obj.hpp"
#include "siteLimit.h"

using namespace std;
openLogin oLogin;

class global_content_form:  public global_content_Obj, public forms_base
{
public: 
  global_content_form(cgiScript & script):global_content_Obj(),forms_base(script)
  {
    setKey(*this);
  } 
  virtual ~global_content_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "site_id", site_id);
    // if it's still 0, then get it from where we are
    if( site_id <= 0 )
    {
      site_id = atoll( currentSite().c_str());
    }
    stringFXfer( "name", name);
    stringFXfer( "content", content);
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    bool ret = false;
    if( mode == "s" && !key() )
    {
      // see if we can get the data from  search crit:  ex: 'name=top&page_id=7'
      ocString clause;
      clause =  "name='" + name + "' and site_id = ";
      clause += currentSite();
      ret = get_data(  clause );
    }
    else
    {
      ret = db_action( mode, changes );
    }
    return ret;

  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  { 
    bool breturn = true;    
    ocString sql;
    script << makeTop("globalEditor.cgi", "Global Content Editor")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("Id", "id", id ,"8");
    script << makeStaticBox("Site Id", "site_id",site_id ,"8");
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");
    script << makeTextBox("Section name", "name", name ,"125","35");
    script << "<br class='clearall'>" << endl; 
    script << makeTextArea( "<br class='clearall'>","content", content, "6","80" );
    script << "<div id='contentView'> &nbsp; </div> <br class='clearall'>" << endl;
    script << "<SCRIPT type=\"text/javascript\">doRichEdit = true;</SCRIPT>" << endl;
    script << makeButtons( key() );
    script << makeBottom( m_result ) << endl;
    return breturn;
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
    pgTemplate.load("Templates/editingPane.htmp");

    script << ocString(pgTemplate.getParagraph("top"))
                     .replaceAll("$heading$","Global Content");

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

