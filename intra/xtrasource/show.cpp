/*

  show.cpp
  main file for show.meta

*/
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
#include "connectstring"
#include "cgiClass.h"
#include "page.hpp"
#include "w3intranet.h"
#include "page_control.hpp"
#include "page_enhancements.hpp"
#include "siteLimit.h"
#include "show_control.hpp"

using namespace std;


// Standard main function
int main(int argc, char ** argv)
{  
  baseUrl = "show.meta";  // this is my compiled name!  
  cgiScript script("text/html",false);
  isSignedOn = check(script);
  script.closeHeader(); 
  show_page pg(script);

  if( pg.load() )
  { 
    page_control ctrl(pg);    
    ctrl.addOp ( "detail",  new detail_functor(pg,script) );    
    ctrl.addOp ( "slides",  new slides_functor(pg,script) );
    // Get the control string
    string control = pg.loaded_control_string();

    // load the valid slide and detail paragraph selectors
    pg.loadSlides(); 
    ctrl.emit (); 
  }
  else if( script.ClientArguments().count("DBG") )
  {
    script << "<html><head><title>Page Unavailable</title></head>" << endl
           << "<body><h1>Sorry</h1>" << endl
           << "<p>This is a valid URL but the page you requested isn't set up yet.</p>";
           
    script << "</p></body></html>";
  }
  else
  {
    script << "<html><head><title>Page Unavailable</title></head>" << endl
           << "<body><h1>Sorry</h1>" << endl
           << "<p>The page you requested is not available</p></body></html>";
  }  
 
  return 0;
}
#include "read_write_base.cpp"
