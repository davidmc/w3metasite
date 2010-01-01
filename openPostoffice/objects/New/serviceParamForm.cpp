/*
  Copyright 2002 by D.K. McCombs.
*/

#include "openLogin.h"
#include "cgiTemplates.h"
#include "objectBase.h"
#include "pconnectstring"
#include "forms.h"

class servicePObject: public objectBase
{
public:
  servicePObject(openLogin &lin, cgiScript & script):objectBase(lin,script)
  {
    // add fields
    addField( "id", pgLongOid );
    addField( "group_id", pgLongOid );
    addField( "service_id", pgLongOid );
    addField( "sequence", pgShortOid );
    addField( "xml_param1", pgVarCharArrayOid );
    addField( "xml_param2", pgVarCharArrayOid );
    addField( "service_items", pgVarCharArrayOid );

    addField( "created", pgTimestampOid );
    addField( "created_by", pgLongOid );
    addField( "modified", pgTimestampOid );
    addField( "modified_by", pgLongOid );

    // add controls
    addControl( "id", staticEntry( "id", "10" ) );
    addControl( "group_id", staticEntry( "group_id", "10" ) );
    addControl( "service_id", cmboEntry( "service_id",
                                         "select id, name from services",
                                         "Select a service: " ) );
    addControl( "sequence", textEntry( "sequence", "10" ) );
    addControl( "xml_param1", areaEntry( "xml_param1" ) );
    addControl( "xml_param2", areaEntry( "xml_param2" ) );
    addControl( "service_items", textEntry( "service_items", "128", "64" ) );

    addControl( "created", staticEntry( "created", "24" ) );
    addControl( "modified", staticEntry( "modified", "24" ) );

    // set name    file:/home/davidmc/development/openPermissions/objects/signIn.html
    setName("service_parameters");
  }
  // must have no role_service_parameters on delete
  bool validate(void){ return true; }

  // will call cleanup after add, delete or change
  bool cleanup(void) { return true; }
};

void servicePForm(cgiScript & script, openLogin & login)
{
  servicePObject obj(login,script);
  cgiTemplates htmlDoc;
  htmlDoc.load("childForm.html");
  ocString top(htmlDoc.getParagraph("top"));
  cgiInput & args = script.ClientArguments();

  script << top.replace( "_entity_", "Service Parameter" )
               .replace( "__link__", "serviceParamForm.cgi" ) << endl;

  obj.go();

  // even if no go() set the group id
  if(args.count("group_id"))
  {
    obj.getControl("group_id")->setContent(args["group_id"].str());
  }
  ocString listTemplate(htmlDoc.getParagraph("formitem"));

  script << listTemplate.replace ( "_label_", "Id" )
                        .replace ( "_form_item_", obj.getControl("id")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Group Id" )
                          .replace ( "_form_item_", obj.getControl("group_id")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Service Id" )
                          .replace ( "_form_item_", obj.getControl("service_id")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Sequence" )
                          .replace ( "_form_item_", obj.getControl("sequence")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "XML Parameter 1" )
                          .replace ( "_form_item_", obj.getControl("xml_param1")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "XML Parameter 2" )
                          .replace ( "_form_item_", obj.getControl("xml_param2")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Service Items" )
                          .replace ( "_form_item_", obj.getControl("service_items")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Date Created" )
                        .replace ( "_form_item_", obj.getControl("created")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Last Modified" )
                        .replace ( "_form_item_", obj.getControl("modified")->getHtml().c_str() );

  ocString bottom(htmlDoc.getParagraph("bottom"));
  script << bottom.replace( "_form_buttons_", formButtons(args).c_str() )
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
    servicePForm(script,login);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
