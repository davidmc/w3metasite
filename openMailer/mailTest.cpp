#include<string>
#include<iostream>
#include<fstream>
#include<iomanip>
#include"oc_Mail.h"

static char * exePath = "127.0.0.1";

int main( int argc, char ** argv )
{
  ocSendMail sendMail(exePath);
  // ocSendMail sendMail("72.18.196.40");  // artisan point server
  // ocSendMail sendMail("71.28.20.185");  // mail.customlogomats.com
  // ocSendMail sendMail("71.28.20.185"); // mail.hydrospec.net
  sendMail.setMimeType("text/html");
  sendMail.loadAddresses( "./addresses" );
  string loco = sendMail.getAddress("local");
  string foreign =   sendMail.getAddress("foreign");
  cout << "local is " << loco << endl;
  cout << "foreign is : "  << foreign << endl;
                          // from , to , subject
  if( ! sendMail.openRoute( loco, foreign, "Mail Test - W3 Systems Design" ) )
  {
     cout << "open route Failure!!!" << sendMail.sendResult() << endl;
     return 1;
  }


  ifstream body( "mailBody.txt" );
  string & mbody = sendMail.body();
  char c;
  while ( body.get(c) )
  {
    mbody += c;   
  }
  if(!sendMail.send())
    cout << "send Failure!!!" << sendMail.sendResult() << endl;

  return 0;
}

