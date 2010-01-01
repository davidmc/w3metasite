/*
  sysadmin.cpp
  ---------------
  source code for login verification
  Sys Admin Sign In.

  Copyright 2002
  Author David McCombs.
*/

#include "connectstring"
#include "openLogin.h"
#include "cgiTemplates.h"

int main( int argcount, char ** args )
{
  bool isSignedOn = false;
  bool isSigningOn = false;

  // An instance of the script must be created before any other output.
  cgiScript script("text/html",false);
  openLogin login;

  if ( (script.ClientArguments().count("login")==1) &&
       (script.ClientArguments().count("password")==1) )
  {
    isSigningOn = true;
    aString & asLogin = script.ClientArguments()["login"];
    aString & asPassword = script.ClientArguments()["password"];
    isSignedOn = login.checkUser( asLogin.str(), asPassword.str() );
  }
  else if( argcount == 3 )
  {
    string sLogin = args[1];
    string sPassword = args[2];
    isSignedOn = login.checkUser( sLogin.c_str(), sPassword.c_str() );
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
    cgiTemplates tmplts;
    tmplts.load("./Templates/main.htmp"); 
    char * onloadbody = "<body onload='remenu()'>";
    ocString content = tmplts.getParagraph("all");
    content = content.replace("Booger", login.First().c_str());
    if(isSigningOn) content = content.replace( "<body>", onloadbody );        
    script << content;
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
#include "read_write_base.cpp"
