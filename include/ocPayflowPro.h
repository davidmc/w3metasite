#ifndef OC_PAYFLOW_PRO_H
#define OC_PAYFLOW_PRO_H
#include <string>
#include <vector>
#include <cstdio>
#include "pfpro.h"

#include "ocString.h"
/*
  ocPayflowParam
  this class holds a payflow pro parameter.
  it also fixes the markup of the key tag to include
  the length of the value, in the case of pfpro delimiters in the value
*/
class ocPfParam
{
  string key;
  string value;
  string strParam;
public:

  ocPfParam(const string & inKey, const string & inValue):key(inKey),value(inValue)
  {
    string::size_type ampIn = value.find("&");
    string::size_type eqIn = value.find("=");
    // If either '&' or '=' are present, do special key processing
    if( ampIn != string::npos || eqIn != string::npos )
    {
      char temp[64];
      sprintf( temp,"%d", value.length() );
      key += "[";
      key += temp;
      key += "]";
    }
  }
  virtual ~ocPfParam()
  {;}
  ocPfParam(const ocPfParam & in):key(in.key),value(in.value)
  {;}
  const ocPfParam operator = ( const ocPfParam in )
  {
     key = in.key;
     value = in.value;
     return * this;
  }
  const char * toString( bool endTag )
  {
    strParam = key;
    strParam += "=";
    strParam += value;
    if( endTag ) strParam += "&";
    return strParam.c_str();
  }
};
/*
   a vector of parameters
*/
typedef class vector< ocPfParam > ocPfParams;

/*
  ocPayflowPro
  ===============
  A thin abstraction class
  over the verisign
  PayflowPro API

*/
class ocPayflowPro
{
  ocPfParams params;
  string hostAddress;
  int portNum;
  string response;
  string proxyAddress;
  int proxyPort;
  string proxyLogon;
  string proxyPassword;
  long timeout;
  int rc;
  int context;
  char * pTranResponse;
  bool isOk;
  string strError;
  string strResult;
  string strPnRef;
  string strResponseMessage;
  string strAuthCode;
  string strParmList;

  // Example: RESULT=24&PNREF=V54A14666103&RESPMSG=Invalid expiration date: 1299
  bool parseTranResponse( void )
  {
    bool bRet = false;
    strResult = ""; strPnRef = ""; strResponseMessage = "";
    ocString res = pTranResponse;
    for( string element=res.parse("&");
         !(element.length()==0 && res.endOfParse() );
         element=res.parse("&") )
    {
      ocString apair(element);
      string name = apair.parse("=");
      string value = apair.parse("=");
      transform(name.begin(),name.end(),name.begin(),(int(*)(int))toupper );
      if( name == "RESULT" )
      {
        strResult = value;
      }
      else if ( name == "PNREF" )
      {
        strPnRef = value;
      }
      else if( name == "RESPMSG" )
      {
        strResponseMessage = value;
      }
      else if( name == "AUTHCODE" )
      {
        strAuthCode = value;
      }
    }
    if( strResult == "0" )
    {
      bRet = true;
    }
    return bRet;
  }

public:

  // destructor
  ~ocPayflowPro( )
  {
    completeTransaction();
    if (context) pfproDestroyContext(context);
  }
  // constructor
  ocPayflowPro ( const char * certPath, const char * host,
                int inPort, long inTimeout,
                const char * inProxyAddr = NULL,
                int inProxyPort = 0,
                const char * inProxyLogon = NULL,
                const char * inProxyPassword = NULL  )
  :portNum(0),proxyPort(0),timeout(0),rc(0),context(0),pTranResponse(NULL),isOk(false)
  {
    // set PFPRO_CERT_PATH to certs directory
    if( setenv( "PFPRO_CERT_PATH",  certPath, 1 ) == 0 )
    {
      // can do payment processing
      hostAddress = host;
      portNum = inPort;
      timeout = inTimeout;
      if( inProxyAddr )
      {
        proxyAddress =  inProxyAddr;
      }
      if( inProxyPort )
      {
        proxyPort = inProxyPort;
      }
      if( inProxyLogon )
      {
        proxyLogon = inProxyLogon;
      }
      // pessimistic pre-fail
      strError = "Failed to initialize";
      rc = pfproInit();
      if( rc == 0 )
      {
        strError = "Failed to create a context.";
        rc = pfproCreateContext( &context,
                                  const_cast<char *>(hostAddress.c_str()),
                                  portNum, timeout,
                                  const_cast<char *>(proxyAddress.c_str()),
                                  proxyPort,
                                  const_cast<char *>(proxyLogon.c_str()),
                                  const_cast<char *>(proxyPassword.c_str()));
        if( rc == 0 )
        {
          isOk = true;
          strError = "OK";
        }
        else
        {
          strError = "pfproCreateContext -- Failed to create context.";
          isOk=false;
        }
      }
    }
    else
    {
      // error
      strError = "Failed to set certificate path env. variable.";
      isOk=false;
    }
  }

  // add a parameter to be passed to the pf pro processor
  void addParam ( string inKey, string inValue )
  {
    ocPfParam temp( inKey, inValue );
    params.push_back( temp );
  }

  // submit the transaction
  bool submitTransaction( void )
  {
    bool bRet = false;
    strParmList = "";
    for( int i=0; i<params.size(); i++)
    {
      ocPfParam & parm = params[i];
      strParmList += parm.toString( (i+1) < params.size() );
    }

    strError = "pfproSubmitTransaction call Failed.";
    rc = pfproSubmitTransaction( context,
                                 const_cast<char *>(strParmList.c_str()),
                                 strParmList.length(),
                                 &pTranResponse);
    if( rc == 0 )
    {
      bRet = parseTranResponse();
      if( bRet )
      {
        response = pTranResponse;
        strError = "Transaction Submitted Successfully.";
      }
      else
      {
        strError = "Transaction ";
        strError += getPnRef();
        strError += " Rejected. Code: ";
        strError += getResultCode();
        strError += " Reason: ";
        strError += getResponseMessage();
      }
    }
    else
    {
      strError += " rc=";
      ocAppend( strError, rc );
    }
    return bRet;
  }

  // complete the transaction, clean up, automatically called in destructor
  void completeTransaction( void )
  {
    if( pTranResponse )
    {
       pfproCompleteTransaction(pTranResponse);
       pTranResponse = NULL;
    }
  }
  // get the version of the pf pro processor
  string getVersion( void )
  {
    return pfproVersion();
  }

  /*
    get state information
  */

  // the last return code from the various payflow pro api's
  int getRc( void )
  {
    return rc;
  }
  // the full error string, or success, or nothing
  string & getError( void )
  {
    return strError;
  }
  // unparsed response string
  string & getUnparsedResponse( void )
  {
    return response;
  }
  // "0" indicates success, anything else means request rejected
  string & getResultCode( void )
  {
    return strResult;
  }
  // transaction id
  string & getPnRef( void )
  {
    return strPnRef;
  }
  // textual description of response code
  string & getResponseMessage( void)
  {
    return strResponseMessage;
  }
  // The authorization code
  string & getAuthorizationCode(void)
  {
    return strAuthCode;
  }
  string & getParameters(void)
  {
    return strParmList;
  }
};

#endif



