/*
  Open Core (oc) Libraries
  Copyright (c) 2003 - 2007 - David K. McCombs - davidmc@w3sys.com
  
  Freely redistributable as a courtesy of 
  W3 Systems Design to the developer community
  
  ocTrustCommerce payment gateway
  C++ wrapper class.
*/ 

#ifndef OC_TRUST_COMMERCE
#define OC_TRUST_COMMERCE
#include <string>
#include "tcInfoClass.h"

extern "C" {
#include "tclink.h"
}

using namespace std;

class ocTCLink 
{
private:
  TCLinkHandle handle;
  tcInfoClass info;
  string transaction_status;
  string transaction_id;
  string transaction_info;
  bool isGood;
  void setTransactionInfo( void )
  {
    const size_t bigBufSize = 1024;
    char * bigBuf = new char[bigBufSize];        
    if( bigBuf && TCLinkGetEntireResponse(handle, bigBuf, bigBufSize) )
    {
      transaction_info = bigBuf;
    }
    delete [] bigBuf;  
  }
public:
  ocTCLink():handle(NULL),isGood(true),info()
  {
    handle = TCLinkCreate();
    if( !handle )
    {
      isGood=false;
      return;
    }  
    PushParam( "custid", info.tccid()); 
    PushParam( "password", info.tcpwd());
  }
  // not planning any 'IS A' relationship, so no virtual destructor  
  ~ocTCLink()
  {
    Close();    
  }
  // Push parameters to be sent
  ocTCLink & PushParam(string name, string value)
  {
    if(  handle ) TCLinkPushParam(handle, name.c_str(), value.c_str());
    return *this;
  } 
  // Send the transaction request
  ocTCLink & Send( void )
  {
    TCLinkSend(handle);
    char responseBuf[PARAM_MAX_LEN];  
    char * ptrStat = NULL;
    if(  handle ) ptrStat = TCLinkGetResponse(handle, "status", responseBuf);
    if( ptrStat )
    {
      transaction_status = responseBuf;
      if( transaction_status == "accepted" || transaction_status == "approved" )
      {
        if( TCLinkGetResponse(handle, "transid", responseBuf) )
        {
          transaction_id = responseBuf;
        }
      }
      else
      {
        isGood=false;
      }  
      setTransactionInfo();
    } // end if a ptrStat
    else
    {
      isGood=false;
    }
    return *this;
  } 
  // Closes the link
  ocTCLink & Close()
  {
    if( handle )
    {
      TCLinkDestroy(handle);
      handle = NULL;
    }  
  }
  bool IsGood( void ) { return isGood; }
  const string & TransactionStatus( void ) { return transaction_status; }
  const string & TransactionId( void ) { return transaction_id; }
  const string & TransactionInfo( void ) { return transaction_info; } 

};
#endif

#ifdef IN_T2_TESTHARNESS
/*
  This is the actual unit test code done on the class,
  which makes it a good example for how to actually 
  use the class in production
  
  Test numbers for  card types
  "4111111111111111", "0404",  "visa" : visa 
  "5411111111111115", "0404",  "mc"   : mastercard 
  
*/
  // Instance of a trust commerce link class
  ocTCLink tcLink;
  tcLink.PushParam("action","preauth")  // do a pre-authorization, could do "sale"
        .PushParam("media", "cc")       // its a credit card
        .PushParam("demo", "y")         // in demo mode        
        .PushParam("name", "George George Jr.")
        .PushParam("ticket","product-14")
       // .PushParam("cc","4111111111111110") // BAD visa cc number
       // .PushParam("cc","4111111111111111") // visa cc number
        .PushParam("cc","5411111111111115") // mastercard cc number
        .PushParam("exp","0404")        // expires 4/2004
        .PushParam("amount","42137");    // amount of $421.37  
  if( tcLink.Send().IsGood() )
  {
    cout << "Good cc preauth - TransactionId: " << tcLink.TransactionId() << endl;
    cout << "TransactionStatus: " <<  tcLink.TransactionStatus() << endl;
    cout << "TransactionInfo: " << tcLink.TransactionInfo() << endl;
    // The current link shouldn't be used for another transaction. 
    //  Close it and do the post auth with a new link.
    tcLink.Close();
    ocTCLink tcLink2;
    tcLink2.PushParam("action","postauth")
           .PushParam("transid", tcLink.TransactionId().c_str() )
           .PushParam("demo", "y");
    
    if( tcLink2.Send().IsGood() )
    {
      cout << "Good cc postauth - TransactionId: " << tcLink2.TransactionId() << endl;
      cout << "TransactionStatus: " <<  tcLink2.TransactionStatus() << endl;
      cout << "TransactionInfo: " << tcLink2.TransactionInfo() << endl;
    }
    else
    {
      cout << "Transaction Failed: " << endl
           << tcLink2.TransactionInfo() << endl;
    }
  }
  else
  {
    cout << "Transaction Failed: " << endl
         << tcLink.TransactionInfo() << endl;
  }
  
#endif
