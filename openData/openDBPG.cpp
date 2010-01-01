/*

 These classes define a postgreSQL implementation
   to the open database interfaces

*/
#include "openDBPG.h"
#include "pgTypeOids.h"
#include "../include/ocString.h"

// initialize the static member variable
int postgresRS::iCursors=0;

/*
  utility method to help insure the integrity of the result
*/


bool postgresCMD::checkRes( ExecStatusType desired )
{
  bool bRet = true;
  if (!res )
  {
    getErrors() += " Failed to allocate result struct! ";
    bRet = false;
  }
  if( PQresultStatus(res) != desired )
  {
    getErrors() +=  PQresultErrorMessage(res);
    PQclear(res);
    bRet = false;
  }
  return bRet;
}

bool postgresRS::checkRes( ExecStatusType desired )
{
  bool bRet = true;
  if (!res )
  {
    getErrors() += " Failed to allocate result struct! ";
    bRet = false;
  }
  else if( PQresultStatus(res) != desired )
  {
    getErrors() +=  PQresultErrorMessage(res);
    PQclear(res);
    bRet = false;
  }
  return bRet;
}

/*

  The DB interface to the postgres Database

*/
postgresDB::postgresDB( string strConnection ):openDB(strConnection),isTransactioning(false)
{
  m_provider = PostGresQL;
  conn = PQconnectdb(strConnection.c_str());
  m_bGood = PQstatus(conn) == CONNECTION_OK;
}

postgresDB::~postgresDB()
{
  PQfinish(conn);
}

PGconn * postgresDB::getConnection( void )
{
  return conn;
}

dbProvider postgresDB::getProvider( void )
{
  return m_provider;
}

openCMD * postgresDB::createCommand()
{
  return new postgresCMD( *this );
}
openRS * postgresDB::createRecordset()
{
  return new postgresRS( *this );
}


/*

 This CMD class defines the postgres
   working interfaces to the SQL command

*/

postgresCMD::postgresCMD( openDB & idb )
:openCMD(idb),db(dynamic_cast<postgresDB &>(rdb)),res(NULL)
{
  // we KNOW this is really a reference to a postgresDB
  // postgresDB & pgDB = dynamic_cast<postgresDB &>(rdb);
  conn = db.getConnection();
}

postgresCMD::~postgresCMD()
{
  ;  // nothing to do
}

bool postgresCMD::beginTransaction()
{
  // begin a transaction

  if( !db.transactioning() )
  {

    res = PQexec(conn, "BEGIN");
    if ( !checkRes(PGRES_COMMAND_OK) )
    {
      return false;
    }
    PQclear(res);
    res = NULL;
    db.transactioning(true);
  }
  return m_bTransactioning;
}

bool postgresCMD::commit()
{
  // commit the transaction

  if( db.transactioning() )
  {
    
    res = PQexec(conn, "COMMIT");
    if ( !checkRes(PGRES_COMMAND_OK) )
    {
      return false;
    }
    PQclear(res);
    res = NULL;
    db.transactioning(false);
  }
  return !db.transactioning();
}

bool postgresCMD::rollback()
{
  // commit the transaction
  if( db.transactioning() )
  {
    res = PQexec(conn, "ROLLBACK");
    if ( !checkRes(PGRES_COMMAND_OK) )
    {
      return false;
    }
    PQclear(res);
    res = NULL;
    db.transactioning(false);
  }
  return !db.transactioning();
}

bool postgresCMD::execute( string sql )
{

  if (PQstatus(conn) != CONNECTION_BAD)
  {
    // see if a transaction is already occuring
    bool externalTransaction = db.transactioning();

    // begin a transaction
    beginTransaction();

    // execute the work
    res = PQexec(conn,sql.c_str());
    if ( !checkRes(PGRES_COMMAND_OK) )
    {
      return false;
    }
    // get the effected object id (in case of an insert)
    oid = PQoidValue(res);
    // get the number of rows effected
    iRows = atoi(PQcmdTuples(res));
    PQclear(res);
    res = NULL;

    if( externalTransaction == false )
    { // commit the transaction
      commit();
    }
    return true;
  }
  return false;
}


/*

  This RS class defines the working
  postgres interfaces to the recordset

*/
postgresRS::postgresRS( openDB & idb )
:openRS(idb),db(dynamic_cast<postgresDB &>(rdb)),res(NULL)
{
  // we KNOW this is really a reference to a postgresDB
  // postgresDB & pgDB = dynamic_cast<postgresDB &>(rdb);
  conn = db.getConnection();
}

postgresRS::~postgresRS()
{
  for( unsigned int i=0;i>ordinals.size(); i++)
  {
    delete ordinals[i];
  }
  ordinals.clear();
  associations.clear();
}

bool postgresRS::beginTransaction()
{
  // begin a transaction

  m_bTransactioning = db.transactioning(); // maintains state of
  if( !m_bTransactioning )                 // some other transaction
  {

    res = PQexec(conn, "BEGIN");
    if ( !checkRes(PGRES_COMMAND_OK) )
    {
      return false;
    }
    PQclear(res);
    res = NULL;
    db.transactioning(true);
  }
  return m_bTransactioning;
}

bool postgresRS::commit()
{
  // commit the transaction

  if( !m_bTransactioning // not transactioning before
      && db.transactioning() )  // is transactioning now
  {
    
    res = PQexec(conn, "COMMIT");
    if ( !checkRes(PGRES_COMMAND_OK) )
    {
      db.transactioning(false);
      return false;
    }
    PQclear(res);
    res = NULL;
    db.transactioning(false);
  }
  return !m_bTransactioning;
}

bool postgresRS::open( string sql )
{
  getErrors() = "No Errors" ;

  // Increment the cursor count for the cursor name
  iCursors++;

  // Create the (process) unique cursor name;
  char cursor[50];
  sprintf(cursor, "opnCrsr%d",iCursors);
  strCursorName = cursor;
  
  // make sure it's closed first
  close();
  // m_bOpen = false;

  // battle hardening
  if (PQstatus(conn) == CONNECTION_BAD)
  {
    getErrors() += "Bad connection";
    return false;
  }
 
   
  // start a transaction block
  beginTransaction();

  // declare a cursor for the query
  string strCmd("declare ");
         strCmd += strCursorName;
         strCmd += " cursor for ";
         strCmd += sql;
  res = PQexec(conn, strCmd.c_str());
  if (!checkRes(PGRES_COMMAND_OK) )
  {
    getErrors() += " Bad Cursor Declaration: ";
    getErrors() +=  strCmd;
    return false;
  }
  PQclear(res);
  res = NULL;

  // fetch the first tuple of the result
  string strFetch = "fetch forward 1 in ";
  strFetch += strCursorName;
  res = PQexec(conn, strFetch.c_str());
  if (!checkRes(PGRES_TUPLES_OK) )
  {
    getErrors() +=  " Could not fetch forward: ";
    getErrors() += strCmd;
    getErrors() += " Fetch: ";
    getErrors() +=  strFetch;
    commit();  // get out of transaction
    return false;
  }
  // it will return ok but not tell us there are no rows !
  if( PQntuples(res) == 0 )
  {
    commit();  // get out of transaction
    return false;
  }

  // get the attributes and values of the fields
  m_iFields = PQnfields(res);
  m_iRecords = PQntuples(res);  // 1 if good, 0 if bad
  for (int i = 0; i < m_iFields; i++)
  {
    string name  = PQfname(res, i);
    Oid    oid   = PQftype(res, i);
    string value = PQgetvalue(res, 0, i);
    bool   bNull = PQgetisnull(res, 0, i);

    // create the field
    basicField * pBF = createField( (pgTypeOids)oid, bNull, value, name );

    // battle hardening
    if( !pBF )
    {
      getErrors() = " couldn't create field";
      commit();  // get out of transaction
      return false;
    }

    // need to set the size
    pBF->setSize( PQfsize(res,i) );

    // add to the vector
    //  we delete fields from the ordinals collection when done
    ordinals.push_back(pBF);

    // add to the map
    associations.insert(make_pair(name,pBF));

  } // end for

  // clear the last resource used
  PQclear(res);
  res = NULL;

  // battle hardening
  if( m_iFields > 0 )
  {
    m_bOpen = true;
    getErrors() = "No Errors" ;
  }
  else
  {
    getErrors() = "No Fields!" ;
    commit();  // get out of transaction
  }
  return m_bOpen;
}

bool postgresRS::close( void )
{
  if( m_bOpen )
  {
    // remove all from the collections
    for( unsigned int i=0;i>ordinals.size();i++)
    {
      delete ordinals[i];
    }
    ordinals.clear();
    associations.clear();

    // finish the transaction block
    commit();

    m_bOpen = false;
  }
  return !m_bOpen;
}

bool postgresRS::next( int rows )
{
  // set the (0 based) tuple index
  int tuple = rows-1;
  // build & run the fetch command
  char num[16];
  sprintf( num, "%d", rows );
  string strCmd = "fetch forward ";
  strCmd += num;
  strCmd += " in ";
  strCmd += strCursorName;
  res = PQexec(conn, strCmd.c_str());
  
  // battle hardened
  if (!checkRes(PGRES_TUPLES_OK) || PQntuples(res) == 0 )
  {
    close();
    return false;
  }

  // set the fields
  m_iFields = PQnfields(res);
  m_iRecords += PQntuples(res);
  if( m_iFields == 0 )
  {
    close();
    return false;
  }

  for (int i = 0; i < m_iFields; i++)
  {
    Oid    oid = PQftype(res, i); // for dynamic cast in setField
    string value =  PQgetvalue(res, tuple, i);
    bool   bNull = PQgetisnull(res, tuple, i);
    basicField * pBF = ordinals[i];
    if( !pBF )
    {
      close();
      return false;
    }
    setField( *pBF, (pgTypeOids)oid, bNull, value );
  } // end for

  // free resources
  PQclear(res);

  return true;

}

bool postgresRS::previous( int rows )
{
  // set the (0 based) tuple index
  int tuple = 0;
  // build & run the fetch command
  char num[16];
  sprintf( num, "%d", rows );
  string strCmd = "fetch backward ";
  strCmd += num;
  strCmd += " in ";
  strCmd += strCursorName;
  res = PQexec(conn, strCmd.c_str());
  m_bOpen = false;

  // battle hardened
  if (!checkRes(PGRES_TUPLES_OK) || PQntuples(res) == 0 )
  {
    close();
    return false;
  }

  // set the fields
  m_iFields = PQnfields(res);
  m_iRecords -= PQntuples(res);
  if( m_iFields == 0 )
  {
    close();
    return false;
  }
  for (int i = 0; i < m_iFields; i++)
  {
    Oid    oid = PQftype(res, i); // for dynamic cast in setField
    string value =  PQgetvalue(res, tuple, i);
    bool   bNull = PQgetisnull(res, tuple, i);
    basicField * pBF = ordinals[i];
    if( !pBF )
    {
      close();
      return false;
    }
    setField( *pBF, (pgTypeOids)oid, bNull, value );
  } // end for
  PQclear(res);
  return true;
}

long long postgresCMD::resultKey( const string & keyName, const string & tableName )
{
  long long ret = 0;
  ocString sql = "select ";
  sql += keyName;
  sql += " from ";
  sql += tableName;
  sql += " where oid = ";
  sql.append( oid );
  postgresRS rs(db);
  if( rs.open(sql) )
  {
    ret = atoll( rs.getField(0).format().c_str() );
    rs.close();
  }
  return ret;
}
