/*

  Copyright 2002
  Author David McCombs.

*/
#include "openLogin.h"
#include "cgiTemplates.h"

void  userList(cgiScript & script, openLogin & login)
{
  cgiTemplates htmlDoc;
  htmlDoc.load("filteredList.html");
  script << ocString(htmlDoc.getParagraph("top"))
            .replace( "_entity_", "User" )
            .replaceAll( "_link_", "userList.cgi" )
            .replaceAll( "_flink_", "userForm.cgi" )
            .replace( "_header_", "First Name</th><th>Last Name</th><th>Login" )
         << endl;

  postgresDB db(CONNECTSTRING);
  postgresRS rs(db);
  string sql = "select id, first, last, login from users";
  if( script.ClientArguments().count("s") > 0 )
  {
    sql += " where lower(last) like '";
    sql += script.ClientArguments()["s"].str();
    sql += "%'";
  }
  if( !login.SiteAdmin() )
  {
    sql+=(script.ClientArguments().count("s") > 0 )?" and ":" where ";
    sql += " group_id = ";
    sql += login.GroupId();
  }

  sql += " order by last";
  bool b = rs.open(sql);
  if( !b )
  {
     script << "SQL: " << rs.getErrors();
  }
  for( ;
       b;
       b= rs.next() )
  {
    string listData = rs.getField(1).format();
    listData += "</td><td>";
    listData += rs.getField(2).format();
    listData += "</td><td>";
    listData += rs.getField(3).format();
    script << ocString(htmlDoc.getParagraph("listitem"))
    .replace( "_link_", "userForm.cgi" )
    .replaceAll( "_id_", rs.getField(0).format().c_str() )
    .replace( "_listdata_", listData.c_str() ) << endl;
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
     userList(script,login);
  }
  else
  {
    // script.Redirect("signIn.html");
    script.closeHeader();
    script << login.getLastError() << "<br>" << endl;
    script << login.getSql() << endl;
    return(0);
  }
}
