/*
  Copyright 2003 by D.K. McCombs.
  // Numeric Integer
  FIELD_TYPE_SHORT,FIELD_TYPE_LONG,
  FIELD_TYPE_LONGLONG,FIELD_TYPE_INT24,
  // Numeric Float
  FIELD_TYPE_FLOAT,FIELD_TYPE_DOUBLE
  // Time and Date
  FIELD_TYPE_TIMESTAMP,FIELD_TYPE_DATE,FIELD_TYPE_TIME,
  FIELD_TYPE_DATETIME,
  // String
  FIELD_TYPE_TINY_BLOB,FIELD_TYPE_MEDIUM_BLOB,
  FIELD_TYPE_LONG_BLOB,FIELD_TYPE_BLOB,
  FIELD_TYPE_VAR_STRING,FIELD_TYPE_STRING 
 */

#include "openLogin.h"
#include "cgiTemplates.h"
#include "objectBase.h"
#include "connectstring"
#include "forms.h"
#include "siteLimit.h"

class linkObject: public objectBase
{
  string siteLimitation,sql;
public:
  linkObject(openLogin &lin, cgiScript & script):objectBase(lin,script)
  {
    string siteLimitation = siteFocus(lin);
    // add fields
    addField( "id", FIELD_TYPE_LONGLONG );
    addField( "site_id", FIELD_TYPE_LONGLONG );
    addField( "name", FIELD_TYPE_STRING );
    addField( "url", FIELD_TYPE_STRING );
    addField( "target", FIELD_TYPE_STRING );   

    // add controls
    addControl( "id", staticEntry( "id", "10" ) );
    sql += "select id, name from sites";
    if( siteLimitation.length() != 0 )
    {
      sql += " where id ";
      sql += siteLimitation;
    }
    sql += " order by name";    
    addControl( "site_id", cmboEntry( "site_id",
                                      sql ) );
    addControl( "name", textEntry( "name", "64" ) );        
    addControl( "url", textEntry( "url", "64" ) );
    addControl( "target", textEntry( "target", "64" ) );
    
    // set name
    setName("links");
  }
  // must have no user roles on user delete
  bool validate(void){ return true; }

  // will use cleanup to add, delete user account, or change account password
  bool cleanup(void) { return true; }
};

void linkForm(cgiScript & script, openLogin & login)
{
  linkObject obj(login,script);
  cgiTemplates htmlDoc;
  htmlDoc.load("Templates/siteAdminForm.html");
  ocString top(htmlDoc.getParagraph("top"));
  cgiInput & args = script.ClientArguments();

  script << top.replace( "_entity_", "Link" )
               .replace( "_link_", "linkList.cgi" )
               .replace( "__link__", "linkForm.cgi" ) << endl;

  obj.go();

  ocString listTemplate(htmlDoc.getParagraph("formitem"));

  script << listTemplate.replace ( "_label_", "Id" )
                        .replace ( "_form_item_", obj.getControl("id")->getHtml().c_str() );
                        
  script << listTemplate.replace ( "_label_", "Site Id" )
                        .replace ( "_form_item_", obj.getControl("site_id")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Name" )
                          .replace ( "_form_item_", obj.getControl("name")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "URL" )
                          .replace ( "_form_item_", obj.getControl("url")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Window Target" )
                          .replace ( "_form_item_", obj.getControl("target")->getHtml().c_str() );
  
  ocString bottom(htmlDoc.getParagraph("bottom"));
  script << bottom.replace( "_form_buttons_", formButtons(args).c_str() )
                  .replace( "__results__", obj.resultString().c_str() )
                  .replace( "__js__", "document.forms[0].name.focus();" ) << endl;
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
    linkForm(script,login);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
