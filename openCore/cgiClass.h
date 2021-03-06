/**
  cgiClass.h
  
  Common Gateway Interface Class Definitions.
  
  Copyright (c) 1996 - 2002
  
  David McCombs davidmc@w3sys.com
  
  Open Core Class Library
  
*/

#ifndef cgiCLASS_H
#define cgiCLASS_H

#include "Astring.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "ocCommon.h"

using namespace std;

/**
  enumerated id's of classes
  enum cgiObjectId
   cgi html head body generic
  
*/
enum cgiObjectId { cgi, html, head, body, generic};



/// forward declaration so as to declare as friend.
class cgiEnvironment;
/**
  typedef map < string, aString > queryStringMap
*/
typedef map < string, aString > queryStringMap;

/**
  class cgiInput
  This class is used to hold form (or querystring)
     data from the caller.
     cgiEnvironment HAS-A cgiInput member variable called clientArguments
     clientArguments is obtained by calling cgiInput::ClientArguments()  
*/
class cgiInput
{
/**
  private member variables:
   queryStringMap theMap aString safe
*/
private:
  queryStringMap theMap;
  ocFiles        fileMap;
  aString safe;
protected:
  aString uploadPath;
public:


public:
/**
  public methods:
  
   cgiInput() Constructor
   ~cgiInput() Destructor
*/
  cgiInput();
  ~cgiInput();
  /** void set( const char * queryString, size_t size = 0 );
     Parses the input string into name value pairs,
        placing them in an associative array.*/
  void set( const char * boundaryString, size_t size = 0 );
  /** void setMultipart( aString boundary );
     Parses stdin name value pairs,
        placing them in an associative array.
        Writes any files out to upload directory.
        Note: upload directory must already exist!
        */
  void setMultipart( aString boundary );
  /** aString & Safe(void)
     Returns an untouched copy of the
        buffer passed to the set method*/
  aString & Safe(void);
  /** queryStringMap & TheMap(void);
     Returns an associative array of name value pairs*/
  queryStringMap & TheMap(void);
  /** ocFiles & FileMap(void);
     Returns an associative array of name ocFile pairs*/
  ocFiles & FileMap(void);

  /** count of input arguments */
  int count( const char * key );
  int count( string key ) { return count( key.c_str() ); }
  /** default [const char * key] array operator returns found cgi query value by key*/
  aString & operator [] ( const char * key );
  aString & operator [] ( string key ) { return (*this)[key.c_str()]; }
  
  friend class cgiEnvironment;
};

/**
  class cgiEnvironment
  
  class that holds the information
  coming from the user, as well as information about the
  browser and server.
  You would not use this directly, but should keep in mind that
  cgiScript IS-A cgiEnvironment through public inheritance.
  
*/
class cgiEnvironment
{
/**
  private member variables:
   aString contentLength aString contentType
       aString gatewayInterface aString httpAccept
       aString httpUserAgent aString pathInfo
       aString pathTranslated aString queryString
       aString remoteAddr aString remoteHost
       aString remoteIdent; aString requestMethod;
       aString remoteUser; aString scriptName;
       aString serverName; aString serverPort;
       aString serverProtocol; aString serverSoftware;
       cgiInput clientArguments; int contentSize;
  
*/
private:

  aString contentLength;
  aString contentType;
  aString contentBoundary;
  aString gatewayInterface;
  aString httpAccept;
  aString httpUserAgent;
  aString httpReferer;
  aString pathInfo;
  aString pathTranslated;
  aString queryString;
  aString remoteAddr;
  aString remoteHost;
  aString remoteIdent;
  aString requestMethod;
  aString remoteUser;
  aString scriptName;
  aString serverName;
  aString serverPort;
  aString serverProtocol;
  aString serverSoftware;
  cgiInput clientArguments;
  int contentSize;

  /// Do we read on create or give the user caller more control over this?
  static bool readOnCreateMode;
  
public:
/**
  public methods:
  
   cgiEnvironment(uploadPath) Constructor (upload path is for file uploads, defaults to null)
   ~cgiEnvironment() Destructor
*/
  cgiEnvironment(const char * uploadPath = NULL);
  ~cgiEnvironment();

  /// cgiEnvironment & setUploadPath( string path ); sets the path for uploaded files.
  aString & ContentLength(void);
  /// aString & ContentType(void);  Indicates the MIME type of user supplied info.
  aString & ContentType(void);
  /// aString & GatewayInterface(void);  Indicates the name of the gateway interface.
  aString & GatewayInterface(void);
  /// aString & HttpAccept(void);  Provides a list of browser acceptible mime types.
  aString & HttpAccept(void);
  /// aString & HttpUserAgent(void);  Indicates the name of the browser.
  aString & HttpUserAgent(void);
  /// aString & HttpReferer(void);  Indicates the refering page.
  aString & HttpReferer(void);
  
  /// aString & PathInfo(void);  holds additional path passed beyond this cgi location.
  aString & PathInfo(void);
  /// aString & PathTranslated(void);  holds the same info as the path info, but translated to
  /// a whole path from the server document root.
  aString & PathTranslated(void);
  /// aString & QueryString(void);  The user passed info, used only if the method is GET.<d/d>
  aString & QueryString(void);
  /// aString & RemoteAddr(void);  indicates the IP address of the browser.
  aString & RemoteAddr(void);
  /// aString & RemoteHost(void);  indicates the hostname of the browser.
  aString & RemoteHost(void);
  /// aString & RemoteIdent(void);  Indicates the users name if set in the users
  /// browsers configuration.
  aString & RemoteIdent(void);
  /// aString & RequestMethod(void);  Indicates the method used in the request,
  /// usually GET or POST.
  aString & RequestMethod(void);
  /// aString & RemoteUser(void);  Indicates the authentication name of the user.
  aString & RemoteUser(void);
  /// aString & ScriptName(void);  Indicates the cgi filename.
  aString & ScriptName(void);
  /// aString & ServerName(void);  indicates the host server name
  aString & ServerName(void);
  /// aString & ServerPort(void);  indicates the server port - typically 80.
  aString & ServerPort(void);
  /// aString & ServerProtocol(void);  Indicates the server protocol
  aString & ServerProtocol(void);
  /// aString & ServerSoftware(void);  Indicates the name and version of the sever software
  aString & ServerSoftware(void);
  /// ContentBoundary  returns the boundary demarcing multipart
  aString & ContentBoundary(void);
  /// ClientArguments  returns the associateive arra of the client input.
  cgiInput & ClientArguments( void );
  /// controls when input is read
  static void ReadOnCreateMode( bool doRead );
  /// manual read here for when ReadOnCreateMode is false
  bool readInput( void );

};


/**
  class cgiBase : public ostream
  
  The base class for all cgi classes that follow
*/
class cgiBase : public ostream
{

protected:
  cgiObjectId id;
  aString opening;
  aString close;
  aString endLine;

public:

  cgiBase();
  cgiBase(cgiBase& input);
  const char * tag( void );
  virtual ~cgiBase();
};



/**
  class cgiScriptLite: public cgiBase
  
  The lightweight cgi script class
  Intended for scalable b to b server interaction.
  (Ie, users get input by querying the environment
    variables and stdin themselves.)
*/
class cgiScriptLite: public cgiBase
{
protected:
  aString mimeType;
  
public:
  cgiScriptLite( const char * mimeString = "text/html", bool bCloseHeader=true , bool bCache = false);
  void openHeader( const char * mimeString = NULL );
  void closeHeader( void );
  ~cgiScriptLite();
  void Redirect( const char * location );
};


/**
  class cgiScript: public cgiEnvironment, public cgiScriptLite
  
  The cgi script class, input is parsed and ready in environment methods and
  a client argument vector of name value pairs.
*/
class cgiScript: public cgiEnvironment, public cgiScriptLite
{
public:
  cgiScript( const char * mimeString = "text/html",
             bool bCloseHeader = true,
             const char * uploadPath = NULL, 
             bool bCache = false );
  ~cgiScript();
   cgiScript & DebugString( void );
};

/**
  class cgiHtml : public cgiBase
  
  The html tag container
*/
class cgiHtml : public cgiBase
{

public:

  cgiHtml( char * attr = "");
  virtual ~cgiHtml();

};

/**
  class cgiHead : public cgiBase
  
  the head tag container
*/
class cgiHead : public cgiBase
{

public:

  cgiHead( char * attr = "");
  virtual ~cgiHead();

};

/**
  class cgiBody : public cgiBase
  
  the body tag container
*/
class cgiBody : public cgiBase {

public:

  cgiBody(char * attr = "");
  virtual ~cgiBody();

};

#endif // sentry

#ifdef IN_T2_TESTHARNESS
/** Testing and example code here - uses T2.cpp testharness needs cgiExtend */
// may as well scope it to prevent name collision etc. with other tests.
{

  cgiEnvironment::ReadOnCreateMode(false);
  
  // script
  cgiScript script("text/html",false);
  {
    script.closeHeader();
    // create a new html instance in its own scope 
    cgiHtml html;
    {
      // create a new head tag
      cgiHead head;
      {
        cgiCan title("title");
        title << "T2 test code";
      }
      {
        cgiCan style("style");
        style << "div.form{ color: navy; background: white; border: navy 1px #645; }\n";
      }
    }  
    {
      // create a body tag
      cgiBody body;
      body << "<h1>The form test</h1>" << endl;
      body << "<p>Content Length " << script.ContentLength() << "</p>\r\n\r\n" << endl;
      {
        cgiCan div("div"," class='form'" );
        div << "<h4>See if anybody typed:?file=...</h4>" << endl;
        {
          cgiCan form("form", "action='' encType='multipart/form-data' method='post'");
          form << "<input type='file' name='file'>" << endl;
          form << "<input type='submit' value='Upload'>" << endl;
        }
        div << "count = " << script.ClientArguments().count("file") << "<br>" << endl;
        div << "variable = " << script.ClientArguments()["file"].str() << "<br>" << endl;
        script.readInput();
        div << "<h4>DONE</h4>" << endl;
      }
    }  // body scope
  }  // html scope

}  
/** end of testing */
#endif


