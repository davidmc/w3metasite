/* 
  Session.hpp

  Object Definition and Implementation 
  for Session.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Session_HPP
#define Session_HPP

#include <uuid/uuid.h>
#include "cgiCookie.h"
#include "ocXML.h"
#include "read_write_base.hpp"

/*
  The session object must be instanced before clossing
  the response header.
  
  Implementaion needs to be separated to allow for static members  

*/

class Session_Obj: public read_write_base
{
public:
  identifier Id;
  llong User_Id;
  string UUID;
  time_date Instance_Stamp;
  string XML_Variables;  
  
protected:

  changeMap chMap;
  bool isGood;
  bool inCharge;
  
  void makeUUID(void)
  {
    uuid_t uuid;
    char uuid_val[37];
    memset(uuid,'\0',sizeof(uuid));
    uuid_generate(uuid);
    uuid_unparse(uuid, uuid_val);    
    ocString uuid_work(uuid_val);    
    UUID = uuid_work.replaceAll("-","");    
  }
 
public: 

  Session_Obj(bool inCharge=true):read_write_base()
  ,Id(0LL)
  ,User_Id(0LL)
  ,UUID("")
  ,Instance_Stamp() // always NOW
  ,XML_Variables("")
  ,isGood(false)
  ,inCharge(inCharge)
  { 

    // set name
    data_name("metasite.Session");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new llongXfer("User_Id", &User_Id ));
    addDXMap( new stringXfer("UUID", &UUID ));
    addDXMap( new time_dateXfer("Instance_Stamp", &Instance_Stamp ));
    addDXMap( new stringXfer("XML_Variables", &XML_Variables ));
    // see if a cookie is available
    cgiCookie cookie;
    cookie.setPath("/"); // root path
    UUID = cookie.get("SessionId");
    if( UUID.length() )
    {
      // get data
      string clause = " UUID = '" + UUID + "'";
      get_data(clause);   
    }
    // Even though you had a session, it may have gotten deleted   
    if(Id==0 && inCharge==true) 
    {
      // insert data
      makeUUID();
      cookie.set("SessionId",UUID.c_str());   
      if( db_insert() )
      {
        Id =  key();   
      }
    }
  }
  ~Session_Obj(){;}
 
  string operator () (string  Variable_Name )
  {
    return GetData( Variable_Name );
  }
  
  string GetData( string Variable_Name )
  {
    string ret;
    xmlParser parser( XML_Variables );
    parser.parse();
    node_map & nodes = parser.states.nodemap;
    node_map::iterator iter;
    iter = nodes.lower_bound(Variable_Name);
    if(iter != nodes.upper_bound(Variable_Name))
    {
      ret = parser.nodeList()[iter->second].data;    
    }
    return ret;
  }
  bool SetData( string Variable_Name, string Data )
  {
    bool ret = true;
    xmlParser parser( XML_Variables );
    parser.parse();
    node_map & nodes = parser.states.nodemap; 
    node_map::iterator pos = nodes.lower_bound(Variable_Name);
    if(pos != nodes.upper_bound(Variable_Name))
    {
      parser.nodeList()[pos->second].data = Data;    
    }
    else
    {
      xmlNode aNode;
      aNode.name = Variable_Name;
      aNode.data = Data;
      parser.addNode(aNode);
    }
    XML_Variables = parser.emit();
    return ret;
  }
  bool Synch()
  {
    if( Id )
    {
      // synch current data to database
      changeMap changes;
      
      // key(Id);
      
      // These are the only things we should update on synch
      changes["User_Id"]="User_Id";
      changes["Instance_Stamp"]="Instance_Stamp";
      changes["XML_Variables"]="XML_Variables";
      
      isGood = db_update( changes );
    }
    else
    {      
      // SELF HEALING CODE! make a key
      if(UUID.length())
      db_insert();
      Id =  key();      
    }
    return isGood;
  }
  // MUST be called before closing header
  void killCookie( void )
  {
    // see if a cookie is available
    cgiCookie cookie;
    cookie.setPath("/"); // root path
    string & date = cookie.setTimeout( 1, 1, 1970, 0, 0 );
    cookie.set( "SessionId", "" ); 
    UUID = "";
    Id = User_Id = 0;
  }    
  bool IsGood( void )
  {    
    return isGood;
  };
};

#endif

