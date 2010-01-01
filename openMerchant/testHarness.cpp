#include <iostream>


using namespace std;

#include "ocPayflowPro.h"

int main( int argc, char ** argv )
{
  int ret=0;
  string test = "Test of ocAppend: ";
  ocAppend( test, 433 );
  ocAppend( test, " Float: " );
  ocAppend( test, 6.344 );
  cout << test << endl;
  ocPayflowPro merchant( "/home/davidmc/development/openMerchant/verisign/payflowpro/linux/certs", // test
                        // production --> "/home/sites/home/users/admin/development/openMerchant/verisign/payflowpro/linux/certs",
                         "test-payflow.verisign.com", 443, 10 );
  if ( merchant.getRc() )
  {
    cout << "Error : " << merchant.getError() << endl;
  }
  else
  {
    cout << "Well Constructed!" << endl;
    cout << "Version: " << merchant.getVersion() << endl;
  }
  merchant.addParam( "USER", "daltonutilities" );
  merchant.addParam( "VENDOR", "daltonutilities" );
  merchant.addParam( "PARTNER", "verisign" );
  merchant.addParam( "PWD", "dalton777site" );
  merchant.addParam( "TRXTYPE", "A" );
  // merchant.addParam( "TRXTYPE", "S" );
  merchant.addParam( "TENDER", "C" );
  merchant.addParam( "ACCT", "5105105105105100" );
  merchant.addParam( "EXPDATE", "1204" );
  // merchant.addParam( "AMT", "27.23" );
  merchant.addParam( "AMT", "0.01" );

  if( merchant.submitTransaction() == false )
  {
    cout << "Failure!" << endl;
  }
  else
  {
    cout << "Success!" << endl;
    cout << "Result Code: " << merchant.getResultCode() << endl;
    cout << "Transaction Reference Number: " << merchant.getPnRef() << endl;
    cout << "Response Message: " << merchant.getResponseMessage() << endl;
    cout << "Authorization Code: " << merchant.getAuthorizationCode() << endl;
    cout << "Params: " << merchant.getParameters() << endl;
  }
  cout << merchant.getError() << " : " <<  merchant.getUnparsedResponse() << endl;
  return ret;
}
