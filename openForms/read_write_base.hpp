/**
  read_write_base.hpp  
  base class for data access   
  Definition file        
  The classes in this file are used strictly as glue 
  between the business objects and the data objects.  
                                   ****                                       
  [DB]<->[DB API]<->[DATA-OBJs]<->[Glue]<->[BUS-OBJs]<->[Glue]<->[GUI]
                    (polymorph)    ****
  
  Copyright © 2003 - 2006 All rights reserved
  David K. McCombs - davidmc@w3sys.com
  W3 Systems Design - www.w3sys.com  
*/

#ifndef Read_Write_Base_Hpp
#define Read_Write_Base_Hpp

// includes
#include "openDB.hpp"
#include "ocTypes.h"
#include "ocString.h"
#include "dataXfer.hpp"

// forward declaration
class ref_base;

/// for determining what data items the user has changed.
typedef map< string, string> changeMap;

/// for individual db instances
class read_write_base 
{  
protected:
  openDbFactory DbMaker;
  openDB & db;
  openRsFactory RsMaker;
  openRS & rs;
  openCmdFactory CmdMaker;   
  openCMD & cmd;
  dataXfers xfers;
  string m_name;
  ocString m_result;
  long long m_key;
  bool actDerived;
public:
  read_write_base();
  read_write_base(dbProvider,const char *);
  
  virtual ~read_write_base(); 

  // validation and supplemental for insert,update and delete )
  virtual bool ivalidate( void  );
  virtual bool uvalidate( changeMap & changes  );
  virtual bool dvalidate( void );
  
  virtual bool isupplemental( void );
  virtual bool usupplemental( changeMap & changes );
  virtual bool dsupplemental( void );
  virtual bool ssupplemental( void ); 
    
 
  // gets,sets
  void data_name( string in ){ m_name=in; }
  const string & data_name ( void ){ return m_name; }
  const string & last_result ( void ){ return m_result; }
  long long key( void ){ return m_key; }
  void key( long long in ){ m_key=in; }
  
  bool db_action( string mode, changeMap & changes );
  virtual bool db_insert( void );
  virtual bool db_update( changeMap & changes );
  virtual bool db_delete(  void );
  bool get_data( void );
  /* 
    custom user called get_data() with where clause input.
    (The ' where ' keyword is provided for you.) 
    Sets the key for subsequent update/delete actions.
    If whereClause is zero length, the where keyword is not added    
  */  
  bool get_data( string whereClause, string orderClause = "" );
  
  // for some of these where clauses, multiple records, allow forward iteration...  
  bool next( void );
  
  bool addDXMap( dataXfer * xfer );
  void clearMaps( void );  
   
  // new for transasction capable dbs
  bool commit( void );
      
  friend class ref_base; 
};


#endif
