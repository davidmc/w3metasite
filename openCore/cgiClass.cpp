/*
  cgiClass.C

  This file contains the implementation of
  the cgi object methods

  copyright (c) 1996 - 2002

  David McCombs davidmc@newcottage.com

  Open Core Class Library

*/

#include "cgiClass.h"
#include <cstdlib> // for getenv()
#include "ocString.h"

// #define DO_OPEN_LOGGING
#include "openLogger.h"

// helper class to parse multipart data
enum multipart_state
{
  init = 0,
  newData,  // past a boundary
  dataSep,  // separator between attributes and data
  fileType, // expect a content-type marker for file content
  fileSep,  // separator between attributes and file data
  readData, // on data line
  readFile, // in file buffer
  fileRead, // after file buffer
  finished, // finished with data (or file) and on a boundary
  eof       // end of parse found by closing boundary
};
class multipart
{
  string boundary;
  size_t boundaryLen;
  string fileBoundary;
  string endboundary;
  size_t fileBoundaryLen;
  string clrf;
  ocString testline;
  string name;
  string filename;
  string value;
  string type;
  char * testdata;
  multipart_state state;
  queryStringMap & rMap;
  ocFiles        & rFileMap;
  string path;
public:
  multipart( string inBoundary, queryStringMap & inVars, ocFiles  & fileMap, string iPath )
  :testdata(NULL),boundaryLen(0),state(init),rMap(inVars),rFileMap(fileMap),path(iPath)
  {
    clrf = "\r\n";
    fileBoundary = clrf;
    boundary = inBoundary;
    writelog2( "boundary: ", boundary );
    fileBoundary += inBoundary;
    endboundary = boundary;
    endboundary += "--";
    writelog2( "endboundary: ", endboundary );
    boundaryLen = boundary.length();
    fileBoundaryLen = fileBoundary.length();
    testdata = new char[fileBoundaryLen+1]; // for terminating null
    memset(testdata, 0, fileBoundaryLen+1); // initially zero length string
  }
  ~multipart()
  {
    delete testdata;
  }
  bool getline(  istream & argstream )
  {
    char term;
    if( argstream.rdstate() == ios::goodbit &&
        state != readFile ) // don't want to steal first line from inline file
    {
      // try this:      
      std::getline(argstream, testline);
      
      // remove the CR of the CRLF pair
      if( testline.length() ) testline.resize( testline.length() - 1 );
      // prepare testline for parsing
      testline.parseInit();
      // to the log if loggin enabled
      writelog2( "multipart::getline got: ", testline );
    }
    return true;
  }
  void addFile( void )
  {
    ocFile tempFile;
    tempFile.name=value;
    tempFile.path=filename;
    tempFile.type=type;
    rFileMap[value] = tempFile;
  }
  void addDataItem( void )
  {
    // add element to (or modify element of) the collection
    queryStringMap::iterator pos = rMap.find(name);
    aString tmpVal(value);
    if( pos == rMap.end() )
    {
      rMap.insert(make_pair(name,tmpVal));
    }
    else
    {
      rMap[name] += "|";
      rMap[name] += tmpVal;
    }
  }
  void fixupFilename( string temp )
  {
    value = "";
    filename = "";
    writelog2("fixupFilename: checking length of",temp);
    if(temp.length())
    {
      // now for stupid IE browsers that give us the WHOLE MSDOSLIKE FILE PATH!            
      string::size_type idx = temp.find_last_of("\\");
      if( idx != string::npos )
      {
        temp = temp.substr(idx+1);
      }       
      // finally set the filename
      writelog("fixupFilename adding path");
      filename = path;
      writelog("fixupFilename adding filename")
      filename += temp;
      value = temp;
    }    
    writelog2("fixupFilename  Done!", value);
  }
  // data consumption is line oriented until we get to a file attachment
  bool consume( istream & argstream )
  {
    bool bret = true;
    ocString test;
    while(  argstream.rdstate() == ios::goodbit &&
            state != eof &&
            getline( argstream ) )
    {
      writelog2("Consumed",testline);
      switch( state )
      {
      case init:
        // check to see if we just consumed the boundary
        if( boundary != testline )
        {
          state = eof;
        }
        else
        {
          state = newData;
        }
        break;
      case newData:
        // should be reading the content disposition line
        test = testline.parse(": ");
        transform(test.begin(),test.end(),test.begin(),::tolower);
        writelog2("Testing",test);
        if( test == "content-disposition" )
        {
          // good - see what the data is
          test = testline.parse("; ");          
          // Added :: prefix to tolower so SGI recognizes global scope C function
          transform(test.begin(),test.end(),test.begin(),::tolower);          
          // expect it to be form-data
          writelog2("Content Testing",test);
          if( test == "form-data" )
          {
            // parse any remaining parameters
            while( testline.length() && !testline.endOfParse() )
            {
              ocString test = testline.parse("; ");
              writelog2("Param Testing",test);
              if( test.length() )
              {
                string paramname=test.parse("=\"");
                // Added :: prefix to tolower so SGI recognizes global scope C function
                transform(paramname.begin(),paramname.end(),paramname.begin(),::tolower);
                // could be name or filename
                if( paramname == "name" )
                {
                  // set the name
                  name = test.parse("\"");
                  state = dataSep;
                }
                else if( paramname == "filename" )
                {
                  writelog2("Fix Filename",test);
                  fixupFilename(test.parse("\""));
                  writelog("Filename Fixed");
                  state = fileType;
                }
              }
            }
          }
        }
        else
        {
          // unexpected place so abort method call
          state = eof;
        }
        break;
      case fileType:
        // expect Content-Type: image/jpeg
        test = testline.parse(": ");
        // Added :: prefix to tolower so SGI recognizes global scope C function
        transform(test.begin(),test.end(),test.begin(),::tolower);
        if( test == "content-type" )
        {
          type = testline.remainder();
          state = fileSep;
        }
        else
        {
          state = eof;
        }
        break;
      case fileSep:
        if(testline.length() == 0)
        {
          state = readFile;
        }
        else
        {
          state = eof;
        }
        break;
      case dataSep:
        // expect an empty line
        if(testline.length() == 0)
        {
          state = readData;
        }
        else
        {
          state = eof;
        }
        break;
      case readData:
        // expect the data
        dataConsume(argstream);
        break;
      case readFile:
        fileConsume(argstream);
        addFile();
        addDataItem();
        // fileConsume also consumes the boundary (up to the (potential) -- closing)
        state = fileRead;
        break;
      case fileRead:
        if( testline == "--" )
        {
          state = eof;
        }
        state = newData;
        break;
      case finished:
        // see if there is another boundary:
        if( boundary == testline )
        {
          state = newData; // note that the terminating boundary will have -- at the end
        }
        else
        {
          state = eof;
        }
        break;
      default:
        state = eof;
        break;
      } // end switch
    }  // end while
    return bret;
  }
  // Data consumption
  bool dataConsume( istream & argstream )
  {
    bool bRet = false;
    if( argstream.rdstate() == ios::goodbit )
    {
      value = testline;
      while( argstream.rdstate() == ios::goodbit &&
             getline( argstream ) &&
             testline != boundary &&
             testline != endboundary )
      {
        value +="\n";
        value += testline;
      }

      // set the proper state
      if(testline == boundary) state = newData;
      else if(testline == endboundary) state = eof;
      // add the whole item
      addDataItem();
    }
    else
    {
      state = eof;
    }
    return bRet;
  }

  // File consumption
  bool fileConsume( istream & argstream )
  {
    char c = '\0';
    size_t pos = 0;
    ofstream ofile;
    bool haveFile = filename.length() > 0;
    writelog2( "Consuming and saving file: ", filename );
    if( haveFile )
    {
      ofile.open( filename.c_str(), ios::out | ios::trunc | ios::binary );
    }
    writelog2( "fileBoundaryLen: " , fileBoundaryLen );
    // fill the buffer
    while( argstream.rdstate() == ios::goodbit &&
           pos < fileBoundaryLen &&
           argstream.get(c) )
    {
      testdata[pos++] = c;
    }
    writelog2( "final file testdata pos: ", pos-1 )

    // scan the buffer
    while( argstream.rdstate() == ios::goodbit &&
           fileBoundary != testdata &&
           argstream.get(c) )
    {
      if( haveFile )  ofile.put(testdata[0]);
      memmove( testdata, testdata + 1, fileBoundaryLen );
      testdata[fileBoundaryLen-1]=c;
    }
    writelog2( "Closing file: ", boundary );
    if( haveFile ) ofile.close();
    
    return true;
  }

  // file dump of cgi input
  bool fileDump( istream & argstream )
  {
    char c = '\0';
    size_t pos = 0;
    ofstream ofile;
    filename = "cgiDump.log";    
    writelog2( "Dumping file: ", filename );
    ofile.open( filename.c_str(), ios::out | ios::trunc | ios::binary );    
    while( argstream.rdstate() == ios::goodbit &&
           argstream.get(c) )
    {
      ofile.put(c);
    }    
    ofile.close();
    
    return true;
  }
};

//
// method implementations for cgiInput class.
//

cgiInput::cgiInput()
{
  ;
}

cgiInput::~cgiInput()
{;}
void cgiInput::setMultipart( aString boundary )
{
  string path;
  if( uploadPath.length() )
  {
    path = uploadPath.str();
    int pathlength =  path.length();
    if( pathlength  && path[pathlength-1] != '/' )
    {
      path += "/";
    }
  }
  multipart mp(boundary.str(),theMap,fileMap,path);
  mp.consume( cin );
  
  // mp.fileDump( cin ); // for testing
  writelog2("cgiInput::setMultipart mp.consume( cin ) called",boundary);
}

void cgiInput::set( const char * queryString, size_t size )
{
  if(  queryString && strlen(queryString) > 0 )
  {
    if( size )
    {
      safe.setSize( size+1 );
      char * buf = (char*) safe;
      if( buf )
      {
        memcpy( buf, queryString, size );
        buf[size] = '\0';
      }
    }
    else
    {
      safe = queryString;
    }
    
    const char * pchTok = safe.token( "&" );

    while ( pchTok && strlen( pchTok ) )
    {
      // a string we can modify
      aString subToken = pchTok;
      subToken.deHexify('%');
      subToken.replaceFoundWith("+"," ");
      subToken.replaceFoundWith("+"," ");
      writelog2("cgiInput::set() subToken",subToken);
      // get the name first.
      string tmpName = subToken.token( "=" );

      // get the variable(s)
      aString tmpVal = subToken.remainder();

      // add element to (or modify element of) the collection
      queryStringMap::iterator pos = theMap.find(tmpName);

      if( pos == theMap.end() )
      {
        theMap.insert(make_pair(tmpName,tmpVal));
      }
      else
      {
        theMap[tmpName] += "|";
        theMap[tmpName] += tmpVal;
      }
      pchTok = safe.token( "&" );
    }
  }
}

aString & cgiInput::Safe(void)
{
  return safe;
}
queryStringMap & cgiInput::TheMap(void)
{
  return theMap;
}
ocFiles & cgiInput::FileMap(void)
{
  return fileMap;
}
int  cgiInput::count( const char * key )
{
  int retVal = 0;
  queryStringMap::iterator pos;
  pos = theMap.find(key);
  if( pos != theMap.end() )
  {
    aString & whole = theMap[key];
    while( whole.token("|") )
    {
      retVal++;
    }
  }
  return retVal;
}
aString & cgiInput::operator [] ( const char * key )
{
  aString & returnValue = theMap[key];
  return returnValue;
}
// default case
bool cgiEnvironment::readOnCreateMode = true;
void cgiEnvironment::ReadOnCreateMode( bool doRead )
{
  readOnCreateMode = doRead;
}

// manual read
bool cgiEnvironment::readInput( void )
{
  if( requestMethod.match("GET") )
  {
    clientArguments.set( queryString.str() );
  }
  else if( requestMethod.match("POST") )
  {
    if( contentType == "multipart/form-data" )
    {
      // client arguments come as boundary delimited values
      clientArguments.setMultipart( contentBoundary );
      writelog("cgiEnvironment::cgiEnvironment back from clientArguments.setMultipart()");
    }
    else
    {
      writelog2( "cgiEnvironment::cgiEnvironment(post buf size: ", contentSize );

      char * tempBuf = new char[ contentSize + 1 ];
      memset(tempBuf,0,contentSize + 1);
      cin.read( tempBuf, contentSize );
      writelog2( "cgiEnvironment::cgiEnvironment(post buffer content: ",tempBuf);
      tempBuf[contentSize]='\0';
      clientArguments.set( tempBuf, contentSize );
      delete [] tempBuf;

    }
      // In addition - get any values on the query string
    if( queryString.length() )
    {
      writelog2("cgiEnvironment::cgiEnvironment found additional data in query string: ", queryString );
      clientArguments.set( queryString.str() );
    }
  } // end else POST method
  return true; 
}

//
// method implementations for cgiEnvironment class.
//
cgiEnvironment::cgiEnvironment(const char * uploadPath) : contentSize(0)
{
  contentLength =     getenv("CONTENT_LENGTH");
  contentType =       getenv("CONTENT_TYPE");
  if( uploadPath ) clientArguments.uploadPath = uploadPath;
  aString aTemp = contentType.token(";");
  if( contentType.remainderPosition() != -1 )
  {
    contentType.token("=");
    /* rfc1521:
       7.2.3. The Multipart/alternative subtype
       Need 2 dashes + boundary for matching
    */
    contentBoundary = "--";
    contentBoundary += contentType.remainder();
    contentType = aTemp;
    contentType = contentType.lower();
  }
  
  gatewayInterface =  getenv("GATEWAY_INTERFACE");
  httpAccept =        getenv("HTTP_ACCEPT");
  httpUserAgent =     getenv("HTTP_USER_AGENT");
  httpReferer =       getenv("HTTP_REFERER");
  pathInfo =          getenv("PATH_INFO");
  pathTranslated =    getenv("PATH_TRANSLATED");
  queryString =       getenv("QUERY_STRING");
  remoteAddr =        getenv("REMOTE_ADDR");
  remoteHost =        getenv("REMOTE_HOST");
  remoteIdent =       getenv("REMOTE_IDENT");
  requestMethod =     getenv("REQUEST_METHOD");
  remoteUser =        getenv("REMOTE_USER");
  scriptName =        getenv("SCRIPT_NAME");
  serverName =        getenv("SERVER_NAME");
  serverPort =        getenv("SERVER_PORT");
  serverProtocol =    getenv("SERVER_PROTOCOL");
  serverSoftware =    getenv("SERVER_SOFTWARE");

  contentSize = atoi( contentLength.str() );

  if( readOnCreateMode ) readInput();

  writelog("cgiEnvironment::cgiEnvironment contructor done" );
}
cgiEnvironment::~cgiEnvironment()
{
  ;
}
// contains the total number of characters in the user input
aString & cgiEnvironment::ContentLength(void)
{
  return contentLength;
}
aString & cgiEnvironment::ContentType(void)
{
  return contentType;
}
aString & cgiEnvironment::GatewayInterface(void)
{
  return gatewayInterface;
}
aString & cgiEnvironment::HttpAccept(void)
{
  return httpAccept;
}
aString & cgiEnvironment::HttpUserAgent(void)
{
  return httpUserAgent;
}
aString & cgiEnvironment::HttpReferer(void)
{
  return httpReferer;
}
aString & cgiEnvironment::PathInfo(void)
{
  return pathInfo;
}
aString & cgiEnvironment::PathTranslated(void)
{
  return pathTranslated;
}
aString & cgiEnvironment::QueryString(void)
{
  return queryString;
}
aString & cgiEnvironment::RemoteAddr(void)
{
  return remoteAddr;
}
aString & cgiEnvironment::RemoteHost(void)
{
  return remoteHost;
}
aString & cgiEnvironment::RemoteIdent(void)
{
  return remoteIdent;
}
aString & cgiEnvironment::RequestMethod(void)
{
  return requestMethod;
}
aString & cgiEnvironment::RemoteUser(void)
{
  return remoteUser;
}
aString & cgiEnvironment::ScriptName(void)
{
  return scriptName;
}
aString & cgiEnvironment::ServerName(void)
{
  return serverName;
}
aString & cgiEnvironment::ServerPort(void)
{
  return serverPort;
}
aString & cgiEnvironment::ServerProtocol(void)
{
  return serverProtocol;
}
aString & cgiEnvironment::ServerSoftware(void)
{
  return serverSoftware;
}
cgiInput & cgiEnvironment::ClientArguments(void)
{
  return clientArguments;
}
aString & cgiEnvironment::ContentBoundary(void)
{
  return contentBoundary;
}
// cgi Base Class methods
cgiBase::cgiBase():ostream(cout.rdbuf()),endLine("\r\n"),id(cgi)
{
  ;
}
cgiBase::cgiBase(cgiBase& input):ostream(cout.rdbuf()),endLine("\r\n"),id(input.id)
{
  ;
}
const char * cgiBase::tag( void )
{
  return opening.str();
}
cgiBase::~cgiBase()
{  
}


cgiScript & cgiScript::DebugString( void )
{
   *this << "<pre>" << endl;
   *this << "mime type: " << mimeType << endl;
   if( RequestMethod().length() > 0 )
   {
     *this << "ContentLength: [[" << ContentLength() << "]]"<< endl;
     *this << "ContentType: [[" << ContentType() << "]]"<< endl;
     *this << "ContentBoundary: [[" << ContentBoundary()  << "]]"<< endl;
     *this << "GatewayInterface: [[" << GatewayInterface() << "]]"<< endl;
     *this << "HttpAccept: [[" << HttpAccept() << "]]"<< endl;
     *this << "HttpUserAgent: [[" << HttpUserAgent() << "]]"<< endl;
     *this << "PathInfo: " << PathInfo() << "]]"<< endl;
     *this << "PathTranslated: [[" << PathTranslated() << "]]"<< endl;
     *this << "QueryString: [[" << QueryString() << "]]"<< endl;
     *this << "RemoteAddr: [[" << RemoteAddr() << "]]"<< endl;
     *this << "RemoteHost: [[" << RemoteHost() << "]]"<< endl;
     *this << "RemoteIdent: [[" << RemoteIdent() << "]]"<< endl;
     *this << "RequestMethod: [[" << RequestMethod() << "]]"<< endl;
     *this << "RemoteUse: [[" << RemoteUser() << "]]"<< endl;
     *this << "ScriptName: [[" << ScriptName() << "]]"<< endl;
     *this << "ServerName: [[" << ServerName() << "]]"<< endl;
     *this << "ServerPort: [[" << ServerPort() << "]]"<< endl;
     *this << "ServerProtocol: [[" << ServerProtocol() << "]]"<< endl;
     *this << "ServerSoftware: [[" << ServerSoftware() << "]]"<< endl;
     *this << "Saved Query String: [[" << ClientArguments().Safe().str() << "]]"<< endl;
   }
   else
   {
     *this << "Invalid call to cgi program - invalid environment variables." << endl;
   }
  *this << endl <<"Client Arguments:" << endl;
   cgiInput & cgiInput = ClientArguments();
   queryStringMap::iterator pos;

   for( pos = cgiInput.TheMap().begin();
        pos != cgiInput.TheMap().end();
        ++pos )
   {
     *this << pos->first.c_str() << ": [[";
     *this << pos->second << "]]" << endl;
   }


   *this << "</pre>" << endl;
   *this << ends;
   return *this;
}

// methods for cgiScriptLite...
cgiScriptLite::cgiScriptLite( const char * mimeString, bool bCloseHeader, bool cache )
{
  id = cgi;
  mimeType=mimeString;
  opening="Content-type: ";
  opening+=mimeType;
  opening+=endLine;
  close=endLine;
  if( !cache )
  {
    openHeader();
  }
  this->flush();
  if (bCloseHeader ) closeHeader();
}
void cgiScriptLite::openHeader( const char * mimeString )
{
  if( mimeString )
  {
    mimeType=mimeString;
    opening="Content-type: ";
    opening+=mimeType;
    opening+=endLine;
  }
  *this << opening; 
  this->flush(); 
}
void cgiScriptLite::closeHeader( void )
{
  *this  << endLine; 
  this->flush(); 
}

cgiScriptLite::~cgiScriptLite()
{
  *this << close << endLine;
}

void cgiScriptLite::Redirect( const char * location )
{
  *this << "Location: " << location << endLine << endLine;
}


/*
  The cgi script class methods
*/

cgiScript::cgiScript( const char * mimeString, 
                      bool bCloseHeader, const char * uploadPath, bool bCache)
:cgiScriptLite(mimeString,bCloseHeader,bCache),cgiEnvironment(uploadPath)
{
  ;
}

cgiScript::~cgiScript()
{
  ;
}


// The html tag container methods

cgiHtml::cgiHtml( char * attr)
{
  id = html;
  opening="<html";
  opening += attr;
  opening += ">";
  close="</html>";
  *this << opening << endl;
}
cgiHtml::~cgiHtml()
{
  *this << close << endl;
}




// the head tag container methods

cgiHead::cgiHead( char * attr)
{
  id = head;
  opening="<head";
  opening += attr;
  opening += ">";
  close="</head>";
  *this << opening << endl;
}
cgiHead::~cgiHead()
{
  *this << close << endl;
}


// the body tag container methods

cgiBody::cgiBody(char * attr)
{
  id = body;
  opening="<body";
  opening += attr;
  opening += ">";
  close="</body>";
  *this << opening << endl;
}
cgiBody::~cgiBody()
{
  *this << close << endl;
}



