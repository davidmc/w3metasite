/*
  Copyright 2002 by D.K. McCombs.
*/

#include "openLogin.h"
#include "cgiTemplates.h"
#include "objectBase.h"
#include "pconnectstring"
#include "forms.h"

class userRoleObject: public objectBase
{
public:
  userRoleObject(openLogin &lin, cgiScript & script):objectBase(lin,script)
  {
    // add fields
    addField( "id", pgLongOid );
    addField( "user_id", pgLongOid );
    addField( "role_id", pgLongOid );
    addField( "enabled", pgBooleanOid );

    addField( "created", pgTimestampOid );
    addField( "created_by", pgLongOid );
    addField( "modified", pgTimestampOid );
    addField( "modified_by", pgLongOid );

    // add controls
    addControl( "id", staticEntry( "id", "10" ) );
    addControl( "user_id", staticEntry( "user_id", "10" ) );

    string sql = "select id, name from roles ";
    // restrict to the logins group
    if( !lin.SiteAdmin() )
    {
      sql = "select role_id, role_name from vw_role_services where group_id = ";
      sql += lin.GroupId();
    }
    addControl( "role_id", cmboEntry( "role_id",
                                       sql,
                                       "Select a role: " ) );

    addControl( "enabled", boolBox( "enabled" ) );
    addControl( "created", staticEntry( "created", "24" ) );
    addControl( "modified", staticEntry( "modified", "24" ) );

    // set name    file:/home/davidmc/development/openPermissions/objects/signIn.html
    setName("user_roles");
  }
  // must have no role_service_parameters on delete
  bool validate(void){ return true; }

  // will call cleanup after add, delete or change
  bool cleanup(void) { return true; }
};

void URForm(cgiScript & script, openLogin & login)
{
  userRoleObject obj(login,script);
  cgiTemplates htmlDoc;
  htmlDoc.load("childForm.html");
  ocString top(htmlDoc.getParagraph("top"));
  cgiInput & args = script.ClientArguments();

  script << top.replace( "_entity_", "Role  Parameter" )
               .replace( "__link__", "userRoleForm.cgi" ) << endl;
  obj.go();
  if( args.count("user_id") )
  {
    // even if no go() set the user id
    obj.getControl("user_id")->setContent(args["user_id"].str());
  }
  ocString listTemplate(htmlDoc.getParagraph("formitem"));

  script << listTemplate.replace ( "_label_", "Id" )
                        .replace ( "_form_item_", obj.getControl("id")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "User Id" )
                          .replace ( "_form_item_", obj.getControl("user_id")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Role Id" )
                          .replace ( "_form_item_", obj.getControl("role_id")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Enabled?" )
                          .replace ( "_form_item_", obj.getControl("enabled")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Date Created" )
                        .replace ( "_form_item_", obj.getControl("created")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Last Modified" )
                        .replace ( "_form_item_", obj.getControl("modified")->getHtml().c_str() );

  ocString bottom(htmlDoc.getParagraph("bottom"));
  script << bottom.replace( "_form_buttons_", formButtons(args).c_str() )
                  .replace( "__results__", obj.resultString().c_str() )
                  .replace( "__js__", "document.forms[0].role_id.focus();" ) << endl;
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
    URForm(script,login);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
