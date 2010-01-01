/*
  WorkList.cpp

  User Interface Definition and Implementation 
  for Report.
  
  Copyright (c) 2005 by D.K. McCombs.
  
  davidmc@w3sys.com
  
  W3 Systems Design Inc.  
  
*/
#include <string>
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
using namespace std;
 // added to resolve compatibility issues where this may be a symbolic link to another project
#include "compatible.hpp"

// one glob
#include "connectstring"
#include "w3intranet.h"
#include "WorkList.hpp"
#include "page_enhancements.hpp"

 // Set static value
bool Report_Parameters_form::hasParamValues = false;

bool intraMain(page & pg ,cgiScript & script)
{
  Work_List mylist(script,oLogin.Session());  
  mylist.loadListTemplates("Templates/worklist.htmp"); 
  script << "<h1> To Do: </h1>" << endl;
  mylist.list_display();

  // dummy form to satisfy client scripts
  script << "<form id='uiForm' name='uiForm'><input type='hidden' id='xmldirections'></form>" << endl;

  return true;
}

// Standard main function
int main(int argc, char ** argv)
{
  baseUrl = "WorkList.meta";  // this is my compiled name!
  // because we are checking
  cgiScript script("text/html",false);
  bool isSignedOn = check(script);
  script.closeHeader();

  /* Replace intranet.meta with anything you please, or not,
     make sure an associated page entry exists in the metasite database. */
  script.ScriptName() =
    ocString( script.ScriptName().c_str()).replace("WorkList.meta","reports").c_str();  

  page pg(script);  

  writelog( "load page" );  
  if( pg.load() )
  {    
    writelog( "instance of page control" );
    page_control ctrl(pg);     
    ctrl.addOp ( "servicelist",  new servicelist_functor(pg,script,isSignedOn) );
    ctrl.addOp ( "userinterface",  new form_functor(pg,script,isSignedOn) );
    ctrl.addOp ( "qrycontent", new qrystr_content_functor(pg,script) );
    ctrl.addOp ( "rolecontent", new role_content_functor (pg,script) );    
    writelog( "page control emit" );
    ctrl.emit();    
  }
  else
  {
    script << "<html><head><title>Page Unavailable</title></head>" << endl
           << "<body><h1>Sorry</h1>" << endl
           << "<p>The page you requested is not available</p></body></html>";
  }
  return 0;
}

// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"
