/*
  templates_Popup.cpp

  User Interface Definition and Popup Implementation for Pop up Dialogs with Data Lists 
  for templates.
  
  Copyright (C) 2007 by D.K. McCombs.
  W3 Systems Design

  needs:
  childPane.htmp
  childdivform.htmp
  list.htmp
  
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
#include "templates.hpp"
#include "openLogin.h" 
#include "flexButtons.hpp"
#include "siteLimit.h"

using namespace std;
openLogin oLogin;
 // Include file
#include "sublist_base.hpp"

// Definition

class templates_List:  public sublist_base
{
  public:
  // Constructor
    templates_List(cgiScript&sc):sublist_base(sc)
    {
      ;
    }
    ~templates_List(){;}
  
    bool list_display( void )
    {
      bool breturn = true;
      string sql =  "select "
          "id, "
          "name, "
          "path "
          "from templates where site_id = '" ;
      sql +=  currentSite();
      sql += "'";
      hotCol=-2;
      editLink = listTemplate.getParagraph("peerlink");
      editLink = editLink.replace("$prog$","templates_Popup.cgi");
      string heading = 
          "id| "
          "name| "
          "path ";
      emitHeadings(heading);
   
      getData( sql );
      emitData();
      ocString list_new = listTemplate.getParagraph("peernew");
      ocString url = "templates_Popup.cgi";
      webIO << list_new.replace("$location",url.c_str()).replace("$cols","3").replace("Item","templates");
      emitEnd();
      return breturn;
    }
};
class templates_form:  public templates_Obj, public forms_base
{
  public:
    templates_form(cgiScript & script):templates_Obj(),forms_base(script)
    {
      setKey(*this);
    }
    virtual ~templates_form(){;}
  
    void form_id_transfer( void )
    {
      llongFXfer( "id", id );
    }
    void form_data_transfer( void )
    {
      string currentsite = currentSite();
      site_id = atoll(currentsite.c_str());
      stringFXfer( "name", name);
      stringFXfer( "path", path);
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
      script << makeTop("templates_Popup.cgi", "Templates")
             << formTemplate.getParagraph("advanced_begin");
      script << makeStaticBox("id", "id", id ,"8")
             << makeStaticBox( "Site Id", "site_id", site_id, "12");
      script << "<br class='clearall'>" << endl;
      script << formTemplate.getParagraph("advanced_end");
                script << makeTextBox("name", "name", name ,"125","35");
      script << "<br class='clearall'>" << endl;

      //   script << makeTextBox("path", "path", path ,"125","35");
      if( path=="" )
      {
        script << ocString( makeTextBox("path", "path",path,"255","35")
                          ).replace("'text'","'file'");
      }
      else
      {
        string vfy = " &nbsp <a href=\"javascript:jopen('Templates/" + path +"')\">"+path+"</a>";
        script << ocString(makeTextBox("Path","path",path,"255","35")
                          ).replace("<!--eogc-->",vfy);
      }

      
                                 
      script << "<br class='clearall'>" << endl;
      script << flexButtons(  key() ,
                            " Insert Template ",
                            " Update Template ",
                            " Delete Template ",
                            "",  // reset new
                            "",  // reset keep
                            true,
                            true,
                            true,
                            false,
                            false  );

          script << makeBottom( m_result ) << endl;
          return breturn;
    }
};


int main( int argcount, char ** args )
{
  cgiScript script("text/html",false,"./Templates");
  templates_form myFrm(script);
   
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;
    pgTemplate.load("Templates/childPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
        .replaceAll("$heading$","templates");

    myFrm.loadControlTemplates("Templates/childdivform.htmp");
    myFrm.form_action();
    myFrm.form_display();

    script << "<br class='clearall'>" << endl;
    
    templates_List templatesList(script);
    templatesList.loadListTemplates("Templates/list.htmp");
    script << "<div class='heading'>Template List:</div>" << endl;
    templatesList.list_display();

    ocString end = pgTemplate.getParagraph("bottom");
    script << end;
  }
  else
  {
    script << "<html><body>"
           << "<a href='javascript:window.close()'><h1>You are not logged in</h1></a>"
           << "</body></html>" << endl;
  }
}
// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"


