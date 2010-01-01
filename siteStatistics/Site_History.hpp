/*
  Site_History.hpp

  Object Definition and Implementation 
  for Site_History.

  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/
#ifndef Site_History_HPP
#define Site_History_HPP

#include <fstream>
#include "read_write_base.hpp"
#include "ocXML.h"
#include <set>

using namespace std;

typedef set<string> filterSets;

class Site_History_Obj: public read_write_base
{
private:
  streampos lastPos;
  filterSets accept;
  filterSets reject;
public:
  identifier Id;
  string Remote_Host;
  string Remote_Log;
  string Remote_User;
  time_date Request_Time;
  string Method;
  string Resource_Requested;
  string Protocol;
  string Original_Status;
  int Response_Size;
  string Referer;
  string User_Agent;
  string log_file;
  Site_History_Obj():read_write_base(),lastPos(0)
  ,Id(0LL)
  ,Remote_Host("")
  ,Remote_Log("")
  ,Remote_User("")
  ,Request_Time( )
  ,Method("")
  ,Resource_Requested("")
  ,Protocol("")
  ,Original_Status("")
  ,Response_Size(0)
  ,Referer("")
  ,User_Agent("")
  ,log_file("")
  {
    // set name
    data_name("Site_History");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new stringXfer("Remote_Host", &Remote_Host ));
    addDXMap( new stringXfer("Remote_Log", &Remote_Log ));
    addDXMap( new stringXfer("Remote_User", &Remote_User ));
    addDXMap( new time_dateXfer("Request_Time", &Request_Time ));
    addDXMap( new stringXfer("Method", &Method ));
    addDXMap( new stringXfer("Resource_Requested", &Resource_Requested ));
    addDXMap( new stringXfer("Protocol", &Protocol ));
    addDXMap( new stringXfer("Original_Status", &Original_Status ));
    addDXMap( new intXfer("Response_Size", &Response_Size ));
    addDXMap( new stringXfer("Referer", &Referer ));
    addDXMap( new stringXfer("User_Agent", &User_Agent ));
    addDXMap( new stringXfer("log_file", &log_file ));
  }
  void getFilters(  xmlParser & parser )
  {
     node_vector::iterator apos = parser.findFirstNodeByName( "accept" );
     while( apos != parser.nodeList().end() )
     {
       accept.insert( apos->data );
       apos = parser.findNextNodeByName( "accept" );
     }
     node_vector::iterator rpos = parser.findFirstNodeByName( "reject" );
     while( rpos != parser.nodeList().end() )
     {
       reject.insert( rpos->data );
       rpos = parser.findNextNodeByName( "reject" );
     }
  }
  /*
     load data from the logfile
  */
  bool load(void)
  {
    bool ret = false;
    // open the setup file
    ifstream setUp( "siteStatSetup.xml" );
    if( setUp )
    {
      cout << "opened setup file" << endl;
      xmlParser parser( &setUp );
      parser.parse();
      
      node_vector::iterator pos = parser.findFirstNodeByName( "logfile" );
      if( pos != parser.nodeList().end() )
      {
        log_file = pos->attr["name"];
        getFilters(parser);
        cout << "Got logfile node from setup: logfile is " << pos->attr["path"] << endl;
        cout << "Initial seek pos is " << pos->attr["seekpos"] << endl;
        ret = load( pos->attr["name"],
                    pos->attr["path"],
                    pos->attr["seekpos"]  );
        ocString lpos;
        lpos.append( lastPos );
        pos->attr["seekpos"] = lpos;
      }
      if( setUp ) setUp.close();
      ofstream wSetUp( "siteStatSetup.xml",ios_base::out | ios_base::trunc);
      if( wSetUp )
      {
        wSetUp << parser.emit() << endl;
      }
      
      
    }
    else
    {
      cout << "no setup file?" << endl;
    }
    return ret;
  }
  bool load( string name, string path, string seekpos )
  {
    ifstream logFile(path.c_str());
    if( logFile )
    {
      char linebuffer[9000];
      cout << " opened " << path << endl;
      lastPos = atoi(seekpos.c_str());
      if( lastPos > 0 )
      {
        // go to the last valid line, then eat it
        logFile.seekg(lastPos);
        logFile.getline(linebuffer,sizeof(linebuffer));
      }
      
      while( logFile )
      {
        lastPos = logFile.tellg();
        // cout << lastPos << endl;
        logFile.getline(linebuffer,sizeof(linebuffer));
        parseItem( linebuffer );
      }
    }
    else
    {
      cout << " could not open " << path << endl;
    }
  }
  bool parseItem( char* lineBuf )
  {
    /*
    Assumed common log file format
    LogFormat "%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-Agent}i\"" combined     (from httpd.conf)
    "%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-agent}i\""
    Where
    %h :=  Remote host
    %l :=  Remote logname
    %u :=  Remote user
    %t :=  Time the request was received
    %r :=  First line of request
    %>s := Status.of original request before any redirects
    %b := Size of response in bytes
    %{Referer}i := Referer Header value
    %{User-Agent}i := User Agent Header value
    */
    if(  lineBuf && strlen( lineBuf ) )
    {
      ocString aiBuf = lineBuf;
      Remote_Host = aiBuf.parse(" ");
      //cout << aiBuf.remainder() << endl;
      Remote_Log  = aiBuf.parse(" ");
      //cout << aiBuf.remainder() << endl;
      Remote_User = aiBuf.parse(" [");
      //cout << aiBuf.remainder() << endl;

      // Do a little conditioning work on the date & time
      ocString timestr = aiBuf.parse("] \"");
      timestr = timestr.replace(":"," ");
      timestr = timestr.parse(" -");
      Request_Time.parse(timestr.c_str(),"%d/%b/%Y %T");
      Request_Time.normalize();

      // the next 3 items are part of the requested resource %r
      Method = aiBuf.parse(" ");
      //cout << aiBuf.remainder() << endl;
      Resource_Requested = aiBuf.parse(" ");
      //cout << aiBuf.remainder() << endl;
      Protocol = aiBuf.parse("\" ");
      //cout << aiBuf.remainder() << endl;
      
      Original_Status = aiBuf.parse(" ");
      //cout << aiBuf.remainder() << endl;
      Response_Size = atoi(aiBuf.parse(" \"").c_str());
      //cout << aiBuf.remainder() << endl;
      Referer = aiBuf.parse("\" \"");
      //cout << aiBuf.remainder() << endl;
      User_Agent = aiBuf.parse("\"");
      //cout << aiBuf.remainder() << endl;
      
      if( filterPass() )
      {
        db_insert();
      }
      
    }
    return true;
  }
  bool filterPass( void )
  {
    bool pass = false;
    filterSets::iterator pos = accept.begin();
    while( pos != accept.end() && pass == false )
    {
      ocString toFilter = Resource_Requested;
      string filter = *pos;
      if( toFilter.regExMatch(filter.c_str() ) )
      {
        pass = true;
      }
      ++pos;
    }
    pos = reject.begin();
    while( pos != reject.end() && pass == true )
    {
      ocString toFilter = Resource_Requested;
      string filter = *pos;
      if( toFilter.regExMatch(filter.c_str() ) )
      {
        pass = false;
      }
      ++pos;
    }
    return pass;
  }
  /*
  TODO: Add Event hooks for business logic
  =========================
  // Done before any action - a false return prevents action
  virtual bool ivalidate( void  ) { return true; }
  virtual bool uvalidate( changeMap & changes  ) { return true; }
  virtual bool dvalidate( void ) { return true; }

  // Done after any action - a false return sets a warning
  virtual bool isupplemental( void ){ return true; }
  virtual bool usupplemental( changeMap & changes ){ return true; }
  virtual bool dsupplemental( void ){ return true; }
  virtual bool ssupplemental( void ){ return true; } 
  */

};
#endif



