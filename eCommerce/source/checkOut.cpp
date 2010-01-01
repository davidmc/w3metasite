/*
  checkOut.cpp    
  
  main file for checking out    
  I am a meta page derivation. 
     
*/
#include "cgiClass.h"
#include "connectstring"

// one global
string baseUrl;

#include "page.hpp"
#include "checkout_control.hpp"

using namespace std;

// Cross domain cart id fixup.. put crtid client argument into the client cookie
void crtidFixup(cgiScript & script)
{
  if( script.ClientArguments().count("crtid") )
  {
    cgiCookie cookie;
    cookie.setPath("/");
    cookie.set("crtid", script.ClientArguments()["crtid"].c_str());
  }  
}

// Standard main function
int main(int argc, char ** argv)
{  
  baseUrl = "checkOut.cgi";  // this is my compiled name!
  cgiScript script("text/html",false); 

  // work around for MSIE cookie confusion and bugs
  crtidFixup(script); 
  
  page pg(script);
  
  if( pg.load() )
  {    
    page_control ctrl(pg);

    // the extra operation besides normal page stuff        
    ctrl.addOp ( "checkout",  new chkout_functor(pg,script) );
    
    // close the http header before emiting any content
    script.closeHeader();         
    ctrl.emit();    
  }
  else
  {
    script.closeHeader(); 
    script << "<html><head><title>Page Unavailable</title></head>" << endl
           << "<body><h1>Sorry</h1>" << endl
           << "<p>The page you requested is not available</p></body></html>";
  }  
  return 0;
}

// As an alternative to building more objects
#include "read_write_base.cpp"
#include "forms_base.cpp"
