/*
  read_base.hpp  
  base class for data read access
  
*/
#ifndef Read_Base_Hpp
#define Read_Base_Hpp

// includes
#include "connectstring"
#include "openDB.hpp"

// for separate db instances
class read_base 
{  
protected:
  openDbFactory DbMaker;
  openDB & db;
  openRsFactory RsMaker;
  openRS & rs;

public:
  read_base():DbMaker(PROVIDER,CONNECTSTRING),db(DbMaker.db())
  ,RsMaker(db),rs(RsMaker.rs()){;}
  virtual~read_base(){;}  
};


// for shared db instances
class ref_base 
{  
protected:
  openDB & db;
  openRsFactory RsMaker;
  openRS & rs;
public:
  ref_base(openDB & dbIn)
   :db(dbIn)
   ,RsMaker(db)
   ,rs(RsMaker.rs())
  {;}
  virtual~ref_base()
  {;}  
};


class quickQuery: private read_base
{
public:
  bool opened;
  quickQuery():read_base(),opened(false){;}
  ~quickQuery(){;}
  openRS & getData ( string query )
  {
    if( rs.open(query) ) opened = true;
    return rs;
  }
  openRS & getRS()
  {
    return rs;
  }
};

/* 
  quickerQuery is like quickQuery, 
  but it re-uses a db connection for faster access
  to data.  
 
*/
class quickerQuery: private ref_base
{
public:
  bool opened;
  quickerQuery(openDB & dbIn):ref_base(dbIn),opened(false){;}
  ~quickerQuery(){;}
  openRS & getData ( string query )
  {
    if( rs.open(query) ) opened = true;
    return rs;
  }
  openRS & getRS()
  {
    return rs;
  }
};

#endif
