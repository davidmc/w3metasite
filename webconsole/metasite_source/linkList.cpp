/*

  Copyright 2002
  Author David McCombs.

*/

#include "openLogin.h"
#include "cgiTemplates.h"
#include "siteLimit.h"

void  linkList(cgiScript & script, openLogin & login)
{
  cgiTemplates htmlDoc;
  htmlDoc.load("Templates/siteAdminList.html");
  script << ocString(htmlDoc.getParagraph("top"))
            .replace( "_entity_", "Link" )
            .replace( "_header_", "Name</th><th>URL" )
            .replace( "_link_", "linkForm.cgi" )
         << endl;
         
  mySqlDB db(CONNECTSTRING);
  mySqlRS rs(db);
  string siteLimitation = siteFocus(login);
  string sql = "select id, name, url from links";
  if( siteLimitation.length() )
  {
    sql += " where site_id ";
    sql += siteLimitation;
  }
  for( bool b = rs.open(sql);
       b;
       b= rs.next() )
  {
    string vis = rs.getField(1).format() + 
                 "</td><td>" + 
                 rs.getField(2).format();
    script << ocString(htmlDoc.getParagraph("listitem"))
    .replace( "_link_", "linkForm.cgi" )
    .replaceAll( "_id_", rs.getField(0).format().c_str() )
    .replace( "_listdata_", vis.c_str() ) << endl;
  }

  script <<  htmlDoc.getParagraph("bottom")  << endl;
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
     linkList(script,login);
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
