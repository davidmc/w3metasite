/*
  contentEditor.cpp

  User Interface Definition and Implementation 
  for paragraphs.
  
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
#include "paragraphs.hpp"
#include "openLogin.h" 

using namespace std;
openLogin oLogin;

class paragraphs_form:  public paragraphs_Obj, public forms_base
{
public: 
  paragraphs_form(cgiScript & script):paragraphs_Obj(),forms_base(script)
  {
    setKey(*this);
  } 
  virtual ~paragraphs_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "site_id", site_id);
     llongFXfer( "page_id", page_id);
     intFXfer( "place_order", place_order);
     stringFXfer( "template_tag", template_tag);
     stringFXfer( "replace_tag", replace_tag);
     stringFXfer( "name", name);
     stringFXfer( "content", content);
     llongFXfer( "author", author);
     dateFXfer( "time_authored", time_authored);
     dateFXfer( "time_start", time_start);
     dateFXfer( "time_end", time_end);
     boolFXfer( "approved", approved);
     stringFXfer( "section", section);
   } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  { 
    bool breturn = true;    
    ocString sql;
    script << makeTop("contentEditor.cgi", "paragraphs")
           << formTemplate.getParagraph("advanced_begin");

   // ADVANCED CONTENT
        
    script << makeStaticBox("id", "id", id ,"8");
    script << "<br class='clearall'>" << endl;
    
    // automatic read only except if site-admin
    sql = "select id, name from sites where site = ";
    sql.append(site_id);
    script << makeComboBox("site_id", "site_id", site_id ,sql);
    script << "<br class='clearall'>" << endl;
    
   // automatic read only except if site-admin
    sql = "select id, name from pages where site = ";
    sql.append(site_id);
    script << makeComboBox("page_id", "page_id", page_id ,sql);
    script << "<br class='clearall'>" << endl;

    // see business logic in object
    script << makeTextBox("place_order", "place_order", place_order ,"8","8");
    script << "<br class='clearall'>" << endl;
    
    // consider making this a string combo based on the section
    script << makeTextBox("template_tag", "template_tag", template_tag ,"125","35");
    script << "<br class='clearall'>" << endl;
    
    // auto set according to value of above and reading the page template
    script << makeTextBox("replace_tag", "replace_tag", replace_tag ,"125","35");
    script << "<br class='clearall'>" << endl;
    
    // automatic read only
    script << makeStaticBox("section", "section", section ,"125","35");
    script << "<br class='clearall'>" << endl;
    
    // automatic readonly based on login
    string authVal = oLogin.FullName();
    authVal += "=";
    authVal += oLogin.Id();
    script << makeManualComboBox("author", "author", author , authVal );
    script << "<br class='clearall'>" << endl;
    
    // automatic read-only based on date created
    script << makeTextBox("time_authored", "time_authored", time_authored ,"24","24");
    script << "<br class='clearall'>" << endl;

    // END ADVANCED CONTENT
    script << formTemplate.getParagraph("advanced_end");


    // name of the content, for calendars, slides faq's and such.
    script << makeTextBox("name", "name", name ,"125","35");
    script << "<br class='clearall'>" << endl;
    
    // this is xformed into a GUI by tinyMCE
    script << makeTextArea("content", "content", content ,"25","135");
    script << "<br class='clearall'>" << endl; 

    // begin of validity for this content (begins to show)
    script << makeTextBox("time_start", "time_start", time_start ,"24","24");
    script << "<br class='clearall'>" << endl; 

    // end of validity for this content (becomes 'archived')
    script << makeTextBox("time_end", "time_end", time_end ,"24","24");
    script << "<br class='clearall'>" << endl;

    // this should be only available to approver roles, defaults to false for non-approvers, true for approvers
    script << makeBoolBox("approved", "approved", approved );
    script << "<br class='clearall'>" << endl; 

    script << makeButtons( key() );        
    script << makeBottom( m_result ) << endl;     
    return breturn;
  }  
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  paragraphs_form myFrm(script); 
   
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;
    
    pgTemplate.load("Templates/editingPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top")).replaceAll("$heading$","W<sup>3</sup> content Editor");

    // includes the essential scripts and styles for editing
    myFrm.loadControlTemplates("Templates/editingForm.htmp");
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
