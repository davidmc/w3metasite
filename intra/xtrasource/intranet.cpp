/*
  intranet.cpp
  main file template for rad generated intranet pages.

*/

#include <string>
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
using namespace std;


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
  isSignedOn = check(script);
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
           << "<p>This is a valid URL but the page you requested isn't set up yet.</p>";
    if( isSignedOn )
    {
      // fixup rel url based on site detection (page url is after the site url)
      ocString url = script.ScriptName().c_str();
      url += script.PathInfo().c_str();
      string sql = "select url from sites where id ";
      sql += siteFocus();
      
      // DBG
      script << sql << endl;
      string siteUrl = tableLookup ( sql );
      if(siteUrl.length())
      {
        url = url.replace(siteUrl,"");  // remove the site url from the start os the string
        // DBG
        script << " ... replaced " <<  siteUrl << endl;
      }
      // get the Page Builder in this level
      ocString rel = script.ScriptName().c_str();
      rel = rel.replace( baseUrl, "" );
      script << "<p>Do you want to create it?<br>" << endl
              << "If so <a href='" << rel << "pageBuilder.cgi?url=" << url << "'>Click Here</a>.<br>" << endl;
    }
  
  }

  return 0;
}

// compile implementations here
#include "read_write_base.cpp"
