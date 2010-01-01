/*
  objectBase.cpp

  Implementation file.
  base class for business objects.

  Copyright 2002 by D.K.McCombs
  Part of open core business object library.

*/

#include "objectBase.h"
#include "pconnectstring"
#include "cgiCookie.h"
#include "ocString.h"

/*
  (roughly) maps openField enumerated types
  to postgres enumerated types
*/
const int pgTypeMap[] = {
  pgShortOid, // shortType,
  pgLongOid, // longType,
  pgFloatOid, // floatType,
  pgDoubleOid, // doubleType,
  pgVarCharArrayOid, // stringType,
  pgNumericOid, // currencyType,
  pgTimestampOid, // dateTimeType, (this is where it's rough)
  pgBooleanOid // boolType
  };

objectBase::objectBase(openLogin &lin,cgiScript & script)
:m_db(CONNECTSTRING),m_rs(m_db),m_cmd(m_db),m_login(lin),m_script(script)
{
  currentTime(&m_current);
}
objectBase::~objectBase()
{

  // delete any m_frMap items
  fieldMap::iterator pos1;
  for( pos1=m_frMap.begin();pos1!=m_frMap.end();++pos1)
  {
    delete pos1->second;
    pos1->second = NULL;
  }
  // delete any m_ctrlMap items
  fieldControls::iterator pos2;
  for( pos2=m_ctrlMap.begin();pos2!=m_ctrlMap.end();++pos2)
  {
    delete pos2->second;
    pos2->second = NULL;
  }
  
}

bool objectBase::audits( bool create )
{
  bool bRet = false;
  char temp[256];
  /*
  Check to see if you have an created field
  */
  string value = m_login.Id();
  fieldMap::iterator pos;
  pos = m_frMap.find("created_by");

  if( pos != m_frMap.end() )
  {
    char temp[256];
    basicField * pfld = NULL;
//    m_script << "doAudits found created..." << endl;
    if( create )
    {
      pfld = m_frMap["created_by"];
      if( pfld )
      {
        setField( *pfld, static_cast<pgTypeOids>(pgTypeMap[pfld->getType()]), false, value, true );
      }
    }
    pfld = m_frMap["modified_by"];
    if( pfld )
    {
//      m_script << "setting modified_by..." << endl;
      setField( *pfld, static_cast<pgTypeOids>(pgTypeMap[pfld->getType()]), false, value, true );
//      m_script << "modified_by set..." << endl;
    }

    fmtTime(temp, sizeof(temp), "%m-%d-%Y %H:%M:%S", &m_current);
    value = temp;
    if( create )
    {
      pfld = m_frMap["created"];
      if( pfld )
      {
        setField( *pfld, static_cast<pgTypeOids>(pgTypeMap[pfld->getType()]), false, value, true );
      }
    }
    pfld = m_frMap["modified"];
    if( pfld )
    {
      setField( *pfld, static_cast<pgTypeOids>(pgTypeMap[pfld->getType()]), false, value, true );
    }
    bRet = true;

  } // end if found create audit field
  return bRet;
}

bool objectBase::go( void )
{
//  m_script << "in go..." << endl;
  // Initially pessimistic estimation of success
  m_bSuccess = false;
  m_bWarning = false;
  cgiCookie cookie;
//  m_script << "getting cookie value formaction..." << endl;
  m_action = cookie.get("formaction");
//  m_script << "getting cookie value changes..." << endl;
  m_changes = cookie.get("changes").c_str();
  m_changes.deHexify('%');
//  m_script << "getting client args..." << endl;
  cgiInput & args = m_script.ClientArguments();
  m_key = "";
  if( args.count("id") )
  {
    m_key = args["id"].str();
  }

//  m_script << "Action: " << m_action << "..." << endl;

  // expect 'u', 'i', or 'd' actions
  if( m_action == "i" )
  {
    m_bSuccess = doInsert(args);
    if( m_bSuccess ) m_bSuccess = getData(args,m_cmd.resultId());
  }
  else if ( m_action ==  "u" )
  {
    if(m_changes.length() > 0 && m_key.length() > 0)
    {
      m_bSuccess = doUpdate(args);
     if( m_bSuccess ) m_bSuccess = getData(args);
    }
    else  m_bSuccess = getData(args);

  }
  else if ( m_action == "d" && m_key.length() > 0 )
  {
    m_bSuccess = doDelete(args);
  }
  else if( (m_action == "s" ||  m_action == "")  && m_key.length() > 0 )
  {
    m_bSuccess = getData(args);
  }
  return m_bSuccess;
} // end go

bool objectBase::getData(cgiInput & args, int oid)
{
  bool bRet = false;
  if( m_key.length() == 0 && oid==0 ) return bRet;

  m_sql = "select ";
  if( m_columns.size() ) m_sql += m_columns[0];
  for( int i=1; i<m_columns.size(); i++)
  {
    m_sql += ",";
    m_sql += m_columns[i];
  }
  m_sql += " from ";
  m_sql += m_name;
  if( oid )
  {
    m_sql += " where oid = ";
    ocAppend( m_sql, oid );
  }
  else
  {
    m_sql += " where id = ";
    m_sql += m_key;
  }
//  m_script << m_sql << endl;
  bRet = m_rs.open(m_sql);
  if( bRet )
  {
    // set the form values to the field values.
    for(int i=0; i<m_rs.getFieldCount();i++)
    {
      string name = m_rs.getField(i).getName();
      fieldControls::iterator pos = m_ctrlMap.find(name);
      if( pos != m_ctrlMap.end() )
      {
        pos->second->setContent(m_rs.getField(i).format());
      }
    }
  }
  else
  {
     m_result = "Failed Query: ";
     m_result += m_sql;
  }
  return bRet;
}

bool objectBase::doInsert(cgiInput & args)
{
  bool bRet = false;
  bool hasAudits = audits(true);
  int iField;

  // Do not proceed if validation fails!
  if( !validate() ) return bRet;

  m_sql = "insert into ";
  m_sql += m_name;
  m_sql += " ( ";

  for( int i=0; i<m_columns.size(); i++)
  {
    string col = m_columns[i];
//    m_script << "Enumerating col: " << col << "..." << endl;
    // do all BUT audit fields and id
    if( col != "id" && col != "created" && col != "created_by" &&  col != "modified" &&  col != "modified_by" )
    {
      m_sql += col;
      if( i+1 < m_columns.size() ) m_sql += ", ";
    }
  }
  if( hasAudits )
  {
    m_sql += " created, created_by, modified, modified_by ";
  }
  m_changes.tokenReset();
  m_sql += ") values ( ";

  for( int i=0; i<m_columns.size(); i++)
  {
    string col = m_columns[i];
    // do all BUT audit fields and id
    if( col != "id" && col != "created" && col != "created_by" &&  col != "modified" &&  col != "modified_by" )
    {
      basicField * pfld = m_frMap[col];
      if( pfld && args.count(col.c_str()) )
      {
        setField( *pfld, static_cast<pgTypeOids>(pgTypeMap[pfld->getType()]), false, args[col.c_str()].str(), true );
        m_sql += pfld->svrFormat();
        if( i+1 < m_columns.size() ) m_sql += ", ";
      }
      else if( pfld && pfld->getType() == boolType ) // case of a bool represented by a checkbox being unchecked
      {
        setField( *pfld, static_cast<pgTypeOids>(pgTypeMap[pfld->getType()]), false, "false", true );
        m_sql += pfld->svrFormat();
        if( i+1 < m_columns.size() ) m_sql += ", ";
      }
      else
      {
        m_result += " Insert Failed: bad field or form variable lookup...";
        m_result += "Values attempting to change: ";
        m_result += m_changes.str();
        m_result += "Current problem: [";
        m_result += col.c_str();
        m_result += "]...\n";
        bRet = false;
        return bRet;
      }
    }
  }
  if( hasAudits )
  {
    m_sql += " ";
    m_sql += m_frMap["created"]->svrFormat();
    m_sql += ", ";
    m_sql += m_frMap["created_by"]->svrFormat();
    m_sql += ", ";
    m_sql += m_frMap["modified"]->svrFormat();
    m_sql += ", ";
    m_sql += m_frMap["modified_by"]->svrFormat();
  }
  m_sql += ")";
  bRet = m_cmd.execute(m_sql);
  if( bRet )
  {
    m_result += " Insert Successful";
    m_bWarning = !cleanup();
  }
  else
  {
    m_result += " Insert Failed: ";
    m_result += m_sql;
  }
  return bRet;
}

bool objectBase::doUpdate(cgiInput & args)
{
  bool bRet = false;
  bool hasAudits = audits();
  int iField;

  // Do not proceed if validation fails!
  if( m_key.length() == 0 ) return bRet;
  if( !validate() ) return bRet;

  m_sql = "update ";
  m_sql += m_name;
  m_sql += " Set ";

//  m_script << "doUpdate getting tokens..." << endl;

  for( char * tok = m_changes.token("`");
        tok && strlen(tok);
        tok = m_changes.token("`") )
  {
    m_sql += tok;
    m_sql += " = ";
    basicField * pfld = m_frMap[tok];
    if( pfld && args.count(tok) )
    {
      setField( *pfld, static_cast<pgTypeOids>(pgTypeMap[pfld->getType()]), false, args[tok].str(), true );
      m_sql += pfld->svrFormat();
    }
    else if( pfld && pfld->getType() == boolType ) // case of a bool represented by a checkbox being unchecked
    {
      setField( *pfld, static_cast<pgTypeOids>(pgTypeMap[pfld->getType()]), false, "false", true );
      m_sql += pfld->svrFormat();
    }
    else
    {
       m_result += " Update Failed: bad field control lookup...\n";
       m_result += "Values attempting to change: ";
       m_result += m_changes.str();
       m_result += "...\n";
       bRet = false;
       return bRet;
    }
    if( string(m_changes.remainder()).length() )
    {
      m_sql += ", ";
    }
  }

  if( hasAudits )
  {
    m_sql += ", modified = ";
    m_sql += m_frMap["modified"]->svrFormat();
    m_sql += ", modified_by = ";
    m_sql += m_frMap["modified_by"]->svrFormat();
  }

  m_sql += " where Id = ";
  m_sql += m_key;

//  m_script << "doUpdate executing: " << m_sql << "..." << endl;

  bRet = m_cmd.execute(m_sql);
  if( bRet )
  {
    m_result += " Update Successful";
    m_bWarning = !cleanup();
  }
  else
  {
    m_result += " Update Failed ";
    m_result += m_sql;
  }
  return bRet;
}

bool objectBase::doDelete(cgiInput & args)
{
  bool bRet = false;

  // Do not proceed if validation fails!
  if( m_key.length() == 0 ) return bRet;
  if( !validate() ) return bRet;

  m_sql = "delete from ";
  m_sql += m_name;
  m_sql += " where id = ";
  m_sql += m_key;
//  m_script << "SQL: " <<  m_sql << endl;
  bRet = m_cmd.execute(m_sql);
  if( bRet )
  {
    m_result += " Delete Successful";
    m_bWarning = !cleanup();
  }
  else
  {
    m_result += " Delete Failed: ";
    m_result += m_cmd.getErrors();
    m_result += " ";
    m_result += m_sql;
  }
  return bRet;
}

bool objectBase::wasSuccessful( void )
{
  return m_bSuccess;
}
bool objectBase::warning( void )
{
  return m_bWarning;
}
string & objectBase::resultString( void )
{
  return m_result;
}

