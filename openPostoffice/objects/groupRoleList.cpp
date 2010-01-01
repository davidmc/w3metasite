/*

  Copyright 2002
  Author David McCombs.

*/

#include "openLogin.h"
#include "cgiTemplates.h"

void  group_roleList(cgiScript & script, openLogin & login)
{
  cgiTemplates htmlDoc;
  htmlDoc.load("adminList.html");
  script << ocString(htmlDoc.getParagraph("top"))
              .replace( "_entity_", "Group Role" )
              .replace( "_header_", "Group</th><th>Role" )
              .replace( "_link_", "groupRoleForm.cgi" )
          << endl;


  postgresDB db(CONNECTSTRING);
  postgresRS rs(db);
  string sql;
  sql = "select gr.id, g.name as group, r.name as role from group_roles gr "
        "inner join groups g on g.id = gr.group_id "
        "inner join roles r on r.id = gr.role_id";

  bool b = rs.open(sql);
  if( !b )
  script << rs.getErrors() << endl;
  for( ;
       b;
       b= rs.next() )
  {

    string item =  rs.getField(1).format().c_str();
    item += "</td><td>";
    item += rs.getField(2).format().c_str();
    script << ocString(htmlDoc.getParagraph("listitem"))
    .replace( "_link_", "groupRoleForm.cgi" )
    .replaceAll( "_id_", rs.getField(0).format().c_str() )
    .replace( "_listdata_", item.c_str() ) << endl;

  }

  script << htmlDoc.getParagraph("bottom") << endl;
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
     group_roleList(script,login);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
