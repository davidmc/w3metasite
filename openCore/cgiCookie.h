#ifndef CGI_COOKIE_H
#define CGI_COOKIE_H 111

#include "cgiClass.h"
#include <ctime>
#include<memory.h>

class cgiCookie : public cgiBase
{

private:
  tm     m_tm;
  string m_strExpires;
  string m_strPath;
  string m_strDomain;
  static aString m_strCookies;  // aString type has easier parse
  static queryStringMap cookieMap; // the parsed associative array
  string m_strGotCookie; // one copy 

public:
  cgiCookie();
  virtual ~cgiCookie();
  string & setTimeout( int month, int day, int year, int hour, int minute );
  string & setPath( const char * path );
  string & setDomain( const char * domain );
  bool set( const char* name, const char * value );
  string & get( const char * key );

  bool set( string name, string value ){ return  set( name.c_str(), value.c_str() );}
  string & get( string key ) { return get( key.c_str() ); }
  
  aString & allCookies( void );
  void setAllCookies( void );
};

#endif
