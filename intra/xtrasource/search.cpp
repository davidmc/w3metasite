/*
 
  search.cpp    
  main file for show.meta
  
*/
#include "connectstring"
#include "cgiClass.h"
// one global
string baseUrl;
#include "page.hpp"
#include "search_control.hpp"

using namespace std;


// Standard main function
int main(int argc, char ** argv)
{  
  baseUrl = "search.meta";  // this is my compiled name!  
  cgiScript script;  
  search_page pg(script);

  if( pg.load() )
  { 
    page_control ctrl(pg);    
    ctrl.addOp ( "search_form",  new form_functor(pg,script) );
    ctrl.addOp ( "results",  new results_functor(pg,script) );
    ctrl.addOp ( "result_slides",  new result_slides_functor(pg,script) );
    ctrl.emit (); 
  }
  else
  {  
    // I should email somebody at this point!
    script << "<html><head><title>Page Unavailable</title></head>" << endl
           << "<body><h1>I'm Sorry!</h1>" << endl
           << "<p>The page you requested is not available.</p>"
           << "<p>It may be that we're doing some new page construction.</p>"
           << "<p>Please check back later.</p>"                               
           << "</body></html>";
  }

  return 0;
}
