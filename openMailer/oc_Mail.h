/*
  oc_Mail.h
  Class wrapper for SMTP
  Author: David McCombs, w3 systems development
  Copyright 2003
  
*/
#ifndef OC_MAIL_H
#define OC_MAIL_H

#include <cstdio> 
#include <string> 
#include <map>
#include "ocCommon.h"
#include "ocString.h"
#include "oc_SMTP.h" 

extern "C"{
#include <time.h>
}

typedef map< string, string> strStrMap;

/*
  Class definition
*/
class ocSendMail
{
private:
  string     m_strExePath;
  string     m_strHeaders;
  ocString   m_strBody;
  string     m_beginBoundary;
  string     m_endBoundary;
  strStrMap  m_addresses;
  string     m_content_type;
  ocFiles    m_files;
  tm         tm_time;
  oc_SMTP    smtp;
  string     send_result;
  
  void getTime( void );
  string makeBoundary( void );
  void decodeFile( ocFiles::iterator & pos );
public:

  
  // constructor & destructor
  ocSendMail( string address = "127.0.0.1", int port=25  );
  ~ocSendMail();

  bool loadAddresses ( string path );
  string getAddress( string name );

  // open mail route to send mail to
  bool openRoute( string from,
                  string to,
                  string subject,
                  string cc = "",
                  string bcc = "",
                  string serverAddress = "127.0.0.1" );

  // add a custom header
  bool addHeader ( string header, string value );

  // set the mime type (text/plain by default)
  bool setMimeType( string type );

  // write a line into the message body
  bool write( string text );

  // send the whole mail message
  bool send( void );

  // add inline attachment
  bool attach ( ocFile & inFile );

  // useful, so make it available
  string makeUUID(void);

  // last string of results of SMTP commands
  inline string & sendResult( void )
  { return  send_result; }
  
    // return the body
  inline string & body( void )
  { return  m_strBody; }

};

#endif

