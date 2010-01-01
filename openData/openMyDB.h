/*
  openMyDB.h
  ==========
  These classes define the MySQL implementation
    to the open database class interface.

  Copyright 2002 - davidmc@w3sys.com
  W3 System Development and Hosting www.w3sys.com

*/

#ifndef OPEN_MYDB_H
#define OPEN_MYDB_H 1

#include <mysql.h>
#include "openDB.h"
#include "openFields.h"
#include "mySqlTypeMap.h"

/*
  MySQL Implementation Classes
  ===========================
*/
class mySqlDB: public openDB
{
protected:

  string     host;
  string     uid;
  string     pwd;
  string     db;
  unsigned int port;
  string     socket;
  const char *     pchSocket;
  unsigned int flags;

  // MYSQL pointer and struct
  MYSQL * connection, mysql;
  bool    transactioning;

  virtual bool parseConnection( string & strConnection );
  virtual openCMD * createCommand();
  virtual openRS * createRecordset();

public:

  mySqlDB( string strConnection );
  virtual ~mySqlDB();
  virtual dbProvider getProvider( void );

  bool error( int state = 0 );

  MYSQL * getConnection( void )
  {
    return connection;
  }
};


/*
  This base class defines the interfaces to the command
  =====================================================
*/
class mySqlCMD: public openCMD
{
protected:
  mySqlDB   *  poDB;

  // return from query
  int state;
  size_t rows;

public:

  mySqlCMD( openDB & idb );
  virtual ~mySqlCMD();

  // methods that must be defined by impl
  virtual bool execute( string );
  virtual int resultId( void );
  virtual long long resultKey( const string & keyName , const string & tableName );
  virtual bool beginTransaction();
  virtual bool commit();
  virtual bool rollback();
};


const size_t maxColLen = 512;
/*
  This openRS base class defines the interfaces to the recordset
  ==============================================================
*/
class mySqlRS : public openRS
{

protected:
  mySqlDB   * poDB;

  // return from query
  int state;
  size_t rows;
  MYSQL_RES * result;
  
  // for field type resolution
  my_col_infoVector colInfo;
  mysqlMap          colMap;

  // transaction management
  bool beginTransaction();
  bool commit();

public:
  mySqlRS( openDB & idb );
  virtual ~mySqlRS();

  virtual bool open( string sql );
  virtual bool close( void );
  virtual bool next( int rows = 1 );
  virtual bool previous( int rows = 1 );
  virtual string & getErrors( void );
};

#endif
