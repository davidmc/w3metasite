/*
  Copyright 2002 by D.K. McCombs.
*/

#include "openLogin.h"
#include "cgiTemplates.h"
#include "objectBase.h"
#include "pconnectstring"
#include "forms.h"
#include "ocAdminProxy.h"

class userObject: public objectBase
{
  string group, user;
  bool can_replicate;
  string mail_group_id;
  string location_name;
  string group_id;
public:
  userObject(openLogin &lin, cgiScript & script):objectBase(lin,script)
  {

    // add fields
    addField( "id", pgLongOid );
    addField( "first", pgVarCharArrayOid );
    addField( "last", pgVarCharArrayOid );
    addField( "login", pgVarCharArrayOid );
    addField( "password", pgVarCharArrayOid );

    addField( "modified", pgTimestampOid );
    addField( "modified_by", pgLongOid );

    // add controls
    addControl( "id", staticEntry( "id", "24" ) );
    addControl( "first", staticEntry( "first", "24" ) );
    addControl( "last", staticEntry( "last", "24" ) );
    addControl( "login", staticEntry( "login", "36", "24" ) );
    addControl( "password", pswdEntry( "password", "12" ) );
    addControl( "password2", pswdEntry( "password", "12" ) );
    addControl( "phone_number", textEntry( "phone_number", "16" ) );
    addControl( "created", staticEntry( "created", "24" ) );
    addControl( "modified", staticEntry( "modified", "24" ) );

    // set name (must be users)
    setName("users");
  }

  // must have no user roles on user delete
  bool validate(void)
  {
    return true;
  }

  // will use cleanup to add, delete user account, or change account password
  bool cleanup(void)
  {
    bool bRet = true;
    return bRet;
  }
};

void userForm(cgiScript & script, openLogin & login)
{
  userObject obj(login,script);
  cgiTemplates htmlDoc;
  htmlDoc.load("adminForm.html");
  ocString top(htmlDoc.getParagraph("top"));
  cgiInput & args = script.ClientArguments();

  script << top.replace( "_entity_", "Password" )
               .replace( "__link__", "changePassword.cgi" ) << endl;

  obj.go();


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

  script << listTemplate.replace ( "_label_", "First" )
                        .replace ( "_form_item_", obj.getControl("first")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Last" )
                        .replace ( "_form_item_", obj.getControl("last")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Login" )
                        .replace ( "_form_item_", obj.getControl("login")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Password" )
                        .replace ( "_form_item_", obj.getControl("password")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Password 2" )
                        .replace ( "_form_item_", obj.getControl("password2")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Date Created" )
                        .replace ( "_form_item_", obj.getControl("created")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Last Modified" )
                        .replace ( "_form_item_", obj.getControl("modified")->getHtml().c_str() );
  // use alternate bottom
  ocString bottom1(htmlDoc.getParagraph("altbottom1"));

  if( id.length() )
  {
    string newLink = "userRoleForm.cgi?user_id=";
    newLink += id;
    script << bottom1.replace("_form_buttons_", formButtons(args).c_str())
                     .replace( "__more_instructions__", "Add or modify a role below:" );
    ocString childtop = htmlDoc.getParagraph("childtop");
    script << childtop.replace("__child__","Roles")
                      .replace("_header_","Name")
                      .replace("_link_",newLink.c_str());
    string sql;
    postgresDB db(CONNECTSTRING);
    postgresRS rs(db);
    // select ur.id, ur.user_id, r.name from user_roles ur inner join roles r on r.id = ur.role_id;
    sql = "select ur.id, r.name from user_roles ur "
          "inner join roles r on r.id = ur.role_id where ur.user_id = ";
    sql+= id;
    sql += " order by r.name";
    // script << "<!-- " << sql << "-->" << endl;
    for( bool b = rs.open(sql);
         b;
         b= rs.next() )
    {
      ocString listitem = htmlDoc.getParagraph("childitem");
      script <<  listitem.replaceAll("_id_",rs.getField(0).format().c_str())
                         .replace("_listdata_",rs.getField(1).format().c_str())
                         .replace("_link_","userRoleForm.cgi");
    }
    script << htmlDoc.getParagraph("childbottom");
  }
  else
  {
    script << bottom1.replace("_form_buttons_", formButtons(args).c_str()).replace( "__more_instructions__", " " );
  }
  ocString bottom2(htmlDoc.getParagraph("altbottom2"));
  script << bottom2.replace( "_link_", "userList.cgi" )
                   .replace( "__results__", obj.resultString().c_str() )
                   .replace( "__js__", "document.forms[0].password.focus();" ) << endl;

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
    userForm(script,login);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
