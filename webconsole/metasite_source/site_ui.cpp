/*
  Copyright 2002 by D.K. McCombs.
  ===============================
  
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
#include "list_base.hpp"
#include "cgiTemplates.h"
#include "openLogin.h"
#include "site.hpp"

using namespace std;
openLogin oLogin;

class global_content_List:  public list_base
{
public: 
  long long site_id;
  
  // Constructor
  global_content_List(cgiScript&sc, long long id):list_base(sc),site_id(id)
  {
    ;                   
  }  
  ~global_content_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","global_content_ui.cgi");
    hotCol=-2;    
    ocString sql= "select id, name, content "
        "from metasite.global_content "
          "where site_id = ";    
    sql.append(site_id);
    emitHeadings("Id|Name|Content"); 
    getData(sql);
    emitData();    
    emitEnd();
    return breturn;
  }
  void derived_complete_event( void )
  {  
    ocString loco = "global_content_ui.cgi";
    if( site_id)
    {
      loco += "?site_id=";
      loco.append(site_id);
    }
    ocString adder = listTemplate.getParagraph("list_new");
    webIO << adder.replace("$cols","2")
                  .replace("$location",loco.c_str())
                  .replace("$item","Item");
  } 
  
  // override this to get the group_id in the link
  virtual void sendHotField( int iField, ocString & td )
  {
    ocString linkId = rs.getField(0).format();
    if( site_id)
    {
      linkId += "&site_id=";
      linkId.append(site_id);
    }
    webIO <<  td.replace( "$data$",
                    editLink.replaceAll( "$key$", linkId.c_str() )
                            .replaceAll( "$col$", rs.getField(iField).format().c_str() ).c_str());
  }
};



class site_form:  public siteObject, public forms_base
{
public: 
  site_form(cgiScript & script):siteObject(),forms_base(script){setKey(*this);} 
  virtual ~site_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    // Make data in controls back to object
    stringFXfer( "name", name);    
    stringFXfer( "domain", domain );
    stringFXfer( "url", url );
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
    script << makeTop("site_ui.cgi", "SITES")
           << makeStaticBox("id", "id", id, "12")
           << makeTextBox("name","name",name,"64")
           << makeTextBox("domain","domain",domain,"64") 
           << makeTextBox("url","url",url,"64") 
           << makeTextBox("path","path",path,"64") 
           << makeButtons( key() );
    if ( id != 0 )
    {
      script << "<tr><td colspan='2'>";
      script << "<b>Site Global Content:</b><br>";
      global_content_List list(script,id);
      list.loadListTemplates("Templates/poplist.htmp");  
      list.list_display();
      script << "</td></tr>";
    }         
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  site_form myFrm(script); 

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
