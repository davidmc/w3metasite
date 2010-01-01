/*
  Main Panel
  ===================================
  CGI User Interface for Pay Drop Main Panel
            
*/

#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "nav_menu.h"
#include "site_pick.hpp"


int main( int argc, char ** argv )
{  
  cgiScript sc("text/html",false); 
  if( oLogin.checkUser(sc.ClientArguments()["login"].c_str(),
                       sc.ClientArguments()["password"].c_str() )|| 
      oLogin.testLoginStatus() )
  {
    /* The next two lines provide an option list, 
       and set the site id cookie if only one site. 
       MUST be invoked before closing header */
    site_pick pick_a_site(sc);
    string & options = pick_a_site.getSiteOptions(sc,oLogin); 
     
    sc .closeHeader();
    cgiHtml html;
    {
      cgiHead head;
      head << "<style type=text/css media=screen>@import url(main.css );</style>" << endl;
    }
    {
      cgiBody body;
      body << "<strong>Hi " <<  oLogin.First() << ",<br>" << endl;
      body << "Paydrop - IMPORTANT MESSAGES:</strong><br>" << endl;
      body << "<div style=\"margin:20px;padding:20px;border:1px solid rgb(0,0,100);\">"
              "In order to have mail work properly, you MUST have the following metasite " << endl;
      body << "pages set up with the mail template:<br> " << endl;
      body << "For monthly credits: 'mail/monthlyrebates' and  'mail/missedrebates'.<br>" << endl;
      body << "For account changes: 'mail/accountchange' and  'mail/updateaccount'.<br>" << endl; 
      body << "For the newsletter, the page is determined by the newsletter setup.<br></div>" << endl;     
      body << endl;
    }   
    
  } 
  else
  {
    sc.Redirect("signIn.html"); 
  } 
};

#include "read_write_base.cpp"



 
