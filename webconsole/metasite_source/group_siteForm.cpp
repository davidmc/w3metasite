/*
  Copyright 2002 by D.K. McCombs.
*/

#include "openLogin.h"
#include "cgiTemplates.h"
#include "objectBase.h"
#include "connectstring"
#include "forms.h"

class group_siteObject: public objectBase
{
public:
  group_siteObject(openLogin &lin, cgiScript & script):objectBase(lin,script)
  {    
    // add fields
    addField( "id", FIELD_TYPE_LONGLONG );
    addField( "group_id", FIELD_TYPE_LONGLONG );
    addField( "site_id", FIELD_TYPE_LONGLONG );

    // add controls
    addControl( "id", staticEntry( "id", "10" ) );

    addControl( "group_id", 
                cmboEntry( "group_id",
                           "select id, name from groups order by name",
                           "Select a group: " ) );
                                      
    addControl( "site_id", 
                cmboEntry( "site_id",
                           "select id, name from sites order by name",
                           "Select a site: " ) );
    // set name
    setName("group_sites");
  }
  // must have no user roles on user delete
  bool validate(void){ return true; }

  // will use cleanup to add, delete user account, or change account password
  bool cleanup(void) { return true; }
};

void group_siteForm(cgiScript & script, openLogin & login)
{
  group_siteObject obj(login,script);
  cgiTemplates htmlDoc;
  htmlDoc.load("Templates/adminForm.html");
  ocString top(htmlDoc.getParagraph("top"));
  cgiInput & args = script.ClientArguments();

  script << top.replace( "_entity_", "Group Site" )
               .replace( "_link_", "group_siteList.cgi" )
               .replace( "__link__", "group_siteForm.cgi" ) 
         << endl;
  obj.go();
  
  ocString listTemplate(htmlDoc.getParagraph("formitem"));

  // lump id and group_id controls together
  string idctls = obj.getControl("id")->getHtml().c_str();

  script << listTemplate.replace ( "_label_", "Id" )
                        .replace ( "_form_item_", idctls.c_str() );

  script << listTemplate.replace ( "_label_", "Group" )
                        .replace ( "_form_item_", 
                                   obj.getControl("group_id")->
                                         getHtml().c_str()  );

  script << listTemplate.replace ( "_label_", "Site" )
                        .replace ( "_form_item_", 
                                   obj.getControl("site_id")->
                                        getHtml().c_str()  );


  ocString bottom(htmlDoc.getParagraph("bottom"));
  script << bottom.replace( "_form_buttons_", 
                            formButtons(args).c_str() )
                  .replace( "__results__", 
                            obj.resultString().c_str() )
                  .replace( "__js__", 
                            "document.forms[0].group_id.focus();" ) 
         << endl;
}

int main( int argcount, char ** args )
{
  bool isSignedOn = false;

  // An instance of the script
  // must be created before any other output.
  cgiScript script("text/html",false);
  openLogin login;

  // attempt to get cookie
  if ( login.testLoginStatus() )
  {
    isSignedOn = true;
  }
  if( isSignedOn )
  {
    script.closeHeader();
    group_siteForm(script,login);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
