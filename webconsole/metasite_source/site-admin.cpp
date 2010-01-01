/*
  Copyright 2002
  Author David McCombs.
  
*/

#include "openLogin.h"
#include "cgiTemplates.h"
#include "site_pick.hpp"


void  show(cgiScript & script, openLogin & login, string & options)
{
  cgiTemplates htmlDoc;
  htmlDoc.load("Templates/site-manage.html");    
  ocString part = htmlDoc.getParagraph("top");  
  script <<  part.replace("$OPTIONS$",options.c_str());
  if( login.SiteAdmin() )
  {
    script << htmlDoc.getParagraph("adminstrators");
  }  
  script << htmlDoc.getParagraph("webmasters");
  script << htmlDoc.getParagraph("end");
}


int main( int argcount, char ** args )
{
  bool isSignedOn = false;

  // An instance of the script
  // must be created before any other output.
  cgiScript script("text/html",false);
  openLogin login;
  
  
  // attempt to get cookie
  if ( login.testLoginStatus() )
  {
    isSignedOn = true;
  }

  if( isSignedOn )
  {   
        /* The next two lines provide an option list, 
       and set the site id cookie if only one site. 
       MUST be invoked before closing header */
    site_pick pick_a_site(script);
    string & options = pick_a_site.getSiteOptions(script,login);    
  
    // just to get it a cookie set for the menu... DKMc 12/20/04
    // selSite = currentSite( script.ClientArguments() );  
    
    //siteOptions = getSiteOptions(script,login );
    
    script.closeHeader();
    
    show(script,login,options);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
