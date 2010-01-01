/*

  Pages.cpp    
  main file for pages.meta
  
*/
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
#include "connectstring"
#include "cgiClass.h"
// one global
string baseUrl;
#include "page.hpp"
#include "page_control.hpp"

using namespace std;

// Standard main function
int main(int argc, char ** argv)
{  
  baseUrl = "pages.meta";  // this is my compiled name!
  writelog( "instance of script" );
  cgiScript script;  
  writelog( "instance of page" );
  page pg(script);  
  writelog( "load page" );  
  if( pg.load() )
  {    
    writelog( "instance of page control" );
    page_control ctrl(pg);     
    writelog( "page control emit" );
    ctrl.emit ();    
  }
  else
  {
    script << "<html><head><title>Page Unavailable</title></head>" << endl
           << "<body><h1>Sorry</h1>" << endl
           << "<p>The page you requested is not available</p></body></html>";
  } 
  return 0;
}

