/*
  openDB.hpp includes openDB.h and any other
   implementation files used in the context of the executible.
   The other implementation file have defines
   that are used by the preprocessor to conditionally
   include cases in the switch statements of the factories

   Copyright 2002 - w3 Systems Development
   Author D.K.McCombs davidmc@w3sys.com
*/
#include "openDB.hpp"

// database object factory
openDbFactory::openDbFactory( dbProvider dbProv, const char * strConnection )
{
  pDB = NULL;
  switch( dbProv )
  {
#ifdef OPENDBPG_H
  case PostGresQL:
    pDB = new postgresDB(strConnection);
    // cout << "PostgreSQL is providing data services" << endl;
    break;
#endif
#ifdef OPEN_ODBC_H
  case ODBC:
    pDB = new odbcDB(strConnection);
    break;
#endif
#ifdef OPEN_MYDB_H
  case MySQL:
    pDB = new mySqlDB(strConnection);
    break;
#endif
  default:
    pDB = NULL;
    break;
  };
}
openDbFactory::~openDbFactory()
{
  delete pDB;
}
openDB & openDbFactory::db()
{
  return * pDB;
}

// recordset object factory
openRsFactory::openRsFactory( openDB & rdb )
{
  pRS = rdb.createRecordset();
}

openRsFactory::~openRsFactory()
{
  delete pRS;
}
openRS & openRsFactory::rs()
{
  return * pRS;
}


// command object factory
openCmdFactory::openCmdFactory( openDB & rdb )
{
  pCMD = rdb.createCommand();
}
openCmdFactory::~openCmdFactory()
{
  delete pCMD;
}
openCMD & openCmdFactory::cmd()
{
  return * pCMD;
}

/*
 This base class defines the interfaces to the database
*/
openDB::openDB(  string strConnection ):m_bGood(false),m_strErrors("")
{
  ;
}
openDB::~openDB()
{
  ;// trivial implementation
}

/*
 This base class defines the interfaces to the command
*/
openCMD::openCMD( openDB & idb ):rdb(idb),m_bTransactioning(false)
{
  ;
}
openCMD::~openCMD()
{
    ; // trivial implementation
}

/*
 This base class defines the interfaces to the recordset

 member vars:
  openDB    & rdb;
  fieldVector ordinals;
  fieldMap    associations;
  int         m_iRecords;
  int         m_iFields;
  bool        m_bOpen;
  bool        m_bTransactioning;

*/
openRS::openRS(openDB & idb )
:rdb(idb),m_iRecords(0),m_iFields(0)
,m_bOpen(false),m_bTransactioning(false)
{
  ;
}
openRS::~openRS()
{
  ; // trivial implementation
}

// non-trivially implemented here
basicField & openRS::getField( int idx )
{
  return * ordinals[idx];
}
// non-trivially implemented here
basicField & openRS::getField( const char * name )
{
  return * associations[name];
}
bool openRS::hasField( const char * name )
{
  fieldMap::iterator pos =  associations.find(name);
  return pos != associations.end();
}

