/*

  ReportUI.cpp

  User Interface Definition and Implementation 
  for Report.

  Copyright (c) 2005 by D.K. McCombs.

  davidmc@w3sys.com  
  W3 Systems Design Inc.

*/



#include <string>
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
using namespace std;

// added to resolve compatibility issues where this may be a symbolic link to another project
#include "compatible.hpp"

#include "connectstring"
#include "w3intranet.h"
#include "Report.hpp"
#include "Query_List.hpp"
#include "ReportUICommon.h"


bool intraMain(page & pg ,cgiScript & script)
{

  Report_form myFrm(script); 
  Report_List mylist(script,oLogin.Session());  


  myFrm.loadControlTemplates("Templates/divform.htmp");  
  myFrm.form_action();  
  myFrm.form_display();

  mylist.loadListTemplates("Templates/navlist.htmp");  
  mylist.list_display();
         
 
  return true;
}

// Standard main function
int main(int argc, char ** argv)
{  
  baseUrl = "ReportUI.meta";  // this is my compiled name!
  // because we are checking
  cgiScript script("text/html",false);
  bool isSignedOn = check(script);
  script.closeHeader();
  
  /* Replace intranet.meta with anything you please, or not,
     make sure an associated page entry exists in the metasite database. */
  script.ScriptName() =
    ocString( script.ScriptName().c_str()).replace("ReportUI.meta","reports").c_str();  
    
  page pg(script);  
  
  writelog( "load page" );  
  if( pg.load() )
  {    
    writelog( "instance of page control" );
    page_control ctrl(pg);     
    ctrl.addOp ( "servicelist",  new servicelist_functor(pg,script,isSignedOn) );    
    ctrl.addOp ( "userinterface",  new form_functor(pg,script,isSignedOn) );    
    writelog( "page control emit" );
    ctrl.emit();
        
  }
  else
  {
    // try to be a regular cgi
    if( oLogin.testLoginStatus() )
    {
      script.closeHeader();
      cgiTemplates pgTemplate;    
      pgTemplate.load("Templates/adminPane.htmp");
      
      script << ocString(pgTemplate.getParagraph("top"))
                      .replaceAll("$heading$","Report");
  
      intraMain(pg ,script);
          
      ocString end = pgTemplate.getParagraph("bottom");
       script <<  end << "NAWG!" << endl;
    }
    else
    {
      script.closeHeader();
      script << "could not sign in  " << oLogin. getLastError() << endl;
      // script.Redirect("signIn.html"); 
    }
  } 
  return 0;
}

// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"
