/*
  Copyright 2002 by D.K. McCombs.
*/

#include "openLogin.h"
#include "cgiTemplates.h"
#include "objectBase.h"
#include "pconnectstring"
#include "forms.h"
#include "groupObject.h"

class locationObject: public objectBase
{
  bool hasGroup;
public:
  locationObject(openLogin &lin, cgiScript & script):objectBase(lin,script)
  {

    // add fields
    addField( "id", pgLongOid );
    addField( "group_id", pgLongOid );
    addField( "name", pgVarCharArrayOid );
    addField( "description", pgVarCharArrayOid );

    addField( "created", pgTimestampOid );
    addField( "created_by", pgLongOid );
    addField( "modified", pgTimestampOid );
    addField( "modified_by", pgLongOid );

    // add controls
    addControl( "id", staticEntry( "id", "10" ) );

    if( lin.SiteAdmin() )
    {
      addControl( "group_id", cmboEntry( "group_id",
                                         "select id, name from groups order by name",
                                         "Select a group: " ) );
    }
    else
    {
      addControl( "group_id", hiddenEntry( "group_id" ) );
    }
    addControl( "name", textEntry( "name", "32" ) );
    addControl( "description", areaEntry( "description" ) );

    addControl( "created", staticEntry( "created", "24" ) );
    addControl( "modified", staticEntry( "modified", "24" ) );

    // set name
    setName("locations");
  }

  // must have no user roles on user delete
  bool validate(void)
  {
    hasGroup = true;
    if( m_action == "i" )
    {
      string sqlCheck = "select count(*) from mail_groups where name = '";
      sqlCheck += m_script.ClientArguments()["name"].str();
      sqlCheck += "'";
      // m_script << sqlCheck;
      // check and see if there is already a group by this name
      if( m_rs.open(sqlCheck) )
      {
        if(  m_rs.getField(0).format() == "0" )
        {
          // no group by this name
          hasGroup = false;
        }
      }
      m_rs.close();
    }
    return true;
  }

  // will use cleanup to do misc stuff
  bool cleanup(void)
  {

    // add a group too if there is no group by this name.
    if( m_action == "i" && hasGroup == false )
    {
      mail_groupObject obj(m_login,m_script);
      obj.go();
      // m_script << obj.resultString();
    }

    return true;
  }
};

void locationForm(cgiScript & script, openLogin & login)
{
  locationObject obj(login,script);
  cgiTemplates htmlDoc;
  htmlDoc.load("adminForm.html");
  ocString top(htmlDoc.getParagraph("top"));
  cgiInput & args = script.ClientArguments();

  script << top.replace( "_entity_", "Location" )
               .replace( "__link__", "locationForm.cgi" ) << endl;
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
                  .replace( "_link_", "locationList.cgi" )
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
    locationForm(script,login);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
