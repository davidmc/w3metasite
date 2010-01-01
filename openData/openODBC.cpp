/*
   These classes define a
   ODBC implementation
   to the open database interfaces.
   ================================
*/

#include "openODBC.h"
#include "ocString.h"


/*
  utility method to help insure the integrity of the result
*/

bool odbcDB::error( SQLHSTMT hstmt )
{
  UCHAR sqlstate[10];
  UCHAR errmsg[SQL_MAX_MESSAGE_LENGTH];
  SDWORD nativeerr;
  SWORD actualmsglen;
  RETCODE rc;
  bool bRet = true;
  m_bGood = false;
  do
  {
    rc = SQLError( henv, hdbc, hstmt,
                   sqlstate, &nativeerr,
                   errmsg,
                   SQL_MAX_MESSAGE_LENGTH - 1, // for the null
                   &actualmsglen );

    if (rc == SQL_ERROR)
    {
      m_strErrors += "SQLError failed!\n";
      bRet = false;
    }
    else if( rc != SQL_NO_DATA_FOUND )
    {
      // append a null to the end of the errmsg buffer
      errmsg[actualmsglen] = '\0';
      m_strErrors += "SQLSTATE = ";
      m_strErrors += (char *)sqlstate;
      m_strErrors += "\nNATIVE ERROR = ";
      m_strErrors += nativeerr;
      m_strErrors += "\nMSG = ";
      m_strErrors += (char *)errmsg;
      m_strErrors += "\n\n";
    }
  } while ( rc != SQL_NO_DATA_FOUND && rc != SQL_ERROR );
  if(hstmt != SQL_NULL_HSTMT)
  {
    SQLFreeStmt(hstmt, SQL_CLOSE) ;
  }
  return bRet;
}


/*

  The DB interface to the odbc Database

*/
odbcDB::odbcDB( string strConnection ):openDB(strConnection)
{
  // hdbc; henv;  rc;
  // expect these for the dsn
  // dsn=mssql1;
  // pwd=webuser;
  // uid=webuser
  m_strErrors = "";
  m_provider = ODBC;

  rc = SQLAllocEnv(&henv);
  if ( (rc == SQL_SUCCESS) || (rc == SQL_SUCCESS_WITH_INFO))
  {
    rc = SQLAllocConnect( henv, &hdbc);
    if( parseConnection( strConnection ) )
    {
      for ( int retries = 1; retries <= 3; retries++ )
      {

        rc = SQLConnect( hdbc,
                        (SQLCHAR*)dsn.c_str(), SQL_NTS,
                        (SQLCHAR*)uid.c_str(), SQL_NTS,
                        (SQLCHAR*)pwd.c_str(), SQL_NTS );

        if( ( rc == SQL_SUCCESS) || (rc == SQL_SUCCESS_WITH_INFO) )
        {
          m_bGood = true;
          m_strErrors = "Connection OK\n";
          break;
        }
        else
        {
          error(SQL_NULL_HSTMT);
        }
      }
    }
  }

  // cout << "End of odbcDB Constructor: " << m_strErrors << endl;
}

bool odbcDB::parseConnection( string & strConnection )
{
  bool bRet=false;
  dsn = uid = pwd = "";
  // expect these for the dsn
  // dsn=mssql1;pwd=webuser;uid=webuser
  ocString connStr = strConnection;
  connStr.parseInit();
  for( string element=connStr.parse(";");
       !(element.length()==0 && connStr.endOfParse() );
       element=connStr.parse(";") )
  {
    ocString apair(element);
    string name = apair.parse("=");
    string value = apair.parse("=");
    transform(name.begin(),name.end(),name.begin(),(int(*)(int)) toupper );
    if( name == "DSN" )
    {
      dsn = value;
    }
    else if ( name == "UID" )
    {
      uid = value;
    }
    else if( name == "PWD" )
    {
      pwd = value;
    }
  }
  if( dsn.length() && uid.length() && pwd.length() )
  {
    bRet = true;
  }

  return bRet;
}
dbProvider odbcDB::getProvider( void )
{
  return m_provider;
}
odbcDB::~odbcDB()
{
  SQLDisconnect(hdbc);
  SQLFreeConnect(hdbc);
  SQLFreeEnv(henv);
}


openCMD * odbcDB::createCommand()
{
  return new odbcCMD( *this );
}

openRS * odbcDB::createRecordset()
{
  return new odbcRS( *this );
}


/*

 This CMD class defines the odbc
   working interfaces to the SQL command

*/

odbcCMD::odbcCMD( openDB & idb ):openCMD(idb)
{
  // we KNOW this is really a reference to an odbcDB
  poDB = & dynamic_cast<odbcDB &>(rdb);
}

odbcCMD::~odbcCMD()
{
  ;  // nothing to do
}

bool odbcCMD::beginTransaction()
{
  // begin a transaction
  if( !m_bTransactioning )
  {

    m_bTransactioning = true;
  }
  return m_bTransactioning;
}

bool odbcCMD::commit()
{
  // commit the transaction
  if( m_bTransactioning )
  {

    m_bTransactioning = false;
  }
  return !m_bTransactioning;
}

bool odbcCMD::rollback()
{
  // commit the transaction
  if( m_bTransactioning )
  {

    m_bTransactioning = false;
  }
  return !m_bTransactioning;
}

bool odbcCMD::execute( string sql )
{
  bool bRet = false;
  rc = SQLAllocStmt( poDB->getDbConnHandle(), &hstmt );
  if ( (rc != SQL_SUCCESS) &&
       (rc != SQL_SUCCESS_WITH_INFO) )
  {
    poDB->error(SQL_NULL_HSTMT);
  }
  else
  {
    // get the data
    rc = SQLExecDirect (hstmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
    if( (rc != SQL_SUCCESS) &&
        (rc != SQL_SUCCESS_WITH_INFO) )
    {
      poDB->error(hstmt);
    }
    else
    {
      bRet = true;
    }
    SQLFreeStmt ((SQLHSTMT)hstmt, SQL_CLOSE) ;
  }
  return bRet;
}

/*

  This RS class defines the working
  odbc interfaces to the recordset

*/
odbcRS::odbcRS( openDB & idb )
:openRS(idb),hstmt(0),rc(0),numCols(0)
{
  // we KNOW this is really a reference to a odbcDB
  poDB = dynamic_cast<odbcDB *>(&idb);
}

odbcRS::~odbcRS()
{
  for( unsigned int i=0;i>ordinals.size();i++)
  {
    delete ordinals[i];
  }
  ordinals.clear();
  associations.clear();
}

bool odbcRS::open( string sql )
{
  rc = SQLAllocStmt( poDB->getDbConnHandle(), &hstmt );
  if ( (rc != SQL_SUCCESS) &&
       (rc != SQL_SUCCESS_WITH_INFO) )
  {
    poDB->error(SQL_NULL_HSTMT);
  }
  else
  {
    // get the data
    rc = SQLExecDirect (hstmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
    if( (rc != SQL_SUCCESS) &&
        (rc != SQL_SUCCESS_WITH_INFO) )
    {
      poDB->error(hstmt);
    }
    else
    {
      // debug cout << "Query OK!" << endl;
      // all is good, get column count
      numCols = 0 ;
      rc = SQLNumResultCols( hstmt, &numCols );
      if( (rc != SQL_SUCCESS) &&
          (rc != SQL_SUCCESS_WITH_INFO) )
      {
        poDB->error(hstmt);
      }
      else if (numCols == 0)
      {
        // no cols means not a select statement
        m_bOpen = false;
      }
      else
      {
        // set the base class field count
        m_iFields = (int) numCols;

        // debug cout << numCols << " Columns!" << endl;
        UWORD icol;
        // describe, allocate and bind the columns

        for( icol = 0 ; icol < numCols ; icol ++ )
        {
          odbcColInfo * pInfo = new odbcColInfo;
          rc = SQLDescribeCol( hstmt,
                               icol + (UWORD)1,
                               pInfo->szColName,
                               cOL_nAMe_Len,
                               &pInfo->cbColName,
                               &pInfo->fSqlType,
                               &pInfo->cbColDef,
                               &pInfo->ibScale,
                               &pInfo->fNullable) ;
          if ((rc != SQL_SUCCESS) &&
            (rc != SQL_SUCCESS_WITH_INFO))
          {
            delete pInfo;
            poDB->error(hstmt);
            break ;
          }
          else
          {
            // cout << " ColName=" << pInfo->szColName << endl;
            // Add the column info. to the back of the vector
            colInfo.push_back(pInfo);
            // get the map item based on sql type
            mapItem * pItem = colMap.data[pInfo->fSqlType];
            // debug cout << " MapItem Allocated: "  << pItem << endl;
            if( pItem )
            {
              // create the right kind of field
              basicField * pField =  pItem->createField( pInfo );
              if( pField )
              {
                /*
                cout << " ColName: " << pInfo->szColName
                     << " has field "  << pField
                     << " and type " << pInfo->fSqlType
                     << " and length " << pInfo->cbColDef
                     << endl;
                */
                // add to the vector
                ordinals.push_back(pField);
                // add to the map
                associations.insert(make_pair((char*)pInfo->szColName,pField));
                // cout << " get data storage " << endl;
                void * pVoid = pItem->thingToBind( pInfo , pField );
                // cout << " bind " << endl;
                // bind results to the data storage
                rc = SQLBindCol( hstmt,
                                 icol + (UWORD)1,
                                 pItem->cType,
                                 pVoid,
                                 pInfo->cbColDef+1,
                                 &pInfo->length );
                if((rc != SQL_SUCCESS) &&
                   (rc != SQL_SUCCESS_WITH_INFO))
                {
                  poDB->error(hstmt);
                  break;
                }
              }
            }
            else
            {
              poDB->errorString() += "\r\nUnable to find type in map\r\n";
            }
          }
        }// end for all cols

        // now fetch the first row
        m_bOpen = next();
      }
    }
  }
  // cout << poDB->errorString();
  return m_bOpen;
}
bool odbcRS::beginTransaction()
{
  // begin a transaction
  if( !m_bTransactioning )
  {

    m_bTransactioning = true;
  }
  return m_bTransactioning;
}

bool odbcRS::commit()
{
  // commit the transaction
  if( m_bTransactioning )
  {

    m_bTransactioning = false;
  }
  return !m_bTransactioning;
}
bool odbcRS::close( void )
{

  return !m_bOpen;
}

bool odbcRS::next( int )
{

  rc = SQLFetch(hstmt);
  if((rc != SQL_SUCCESS) &&
    (rc != SQL_SUCCESS_WITH_INFO))
  {
    poDB->error(hstmt);
    return false;
  }
  UWORD icol;
  // now do post fetch fixips   numCols
  for( icol = 0 ; icol < numCols; icol ++ )
  {
    odbcColInfo * pInfo = colInfo[icol];
    if( pInfo )
    {
      mapItem * pItem = colMap.data[pInfo->fSqlType];
      if( pItem )
      {
        basicField * pField = ordinals[icol];
        if( pField)
        {
          pItem->postFetchFixup(pInfo,pField);
        }
      }
    }
  }

  return true;
}

bool odbcRS::previous( int )
{

  return true;
}
