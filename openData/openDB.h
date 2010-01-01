#ifndef open_db_h
#define open_db_h
#include <string>
#include "openFields.h"


// forward class declarations
class openCMD;
class openRS;
class openDB;

/*
  This emumeration is my best first guess at possible
  providers
  
*/
enum dbProvider
{
  PostGresQL,
  MySQL,
  ODBC,
  DB2,
  Informix,
  Sybase // should work for MSSQL
};

/*

  The next 3 object factories perform the following:
  1) Create the requested underlying implementation.
  2) Hide the pointers and pass back references instead.
  3) Manage the lifetime of the underlying heap object instance.

*/

// database object factory
class openDbFactory
{
private:
  openDB * pDB;
public:
  openDbFactory( dbProvider dbImplementation, const char * strConnection );
  ~openDbFactory();
  openDB & db();
};

// recordset object factory
class openRsFactory
{
private:
  openRS * pRS;
public:
  openRsFactory( openDB & rdb );
  ~openRsFactory();
  openRS & rs();
};

// command object factory
class openCmdFactory
{
private:
  openCMD * pCMD;
public:
  openCmdFactory( openDB & rdb );
  ~openCmdFactory();
  openCMD & cmd();
};


/*
 This base class defines the interfaces to the database
*/
class openDB
{
protected:  
  string     m_strErrors;
  dbProvider m_provider;
  bool m_bGood;
  virtual openCMD * createCommand() = 0;
  virtual openRS * createRecordset() = 0;

public:

  openDB( string strConnection );
  virtual ~openDB();
  
  // pure virtual
  virtual dbProvider getProvider( void ) = 0;
  inline bool isGood( void ){ return m_bGood; }
  
  string & errorString( void ){ return m_strErrors; }
    
  friend class openRsFactory;
  friend class openCmdFactory;  
};

/*
 This base class defines the interfaces to the command
*/
class openCMD
{

protected:
  openDB & rdb;
  bool    m_bTransactioning;
public:

  openCMD( openDB & idb );

  virtual ~openCMD();

  // pure virtual methods that must be defined by impl
  virtual bool execute( string ) = 0;
  virtual int resultId( void ) = 0;
  virtual long long resultKey( const string & keyName, const string & tableName ) = 0;
  virtual bool beginTransaction() = 0;
  virtual bool commit() = 0;
  virtual bool rollback() = 0;

  // methods implemented here
  virtual string & getErrors( void ) { return rdb.errorString(); }

};


/*
 This openRS base class defines the interfaces to the recordset
*/
class openRS
{

protected:

  openDB    & rdb;
  fieldVector ordinals;
  fieldMap    associations;
  int         m_iRecords;
  int         m_iFields;
  bool        m_bOpen;
  bool        m_bTransactioning;

  // pure virtual
  virtual bool beginTransaction()=0;
  virtual bool commit()=0;

public:
  openRS(openDB & idb );
  virtual ~ openRS();
  // pure virtual
  virtual bool open( string sql )=0;
  virtual bool next( int rows = 1 )=0;
  virtual bool previous( int rows = 1 )=0;
  virtual bool close( void )=0;


  basicField & getField( int );  
  basicField & getField( const char * );
  bool hasField( const char * );

  inline int getFieldCount( void ){ return m_iFields;}
  inline int getRecordCount( void ){ return m_iRecords; }
  inline int isOpen( void ){ return m_bOpen; }
  virtual string & getErrors( void ) { return rdb.errorString(); }

};

#endif
