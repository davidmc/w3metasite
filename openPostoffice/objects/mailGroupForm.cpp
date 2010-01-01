/*
  Copyright 2002 by D.K. McCombs.
*/

#include "openLogin.h"
#include "cgiTemplates.h"
#include "objectBase.h"
#include "pconnectstring"
#include "forms.h"
#include "groupObject.h"

void mail_groupForm(cgiScript & script, openLogin & login)
{
  mail_groupObject obj(login,script);
  cgiTemplates htmlDoc;
  htmlDoc.load("adminForm.html");
  ocString top(htmlDoc.getParagraph("top"));
  cgiInput & args = script.ClientArguments();

  script << top.replace( "_entity_", "Mail Group" )
               .replace( "__link__", "mailGroupForm.cgi" ) << endl;
  obj.go();

  // Have to get the group from the login if an add and not admin
  if( obj.getControl("group_id")->getContent().length() == 0 &&
      login.SiteAdmin() == false  )
  {
    obj.getControl("group_id")->setContent( login.GroupId() );
  }

  ocString listTemplate(htmlDoc.getParagraph("formitem"));

  // lump id and group_id controls together
  string idctls = obj.getControl("id")->getHtml().c_str();

  if( login.SiteAdmin() == false  )
  {
    idctls += obj.getControl("group_id")->getHtml().c_str();
  }

  script << listTemplate.replace ( "_label_", "Id" )
                        .replace ( "_form_item_", idctls.c_str() );

  if( login.SiteAdmin() == true  )
  {
      script << listTemplate.replace ( "_label_", "Group" )
                        .replace ( "_form_item_", obj.getControl("group_id")->getHtml().c_str()  );
  }

  script << listTemplate.replace ( "_label_", "Name" )
                          .replace ( "_form_item_", obj.getControl("name")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Description" )
                          .replace ( "_form_item_", obj.getControl("description")->getHtml().c_str() );


  script << listTemplate.replace ( "_label_", "Date Created" )
                        .replace ( "_form_item_", obj.getControl("created")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Last Modified" )
                        .replace ( "_form_item_", obj.getControl("modified")->getHtml().c_str() );

  ocString bottom(htmlDoc.getParagraph("bottom"));
  script << bottom.replace( "_form_buttons_", formButtons(args).c_str() )
                  .replace( "_link_", "mailGroupList.cgi" )
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
    mail_groupForm(script,login);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
