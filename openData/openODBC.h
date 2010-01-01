/*

  openODBC.h
  ==========
  These classes define the ODBC implementation
    to the open database class interface.

*/
#ifndef OPEN_OBDC_H
#define OPEN_ODBC_H 1

#include <sqlext.h>
#include "openDB.h"
#include "openFields.h"
#include "odbcTypeMaps.h"

/* ODBC Implementation Classes
   ===========================*/
class odbcDB: public openDB
{
protected:
  SQLHDBC    hdbc;
  SQLHENV    henv;
  SQLRETURN  rc;
  string     m_strErrors;
  string     dsn;
  string     uid;
  string     pwd;
  bool       transactioning;

  virtual bool parseConnection( string & strConnection );
  virtual openCMD * createCommand();
  virtual openRS * createRecordset();

public:
  odbcDB( string strConnection );
  virtual ~odbcDB();
  virtual dbProvider getProvider( void );

  // specific to the ODBC implementation
  SQLHDBC & getDbConnHandle(){ return hdbc; }
  SQLHENV & getEnvHandle(){ return henv; }
  bool error( SQLHSTMT hstmt );
  string  errorString( void ){ return m_strErrors; }
};


/*
 This base class defines the interfaces to the command
*/
class odbcCMD: public openCMD
{
protected:
  odbcDB   *  poDB;
  SQLHSTMT    hstmt;
  SQLRETURN   rc;
  int id;

public:

  odbcCMD( openDB & idb );
  virtual ~odbcCMD();

  // methods that must be defined by impl
  virtual bool execute( string );
  virtual int resultId( void ){ return static_cast<int>(id); };

  virtual bool beginTransaction();
  virtual bool commit();
  virtual bool rollback();
};


const size_t maxColLen = 512;
/*
 This openRS base class defines the interfaces to the recordset
*/
class odbcRS : public openRS
{

protected:
  odbcDB   * poDB;
  SQLHSTMT   hstmt;
  SQLRETURN  rc;
  SWORD      numCols;

  string     strCursorName;

  // for SQLDescribe and SQLBind
  colInfoVector colInfo;
  odbcMap       colMap;

  // transaction management
  bool beginTransaction();
  bool commit();

public:
  odbcRS( openDB & idb );
  virtual ~odbcRS();

  virtual bool open( string sql );
  virtual bool close( void );
  virtual bool next( int rows = 1 );
  virtual bool previous( int rows = 1 );

};

#endif
