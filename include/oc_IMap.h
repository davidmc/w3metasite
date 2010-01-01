#include "ocSocket.h"
#include "ocString.h"

// reasonable first guesses to prevent too much reallocation
const int reasonableBufSize = 200;

struct mlistItem
{
  string id;
  string to;
  string from;
  string sent;
  string reply_to;
  string cc;
  string in_reply_to;
  string subject;
  string received;
  string flags;
  string size;
  /*
  Example:
  1 FLAGS (\Seen) INTERNALDATE "24-Apr-2002 20:25:02 -0400" RFC822.SIZE 975 ENVELOPE ("Wed, 24 Apr 2002 20:25:02 -0400" "CCI Health Inquiry from a TCHP Customer" ((NIL NIL "davidmc" "localhost.localdomain")) ((NIL NIL "davidmc" "localhost.localdomain")) ((NIL NIL "davidmc" "localhost.localdomain")) ((NIL NIL "davidmc" "localhost.localdomain")) NIL NIL NIL "<200204250025.g3P0P2m13679@localhost.localdomain>"))
    date                              subject             from                                          sender                                        reply_to                                      to                                           cc  bcc in_reply_to    message_id
  ("Wed, 24 Apr 2002 20:25:02 -0400" "CCI Health Inquiry" ((NIL NIL "davidmc" "localhost.localdomain")) ((NIL NIL "davidmc" "localhost.localdomain")) ((NIL NIL "davidmc" "localhost.localdomain")) ((NIL NIL "davidmc" "localhost.localdomain")) NIL NIL NIL "<200204250025.g3P0P2m13679@localhost.localdomain>"))
                                                            name route mailbox  host
  */
  mlistItem( string & listItem )
  { 
    ocString parser = listItem;
    parser.parseInit();
    ocString worker;
    
    //1 FLAGS ...
    id = parser.parse(" ");
    //FLAGS (\Seen) ...
    parser.parse(" (");
    //\Seen) INTERNALDATE  "
    flags = parser.parse(") ");
    //INTERNALDATE "24-Apr-2002 20:25:02 -0400" RFC822.SIZE
    parser.parse("RFC822.SIZE ");
    //975 ENVELOPE (
    size = parser.parse(" ENVELOPE (\"");
    /*  2060 ENVELOPE DESCRIPTION
    A parenthesized list that describes the envelope
    structure of a message.  This is computed by the
    server by parsing the [RFC-822] header into the
    component parts, defaulting various fields as
    necessary.

    The fields of the envelope structure are in the
    following order: date, subject, from, sender,
    reply-to, to, cc, bcc, in-reply-to, and message-id.
    The date, subject, in-reply-to, and message-id
    fields are strings.  The from, sender, reply-to,
    to, cc, and bcc fields are parenthesized lists of
    address structures.

    An address structure is a parenthesized list that
    describes an electronic mail address.  The fields
    of an address structure are in the following order:
    personal name, [SMTP] at-domain-list (source
    route), mailbox name, and host name.

    [RFC-822] group syntax is indicated by a special
    form of address structure in which the host name
    field is NIL.  If the mailbox name field is also
    NIL, this is an end of group marker (semi-colon in
    RFC 822 syntax).  If the mailbox name field is
    non-NIL, this is a start of group marker, and the
    mailbox name field holds the group name phrase.

    Any field of an envelope or address structure that
    is not applicable is presented as NIL.  Note that
    the server MUST default the reply-to and sender
    fields from the from field; a client is not
    expected to know to do this.

    ENVELOPE ("Mon, 16 Sep 2002 05:49:58 -0700"                 date
              "Test"                                            subject
           personal-name at-domain-list mailbox-name hostname
              (("test" NIL "davidmc" "localhost.localdomain"))  from
              (("test" NIL "davidmc" "localhost.localdomain"))  sender
              (("test" NIL "davidmc" "localhost.localdomain"))  reply-to
              ((NIL NIL "davidmc" "localhost.localdomain"))     to
           example of totally nil data
              NIL                                               cc
              NIL                                               bcc
              NIL                                               in-reply-to
              "<HDELJAKOCNDFLIPKMGBGGEKHCDAA.davidmc@lindev>")) message-id
    */

    received = parser.parse("\" ");              // parse date
    if( parser.remainder()[0] == '\"' ) // see if quoted
    {
      worker = parser.parse("\" ");
      worker = worker.remove("\"");              // parse subject
    }
    else
    {
      worker = parser.parse(" ");                // parse subject
    }
    subject = worker;
    setAddress(from,parser);                     // parse  from
    setAddress(sent,parser);                     // parse  sender
    setAddress(reply_to,parser);                 // parse  reply-to
    setAddress(to,parser);                       // parse  to
    setAddress(cc,parser);                       // parse  cc
    setAddress(in_reply_to,parser);              // parse  in-reply-to
    if( subject.length() == 0 || subject == "NIL" ) subject="(None)";
  }
  // RFC2060 say whole address maybe NIL
  //  or any element in the address
  void setAddress( string & address, ocString & data )
  {
    // NIL
    // (("test" NIL "davidmc" "localhost.localdomain"))
    // ((NIL NIL "address" "domain"))
    ocString worker = data.parse(" ");
    address = "";
    if( worker[0] == '(' ) // if not NIL
    {
      // a real address structure - get personal-name
      worker = worker.remove("((");
      if( worker[0] == '\"' )
      {
        // have a real name
        address += worker;
        if( worker.length() && worker[worker.length()-1] != '\"' )
        {
          // we need to get the rest of the personal name (end of quoted string)
          address += " "; // restore eaten space
          address += data.parse("\" ");
          address += "\"";
        }
        address += " ";
      }
      // not using this at-domain-list, just go over it cleanly
      worker = data.parse(" ");
      if( worker[0] == '\"' )
      {
         data.parse("\" ");
      }

      // mailbox-name (must have)
      worker = data.parse(" ");
      if( worker[0] == '\"' )
      {
        address += worker.remove("\"").remove("\"");
        address += "@";
      }

      //"localhost.localdomain"))
      worker = data.parse(")) ");
      if( worker[0] == '\"'  )
      {
        address += worker.remove("\"").remove("\"");
      }

    } // end if real address
    // should leave data at the begin of next valid token
  }

};

class oc_IMap: protected ocSocket
{
  vector <string> responseLines;
  vector <string> listItems;
  ocString sequenceBuffer;
  ocString currentLine;
  string callPrefix;
  string currentPrefix;

  int    callSequence;
  bool   responseComplete;
  // cache data in accordance with SHOULD of rfc 2060 section 2.2.2
  bool   cache;

  // create a sequence string
  const char * sequence( void )
  {
    sequenceBuffer="";
    return sequenceBuffer.append( callSequence++ ).c_str();
  }
  bool checkCommandResponse( void )
  {
    // for all commands
    responseLines.clear();
    quenchQueue();
    return responseOK();
  }
  void quenchQueue( void )
  {
    do
    {
      Response();
    } while(!responseComplete);
  }
  bool responseOK( void )
  {
    bool bret = false;
    currentLine.parseInit();
    string token = currentLine.parse(" ");
    if( token == currentPrefix )
    {
      token = currentLine.parse(" ");
      // bad: webMail1 BAD ... or webMail1 NO ...
      // good: webMail2 OK ... (so says 2060!)
      if( token == "OK" )
      {
        bret = true;
      }
    }
    return bret;
  }


public:
  oc_IMap( const char * chPrefix, const char * address, int port )
  :ocSocket( address, port )
  ,responseLines(reasonableBufSize)
  ,listItems(reasonableBufSize)
  ,callPrefix(chPrefix)
  ,callSequence(1)
  ,responseComplete(false)
  ,cache(false)
  { ; };

  ~oc_IMap()
  {;}

  /*
    Cache all response data from the server
    ( reset on each 'specific command' request )
  */
  void CacheData( bool bVal = true )
  {
    cache = bVal;
  }
  void ResetCache( void )
  {
    responseLines.clear();
  }
  vector <string> & GetDataCache( void )
  {
    return responseLines;
  }
  vector <string> & GetListItems( void )
  {
    return listItems;
  }
  /* raw command response */
  bool Command( const char * in )
  {
    bool bRet = false;
    if( in && strlen( in ) )
    {
      string cmdIn = in;
      int cmdLen = cmdIn.length();
      // remove ending crlf pair - we will do that ourselves!
      if( cmdIn[cmdLen-1] == '\n' )  cmdIn.resize(cmdLen-1);
      if( cmdLen > 1 && cmdIn[cmdLen-2] == '\r' ) cmdIn.resize(cmdLen-2);

      // build a well formed RFC2060 imap command
      currentPrefix = callPrefix;
      currentPrefix += sequence();
      string command = currentPrefix;
      command += " ";
      command += cmdIn;
      command += "\r\n";
      // update the length for comparison
      cmdLen = command.length();

      // if the amount written is the
      //  same as the command length, return is true
      bRet = ( cmdLen == Write( command.c_str(), cmdLen ) );
    }
    return bRet;
  }
  string & Response( void )
  {
     string token;
     responseComplete = false;
     currentLine = ReadLine();

     currentLine.parseInit();
     token = currentLine.parse(" ");
     if( token == currentPrefix )
     {
       responseComplete = true;
     }
     token = currentLine.parse(" ");
     if( token == "BYE" )
     {
       responseComplete = true;
     }
     if( cache )
     {
       responseLines.push_back( currentLine );
     }
     return currentLine;
  }

  // Tells if the server has complete it's response
  bool GetResponseComplete( void )
  {
    return responseComplete;
  }

  /* specific commands as per IETF RFC2060 */

  // Login
  bool login( const char * user, const char * password)
  {
    bool bret = false;
    if( user && strlen(user) &&
        password && strlen(password) )
    {
      string command = "LOGIN ";
      command += user;
      command += " ";
      command += password;
      if( Command(command.c_str()) ) bret = checkCommandResponse();
    }
    return bret;
  }

  // select mailbox
  bool select( const char * mailbox )
  {
    bool bret = false;
    if( mailbox && strlen(mailbox)  )
    {
      string command = "SELECT ";
      command += mailbox;
      if( Command(command.c_str()) ) bret = checkCommandResponse();
    }
    return bret;
  }
  bool examine( const char * mailbox )
  {
    bool bret = false;
    if( mailbox && strlen(mailbox)  )
    {
      string command = "EXAMINE ";
      command += mailbox;
      if( Command(command.c_str()) ) bret = checkCommandResponse();
    }
    return bret;
  }
  bool create( const char * mailbox )
  {
    bool bret = false;
    if( mailbox && strlen(mailbox)  )
    {
      string command = "CREATE ";
      command += mailbox;
      if( Command(command.c_str()) ) bret = checkCommandResponse();
    }
    return bret;
  }
  // Get mail list (fetch iteration, not directly part of rfc2060)
  // must be logged in and have selected a mailbox first
  bool fetchHeaders( void )
  {
    bool bret = false;
    bool savedCache = cache;
    cache = true;
    string command = "SEARCH ALL";

    if( Command(command.c_str()) && checkCommandResponse() )
    {
      // iterate through the responseLines looking for * SEARCH
      for( size_t x=0; x<responseLines.size(); x++ )
      {
        ocString line = responseLines[x];
        line = line.replace("\r\n"," ");
        line.parseInit();

        // typical response
        // '* SEARCH 1 2 3 4 5'
        // 'webMail3 OK SEARCH completed'
        string token = line.parse(" ");
        if( token == "*" )
        {
          token = line.parse(" ");
          if( token == "SEARCH" )
          {
            listItems.clear();
            // now just parse the numbers till at the end of parse
            do
            {
              token = line.parse(" ");
              if( token.length() )
              {
                cache = false;
                command = "FETCH ";
                command += token;
                command += " ALL";
                if( Command(command.c_str()) )
                {
                  ocString response = Response();

                  response.parseInit();
                  token = response.parse( " " );

                  if( token == "*" )
                  {
                    // hold over number
                    string item = response.parse( " " );
                    item += " ";
                    if( !response.endOfParse() )
                    {
                      token = response.parse( " (" );
                      if( token == "FETCH" )
                      {
                        item += response.remainder();

                        listItems.push_back( item );
                      }
                    }
                  }
                }
                // finish with the response
                quenchQueue();
                // response should be ok
                if( ! responseOK() )
                {
                  non_fatal_error( "detected failure in 'fetch all' response" );
                  return false;
                }
              }
            } while(!line.endOfParse());

            break; // outof the for loop
          } // end if SEARCH token
        } // end if * token
      } // end for
    } // end if command and response success
    cache = savedCache;
    bret = listItems.size() > 0;
    return bret;
  }

  bool deleteItems( const char * itemList )
  {
    bool bret = false;
    string command = "STORE ";
    command += itemList;
    command += " FLAGS.SILENT (\\Deleted)";
    if( Command(command.c_str()) && checkCommandResponse() )
    {
      command = "EXPUNGE";
      if( Command(command.c_str()) && checkCommandResponse() )
      {
        bret = true;
      }
    }
    return bret;
  }

  bool copyItems( const char * itemList, const char * toBox )
  {
    bool bret = false;
    string command = "COPY ";
    command += itemList;
    command += " ";
    command += toBox;
    if( Command(command.c_str()) && checkCommandResponse() )
    {
      bret = true;
    }
    return bret;
  }

  // get mail item (fetch rfc822 symantics)
  string & fetchRFC822 ( const char * item )
  {
    bool savedCache = cache;
    cache = true;
    string command = "FETCH ";
    command += item;
    command += " RFC822";
    if( Command(command.c_str()) && checkCommandResponse() )
    {
      currentLine = "";
      // iterate through the responseLines
      for( size_t x=0; x<responseLines.size(); x++ )
      {
        ocString examine = responseLines[x];
        examine.parseInit();
        ocString firstToken = examine.parse(" "); // * 1 FETCH (RFC822 {975}
        firstToken = firstToken.remove("\r\n");
        if( firstToken == "*" &&                // Return-Path: <apache@localhost.localdomain>
            examine.parse(" ") == item &&       // Received: (from apache@localhost)
            examine.parse(" ") == "FETCH" )     // 	by localhost.localdomain (8.11.2/8.11.2) id g3P0P2
        {                                       // 	for davidmc; Wed, 24 Apr 2002 20:25:02 -0400
          x+=4; // skip the next 4 lines        // ...
        }                                       // )
        else if( firstToken != ")" &&           // webMail9 OK FETCH completed
                 firstToken != currentPrefix )
        {
          currentLine += responseLines[x];
        }
      }
    }
    cache = savedCache;
    return currentLine;
  }

  // Start to get mail item
  //  (fetch rfc822 symantics)
  // The caller will have to parse the incoming lines
  string & rawFetchRFC822 ( const char * item )
  {
    cache = false;
    currentLine = "";
    string command = "FETCH ";
    command += item;
    command += " RFC822";
    // send the command
    if( Command(command.c_str())  )
    {
      // get the first line
      Response();
    }
    // return the first line
    return currentLine;
  }
  // logout
};

