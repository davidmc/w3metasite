//  Includes and defines for TCP
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include <errno.h>

// Shortens casts of pointer arguments
#define	SA	struct sockaddr *
// When MSG_EOF can't be found elsewhere
#define MSG_EOF  MSG_FIN
// for default read buffer size
const size_t MAXLINE = 2048;

// Includes and defines for STL
// #include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;
/*
  Open Core (oc)
  ip version 4
  client socket abstraction
*/
class ocSockServer
{
  // I/O vars
private:
  // Read data may be ascii or binary
  vector <char> readBuff;
  string error;

  // Common Socket vars
  struct sockaddr_in serv;
  struct sockaddr_in cli;
  int sockfd;
  int listenfd;

  // accounting
  ssize_t amountRead;

protected:

  // protected error routines
  void fatal_err( const char * errIn )
  {
    error = "\r\nFatal Error: ";
    error += errIn;
    throw exception();
  }
  void non_fatal_error( const char * errIn )
  {
    error = "\r\nError: ";
    error += errIn;
  }

public:

  /*
    ALWAYS use this constructor!
     ( The default constructor that
       compiler makes just ain't no good. )
  */
  ocSockServer( int port, int connections, const char * address = NULL )
  :readBuff(MAXLINE),error("")
  {
    if( (listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
      fatal_err("socket function error");
    }
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;

    if( address == NULL )
    {
      serv.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else if( inet_aton(address, &serv.sin_addr) != 1 )
    {
      fatal_err("bad address string");
    }
    serv.sin_port = htons( port );

    // bind the socket
    if (bind(listenfd, (SA) &serv, sizeof(serv)) < 0)
    {
      fatal_err("connect function error");
    }

    // Indicate we are listening
    if (listen(listenfd, connections) < 0)
    {
      fatal_err("listen error");
    }
  }

  virtual ~ocSockServer()
  {
    close(sockfd);
  }

private:
  // pure virtual method MUST be defined by derived class
  virtual void daemonAction( void ) = 0;

public:

  void daemonServe( void )
  {
    for ( ; ; )
    {
      size_t clilen = sizeof(cli);
      sockfd = accept(listenfd, (SA) &cli, &clilen);
      if ( sockfd < 0 )
      {
        fatal_err("accept error");
      }
      daemonAction();
    }
  }

  // Use this to make the read (blocking call) timeout.
  // Otherwise it will just wait till the dogs come home.
  bool SetTimeoutTime( size_t seconds )
  {
    struct timeval tv;
    tv.tv_sec = seconds;
    tv.tv_usec = 0;
    return  (setsockopt( sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv) ) == 0);
  }

  /*
    Write to the socket stream
  */
  ssize_t Write( const char * in, size_t amount )
  {
    size_t nwrote;
    const char *ptr;
    ptr = in;
    nwrote = 0;

    while( (amount-nwrote) > 0)
    {
      nwrote = write(sockfd, ptr, amount - nwrote);
      if( nwrote <= 0 )
      {
        if (errno == EINTR) nwrote = 0;
        else fatal_err("could not write everything to socket");
      }
      ptr += nwrote;
    }
    return(amount);
  }

  /*
     Read from the socket stream
  */
  char * Read( size_t amount )
  {
    amountRead = 0;

    // the extra byte is for string oriented content
    // so the caller can place a terminating null at the end.
    if( readBuff.size()  < amount+1 )
    {
      readBuff.resize(amount+1);
    }
    char * base = & readBuff[0];
    do
    {
      amountRead += read( sockfd, base, amount-amountRead );
      if( amountRead > 0 ) base = & readBuff[amountRead];

      // the EINTR says that the socket was interupted
      // by a signal so it is ok to continue the read.
    } while( amountRead < (ssize_t)amount && errno == EINTR  );

    // The case of the timeout, won't happen unless
    //  the SetTimeoutTime() method was called
    //  before the read.
    if( amountRead == -1 && errno ==  EWOULDBLOCK)
    {
      non_fatal_error( "socket timeout on read" );
      return NULL;
    }
    return & readBuff[0];
  }

  /*
    Read a line from the socket stream
  */
  char * ReadLine( size_t amount = 4096L )
  {
    amountRead = 0;

    // leave room for the terminating null.
    if( readBuff.size()  < (size_t)(amount+1) )
    {
      readBuff.resize(amount+1);
    }

    // set the base to the start of the pointer
    char * base = & readBuff[0];
    do
    {
      // peek ahead
      ssize_t nRead =  recv( sockfd, base, amount-amountRead, MSG_PEEK );
      if( nRead > 0 )
      {
        base[nRead] = '\0';
        string temp( base );
        // search for the newline character
        string::size_type idx;
        idx = temp.find( "\n" );
        // knowlegeable read, removing from stream.
        if(idx != string::npos )
        {
          amountRead += read( sockfd, base, idx+1 );
          break;
        }
        else
        {
          amountRead += read( sockfd, base, nRead );
        }
        // increment the base
        if( amountRead > 0 )
        {
           base = & readBuff[amountRead];
        }
      }
      else
      {
        // Pass bad read amount to member variable
        amountRead = nRead;
      }

    }
    while( amountRead < (ssize_t) amount && errno == EINTR  );

    if( amountRead )
    {
      readBuff[amountRead] = '\0';
    }

    // this happens only if timeout value was set before the call
    if( amountRead == -1 && errno ==  EWOULDBLOCK)
    {
      non_fatal_error( "socket timeout on read" );
      return NULL;
    }
    return & readBuff[0];
  }

  /*
    See how much was read last
  */
  ssize_t GetAmountRead( void )
  {
    return amountRead;
  }

  /*
    See what the last error was
  */
  string & getError(void)
  {
    return error;
  }
};
