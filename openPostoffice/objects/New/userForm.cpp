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
public:
  userObject(openLogin &lin, cgiScript & script):objectBase(lin,script)
  {
    string sqlGroups, sqlLocations;
    if( lin.SiteAdmin() )
    {
      sqlGroups = "select id, name from groups order by name";
      sqlLocations = "select l.id, g.name || ' - ' || l.name as name from locations l "
                     "inner join groups g on g.id = l.group_id order by g.name, l.name";

    }
    else
    {
      sqlLocations = "select id, name from locations where group_id = ";
      sqlLocations += lin.GroupId();
      sqlLocations += " order by name";
    }
    // add fields
    addField( "id", pgLongOid );
    addField( "group_id", pgLongOid );
    addField( "first", pgVarCharArrayOid );
    addField( "last", pgVarCharArrayOid );
    addField( "login", pgVarCharArrayOid );
    addField( "password", pgVarCharArrayOid );
    addField( "phone_number", pgVarCharArrayOid );
    addField( "email", pgVarCharArrayOid );
    addField( "location_id", pgLongOid );
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
    addControl( "first", textEntry( "first", "24" ) );
    addControl( "last", textEntry( "last", "24" ) );
    addControl( "login", textEntry( "login", "12" ) );
    addControl( "password", pswdEntry( "password", "12" ) );
    addControl( "phone_number", textEntry( "phone_number", "16" ) );
    addControl( "email", emailEntry( "email", "64", "32" ) );
    addControl( "location_id", cmboEntry( "location_id",
                                       sqlLocations,
                                       "Select a location: " ) );
    addControl( "created", staticEntry( "created", "24" ) );
    addControl( "modified", staticEntry( "modified", "24" ) );

    // set name
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
    ocAdminProxy admin;
    cgiInput & args = m_script.ClientArguments();
    // This is where we add/delete the user to/from the operating system
    // We should also check to change the user password here
    if(m_action=="i")
    {
      // Add the user account to the OS
      postgresDB      db(CONNECTSTRING);
      postgresRS      rs(db);

      string sql = "select name from groups where id = ";
      sql += args["group_id"].str();
      if( rs.open(sql) )
      {
        group = rs.getField(0).format();
        user = args["login"].str();
        string userdir = "/home/mailgroups/";
        userdir += group;
        userdir += "/";
        userdir +=  user;
        return admin.addUser( user,
                              args["password"].str(),
                              userdir, group );
        rs.close();
      }
      else
      {
        m_result += "unable to get group name from query to add user to OS: ";
        m_result += sql;
      }
    }
    else if(m_action=="d")
    {
      // Delete the user account from the OS
      string user = args["login"].str();
      return admin.deleteUser( user );
    }
    else if(m_action=="u")
    {
      // see if the password was changed
      if( m_changes.matchIn("password") )
      {
        // It has - so change the OS account password too
        string user = args["login"].str();
        return admin.changePassword( user, args["password"].str() );
      }
    }
    return true;
  }
};

void userForm(cgiScript & script, openLogin & login)
{
  userObject obj(login,script);
  cgiTemplates htmlDoc;
  htmlDoc.load("adminForm.html");
  ocString top(htmlDoc.getParagraph("top"));
  cgiInput & args = script.ClientArguments();

  script << top.replace( "_entity_", "User" )
               .replace( "__link__", "userForm.cgi" ) << endl;

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
  script << listTemplate.replace ( "_label_", "First" )
                        .replace ( "_form_item_", obj.getControl("first")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Last" )
                        .replace ( "_form_item_", obj.getControl("last")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Login" )
                        .replace ( "_form_item_", obj.getControl("login")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Password" )
                        .replace ( "_form_item_", obj.getControl("password")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Phone Number" )
                        .replace ( "_form_item_", obj.getControl("phone_number")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Email" )
                        .replace ( "_form_item_", obj.getControl("email")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "Location" )
                        .replace ( "_form_item_", obj.getControl("location_id")->getHtml().c_str() );

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
                   .replace( "__js__", "document.forms[0].first.focus();" ) << endl;

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
