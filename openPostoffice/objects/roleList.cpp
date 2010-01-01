/*

  Copyright 2002
  Author David McCombs.

*/

#include "openLogin.h"
#include "cgiTemplates.h"

void  roleList(cgiScript & script, openLogin & login)
{
  cgiTemplates htmlDoc;
  htmlDoc.load("adminList.html");
  script << ocString(htmlDoc.getParagraph("top"))
            .replace( "_entity_", "Role" )
            .replace( "_header_", "Name" )
            .replace( "_link_", "roleForm.cgi" )
         << endl;

  postgresDB db(CONNECTSTRING);
  postgresRS rs(db);

  for( bool b = rs.open("select id, name from roles" );
       b;
       b= rs.next() )
  {
    script << ocString(htmlDoc.getParagraph("listitem"))
    .replace( "_link_", "roleForm.cgi" )
    .replaceAll( "_id_", rs.getField(0).format().c_str() )
    .replace( "_listdata_", rs.getField(1).format().c_str() ) << endl;
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
     roleList(script,login);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
