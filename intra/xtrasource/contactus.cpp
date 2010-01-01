/*
  contactus.cpp
  ==============================================
  main file for contactus.meta
  In addition to standard page tags, this requires a control item:
    - contactus
  elements:
    - mailform, -- the container for the form, a typical content tag
      mailbodystart, mailbodyitem, mailbodyend, - all to create the html content for the actual mail
      thankyou,  -- the thank you response
      sendto   -- the sendto address

  Note: form data items must start with mi(1,n)_
      (1,n) is an ordering number from 1 to as big as you need it
      (be aware that an alpha sort is going on in a map,
      so 11 goes before 2 (solution: use 02)!)
      examples: <input name='mi1_First Name'> <input name='mi2_Address 1'>
      Case Sensitive!
  Copyright (C) 2004 - 2008 David K. McCombs w3sys.com
  ===============================================
  
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

#include"oc_Mail.h"
using namespace std;

#include "contact_control.hpp"

// Standard main function
int main(int argc, char ** argv)
{
  baseUrl = "contactus.meta";  // this is my compiled name!

  cgiScript script("text/html",false);
  isSignedOn = check(script);
  script.closeHeader();
  
  page pg(script);
  // load the data
  if( pg.load() )
  {
    page_control ctrl(pg);     
    ctrl.addOp ( "contactus",  new mail_functor(pg,script) );
    // emit the form, or the send acknowledment
    ctrl.emit();
    //     script << "Signed In = " <<  isSignedOn << endl;
  }
  else
  {
    script << "<html><head><title>Page Unavailable</title></head>" << endl
           << "<body><h1>Sorry</h1>" << endl
           << "<p>The page you requested is not available</p></body></html>";  
  }
    
  return 0;
}
#include "read_write_base.cpp"
#include "forms_base.cpp"
