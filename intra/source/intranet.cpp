/*
  intranet.cpp
  main file template for rad generated intranet pages.
  
*/

#include <string>
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
using namespace std;

// one global
string baseUrl;

#include "w3intranet.h"
#include "page_enhancements.hpp"

bool intraMain(page & pg ,cgiScript & script)
{
  return true;
}

// Standard main function
int main(int argc, char ** argv)
{  
  baseUrl = "intranet.meta";  // this is my compiled name!
  // because we are checking
  cgiScript script("text/html",false);
  bool isSignedOn = check(script);
  script.closeHeader();
  
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
