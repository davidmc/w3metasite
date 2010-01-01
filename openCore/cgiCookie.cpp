#include "cgiCookie.h"

queryStringMap cgiCookie::cookieMap; // the static member parsed associative array
aString cgiCookie::m_strCookies;     // the static member unparsed cookies

cgiCookie::cgiCookie():cgiBase()
{
  // the unparsed cookie string and the map are now static
  if( m_strCookies.length() == 0 )
  { 
    m_strCookies = getenv("HTTP_COOKIE");
    char * pchTok = m_strCookies.token( ";" );
    
    while ( pchTok && strlen( pchTok ) )
    {
      // a string we can modify
      aString subToken = pchTok;
  
      // get the name first.
      aString tmpRawName = subToken.token( "=" );
      string tmpName = tmpRawName.trim(aString::both).str();
  
      // get the variable(s)
      aString tmpVal = subToken.token( "=" );
      // replace the chars encoded with '%XX'
      tmpVal.replaceFoundWith("%3d","=");
      tmpVal.replaceFoundWith("%3b",";");
      
      // add element to (or modify element of) the collection
      queryStringMap::iterator pos = cookieMap.find(tmpName);
  
      if( pos == cookieMap.end() )
      {
        cookieMap.insert(make_pair(tmpName,tmpVal));
      }
      pchTok = m_strCookies.token( ";" );
    }
  }
}

cgiCookie::~cgiCookie(){;}

string & cgiCookie::setTimeout( int month, int day, int year, int hour, int minute )
{
  char res[512];
  memset( &m_tm, 0, sizeof(m_tm) );
  m_tm.tm_min = minute;
  m_tm.tm_hour = hour;
  m_tm.tm_mon = month-1;
  m_tm.tm_mday = day;
  m_tm.tm_year = year-1900;
  mktime( & m_tm );
  strftime(res, sizeof(res), "%a %d-%b-%Y %H:%M:%S GMT", &m_tm );
  m_strExpires = res;
  return m_strExpires;
}

void cgiCookie::setAllCookies( void )
{
  m_strCookies = "";
  for( queryStringMap::iterator pos = cookieMap.begin();
       pos != cookieMap.end();  )
  {
    m_strCookies += pos->first.c_str();
    m_strCookies += "=";
    m_strCookies += pos->second.c_str();
    ++pos;
    if( pos != cookieMap.end() )
      m_strCookies += ";";
  }
  setenv("HTTP_COOKIE", m_strCookies.c_str(),1);
}
string & cgiCookie::setPath( const char * path )
{
  m_strPath  = path;
  return m_strPath;
}

string & cgiCookie::setDomain( const char * domain )
{
  m_strDomain = domain;
  return  m_strDomain;
}

bool cgiCookie::set( const char* name, const char * value )
{
  //Set-Cookie: foo=; path=/; expires Thu, 01-Jan-1970 00:00:00 GMT
  if ( name && value && strlen(name) )
  {
    // replace any = or ; signs with the %XX encoding
    aString aValue = value;
    aValue.replaceFoundWith("=","%3d" );
    aValue.replaceFoundWith(";","%3b");
    *this << "Set-Cookie: " << name << "=" << aValue;
    cookieMap[string(name)] = aString(value);
  }
  if ( m_strPath.length() )
  {
    *this << "; path=" << m_strPath;   
  }
  if ( m_strDomain.length() )
  {
    *this << "; domain=" << m_strDomain;
  }
  if( m_strExpires.length() )
  {
    *this << "; expires=" << m_strExpires;
  }  
  *this << endLine;

  return true;
}

string & cgiCookie::get( const char * key )
{
  m_strGotCookie = "";

  queryStringMap::iterator pos;
  pos = cookieMap.find( string(key) );
  if( pos != cookieMap.end() )
  {
    m_strGotCookie = pos->second.str();
  }

  return m_strGotCookie;
}
aString & cgiCookie::allCookies( void )
{
  return m_strCookies;
}
