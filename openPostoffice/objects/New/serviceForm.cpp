/*
  Copyright 2002 by D.K. McCombs.
*/

#include "openLogin.h"
#include "cgiTemplates.h"
#include "objectBase.h"
#include "pconnectstring"
#include "forms.h"

class serviceObject: public objectBase
{
public:
  serviceObject(openLogin &lin, cgiScript & script):objectBase(lin,script)
  {

    // add fields
    addField( "id", pgLongOid );
    addField( "sequence", pgLongOid );
    addField( "tree_depth", pgShortOid );
    addField( "enabled", pgBooleanOid );
    addField( "name", pgVarCharArrayOid );
    addField( "menu_name", pgVarCharArrayOid );
    addField( "uri", pgVarCharArrayOid );
    addField( "local_uri", pgBooleanOid );
    addField( "secure", pgBooleanOid );
    addField( "xml_params", pgVarCharArrayOid );

    addField( "created", pgTimestampOid );
    addField( "created_by", pgLongOid );
    addField( "modified", pgTimestampOid );
    addField( "modified_by", pgLongOid );

    // add controls
    addControl( "id", staticEntry( "id", "10" ) );                     
    addControl( "sequence", textEntry( "sequence", "10" ) );
    addControl( "tree_depth", textEntry( "tree_depth", "4" ) );
    addControl( "enabled", boolBox( "enabled" ) );
    addControl( "name", textEntry( "name", "32" ) );
    addControl( "menu_name", textEntry( "menu_name", "32" ) );
    addControl( "uri", textEntry( "uri", "64" ) );
    addControl( "local_uri", boolBox( "local_uri" ) );
    addControl( "secure", boolBox( "secure" ) );
    addControl( "xml_params", areaEntry( "xml_params" ) );

    addControl( "created", staticEntry( "created", "24" ) );
    addControl( "modified", staticEntry( "modified", "24" ) );

    // set name
    setName("services");
  }
  // must have no user roles on user delete
  bool validate(void){ return true; }

  // will use cleanup to add, delete user account, or change account password
  bool cleanup(void) { return true; }
};

void serviceForm(cgiScript & script, openLogin & login)
{
  serviceObject obj(login,script);
  cgiTemplates htmlDoc;
  htmlDoc.load("adminForm.html");
  ocString top(htmlDoc.getParagraph("top"));
  cgiInput & args = script.ClientArguments();

  script << top.replace( "_entity_", "Service" )
               .replace( "__link__", "serviceForm.cgi" ) << endl;

  obj.go();

  ocString listTemplate(htmlDoc.getParagraph("formitem"));

  script << listTemplate.replace ( "_label_", "Id" )
                        .replace ( "_form_item_", obj.getControl("id")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Sequence" )
                          .replace ( "_form_item_", obj.getControl("sequence")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Tree Depth" )
                          .replace ( "_form_item_", obj.getControl("tree_depth")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Enabled?" )
                          .replace ( "_form_item_", obj.getControl("enabled")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Name" )
                          .replace ( "_form_item_", obj.getControl("name")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Menu Name" )
                          .replace ( "_form_item_", obj.getControl("menu_name")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "URL" )
                          .replace ( "_form_item_", obj.getControl("uri")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Local?" )
                          .replace ( "_form_item_", obj.getControl("local_uri")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Secure?" )
                          .replace ( "_form_item_", obj.getControl("secure")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "XML Parameters" )
                          .replace ( "_form_item_", obj.getControl("xml_params")->getHtml().c_str() );


  script << listTemplate.replace ( "_label_", "Date Created" )
                        .replace ( "_form_item_", obj.getControl("created")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Last Modified" )
                        .replace ( "_form_item_", obj.getControl("modified")->getHtml().c_str() );

  ocString bottom(htmlDoc.getParagraph("bottom"));
  script << bottom.replace( "_form_buttons_", formButtons(args).c_str() )
                  .replace( "_link_", "serviceList.cgi" )
                  .replace( "__results__", obj.resultString().c_str() )
                  .replace( "__js__", "document.forms[0].sequence.focus();" ) << endl;
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
    serviceForm(script,login);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
