/*

  Copyright 2002
  Author David McCombs.

*/

#include "openLogin.h"
#include "cgiTemplates.h"

void  locationList(cgiScript & script, openLogin & login)
{
  cgiTemplates htmlDoc;
  htmlDoc.load("adminList.html");
  if( login.SiteAdmin() )
  {
    script << ocString(htmlDoc.getParagraph("top"))
              .replace( "_entity_", "Location" )
              .replace( "_header_", "Group</th><th>Location" )
              .replace( "_link_", "locationForm.cgi" )
          << endl;
  }
  else
  {
    script << ocString(htmlDoc.getParagraph("top"))
              .replace( "_entity_", "Location" )
              .replace( "_header_", "Name" )
              .replace( "_link_", "locationForm.cgi" )
          << endl;
  }
  postgresDB db(CONNECTSTRING);
  postgresRS rs(db);
  string sql;
  if( login.SiteAdmin() )
  {
    sql = "select l.id, g.name as group, l.name from locations l "
          "inner join groups g on g.id = l.group_id" ;
  }
  else
  {
    sql = "select id, name from locations where group_id = ";
    sql += login.GroupId();
  }
  bool b = rs.open(sql);
  if( !b )
  script << rs.getErrors() << endl;
  for( ;
       b;
       b= rs.next() )
  {
    if( login.SiteAdmin() )
    {
      string item =  rs.getField(1).format().c_str();
      item += "</td><td>";
      item += rs.getField(2).format().c_str();
      script << ocString(htmlDoc.getParagraph("listitem"))
      .replace( "_link_", "locationForm.cgi" )
      .replaceAll( "_id_", rs.getField(0).format().c_str() )
      .replace( "_listdata_", item.c_str() ) << endl;
    }
    else
    {
      script << ocString(htmlDoc.getParagraph("listitem"))
      .replace( "_link_", "locationForm.cgi" )
      .replaceAll( "_id_", rs.getField(0).format().c_str() )
      .replace( "_listdata_", rs.getField(1).format().c_str() ) << endl;
    }
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
     locationList(script,login);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
