/*
  Pages.cpp

  main file for pages.meta

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
#include "lookup.hpp"

using namespace std;

// Standard main function
int main(int argc, char ** argv)
{  
  baseUrl = "pages.meta";  // this is my compiled name!

  cgiScript script("text/html",false);
  isSignedOn = check(script);
  script.closeHeader();

  page pg(script);
  if( pg.load() )
  {
    page_control ctrl(pg);
    ctrl.emit ();
    // script << "Signed In = " <<  isSignedOn << endl;
  }
  else
  {
    script << "<html><head><title>Page Unavailable</title></head>" << endl
           << "<body><h1>Sorry</h1>" << endl
           << "<p>This is a valid URL but the page you requested isn't set up yet.</p>";
    if( isSignedOn )
    {
      script << pg.getLoadResult() << endl;
      // fixup rel url based on site detection (page url is after the site url)
      ocString url = script.ScriptName().c_str();
      url += script.PathInfo().c_str();
      string sql = "select url from sites where id ";
      sql += siteFocus();

      // DBG script << sql << endl;
      string siteUrl = tableLookup ( sql );
      if(siteUrl.length())
      {
        url = url.replace(siteUrl,"");  // remove the site url from the start os the string
        // DBG  script << " ... replaced " <<  siteUrl << endl;
      }
      // get the Page Builder in this level
      ocString rel = script.ScriptName().c_str();
      rel = rel.replace( baseUrl, "" );
      script << "<p>Do you want to create it?<br>" << endl
              << "If so <a href='" << rel << "pageBuilder.cgi?url=" << url << "'>Click Here</a>.<br>" << endl;
    }

    ocString domain = script.ServerName().c_str();
    ocString relpath = script.ScriptName().c_str();
  
    string::size_type pos = relpath.find_last_of('/');
    relpath = relpath.substr(0,pos+1);
  
    script << "<!-- " << oLogin.getLastError() << endl;
    script << " --></body></html>";
  }
  return 0;
}
#include "read_write_base.cpp"



