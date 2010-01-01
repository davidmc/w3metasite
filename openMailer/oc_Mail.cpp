/*
  Class wrapper for ocSMTP

  Copyright ocSendMail 2002 w3 systems
  Author: David McCombs of w3 systems

*/

#include <fstream>
#include <string>
#include <uuid/uuid.h>
#include "oc_Mail.h"
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

string ocSendMail::makeUUID(void)
{
  string UUID;
  uuid_t uuid;
  char uuid_val[37];
  memset(uuid,'\0',sizeof(uuid));
  uuid_generate(uuid);
  uuid_unparse(uuid, uuid_val);
  ocString uuid_work(uuid_val);
  UUID = uuid_work.replaceAll("-","");
  return UUID;
}

string ocSendMail::makeBoundary( void )      
{
  // TODO: use uuid for this !!!
  ocString ret = "-Open_Mail_Boundary-";
  getTime();
  ret += makeUUID();
  m_beginBoundary = "--";
  m_beginBoundary += ret;
  m_endBoundary = m_beginBoundary;
  m_endBoundary += "--";
  return ret;
}

bool ocSendMail::loadAddresses ( string path )
{
  bool bRet = false;
  ifstream addressFile( path.c_str() );
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

string ocSendMail::getAddress( string name )
{
  string retval;
  strStrMap::iterator pos = m_addresses.find(name);
  if( pos != m_addresses.end() )
  {
    retval = pos->second.c_str();
  }
  return retval;
}

ocSendMail::ocSendMail( string address, int port ):smtp(address,port)
{
  // default to plain text
  setMimeType( "text/plain" );
}

ocSendMail::~ocSendMail()
{
  ;
}

bool ocSendMail::addHeader ( string header, string value )
{
  bool bRet = false;
  if( header.length() &&
      value.length() )
  {
    m_strHeaders = header;
    m_strHeaders += ": ";
    m_strHeaders += value;
    bRet = smtp(m_strHeaders);
  }
  return bRet;
}

bool ocSendMail::setMimeType( string type )
{
  bool bRet = false;
  if( type.length() )
  {
    m_content_type = "Content-Type";
    m_content_type += ": ";
    m_content_type += type;
    m_content_type += "\r\n";
    bRet = true;
  }
  return bRet;
}

bool parseRecipients( ocString addresses  )
{


}
bool ocSendMail::openRoute( string from,
                            string to,
                            string subject,
                            string cc,
                            string bcc,
                            string serverAddress  )
{
  bool bRet = false;

  // reset headers and body
  m_strHeaders = "";
  m_strBody = "";

  if( smtp.isConnected ) // in constuctor
  {
    if( smtp.helo(serverAddress) == false )
    {
      send_result += "Hello fail ";
      send_result += smtp.error;
      send_result += "\n";
      smtp.quit();
      return bRet;
    }
    
    if( smtp.mail_from(from)  == false )
    {
      send_result += "Mail fail ";
      send_result += smtp.error;
      send_result += "\n";
      smtp.quit();
      return bRet;
    }
    writelog( smtp.response );

    if( smtp.rcpt_to( to ) == false )
    {
      send_result = "Recipient fail " + smtp.error;
      smtp.quit();
      return bRet;
    }
    
    if( smtp.data() == false )
    {
      send_result += "Data fail";
      send_result += smtp.error;
      send_result += "\n";
      smtp.quit();
      return bRet;
    }
    bRet = true;
    if( from.length() )
    {
      addHeader( "From", from );
      addHeader( "Sender", from );
    }
    if( to.length() )
    {
      addHeader( "To", to );
    }
    if( cc.length() )
    {
      addHeader( "Cc", cc );
    }
    if( bcc.length() )
    {
      addHeader( "Bcc", bcc );
    }
    if( subject.length() )
    {
      addHeader( "Subject", subject );
    }
  }
  else
  {
    send_result =  "Did not recieve proper connect (220) response: " + smtp.response;
  }
  return bRet;
}

bool ocSendMail::write( string text )
{
  bool bRet = false;
  if( text.length() )
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
  if(smtp.isConnected)
  {
    /*
    Importance: Normal | High
    If Attachments...
    MIME-Version: 1.0
    Content-Type: multipart/mixed;
      boundary="----=_NextPart_000_0000_01C263CE.6186C430"
    */

    // Prepare to attachments
    if( m_files.size() )
    {
      smtp( "MIME-Version: 1.0" );
      smtp( "MIME-Version: 1.0" );
      smtp( "Content-Type: multipart/mixed;" );
      string boundary = "\tboundary=\"";
      boundary += makeBoundary();
      smtp( boundary );
      boundary = m_beginBoundary;
      smtp( boundary );
    }

    // At some point I will want to add multipart alternative capability here
    
    smtp( m_content_type );

    // first ouput the body
    smtp( "" ); // crlf to seperate the headers from body
    
    smtp( m_strBody.replaceAll( "\n.\n", "\n..\n" ) );
    
    writelog2("wrote body",m_strBody);

    // finish with attachments, if any
    if( m_files.size() )
    {
      string tmp;
      for( ocFiles::iterator pos = m_files.begin();
           pos != m_files.end();
           pos ++ )
      {
        writelog2( "Writing out attachment: ",  pos->second.name );
        smtp( m_beginBoundary );
        tmp = "Content-Type: ";
        tmp += pos->second.type;
        tmp += "\"";
        smtp(  tmp );
        tmp = "\tname=\"";
        tmp += pos->second.name;
        tmp += "\"";
        smtp(  tmp );
        smtp(  "Content-Transfer-Encoding: base64" );
        smtp(  "Content-Disposition: attachment;" );
        tmp = "\tfilename=\"";
        tmp += pos->second.name;
        tmp += "\"";
        smtp(  tmp );
        smtp( "" ); // crlf to seperate the file header from file
        decodeFile( pos );
      }
      smtp(  m_endBoundary );
    }
    writelog( "closing mail" );
    smtp.end_data();
    smtp.quit();
    bRet = true;
  }
  else
  {
    writelog("not connected!!!!!!!!!!");
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
    smtp( decoded );
  }
}

bool ocSendMail::attach ( ocFile & inFile )
{
  bool bRet = false;
  m_files[ inFile.name ] = inFile;
  return bRet;
}

