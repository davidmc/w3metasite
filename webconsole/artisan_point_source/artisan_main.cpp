/*
  Artisan Main
  ===================================
  CGI List for Artisan Point Category  
    
*/
#include "connectstring"
#include "nav_menu.h"
#include "cgiClass.h"
#include "site_pick.hpp"

class Artisan_Main
{
  artisanLogin & login;
  cgiScript & sc;
public: 
  // Constructor
  Artisan_Main(cgiScript&sc_in, 
               artisanLogin & login_in):sc(sc_in),login(login_in)
  {;}  
  ~Artisan_Main()
  {;}
  
  bool display( string & options )
  {
    bool breturn = true;
    sc << "<div class=welcome>" << endl;
    sc << "<h4>Welcome " << login.First_Name << " " << login.Last_Name << "!</h4>" << endl;
    sc << "<p>This is the Main Page for Artisans.</p>" << endl;    
    sc << "<p>First, please check that the site you want to work on is selected:"
       << "<form action=\"artisan_main.cgi\" method=\"post\">"
       << "<input type=\"hidden\" name=\"changeSite\" value=\"true\">"
       << "<select name=\"site_id\" onChange=\"this.form.submit()\">"
       << options
       << "</select>"
       << " &nbsp; (If there's only one site in the list, it's automatically selected.)"
       << "</p>";
    sc << "<p>Then select from the menu items above</p>" << endl;
    sc << "<p><a href='../metasite/site-admin.cgi' target='webmanage'>Click Here to Manage Webpages.</a></p>" << endl;  
    sc << "<p><a href='artisan_main.cgi?logout=true'>Click Here to Log Out.</a></p>" << endl;   
    
    sc << "</div>" << endl;  
    return breturn;
  }      
}; 

int main( int argc, char ** argv )
{
  cgiScript sc("text/html", false);
  cgiInput & args = sc.ClientArguments();
  bool loggedIn = false;
  if( args.count("logout") )
  {
    oLogin.logout();
  } 
  else
  {
    loggedIn = oLogin.testLoginStatus();
    if( !loggedIn )
    {    
      if(args.count("login") && args.count("password"))
      {
        loggedIn = oLogin.checkUser( args["login"], args["password"] );
      }
    }
  }
  if(loggedIn)
  {
    /* The next two lines provide an option list, 
       and set the site id cookie if only one site. 
       MUST be invoked before closing header */
    site_pick pick_a_site(sc);
    string & options = pick_a_site.getSiteOptions(sc,oLogin);
    
    sc.closeHeader();     
        
    Artisan_Main myMain(sc,oLogin);  
    cgiTemplates pgTemplate;
    pgTemplate.load("Templates/adminPane.htmp");
    sc << ocString(pgTemplate.getParagraph("top"))
            .replaceAll("$heading$","Artisan Point")
            .replace("$menu$",getNavMenu().c_str())
            .replace("$instructions$",
                    "Main");                     
    myMain.display(options);
    sc << pgTemplate.getParagraph("list-bottom");
  }
  else
  {   
    showSignOnForm ( sc ); 
  }
};
