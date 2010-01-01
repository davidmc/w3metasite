/*

  These classes defines a postgreSQL implementation
    to the open database interfaces

*/
#ifndef OPENDBPG_H
#define OPENDBPG_H 1

extern "C"
{
#include <libpq-fe.h>
}
#include "openDB.h"
#include "pgTypeOids.h"

class postgresDB: public openDB
{

protected:
  PGconn     *conn;
  bool       isTransactioning;
  virtual openCMD * createCommand();
  virtual openRS * createRecordset();
  
public:

  postgresDB( string strConnection );
  virtual ~postgresDB();
  PGconn * getConnection( void );
  virtual dbProvider getProvider( void );
  inline bool transactioning( void ){ return isTransactioning; }
  inline void transactioning( bool in ){ isTransactioning=in; }
  
  
};


/*
 This base class defines the interfaces to the command
*/
class postgresCMD: public openCMD
{

protected:
  PGconn *   conn;
  postgresDB &db;
  Oid        oid;
  int        iRows;
  PGresult   *res;
  bool checkRes( ExecStatusType desired );


public:

  postgresCMD( openDB & idb );

  virtual ~postgresCMD();

  // methods that must be defined by impl
  virtual bool execute( string );
  virtual int resultId( void ){ return static_cast<int>(oid); };
  virtual long long resultKey( const string & keyName, const string & tableName );
  virtual bool beginTransaction();
  virtual bool commit();
  virtual bool rollback();

};


/*
 This openRS base class defines the interfaces to the recordset
*/
class postgresRS: public openRS
{ 
protected:

  PGconn     *conn;
  postgresDB &db;
  static int iCursors;
  string     strCursorName;
  PGresult   *res;
  bool       checkRes( ExecStatusType desired );
  bool beginTransaction();
  bool commit();
public:

  postgresRS( openDB & idb );

  virtual ~ postgresRS();

  virtual bool open( string sql );
  virtual bool close( void );
  virtual bool next( int rows = 1 );

  virtual bool previous( int rows = 1 );

};

#endif
