/*
  Copyright 2002 by D.K. McCombs.
*/

#include "openLogin.h"
#include "cgiTemplates.h"
#include "objectBase.h"
#include "pconnectstring"
#include "forms.h"
#include "ocAdminProxy.h"

class groupObject: public objectBase
{
  string groupName;
public:
  groupObject(openLogin &lin, cgiScript & script):objectBase(lin,script)
  {
    // add fields
    addField( "id", pgLongOid );
    addField( "name", pgVarCharArrayOid );
    addField( "site_admin", pgBooleanOid );
    addField( "created", pgTimestampOid );
    addField( "created_by", pgLongOid );
    addField( "modified", pgTimestampOid );
    addField( "modified_by", pgLongOid );

    // add controls
    addControl( "id", staticEntry( "id", "24" ) );
    addControl( "name", textEntry( "name", "24" ) );
    addControl( "site_admin", boolBox( "site_admin" ) );
    addControl( "created", staticEntry( "created", "24" ) );
    addControl( "modified", staticEntry( "modified", "24" ) );

    // set name
    setName("groups");
  }
  bool validate(void)
  {
    return true;
  }
  bool cleanup(void)
  {
    ocAdminProxy admin;
    cgiInput & args = m_script.ClientArguments();
    m_result += " Action: ";
    m_result += m_action;
    if(m_action=="i")
    {
      string group = args["name"].str();
      return admin.addGroup(group);
    }
    else if(m_action=="d")
    {
      string group = args["name"].str();
      return admin.deleteGroup(group);
    }
    return true;
  }
};

void groupForm(cgiScript & script, openLogin & login)
{
  groupObject obj(login,script);
  obj.go();
  cgiTemplates htmlDoc;
  htmlDoc.load("adminForm.html");
  ocString top(htmlDoc.getParagraph("top"));
  cgiInput & args = script.ClientArguments();

  script << top.replace( "_entity_", "Group" )
               .replace( "__link__", "groupForm.cgi" ) << endl;

  ocString listTemplate(htmlDoc.getParagraph("formitem"));

  // get the current id
  string id = obj.getControl("id")->getContent();

  script << listTemplate.replace ( "_label_", "Id" )
                        .replace ( "_form_item_", obj.getControl("id")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Name" )
                        .replace ( "_form_item_", obj.getControl("name")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Website Admin" )
                        .replace ( "_form_item_", obj.getControl("site_admin")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Date Created" )
                        .replace ( "_form_item_", obj.getControl("created")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Last Modified" )
                        .replace ( "_form_item_", obj.getControl("modified")->getHtml().c_str() );


  // end service parameter list
  ocString bottom1(htmlDoc.getParagraph("altbottom1"));
  if( id.length() )
  {
    string newLink = "serviceParamForm.cgi?group_id=";
    newLink += id;
    script << bottom1.replace("_form_buttons_", formButtons(args).c_str())
                     .replace( "__more_instructions__", "Add or modify a service below:" );
    ocString childtop = htmlDoc.getParagraph("childtop");
    script << childtop.replace("__child__","Services")
                      .replace("_header_","Name")
                      .replace("_link_",newLink.c_str());
    string sql;
    postgresDB db(CONNECTSTRING);
    postgresRS rs(db);
    sql = "select sp.id, s.name from service_parameters sp "
          "inner join services s on s.id = sp.service_id where sp.group_id = ";
    sql+= id;
    sql += " order by s.name";
    for( bool b = rs.open(sql);
         b;
         b= rs.next() )
    {
      ocString listitem = htmlDoc.getParagraph("childitem");
      script <<  listitem.replaceAll("_id_",rs.getField(0).format().c_str())
                         .replace("_listdata_",rs.getField(1).format().c_str())
                         .replace("_link_","serviceParamForm.cgi");
    }
    script << htmlDoc.getParagraph("childbottom");
  }
  else
  {
    script << bottom1.replace("_form_buttons_", formButtons(args).c_str()).replace( "__more_instructions__", " " );
  }
  ocString bottom2(htmlDoc.getParagraph("altbottom2"));
  script << bottom2.replace( "_link_", "groupList.cgi" )
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
    groupForm(script,login);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
