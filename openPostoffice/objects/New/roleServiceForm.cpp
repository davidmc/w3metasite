/*
  Copyright 2002 by D.K. McCombs.
*/

#include "openLogin.h"
#include "cgiTemplates.h"
#include "objectBase.h"
#include "pconnectstring"
#include "forms.h"

class roleServiceObject: public objectBase
{
public:
  roleServiceObject(openLogin &lin, cgiScript & script):objectBase(lin,script)
  {
    // add fields
    addField( "id", pgLongOid );
    addField( "role_id", pgLongOid );
    addField( "service_id", pgLongOid );
    addField( "enabled", pgBooleanOid );

    addField( "created", pgTimestampOid );
    addField( "created_by", pgLongOid );
    addField( "modified", pgTimestampOid );
    addField( "modified_by", pgLongOid );

    // add controls
    addControl( "id", staticEntry( "id", "10" ) );
    addControl( "role_id", staticEntry( "role_id", "10" ) );

    addControl( "service_id", cmboEntry( "service_id",
                                         "select id, name from services",
                                         "Select a service: " ) );


    addControl( "enabled", boolBox( "enabled" ) );
    addControl( "created", staticEntry( "created", "24" ) );
    addControl( "modified", staticEntry( "modified", "24" ) );

    // set name    file:/home/davidmc/development/openPermissions/objects/signIn.html
    setName("role_services");
  }
  // must have no role_service_parameters on delete
  bool validate(void){ return true; }

  // will call cleanup after add, delete or change
  bool cleanup(void) { return true; }
};

void RSForm(cgiScript & script, openLogin & login)
{
  roleServiceObject obj(login,script);
  cgiTemplates htmlDoc;
  htmlDoc.load("childForm.html");
  ocString top(htmlDoc.getParagraph("top"));
  cgiInput & args = script.ClientArguments();

  script << top.replace( "_entity_", "Role Service Parameter" )
               .replace( "__link__", "roleServiceForm.cgi" ) << endl;
  obj.go();
  if( args.count("role_id") )
  {
    // even if no go() set the user id
    obj.getControl("role_id")->setContent(args["role_id"].str());
  }
  ocString listTemplate(htmlDoc.getParagraph("formitem"));

  script << listTemplate.replace ( "_label_", "Id" )
                        .replace ( "_form_item_", obj.getControl("id")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Role Id" )
                          .replace ( "_form_item_", obj.getControl("role_id")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Service" )
                          .replace ( "_form_item_", obj.getControl("service_id")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Enabled?" )
                          .replace ( "_form_item_", obj.getControl("enabled")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Date Created" )
                        .replace ( "_form_item_", obj.getControl("created")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Last Modified" )
                        .replace ( "_form_item_", obj.getControl("modified")->getHtml().c_str() );

  ocString bottom(htmlDoc.getParagraph("bottom"));
  script << bottom.replace( "_form_buttons_", formButtons(args).c_str() )
                  .replace( "__results__", obj.resultString().c_str() )
                  .replace( "__js__", "document.forms[0].service_id.focus();" ) << endl;
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
    RSForm(script,login);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
