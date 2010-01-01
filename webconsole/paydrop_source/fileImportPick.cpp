/*
  Copyright (c) 2004 w3 systems
  fileImportPick.cpp 
  Author David McCombs -- www.w3sys.com
    
*/
#include "filePick.hpp"

int main( int argcount, char ** args )
{
  // An instance of the script
  // must be created before any other output.
  // the last param automatically save files to the path
  openLogin login;
  filePicker picker(login,"sys/import","fileImportPick.cgi");
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

