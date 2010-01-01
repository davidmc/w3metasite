/*
  openPgSqlCatalog.cpp
  
*/
#include "openDBPG.h"
#include "openCatalog.h"

openCatalog::openCatalog(  string connectstring  ):m_pDB(0),m_pRS(0)
{
  m_connectstring = connectstring;
  postgresDB * pgdb = new postgresDB( m_connectstring);
  if( pgdb )
  {
    m_pDB = pgdb;
    m_pRS = new postgresRS(*pgdb);
  }  
}

openCatalog::~openCatalog()
{
  delete m_pRS;
  delete m_pDB;
}

bool openCatalog::open()
{
  bool bret = false;
  string sql = "select cast(relname as text) from pg_class where relnamespace=2200 and relkind='r'";
  
  bret = m_pRS->open(sql);
  if( bret )
  {
    m_name = m_pRS->getField(0).format();
  }
  return bret;
}
bool openCatalog::next()
{
  bool bret = m_pRS->next();
  if( bret )
  {
    m_name = m_pRS->getField(0).format();
  }  
  return bret;
}
bool openCatalog::close()
{
  return m_pRS->close();
}
openFields & openCatalog::fieldsList()
{
  m_fields.clear();
  string sql = "select cast(attname as text), cast(atttypid as text) from pg_attribute "
               "where attrelid = (select oid from pg_class where relname = '";
  sql += m_name;
  sql += "') and attstattarget = -1";
  // clone the db and rs
  postgresDB clonedDB( m_connectstring );
  postgresRS freshRS(clonedDB);
  if( freshRS.open(sql) )
  {
    do
    {
      openField fld;
      fld.name(freshRS.getField(0).format());
      fld.type(freshRS.getField(1).format());
      m_fields.push_back(fld);
    } while( freshRS.next() );
  }
  freshRS.close();
  return m_fields;
}
string & openCatalog::name()
{
  return m_name;
}
