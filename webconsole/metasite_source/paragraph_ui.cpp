/*
  Copyright 2004 by D.K. McCombs.
  =============================== 
  paragraph_ui.cpp 
  
  webpage that edits paragraphs in html format
     
*/

#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "paragraph.hpp"
#include "forms.h"
#include "richForm.hpp"
#include "list_base.hpp"
#include "cgiTemplates.h"
#include "openLogin.h"
#include "siteLimit.h"
#include "site.hpp"

using namespace std;
openLogin oLogin;



class paragraph_form:  public paragraph, public richForm
{
public: 
  paragraph_form(cgiScript & script):paragraph(),richForm(script){setKey(*this);} 
  virtual ~paragraph_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  
  void form_data_transfer( void )
  {
    // Make data in controls back to object
    llongFXfer( "site_id", site_id );
    llongFXfer( "page_id", page_id );
    intFXfer( "place_order", place_order );
    stringFXfer( "template_tag", template_tag); 
    stringFXfer( "replace_tag", replace_tag);         
    stringFXfer( "name", name);    
    stringFXfer( "content", content  ); 
    
    // get section from caller query string
    stringFXfer( "section", section );    
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // Implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    ocString str_page_id;
    str_page_id.append(page_id);
    string templateFile = templateFromPageId(str_page_id.c_str());
    ocString sql = "select id, name from metasite.sites ";
    sql += "where id ";
    sql += siteFocus(oLogin);

    script << makeTop("paragraph_ui.cgi", "Paragraph Content");
    
    script << formTemplate.getParagraph("advanced_begin")
           << makeHiddenBox( "section", section )    
           << makeStaticBox("Id", "id", id, "12")
           << makeComboBox("Site", "site_id", site_id, sql.c_str() );
    script << "<br class='clearall'>" << endl;
    
    // relaxed restriction allows copies and moves.                  
    sql = "select id, name from metasite.pages where site_id " + siteFocus(oLogin);
    script << makeComboBox("Page", "page_id", page_id, sql.c_str() );     
    script << makeTextBox("Place Order","place_order",place_order,"8");
    script << "<br class='clearall'>" << endl
           << makeMetaBox("Template Tag","template_tag",template_tag,"50",
                          templateFile,"content items" )
           << "<br class='clearall'>" << endl
           << makeTextBox("Replacement Tag","replace_tag",replace_tag,"50")
           << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");                                                   
    script << makeTextBox("Name","name",name,"64")
           << "<br class='clearall'>" << endl
           << makeRichEntry( "Content","content", content )
           << makeButtons( key() );
           
    script << makeBottom( m_result ) << endl;
    return breturn;
  }
  
  bool ivalidate( void  ) 
  { 
    if(paragraph::ivalidate())
    {
      content =htmlDecode( content );
      return true; 
    }
    return false;
  }
  bool uvalidate( changeMap & changes  ) 
  { 
    if(paragraph::uvalidate(changes))
    {
      content =htmlDecode( content );
      return true;  
    }
    return false; 
  }
  
  bool ssupplemental( void )
  { 
    bool bret = paragraph::ssupplemental();
    content = htmlFixup( content );
    return bret;   
  }  
};

int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  paragraph_form myFrm(script); 

  
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiInput & args = script.ClientArguments();
    myFrm.template_tag = args["template_tag"].c_str();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/editorPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
            .replaceAll("$heading$","SITES");

    myFrm.loadControlTemplates("Templates/childdivform.htmp");  
    myFrm.form_action();  
    myFrm.form_display();
    
    // make contentControl.js aware of relativePath so we can to base relative pathing
    string relativePath;     
    if( myFrm.site_id > 0 )
    {
      siteObject site;
      site.key( myFrm.site_id );
      if( site.get_data() )
      {
        relativePath = "relativePath=\"";
        relativePath += site.url;
        relativePath += "\";";
      }
    }
    ocString bottom = pgTemplate.getParagraph("bottom");
    script << bottom.replace("/*_extra_js_*/",relativePath);
  }
  else
  {
    script.Redirect("signIn.html"); 
  } 
}

// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"
