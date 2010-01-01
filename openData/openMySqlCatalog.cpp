/*
  openMySqlCatalog.cpp
  
*/
#include "openMyDB.h"
#include "openCatalog.h"

openCatalog::openCatalog(  string connectstring  ):m_pDB(0),m_pRS(0)
{
  m_connectstring = connectstring;
  mySqlDB * mydb = new mySqlDB( m_connectstring);
  if( mydb )
  {
    m_pDB = mydb;
    m_pRS = new mySqlRS(*mydb);
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
  string sql = "show tables";
  
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
  string sql = "show fields from ";
  sql += m_name;
    
  // clone the db and rs
  mySqlDB clonedDB( m_connectstring );
  mySqlRS freshRS(clonedDB);
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
