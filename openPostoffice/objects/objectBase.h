/*
  objectBase.h

  Definition File for
  base object of business object which
  present form do validation and update data.

  This is hardwired for postgres.
  I hope to make this more generic when I get time:)

  Copyright 2002 by David McCombs
  Open Core Class Library.

*/

#include "openDB.hpp"
#include "fmtTime.h"
#include "Astring.h"
#include "ocControls.h" // caching controls for generic forms
#include "openLogin.h"  // for user id etc.
#define JUST_PG_FIELD_INTERFACES
#include "pgTypeOids.h" // for field map

// links field names to controls and field obj's
typedef map< string, ocBase * > fieldControls;
typedef map< string, basicField * > fieldMap;
typedef vector < string > columnVector;

class objectBase
{
protected:
  fieldMap        m_frMap;    // bag o' fields
  fieldControls   m_ctrlMap;  // bag o' controls
  columnVector    m_columns;  // column vector
  postgresDB      m_db;
  postgresRS      m_rs;       // data stuff
  postgresCMD     m_cmd;
  string          m_sql;
  string          m_key;      // physical key from readonly id field
  string          m_name;     // name of the data object (table)
  openLogin     & m_login;    // for user id on update delete audit fields
  string          m_action;   // s=show, i=insert, u=update, d=delete
  aString         m_changes;  // tracks names of what we want to change - backtick ` delimited.
  tm              m_current;  // holds current time
  string          m_result;   // holds result text
  bool            m_bSuccess; // holds success indicator
  bool            m_bWarning; // holds indication of wanrings
  cgiScript &     m_script;
  // for setting audit fields.
  bool audits( bool create=false );

public:
  objectBase(openLogin &lin, cgiScript & script);
  ~objectBase();

  objectBase & addField( string name, pgTypeOids oid )
  {
    basicField * pBF = createField( oid, false, "", name, true );
    m_columns.push_back( name );
    m_frMap.insert( make_pair(name, pBF) );
    return *this;
  }
  basicField * getField( string name )
  {
    return m_frMap[name];
  }
  objectBase & addControl( string name, ocBase * pControl )
  {
    m_ctrlMap.insert( make_pair(name, pControl) );
    return *this;
  }
  ocBase * getControl( string name )
  {
    return m_ctrlMap[name];
  }
  bool go( void );

  bool doInsert(cgiInput & args);
  bool doUpdate(cgiInput & args);
  bool doDelete(cgiInput & args);
  bool getData(cgiInput & args, int oid=0);

  objectBase &  setName( string name )
  {
    m_name = name;
    return *this;
  }

  // pure virtual validate MUST be defined in subclass
  virtual bool validate() = 0;
  // plain virtual cleanup MAY be over-ridden, default action is do nothing.
  virtual bool cleanup(){ return true; }

  bool wasSuccessful( void );
  bool warning( void );
  string & resultString( void );
  columnVector & getColumns(void) { return m_columns; }
};
