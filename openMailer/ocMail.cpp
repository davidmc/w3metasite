/*
  Class wrapper for postfix sendmail

  Copyright ocSendMail 2002 w3 systems
  Author: David McCombs of w3 systems

*/

#include <fstream>
#include <string>
#include "ocMail.h"
#include "ocString.h"
#include "ocCoder.h"
// #define DO_OPEN_LOGGING
#include "openLogger.h"



void ocSendMail::getTime( void )
{
  time_t lt;
  time(&lt);
  localtime_r(&lt,&tm_time);
}
string ocSendMail::makeBoundary( void )
{
  ocString ret = "=_-_-Open_Mail_Boundary_-";
  getTime();
  ret.append( tm_time.tm_year );
  ret.append( tm_time.tm_mon );
  ret.append( tm_time.tm_mday );
  ret += "_";
  ret.append( tm_time.tm_hour );
  ret.append( tm_time.tm_min );
  ret.append( tm_time.tm_sec );
  while( m_strBody.find( ret ) != string::npos ) ret += "_=";
  m_beginBoundary = "--";
  m_beginBoundary += ret;
  m_endBoundary = m_beginBoundary;
  m_endBoundary += "--";
  return ret;
}

bool ocSendMail::loadAddresses ( const char * path )
{
  bool bRet = false;
  ifstream addressFile( path );
  if( addressFile )
  {
    char addressBuffer[512];
    addressFile.getline( addressBuffer, sizeof(addressBuffer) );
    while( addressFile.rdstate() == ios::goodbit )
    {
      string valuePair = addressBuffer;
      string::size_type pos = valuePair.find("=");
      if( pos != string::npos )
      {
        string name = valuePair.substr(0, pos); // start, len
        string value = valuePair.substr( pos+1 ); // start - end
        m_addresses[name] = value;
      }
      addressFile.getline( addressBuffer, sizeof(addressBuffer) );
    }
    bRet = true;
  }
  return bRet;
}

const char * ocSendMail::getAddress( const char * name )
{
  const char * retval = NULL;
  strStrMap::iterator pos = m_addresses.find(name);
  if( pos != m_addresses.end() )
  {
    retval = pos->second.c_str();
  }
  return retval;
}

ocSendMail::ocSendMail( const char * sendMailPath )
{
  m_strExePath = sendMailPath;
}

ocSendMail::~ocSendMail()
{
  ;
}

bool ocSendMail::addHeader ( const char * header, const char * value )
{
  bool bRet = false;
  if( header && strlen(header) &&
      value && strlen(value) )
  {
    m_strHeaders += header;
    m_strHeaders += ": ";
    m_strHeaders += value;
    m_strHeaders += "\r\n";
    bRet = true;
  }
  return bRet;
}



bool ocSendMail::setMimeType( const char * type )
{
  bool bRet = false;
  if( type && strlen( type ) )
  {
    m_content_type = "Content-Type";
    m_content_type += ": ";
    m_content_type += type;
    m_content_type += "\r\n";
    bRet = true;
  }
  return bRet;
}

bool ocSendMail::openRoute( const char * from,
                            const char * to,
                            const char * subject,
                            const char * cc,
                            const char * bcc )
{
  bool bRet = false;

  // reset headers and body
  m_strHeaders = "";
  m_strBody = "";

  // construct the process command
  string command = m_strExePath;

  command += " -f\"";
  command += from;
  command += "\" ";
  
  command += to;
  
  if( cc )
  {
    command += ", ";
    command += cc;
  }
  if( bcc )
  {
    command += ", ";
    command += bcc;
  }
  // command += " 2>&1 >sendmail_errors.log"; // this caused more errors than it helped!
  // open the process
  m_pfOut = popen( command.c_str(), "w" );
  if(m_pfOut != NULL)
  {
    bRet = true;
    if( from && strlen(from) )
    {
      addHeader( "From", from );
    }
    if( to && strlen(to) )
    {
      addHeader( "To", to );
    }
    if( cc && strlen(cc) )
    {
      addHeader( "Cc", cc );
    }
    if( bcc && strlen(bcc) )
    {
      addHeader( "Bcc", bcc );
    }
    if( subject && strlen(subject) )
    {
      addHeader( "Subject", subject );
    }
  }
  // default to plain text
  setMimeType( "text/plain" );
  return bRet;
}

bool ocSendMail::write( const char * text )
{
  bool bRet = false;
  if( m_pfOut != NULL && text && strlen(text) )
  {
    bRet = true;
    m_strBody += text;
    m_strBody += "\r\n";
  }
  return bRet;
}

bool ocSendMail::send( void )
{
  bool bRet = false;
  if(m_pfOut != NULL)
  {
    /*
    Importance: Normal | High
    If Attachments...
    MIME-Version: 1.0
    Content-Type: multipart/mixed;
      boundary="----=_NextPart_000_0000_01C263CE.6186C430"
    */
    writelog("writing headers to process");
    fprintf( m_pfOut, m_strHeaders.c_str() );
    // Prepare to attachments
    if( m_files.size() )
    {
      fprintf( m_pfOut, "MIME-Version: 1.0\r\n" );
      fprintf( m_pfOut, "Content-Type: multipart/mixed;\r\n" );
      fprintf( m_pfOut, "\tboundary=\"%s\"\r\n", makeBoundary().c_str() );
      fprintf( m_pfOut, "%s\r\n", m_beginBoundary.c_str() );
    }

    // At some point I will want to add multipart alternative capability here
    fprintf( m_pfOut, m_content_type.c_str() );

    // first ouput the body
    fprintf( m_pfOut, "\r\n" );
    fprintf( m_pfOut, m_strBody.c_str() );
    writelog("wrote body");

    // finish with attachments, if any
    if( m_files.size() )
    {
      for( ocFiles::iterator pos = m_files.begin();
           pos != m_files.end();
           pos ++ )
      {
         writelog2( "Writing out attachment: ",  pos->second.name );
         fprintf( m_pfOut, "%s\r\n", m_beginBoundary.c_str() );
         fprintf( m_pfOut, "Content-Type: %s;\r\n", pos->second.type.c_str() );
         fprintf( m_pfOut, "\tname=\"%s\"\r\n", pos->second.name.c_str() );
         fprintf( m_pfOut, "Content-Transfer-Encoding: base64\r\n" );
         fprintf( m_pfOut, "Content-Disposition: attachment;\r\n" );
         fprintf( m_pfOut, "\tfilename=\"%s\"\r\n\r\n", pos->second.name.c_str() );
         decodeFile( pos );
      }
      fprintf( m_pfOut, "%s\r\n", m_endBoundary.c_str() );
    }
    writelog( "closing process" );
    bRet = ( pclose( m_pfOut ) != -1 );
    bRet = true;
  }
  return bRet;
}

void ocSendMail::decodeFile( ocFiles::iterator & pos )
{
  ifstream testen( pos->second.path.c_str() );
  ocCoder enc;
  unsigned char buff[64];
  while( testen.eof() == false )
  {
    // Cause 57 is 3/4 of 76
    testen.read((char*)buff,57);
    string & decoded = enc.base64encode(buff,testen.gcount());
    writelog2( "Decoded: ", decoded );
    fprintf( m_pfOut, "%s\r\n", decoded.c_str() );
  }
}

bool ocSendMail::attach ( ocFile & inFile )
{
  bool bRet = false;
  m_files[ inFile.name ] = inFile;
  return bRet;
}

bool ocSendMail::urlAttach ( const char * url )
{
  bool bRet = false;
  if( m_pfOut != NULL && url && strlen(url) )
  {
    bRet = true;
    m_strBody += " &nbsp; <a href='";
    m_strBody += url;
    m_strBody += "'>Attachment</a> &nbsp; ";
  }
  return bRet;
}

