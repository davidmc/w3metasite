/*

  Copyright 2002
  Author David McCombs.

*/

#include "openLogin.h"
#include "cgiTemplates.h"

void  group_siteList(cgiScript & script, openLogin & login)
{
  cgiTemplates htmlDoc;
  htmlDoc.load("Templates/adminList.html");
  script << ocString(htmlDoc.getParagraph("top"))
              .replace( "_entity_", "Group Site" )
              .replace( "_header_", "Group</th><th>Site" )
              .replace( "_link_", "group_siteForm.cgi" )
          << endl;


  mySqlDB db(CONNECTSTRING);
  mySqlRS rs(db);
  string sql;
  sql = "select gs.id, g.name, s.name from group_sites gs "
        "inner join groups g on g.id = gs.group_id "
        "inner join sites s on s.id = gs.site_id";

  bool b = rs.open(sql);
  if( !b )
  {
    script << " no group site found <!--";
    script << db.errorString();
    script << "-->" << endl;
  }

  for( ;
       b;
       b= rs.next() )
  {
    string item =  rs.getField(1).format().c_str();
    item += "</td><td>";
    item += rs.getField(2).format().c_str();
    script << ocString(htmlDoc.getParagraph("listitem"))
    .replace( "_link_", "group_siteForm.cgi" )
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
     group_siteList(script,login);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
