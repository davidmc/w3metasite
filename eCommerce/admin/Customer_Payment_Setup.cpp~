/*
  Customer_Payment_Setup.cpp

  User Interface Definition and Implementation 
  for Customer_Payment_Parameters.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  
  
*/

#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "list_base.hpp"
#include "cgiTemplates.h"
#include "list_base.hpp"
#include "openLogin.h" // open-login has own connectstring
#include "userLogin.h" // see if they are logged in
#include "Session.hpp" // see about the uuid
#include "../../eCommerce/source/pay_form.hpp"

using namespace std;
openLogin oLogin;
void paymentForm( cgiScript & script, long long customerId )
{
  Payment_Form pay(script); 
  pay.loadControlTemplates("Templates/whatever");
  
  script << pay.makeTop( "?step=verify", "Payment Setup" ) ;
  
  // show??
  pay.getMode() = "s";
  llong Payment_Service_Id;
  if( args.count("Payment_Service_Id") > 0 )
  {
    Payment_Service_Id = atoll(args["Payment_Service_Id"].c_str());
  }
  else
  {  // look for an existing customer payment service
    ocString qry = "select Id from Customer_Payment_Service where Customer_Id = ";
    qry.append(customer.Id);
    string psid =  tableLookup ( qry );
    if( psid.length() ) Payment_Service_Id = atoll(psid.c_str());
  }
  pay.key(Payment_Service_Id);
  pay.Customer_Id = customer.Id;
  pay.form_action();
  pay.form_display();
  script << pay.makeButtonedBottom(  Action );
}

int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );

  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
                     .replaceAll("$heading$","Customer_Payment_Parameter");

    myFrm.loadControlTemplates("Templates/childdivform.htmp");  
    myFrm.form_action();  
    myFrm.form_display();
        
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

