/*
  Copyright (c) 2004-2008 W3 Sys.
  filePick.cpp 
  Author David McCombs -- www.w3sys.com
*/
#include "connectstring"
#include "read_write_base.hpp"
#include "openLogin.h"
#include "filePick.hpp"
#include "Session.hpp"

int main( int argcount, char ** args )
{
  // the last param automatically save files to the path
  ocString path =  getenv("PATH_INFO");    // where we were invoked

  Session_Obj session;
  if( path.length() )
  {
     session.SetData( "LAST_PATH_INFO",path);
     session.Synch();
  }
  else
  {
     path = session( "LAST_PATH_INFO" );
  }
  if( path.length() && path[0] == '/' )
  {
    path = path.substr(1);
  }

  openLogin login;

  filePicker picker(login,path);
  cgiScript script("text/html", false, picker.fileDir().c_str());

  // attempt to get cookie
  if ( login.testLoginStatus() )
  {
    script.closeHeader();
    picker.checkForfilesToDelete(script);
    picker.fileList(script);
    // DBG script << picker. debug << endl;
  }
  else
  {
    script.closeHeader();
    script << "<html><body>" << endl;
    script << "Not Logged in <!--";
    script << login.getLastError() << endl;
    script << " --></body></html>" << endl;
    writelog("INVALID ACCESS ATTEMPT - FILE PICK SIGN IN REQUIRED!");
  }
  writelog("OK - All Done");
  return(0);
}

#include "read_write_base.cpp"

