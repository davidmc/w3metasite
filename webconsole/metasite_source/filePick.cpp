/*
  Copyright (c) 2004 W3 Systems Inc.
  filePick.cpp 
  Author David McCombs -- www.w3sys.com


*/
#include "connectstring"
#include "read_write_base.hpp"
#include "openLogin.h"
#include "filePick.hpp"


int main( int argcount, char ** args )
{

  // the last param automatically save files to the path
  char * pinf =  getenv("PATH_INFO");
  string path;
  if( pinf )path = pinf;

  openLogin login;
  if( path.length() )
  {
    // take off the leading /
    path = path.substr(1);
  }
  path += "Templates";

  filePicker picker(login, path, "filePick.cgi","/sys");
  // filePicker picker(login);  
  cgiScript script("text/html", false, picker.fileDir().c_str());
  writelog("OK - script made");
  // attempt to get cookie
  if ( login.testLoginStatus() )
  {
    script.closeHeader();
        
    writelog("Checking for files to delete");
    picker.checkForfilesToDelete(script);
    
    writelog("Generating file list");        
    picker.fileList(script);

    // DBG
    script << picker.debug << endl;
    
    writelog("Done");
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    writelog("INVALID ACCESS ATTEMPT");      
  }
  writelog("OK - All Done");
  return(0);
}

#include "read_write_base.cpp"

