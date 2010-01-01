/*
  urlpick.cpp
  ===================================
  CGI program    
*/

#include "cgiClass.h"
#include "cgiTemplates.h"
#include "ocFileSys.h"
#include "connectstring"
#include "openLogin.h"
#include "siteLimit.h"
#include "site.hpp"

using namespace std;

int main( int argc, char ** argv )
{
  cgiScript sc("text/html", false);
  cgiTemplates tmplt;
  tmplt.load("./Templates/pickurl.htmp");  
  openLogin login;
    
  if ( login.testLoginStatus() )
  { 
    // now close header
    sc.closeHeader();
    string siteLimitation = currentSite(sc.ClientArguments());
    siteObject site;
    site.id = atoll(siteLimitation.c_str());
    site.key(site.id);
    site.get_data();
    
    sc << tmplt.getParagraph("pickformstart"); 
    ocString opt = tmplt.getParagraph("pickoption"); 
    ocFileSys fs;
    if( fs.openDir(site.path) )
    {
      ocDirectory & entries = fs.getDirectoryEntries();
      for(int i = 0; i < entries.size(); i++ )
      {
        ocString sotest = entries[i].name;
        if( sotest.regExMatch(".cgi$") ||
            sotest.regExMatch(".pages$") || 
            sotest.regExMatch(".meta$") )
        {
          sc << opt.replace("$value",entries[i].name.c_str())
                   .replace("$label",entries[i].name.c_str());
        }
      }
    }
    
    ocString  pickformend =   tmplt.getParagraph("pickformend");    
    sc << pickformend;

  }  
  return 0;
};
// compile implementations here
#include "read_write_base.cpp"
