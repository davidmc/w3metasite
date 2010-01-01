/*
  admin.cpp
  ---------------
  source code for login verification
  And admin service listing.

  Copyright 2002
  Author David McCombs.

*/

#include "openLogin.h"
#include "cgiTemplates.h"
void  adminList(cgiScript & script, openLogin & login)
{
  cgiTemplates htmlDoc;
  htmlDoc.load("admin.html");
  script << htmlDoc.getParagraph("top") << endl;

  if( login.fetchServices() )
  {
    // add all of the services to the menu
    serviceMap & services = login.Services();
    serviceMap::iterator pos;
    for( pos=services.begin(); pos!=services.end(); ++pos )
    {

      openService & service = pos->second;
      script << ocString(htmlDoc.getParagraph("listitem"))
                .replace("_uri_",service.uri.c_str())
                .replace("_menu_name_",service.menu_name.c_str()) << endl;
    }
  }
  script << htmlDoc.getParagraph("bottom") << endl;
}

int main( int argcount, char ** args )
{
  bool isSignedOn = false;

  // An instance of the script must be created before any other output.
  cgiScript script("text/html",false);
  openLogin login;

  if ( (script.ClientArguments().count("login")==1) &&
       (script.ClientArguments().count("password")==1) )
  {
    aString & asLogin = script.ClientArguments()["login"];
    aString & asPassword = script.ClientArguments()["password"];
    isSignedOn = login.checkUser( asLogin.str(), asPassword.str() );
  }
  else
  {
    // attempt to get cookie
    if ( login.testLoginStatus() )
    {
      isSignedOn = true;
    }
  }
  if( isSignedOn )
  {
    script.closeHeader();
    adminList(script,login);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
