/*

 oc_SMTP.h 
 
 SMTP Client Implementation by
 David McCombs davidmc@w3sys.com
  
 Steps in sending mail
 =====================
 
 0. Connect to the socket:port of the SMTP server, wait for a response
    response should be '220 <SP> <domain> <SP> Service ready <CRLF>'
 
 1. Say "Hello, I am  <domain>".
 HELO <SP> <domain> <CRLF>
   response should be '250 OK'
   
 2. Send the mail command
 MAIL <SP> FROM:<reverse-path> <CRLF>
  response should be '250 OK'
  
 3. Indicate the recipient
 RCPT <SP> TO:<forward-path> <CRLF>
  response should be '250 OK'
  a response of '550 Failure' indicates the recipient is unknown
  
 4. Send the DATA command.
 DATA <CRLF>
  response should be  '250 OK'
  send mail data on the stream.  
    |mail data includes the memo header
    |items such as Date, Subject, To, Cc, From
            
  to end data send a line with a single '.'
    '250 OK' is the response on success
    
  5. Close the stream  
  QUIT <CRLF> 
    '250 OK' is the response on success
    
RFC 821 - 4.5.1. MINIMUM IMPLEMENTATION   
          In order to make SMTP workable, the following minimum
          implementation is required for all receivers:

            COMMANDS -- HELO
                        MAIL
                        RCPT
                        DATA
                        RSET
                        NOOP
                        QUIT

*/

#ifndef OC_SMTP_H 
#define OC_SMTP_H
// #define DO_OPEN_LOGGING
#include "../include/openLogger.h"
#include "ocSocket.h"
#include "ocString.h"

// reasonable first guesses to prevent too much reallocation
const int reasonableBufSize = 200;


class oc_SMTP: protected ocSocket
{
  
  string crlf;
  string sender;  
  
 public:
 
  bool isConnected;
  ocString response;
  string error;
 
  oc_SMTP( string address, int port=25 )
    :ocSocket( address.c_str(), port ),crlf("\r\n")
  {
    // add a timeout
    SetTimeoutTime( 5, 0 );
    isConnected = Response("220");
    // DBG   cout << "oc_SMTP constructed with timeout." << endl;
  }

  virtual ~oc_SMTP()
  {;}

  
  /* raw command response */
  bool Command( string cmdIn )
  {
    bool bRet = false;
    if( cmdIn.length() )
    {
      int cmdLen = cmdIn.length();
      // DBG   cout << "oc_SMTP.Command " << cmdIn.substr(0,50)  << endl;
      // remove ending crlf pair - we'll do that ourselves!
      if( cmdLen > 1 && cmdIn[cmdLen-1] == '\n' )  cmdIn.resize(cmdLen-1);
      if( cmdLen > 2 && cmdIn[cmdLen-2] == '\r' ) cmdIn.resize(cmdLen-2);
      // add the pristine CRLF
      cmdIn += crlf;
      cmdLen = cmdIn.length();

      // if the amount written is the
      //  same as the command length, return is true
      bRet = ( cmdLen == Write( cmdIn.c_str(), cmdLen ) );
    }
    return bRet;
  }
  bool Response( string expected = "250" )
  {
     bool bret = false;
     error = "";
      
     // All SMTP responses are single lines.
     response = ReadLine();
     response.parseInit();
     if( response.parse(" ") == expected )
        bret=true;
     else
        error = response;
     // DBG      cout << "exited oc_SMTP.Response() with " <<  response << endl;
     return bret;
  }
  
  /* specific commands as per IETF RFC821 */

  // 1) establish a connection
  bool helo( string domain )
  {
    bool bret = false;
    string command = "HELO ";
    command += domain;
    if( Command(command.c_str()) ) 
    {
      bret = Response(); 
    }    
    return bret;
  }
  // 2) start mail with the sender indicated
  bool mail_from( string sender )
  {
    bool bret = false;
    string command = "MAIL FROM:<";
    command += sender;
    command += ">";
    if( Command(command.c_str()) ) 
    {
      bret = Response(); 
    }    
    return bret;
  }
  
  // 3) invoke as many times as needed to send to all recipients
  bool rcpt_to( string recipient )
  {
    bool bret = false;
    string command = "RCPT TO:<";
    command += recipient;
    command += ">";
    if( Command(command.c_str()) ) 
    {
      bret = Response(); 
    }    
    return bret;  
  }
  
  // 4) start of message data
  bool data( void )
  {
    bool bret = false;
    string command = "DATA";
    if( Command(command.c_str()) ) 
    {
      writelog2("data Command", command );
      bret = Response("354"); 
    }    
    return bret;   
  }
  
  // 5) write out the message
  bool operator () ( string msg )
  {
    bool bret = false;
    string command;
    // transparency procedure compliance - section 4.5.2. of rfc 821
    if (msg.length() && msg[0] == '.') command=".";
    command += msg;
    bret = Command(command.c_str());
    return bret;
  }
  
  // 6) send the period to end the data
  bool end_data( void )
  {
    bool bret = false;
    string command = crlf;
    command += ".";
    if( Command(command.c_str()) ) 
    {
      bret = Response();
    }
    return bret;    
  }
  
  // send the quit signal to close the com channel
  bool quit( void )
  {
    bool bret = false;
    string command = "QUIT";
    if( Command(command.c_str()) ) 
    {
      bret = Response(); 
    }    
    return bret;      
  }
  // reset
  bool reset( void )
  {
    bool bret = false;
    string command = "RESET";
    if( Command(command.c_str()) ) 
    {
      bret = Response(); 
    }    
    return bret;      
  }
  // no-operation
  bool noop( void )
  {
    bool bret = false;
    string command = "NOOP";
    if( Command(command.c_str()) ) 
    {
      bret = Response(); 
    }    
    return bret;      
  }
  bool vrfy( string address )
  {
    bool bret = false;
    string command = "VRFY ";
    command += address;
    if( Command(command.c_str()) ) 
    {
      bret = Response(); 
    }    
    return bret;      
  }  
};

#endif

#ifdef IN_T2_TESTHARNESS

  // White Box test cases...  
  
  // open server on the local host
  oc_SMTP smtp( "127.0.0.1" );
  cout << smtp.response << endl;
  if( smtp.isConnected )
  {
    if( smtp.helo("127.0.0.1") == false ) 
    {
      cout << "Hello fail " << smtp.error << endl;
      smtp.quit();
      return 0;
    }
    cout << smtp.response << endl;
    
    if( smtp.vrfy("davidmc") )
    {
      cout << "Good! " << smtp.response << endl;
    }
    else
    {
      cout << "Bad! " << smtp.error << endl;
    }
    
    if( smtp.mail_from( "davidmc@w3sys.com" ) == false )
    {
      smtp.quit();
      return 0;
    }
    cout << smtp.response << endl;
    
    if( smtp.rcpt_to( " w3sysdesign@gmail.com" ) == false )
    {
      smtp.quit();
      return 0;
    }
    cout << smtp.response << endl;
    if( smtp.data() == false )
    {
      cout << "Data fail " << smtp.error << endl;
      smtp.quit();
      return 0;  
    }
    cout << smtp.response << endl;
    smtp("From: davidmc@w3sys.com");
    smtp("Sender: davidmc@w3sys.com");
    smtp("To: w3sysdesign@gmail.com");
    smtp("Subject: Testing SMTP");
    smtp("");
    smtp("This is a test");
    smtp.end_data();
    smtp.quit();
  }
#endif
