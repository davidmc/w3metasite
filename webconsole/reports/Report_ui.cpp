/*
  Report_ui.cpp

  User Interface Definition and Implementation 
  for Report.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  
  
*/
#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "cgiTemplates.h"
#include "openLogin.h"
#include "Report.hpp"
#include "superlist_base.hpp"
#include "forms_base.hpp"
#include "siteLimit.h"
#include "Query_List.hpp"

using namespace std;
openLogin oLogin;

#include "ReportUICommon.h"


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  Report_form myFrm(script); 
  Report_List mylist(script,oLogin.Session());  
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/reportPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
                     .replaceAll("$heading$","Report");

    myFrm.loadControlTemplates("Templates/divform.htmp");  
    myFrm.form_action();  
    myFrm.form_display();
    
    mylist.loadListTemplates("Templates/list.htmp");  
    mylist.list_display();
        
    ocString end = pgTemplate.getParagraph("bottom");
    script << end;          
  }
  else
  {
    script.Redirect("signIn.html"); 
  } 
}
// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"

