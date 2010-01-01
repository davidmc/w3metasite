/*
  Class wrapper for postfix sendmail
  Postfix was written by:
   Wietse Venema of the IBM T.J. Watson Research (while on sabbatical.)
  Copyright ocSendMail 2002 - w3 systems development
  Author: David McCombs of - w3 systems development
*/
#ifndef OC_MAIL_H
#define OC_MAIL_H

#include <cstdio> 
#include <string> 
#include <map>
#include "ocCommon.h" 

extern "C"{
#include <time.h>
}

using namespace std;

typedef map< string, string> strStrMap;

/*
  class definition
*/
class ocSendMail
{
private:
  string     m_strExePath;
  string     m_strHeaders;
  string     m_strBody;
  string     m_beginBoundary;
  string     m_endBoundary;
  FILE *     m_pfOut;
  strStrMap  m_addresses;
  string     m_content_type;
  ocFiles    m_files;
  tm         tm_time;

  void getTime( void );
  string makeBoundary( void );
  void decodeFile( ocFiles::iterator & pos );
public:

  // constructor & destructor
  ocSendMail( const char * sendMailPath );
  ~ocSendMail();

  bool loadAddresses ( const char * path );
  const char * getAddress( const char * name );

  // open mail route to send mail to
  bool openRoute( const char * from,
                  const char * to,
                  const char * subject,
                  const char * cc = NULL,
                  const char * bcc = NULL  );

  // add a custom header
  bool addHeader ( const char * header, const char * value );

  // set the mime type (text/plain by default)
  bool setMimeType( const char * type );

  // write a line into the message body
  bool write( const char * text );

  // send the whole mail message
  bool send( void );

  // add inline attachment
  bool attach ( ocFile & inFile );

  // add an attachment as a public url
  bool urlAttach ( const char * url );

  // return the body
  inline string & body( void ) { return  m_strBody; }

};

#endif

