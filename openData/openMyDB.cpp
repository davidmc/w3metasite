/*
   These classes define a
   mySQL implementation
   to the open database interfaces.
   ================================
*/

#include "openMyDB.h"
#include "ocString.h"
#include <cstdlib>

// #define DO_OPEN_LOGGING
# include <iostream>
using namespace std;

#include "openLogger.h"
/*
  utility method to help insure the integrity of the result
*/

bool mySqlDB::error( int state ) // defaulted to 0
{
  bool bRet = true;
  // regardless, get any error in the buffer
  m_strErrors = mysql_error(&mysql);
  // state is the return from a query, error if non-zero
  // also check if the returned error buffer has length
  bRet = state != 0 || m_strErrors.length() > 0;
  return bRet;
}


/*

  The DB interface to the mySql Database

*/
mySqlDB::mySqlDB( string strConnection )
:openDB(strConnection)
,port(0)
,pchSocket(NULL)
,connection(NULL)
,transactioning(false)
{
  m_bGood = true;  
  m_provider = MySQL;
  parseConnection(strConnection);
  // Initialize the database handler
  mysql_init(&mysql);
  // As opposed to a fake connect
  connection = mysql_real_connect( &mysql,
                                   host.c_str(),
                                   uid.c_str(),
                                   pwd.c_str(),
                                   db.c_str(),
                                   port,
                                   pchSocket,
                                   flags  );
  if( connection == NULL )
  {
    m_bGood = false;
    m_strErrors = "No Real Connection Possible at present: ";
    m_strErrors += mysql_error(&mysql);
  }
}

bool mySqlDB::parseConnection( string & strConnection )
{
  bool bRet=false;
  host = "";
  uid = "";
  pwd = "";
  db = "";
  port = 0;
  socket = "";
  pchSocket = NULL;
  flags = 0;

  ocString connStr = strConnection;
  connStr.parseInit();
  for( string element=connStr.parse(";");
       !(element.length()==0 && connStr.endOfParse() );
       element=connStr.parse(";") )
  {
    ocString apair(element);
    string name = apair.parse("=");
    string value = apair.parse("=");
    /*
        parse params to connect:
        host,  uid,  pwd,  db, port,  socket, flags
    */
    transform(name.begin(),name.end(),name.begin(),(int(*)(int)) tolower );
    if( name == "host" )
    {
      host = value;
    }
    else if ( name == "uid" )
    {
      uid = value;
    }
    else if( name == "pwd" )
    {
      pwd = value;
    }
    else if( name == "db" )
    {
      db = value;
    }
    else if( name == "port" )
    {
      port = atol(value.c_str());
    }
    else if( name == "socket" )
    {
      socket = value;
      if( socket.length() )
      {
        pchSocket = socket.c_str();
      }
    }
    else if( name == "socket" )
    {
      socket = value;
    }
    else if( name == "flags" )
    {
      flags = atol(value.c_str());
    }
  } // end for

  // Minimum result for a good parse
  if( db.length() && uid.length() )
  {
    bRet = true;
  }

  return bRet;
}
dbProvider mySqlDB::getProvider( void )
{
  return m_provider;
}

mySqlDB::~mySqlDB()
{
  mysql_close( connection );
}


openCMD * mySqlDB::createCommand()
{
  return new mySqlCMD( *this );
}

openRS * mySqlDB::createRecordset()
{
  return new mySqlRS( *this );
}


/*

 This CMD class defines the mySql
   working interfaces to the SQL command

*/
mySqlCMD::mySqlCMD( openDB & idb ):openCMD(idb),state(0),rows(0)
{
  // we KNOW this is really a reference to a mySqlDB
  poDB = & dynamic_cast<mySqlDB &>(rdb);
}

mySqlCMD::~mySqlCMD()
{
  ;  // nothing to do
}

bool mySqlCMD::beginTransaction()
{
  // begin a transaction
  if( !m_bTransactioning )
  {
    m_bTransactioning = true;
  }
  return m_bTransactioning;
}

bool mySqlCMD::commit()
{
  // commit the transaction
  if( m_bTransactioning )
  {
    m_bTransactioning = false;
  }
  return !m_bTransactioning;
}

bool mySqlCMD::rollback()
{
  // commit the transaction
  if( m_bTransactioning )
  {
    m_bTransactioning = false;
  }
  return !m_bTransactioning;
}

bool mySqlCMD::execute( string sql )
{
  bool bRet = false;
  if( poDB )
  {
    if( poDB->isGood() )
    {
      writelog2("mySqlCMD::execute: ", sql );
      state = mysql_real_query( poDB->getConnection(),
                                sql.c_str(),
                                sql.length() );
      bRet = poDB->error(state) == false;
    }
    if( bRet )
    {
      // get the effected row count count
      rows = mysql_affected_rows(poDB->getConnection());
    }
  }
  return bRet;
}
int mySqlCMD::resultId( void )
{
  return (int) mysql_insert_id(poDB->getConnection());
}
long long mySqlCMD::resultKey( const string & , const string &  )
{
  return (int) mysql_insert_id(poDB->getConnection());
}
/*
  This RS class defines the working
  mySql interfaces to the recordset
*/
mySqlRS::mySqlRS( openDB & idb )
:openRS(idb),state(0),rows(0),result(NULL)
{
  // we KNOW this is really a reference to a mySqlDB
  poDB = dynamic_cast<mySqlDB *>(&idb);
}

mySqlRS::~mySqlRS()
{
  close();
}

bool mySqlRS::open( string sql )
{
  // make sure it's closed first!
  close();
  m_iFields = 0;
  if( poDB && poDB->isGood() )
  {
    //perform the query
    writelog2("mySqlRS::open: ", sql ); 
    state = mysql_real_query(poDB->getConnection(),sql.c_str(),sql.length());
    writelog2("mySqlRS::open mysql_real_query state is: ", state);
    m_bOpen = poDB->error(state) == false;
    writelog2("mySqlRS::open opened? ", m_bOpen);
    if( m_bOpen )
    {
      // set the base class field count
      m_iFields = mysql_field_count(poDB->getConnection());
      writelog2("mySqlRS::open field count: ", m_iFields );

      // get the results
      result = mysql_store_result(poDB->getConnection());
      // result = mysql_use_result(poDB->getConnection());
      writelog2("mySqlRS::open result: ", result );

      if( result )
      {
        // set the record count of the current set
        m_iRecords = mysql_num_rows( result );
        // return fields info
        MYSQL_FIELD * pFields = mysql_fetch_fields( result );
        // writelog2("mySqlRS::open field pointer: ", pFields );

        for( int icol = 0 ; icol < m_iFields; icol ++ )
        {
          mysqlColInfo * pInfo = new mysqlColInfo;
          // writelog2("mySqlRS::open col info pointer: ", pInfo );
          if(pInfo)
          {
            // Add the column info. to the back of the vector
            colInfo.push_back(pInfo);

            // Set the info properties
            pInfo->mySqlType = pFields[icol].type;
            pInfo->length = pFields[icol].length;
            pInfo->decimals = pFields[icol].decimals;
            pInfo->ColName = pFields[icol].name;

            // Get the map item based on sql type
            my_mapItem * pItem = colMap.data[pInfo->mySqlType];

            // Debug cout << " MapItem Allocated: "  << pItem << endl;
            if( pItem )
            {
              // create the right kind of field
              basicField * pField =  pItem->createField( pInfo );
              if( pField )
              {
                // Add to the vector
                ordinals.push_back(pField);
                // Add to the map
                associations.insert(make_pair(pInfo->ColName,pField));
                // writelog3(pFields[icol].name, " Type", pField->getType() );
              }
            }
            else
            {
              poDB->errorString() += "\r\nUnable to find type in map\r\n";
              // writelog2("mySqlRS::open error: ", poDB->errorString() );
            }
          }
        }// end for all cols
        // get the first record values
        m_bOpen = next();
      } // end if result
    } // end if record opened
  } // end if db
  // cout << poDB->errorString();
  return m_bOpen;
}

// Override to base class
string & mySqlRS::getErrors( void )
{
  string temp = poDB->errorString();
  poDB->error();
  poDB->errorString() += temp;
  return poDB->errorString();
}

bool mySqlRS::beginTransaction()
{
  // begin a transaction
  if( !m_bTransactioning )
  {
    m_bTransactioning = true;
  }
  return m_bTransactioning;
}

bool mySqlRS::commit()
{
  // commit the transaction
  if( m_bTransactioning )
  {
    m_bTransactioning = false;
  }
  return !m_bTransactioning;
}

bool mySqlRS::close( void )
{
  writelog("mySqlRS::close: " );
  unsigned int i;
  // remove all from the collections
  for( i=0;i<ordinals.size();i++)
  {
    delete ordinals[i];
  }
  ordinals.clear();
  associations.clear();
  for( i=0;i<colInfo.size();i++)
  {
    // writelog2("mySqlRS::close delete info pointer: ", colInfo[i] );
    delete colInfo[i];
  }
  colInfo.clear();
  if( result )
  {
    mysql_free_result( result );
    result = NULL;
  }
  m_bOpen=false;
  return !m_bOpen;
}

bool mySqlRS::next( int )
{
  // return row
  bool ret = false;
  writelog("Getting another row..");
  MYSQL_ROW row = mysql_fetch_row(result);
  writelog2("Got row.", row );
  if( row )
  {
    ret = true;
    for( int icol = 0 ; icol < m_iFields; icol ++ )
    {
      writelog2("Moving Col", icol );
      // get the colInfo (by ordinal)
      mysqlColInfo * pInfo = colInfo[icol];
      // get the map item based on sql type
      my_mapItem * pItem = colMap.data[pInfo->mySqlType];
      // get the field (by ordinal)
      basicField * pField = ordinals[icol];
      //finally, set the field
      if(pInfo&&pItem&&pField)
      {
        // writelog2("Data Raw: ", row[icol] );
        pItem->setField( pField, row[icol] );
        // writelog2("Data After: ", pField->format() );
      }
    }
  }
  else
  {
    close();
    poDB->error();
  }
  return ret;
}

bool mySqlRS::previous( int )
{
  // mySQL doesn't cursor in reverse
  return false;
}
