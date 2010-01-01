/*
  Copyright 2002 by D.K. McCombs.
*/

#include "openLogin.h"
#include "cgiTemplates.h"
#include "objectBase.h"
#include "pconnectstring"
#include "forms.h"
#include "ocAdminProxy.h"

class globalDirObject: public objectBase
{
public:
  globalDirObject(openLogin &lin, cgiScript & script):objectBase(lin,script)
  {
    string sqlGroups, sqlMailGroups, sqlUsers;
    if( lin.SiteAdmin() )
    {
      sqlGroups = "select id, name from groups order by name";
      sqlMailGroups = "select mg.id, g.name || ' - ' || mg.name as name from mail_groups mg "
                      "inner join groups g on g.id = mg.group_id order by g.name, mg.name";
      sqlUsers = "select u.id , g.name || ' - ' || u.first || ' ' || u.last as name "
                 "from users u inner join groups g on g.id = u.group_id";

    }
    else
    {
      sqlMailGroups = "select id, name from mail_groups where group_id = ";
      sqlMailGroups += lin.GroupId();
      sqlMailGroups += " order by name";
      sqlUsers = "select id ,  first || ' ' || last as name "
                 "from users where group_id = ";
      sqlUsers += lin.GroupId();
    }
    // add fields
    addField( "id", pgLongOid );
    addField( "group_id", pgLongOid );
    addField( "mail_group_id", pgVarCharArrayOid );
    addField( "mailto_id", pgLongOid );
    addField( "ex_first", pgVarCharArrayOid );
    addField( "ex_last", pgVarCharArrayOid );
    addField( "ex_phone_number", pgVarCharArrayOid );
    addField( "ex_email", pgVarCharArrayOid );
    addField( "comments", pgVarCharArrayOid );
    addField( "created", pgTimestampOid );
    addField( "created_by", pgLongOid );
    addField( "modified", pgTimestampOid );
    addField( "modified_by", pgLongOid );

    // add controls
    addControl( "id", staticEntry( "id", "24" ) );
    if( lin.SiteAdmin() )
    {
      addControl( "group_id", cmboEntry( "group_id",
                                         sqlGroups,
                                         "Select a group: " ) );
    }
    else
    {
      addControl( "group_id",hiddenEntry( "group_id" ) );
    }
    addControl( "mail_group_id", cmboEntry( "mail_group_id",
                                  sqlMailGroups,
                                  "Select a mail group: " ) );
    addControl( "mailto_id", cmboEntry( "mailto_id",
                                         sqlUsers,
                                         "Select an employee: " ) );
    addControl( "ex_first", textEntry( "ex_first", "24" ) );
    addControl( "ex_last", textEntry( "ex_last", "24" ) );
    addControl( "ex_phone_number", textEntry( "ex_phone_number", "16" ) );
    addControl( "ex_email", emailEntry( "ex_email", "32" ) );
    addControl( "comments", areaEntry( "comments" ) );
    addControl( "created", staticEntry( "created", "24" ) );
    addControl( "modified", staticEntry( "modified", "24" ) );

    // set name
    setName("global_directory");
  }
  // must define
  bool validate(void)
  {
    cgiInput & args = m_script.ClientArguments();
    string group_id = args["group_id"].str();
    string mail_group_id = args["mail_group_id"].str();
    if( group_id == "-1" )
    {
      m_result = "No group selected! Please select a group";
      return false;
    }
    if( mail_group_id == "-1" )
    {
      m_result = "No mail group Selected! Please select a mail group";
      return false;
    }
    return true;
  }
  // cleanup to add, delete globalDir account, or change account password
  bool cleanup(void)
  {
    return true;
  }
};

void globalDirForm(cgiScript & script, openLogin & login)
{
  globalDirObject obj(login,script);
  cgiTemplates htmlDoc;
  htmlDoc.load("adminForm.html");
  ocString top(htmlDoc.getParagraph("top"));
  cgiInput & args = script.ClientArguments();

  script << top.replace( "_entity_", "Directory Entry" )
               .replace( "__link__", "globalDirForm.cgi" ) << endl;

  obj.go();

  // get the current id
  string id = obj.getControl("id")->getContent();

  // Have to get the group from the login if an add and not admin
  if( obj.getControl("group_id")->getContent().length() == 0 &&
      login.SiteAdmin() == false  )
  {
    obj.getControl("group_id")->setContent( login.GroupId() );
  }

  // get the form item template
  ocString listTemplate(htmlDoc.getParagraph("formitem"));

  // lump id and group_id controls together
  string idctls = obj.getControl("id")->getHtml().c_str();

  if( login.SiteAdmin() == false  )
  {
    idctls += obj.getControl("group_id")->getHtml().c_str();
  }

  script << listTemplate.replace ( "_label_", "Id" )
                        .replace ( "_form_item_", idctls.c_str() );
  if( login.SiteAdmin() )
  {
    script << listTemplate.replace ( "_label_", "Group" )
                          .replace ( "_form_item_", obj.getControl("group_id")->getHtml().c_str() );
  }

  script << listTemplate.replace ( "_label_", "Mail Group" )
                        .replace ( "_form_item_", obj.getControl("mail_group_id")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Mail To" )
                        .replace ( "_form_item_", obj.getControl("mailto_id")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Other First" )
                        .replace ( "_form_item_", obj.getControl("ex_first")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Other Last" )
                        .replace ( "_form_item_", obj.getControl("ex_last")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Other Phone Number" )
                        .replace ( "_form_item_", obj.getControl("ex_phone_number")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Other Email" )
                        .replace ( "_form_item_", obj.getControl("ex_email")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Comments" )
                        .replace ( "_form_item_", obj.getControl("comments")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Date Created" )
                        .replace ( "_form_item_", obj.getControl("created")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Last Modified" )
                        .replace ( "_form_item_", obj.getControl("modified")->getHtml().c_str() );
  // bottom
  ocString bottom(htmlDoc.getParagraph("bottom"));
  script << bottom.replace("_form_buttons_", formButtons(args).c_str())
                  .replace( "_link_", "globalDirList.cgi" )
                  .replace( "__results__", obj.resultString().c_str() )
                  .replace( "__js__", "document.forms[0].mail_group_id.focus();" ) << endl;

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
    globalDirForm(script,login);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
