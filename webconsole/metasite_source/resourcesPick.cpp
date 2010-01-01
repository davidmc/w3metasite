/*
  Copyright (c) 2004 W3 Systems Inc.
  resourcesPick.cpp 
  Author David McCombs -- www.w3sys.com


*/
#include "connectstring"

#include "read_write_base.hpp"
#include "openLogin.h"

#include "filePick.hpp"

int main( int argcount, char ** args )
{
  // An instance of the script
  // must be created before any other output.
  // the last param automatically save files to the path
  openLogin login;    
  filePicker picker(login,"resources","resourcesPick.cgi"); 
  picker.FLink( "<a href='javascript:setResource(\"_img_\")'>Pick</a></div>" );
  cgiScript script("text/html", false, picker.fileDir().c_str());
  writelog("OK - script made");
  
  // attempt to get cookie
  if ( 1 ) // login.testLoginStatus() )
  {
    script.closeHeader();    
        
    writelog("Checking for files to delete");    
    picker.checkForfilesToDelete(script);
    
    writelog("Generating file list");        
    picker.fileList(script); 
       
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

