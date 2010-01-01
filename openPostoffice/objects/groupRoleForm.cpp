/*
  Copyright 2002 by D.K. McCombs.
*/

#include "openLogin.h"
#include "cgiTemplates.h"
#include "objectBase.h"
#include "pconnectstring"
#include "forms.h"

class group_roleObject: public objectBase
{
public:
  group_roleObject(openLogin &lin, cgiScript & script):objectBase(lin,script)
  {

    // add fields
    addField( "id", pgLongOid );
    addField( "group_id", pgLongOid );
    addField( "role_id", pgLongOid );

    addField( "created", pgTimestampOid );
    addField( "created_by", pgLongOid );
    addField( "modified", pgTimestampOid );
    addField( "modified_by", pgLongOid );

    // add controls
    addControl( "id", staticEntry( "id", "10" ) );

    addControl( "group_id", cmboEntry( "group_id",
                                      "select id, name from groups order by name",
                                      "Select a group: " ) );
    addControl( "role_id", cmboEntry( "role_id",
                                      "select id, name from roles order by name",
                                      "Select a role: " ) );

    addControl( "created", staticEntry( "created", "24" ) );
    addControl( "modified", staticEntry( "modified", "24" ) );

    // set name
    setName("group_roles");
  }
  // must have no user roles on user delete
  bool validate(void){ return true; }

  // will use cleanup to add, delete user account, or change account password
  bool cleanup(void) { return true; }
};

void group_roleForm(cgiScript & script, openLogin & login)
{
  group_roleObject obj(login,script);
  cgiTemplates htmlDoc;
  htmlDoc.load("adminForm.html");
  ocString top(htmlDoc.getParagraph("top"));
  cgiInput & args = script.ClientArguments();

  script << top.replace( "_entity_", "Group Role" )
               .replace( "__link__", "groupRoleForm.cgi" ) << endl;
  obj.go();

  ocString listTemplate(htmlDoc.getParagraph("formitem"));

  // lump id and group_id controls together
  string idctls = obj.getControl("id")->getHtml().c_str();

  script << listTemplate.replace ( "_label_", "Id" )
                        .replace ( "_form_item_", idctls.c_str() );

  script << listTemplate.replace ( "_label_", "Group" )
                    .replace ( "_form_item_", obj.getControl("group_id")->getHtml().c_str()  );

  script << listTemplate.replace ( "_label_", "Role" )
                    .replace ( "_form_item_", obj.getControl("role_id")->getHtml().c_str()  );

  script << listTemplate.replace ( "_label_", "Date Created" )
                        .replace ( "_form_item_", obj.getControl("created")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Last Modified" )
                        .replace ( "_form_item_", obj.getControl("modified")->getHtml().c_str() );

  ocString bottom(htmlDoc.getParagraph("bottom"));
  script << bottom.replace( "_form_buttons_", formButtons(args).c_str() )
                  .replace( "_link_", "groupRoleList.cgi" )
                  .replace( "__results__", obj.resultString().c_str() )
                  .replace( "__js__", "document.forms[0].group_id.focus();" ) << endl;
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
    group_roleForm(script,login);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
