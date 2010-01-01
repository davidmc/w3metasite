/*

  odbcTypeMaps.h
  ==============
  ODBC to openField conversion and fixup.

*/
#ifndef ODBC_TYPE_MAPS_H
#define ODBC_TYPE_MAPS_H
#include "fmtTime.h"
const int cOL_nAMe_Len = 255;
/*
  Helper Classes
  ==============
*/

struct odbcColInfo
{
  UCHAR  szColName [cOL_nAMe_Len] ;
  SWORD  cbColName;
  SWORD  fSqlType;
  UDWORD cbColDef; // Will have length, SQL_NULL_DATA if the data is NULL, or SQL_NO_TOTAL
  SWORD  ibScale;
  SWORD  fNullable;
  CHAR * pCharData;
  SDWORD length;   // Will have length, SQL_NULL_DATA if the data is NULL, or SQL_NO_TOTAL
  TIMESTAMP_STRUCT timeData;

  odbcColInfo() :cbColName(sizeof(szColName)),fSqlType(0)
             ,cbColDef(0),ibScale(0),fNullable(0),pCharData(NULL),length(0)
  {
    memset(szColName, 0, sizeof(szColName) );
  }
  ~odbcColInfo() { delete [] pCharData; }
};
// col. pos. based lookup
typedef vector< odbcColInfo * > colInfoVector;

struct mapItem
{
  SWORD cType;
  mapItem(SWORD c):cType(c){;}
  virtual ~mapItem(){;}
  virtual basicField * createField( odbcColInfo * info, bool isClient=false ) = 0;
  virtual void * thingToBind( odbcColInfo * info, basicField * field ) = 0;
  virtual void postFetchFixup( odbcColInfo * info, basicField * field ) = 0;
};
// SQL type based lookup
typedef map <SWORD, mapItem *> sql_to_c_types;

struct mapString : public mapItem
{
  mapString():mapItem(SQL_C_CHAR){;}
  virtual ~mapString(){;}
  virtual basicField * createField( odbcColInfo * info, bool isClient=false )
  {
    // first allocate buffer for bind op on string data.
    if( (SDWORD)info->cbColDef != SQL_NO_TOTAL  )
    {
      info->pCharData = new char[info->cbColDef+1];  // +1 adds the string terminator
    }
    // next create the field
    return new stringField( "", false, true, (char*) info->szColName, "" );
  }
  virtual void * thingToBind( odbcColInfo * info, basicField * field )
  {
    return (void*) info->pCharData;
  }
  virtual void postFetchFixup( odbcColInfo * info, basicField * field )
  {
    field->setNull(false);

    /* debugging
    cout << info->szColName
         << " info->length: "
         << info->length
         << " info->pCharData: "
         << &info->pCharData << endl;
    */
    if( info->length == SQL_NULL_DATA )
    {
      field->setNull();
      dynamic_cast<stringField*>(field)->set("");
    }
    else
    {
      dynamic_cast<stringField*>(field)->set(string(info->pCharData));
    }
  }
};  // end of mapString class


struct mapBool  : public mapItem
{
  mapBool():mapItem(SQL_C_BIT){;}
  virtual ~mapBool(){;}
  virtual basicField * createField( odbcColInfo * info, bool isClient=false )
  {
    return new boolField( false, false, true, (char*)info->szColName, "" );
  }
  virtual void * thingToBind( odbcColInfo * info, basicField * field )
  {
    return (void*) & dynamic_cast<boolField*>(field)->get();
  }
  virtual void postFetchFixup( odbcColInfo * info, basicField * field )
  {
    field->setNull(false);
    if( info->length == SQL_NULL_DATA )
    {
      field->setNull();
      dynamic_cast<boolField*>(field)->set(false);
    }
    else if( dynamic_cast<boolField*>(field)->get() )
      dynamic_cast<boolField*>(field)->set(true);
    else
      dynamic_cast<boolField*>(field)->set(false);
  }
}; // end of mapBool class

struct mapShort  : public mapItem
{
  mapShort():mapItem(SQL_C_SHORT){;}
  virtual ~mapShort(){;}
  virtual basicField * createField( odbcColInfo * info, bool isClient=false )
  {
    return new shortField( 0, false, true, (char*)info->szColName, "" );
  }
  virtual void * thingToBind( odbcColInfo * info, basicField * field )
  {
    return (void*) & dynamic_cast<shortField*>(field)->get();
  }
  virtual void postFetchFixup( odbcColInfo * info, basicField * field )
  {
    field->setNull(false);
    if( info->length == SQL_NULL_DATA )
    {
      field->setNull();
      dynamic_cast<shortField*>(field)->set(0);
    }
  }
}; // end of mapShort class

struct mapLong  : public mapItem
{
  mapLong():mapItem(SQL_C_LONG){;}
  virtual ~mapLong(){;}
  virtual basicField * createField( odbcColInfo * info, bool isClient=false )
  {
    return new longField( 0, false, true, (char*)info->szColName, "" );
  }
  virtual void * thingToBind( odbcColInfo * info, basicField * field )
  {
    return (void*) & dynamic_cast<longField*>(field)->get();
  }
  virtual void postFetchFixup( odbcColInfo * info, basicField * field )
  {
    field->setNull(false);
    if( info->length == SQL_NULL_DATA )
    {
      field->setNull();
      dynamic_cast<longField*>(field)->set(0L);
    }
  }
}; // end of mapLong class

struct mapFloat  : public mapItem
{
  mapFloat():mapItem(SQL_C_FLOAT){;}
  virtual ~mapFloat(){;}
  virtual basicField * createField( odbcColInfo * info, bool isClient=false )
  {
    return new floatField( 0.0, false, true, (char*)info->szColName, "" );
  }
  virtual void * thingToBind( odbcColInfo * info, basicField * field )
  {
    return (void*) & dynamic_cast<floatField*>(field)->get();
  }
  virtual void postFetchFixup( odbcColInfo * info, basicField * field )
  {
    field->setNull(false);
    if( info->length == SQL_NULL_DATA )
    {
      field->setNull();
      dynamic_cast<floatField*>(field)->set(0.0);
    }
  }
}; // end of mapFloat class

struct mapDouble  : public mapItem
{
  mapDouble():mapItem(SQL_C_DOUBLE){;}
  virtual ~mapDouble(){;}
  virtual basicField * createField( odbcColInfo * info, bool isClient=false )
  {
    return new doubleField( 0.0, false, true, (char*)info->szColName, "" );
  }
  virtual void * thingToBind( odbcColInfo * info, basicField * field )
  {
    return (void*) & dynamic_cast<doubleField*>(field)->get();
  }
  virtual void postFetchFixup( odbcColInfo * info, basicField * field )
  {
    field->setNull(false);
    if( info->length == SQL_NULL_DATA )
    {
      field->setNull();
      dynamic_cast<doubleField*>(field)->set(0.0);
    }
  }
};  // end of mapDouble class

struct mapDateTime  : public mapItem
{
  mapDateTime():mapItem(SQL_C_TIMESTAMP){;}
  virtual ~mapDateTime(){;}
  virtual basicField * createField( odbcColInfo * info, bool isClient=false )
  {
    tm t;
    currentTime(&t);
    return new dateTimeField( t, false, true, (char*)info->szColName, "" );
  }
  virtual void * thingToBind( odbcColInfo * info, basicField * field )
  {
    return (void*) & info->timeData;
  }
  virtual void postFetchFixup( odbcColInfo * info, basicField * field )
  {
    tm t;
    currentTime(&t);
    field->setNull(false);
    if( info->length == SQL_NULL_DATA )
    {
      field->setNull();
      dynamic_cast<dateTimeField*>(field)->set(t);
    }
    else
    {
      t.tm_min =  info->timeData.minute;
      t.tm_hour =  info->timeData.hour;
      t.tm_mday =  info->timeData.day;
      t.tm_year =  info->timeData.year-1900;
      t.tm_mon =  info->timeData.month-1;
      t.tm_sec =  info->timeData.second;
      fixTime(&t);
      dynamic_cast<dateTimeField*>(field)->set(t);
    }
  }
}; // end of mapDateTime class

struct mapNumeric : public mapItem
{
  mapNumeric():mapItem(SQL_C_DOUBLE){;}
  virtual ~mapNumeric(){;}
  virtual basicField * createField( odbcColInfo * info, bool isClient=false )
  {
    currency temp;
    temp.amount=0.0;
    return new currencyField( temp, false, true, (char*)info->szColName, "" );
  }
  virtual void * thingToBind( odbcColInfo * info, basicField * field )
  {
    return (void*) &(dynamic_cast<currencyField*>(field)->get().amount);
  }
  virtual void postFetchFixup( odbcColInfo * info, basicField * field )
  {
    currency temp;
    temp.amount=0.0;
    field->setNull(false);
    if( info->length == SQL_NULL_DATA )
    {
      field->setNull();
      dynamic_cast<currencyField*>(field)->set(temp);
    }
  }
};  // end of mapNumeric class

/*
  Finally the point of the file, to provide helpers
  in the context of the ODBC return type from SQLDescribeCol():
  Map of SQL ODBC Type codes to Native C Types to field Objects to maps
  ========================================================================
  SQL_UNKNOWN_TYPE    0  -
  SQL_CHAR            1  SQL_C_CHAR          stringField       mapString
  SQL_NUMERIC         2  SQL_C_DOUBLE        currencyField     mapNumeric
  SQL_DECIMAL         3  SQL_C_DOUBLE        currencyField     mapNumeric
  SQL_INTEGER         4  SQL_C_LONG          longField         mapLong
  SQL_SMALLINT        5  SQL_C_SHORT         shortField        mapShort
  SQL_FLOAT           6  SQL_C_FLOAT         floatField        mapFloat
  SQL_REAL            7  SQL_C_FLOAT         floatField        mapFloat
  SQL_DOUBLE          8  SQL_C_DOUBLE        doubleField       mapDouble
  SQL_DATETIME        9  SQL_C_TIMESTAMP     dateTimeField     mapDateTime
  SQL_VARCHAR        12  SQL_C_CHAR          stringField       mapString
  SQL_DATE            9  SQL_C_TIMESTAMP     dateTimeField     mapDateTime
  SQL_INTERVAL       10  -
  SQL_TIME           10  SQL_C_TIMESTAMP     dateTimeField     mapDateTime
  SQL_TIMESTAMP      11  SQL_C_TIMESTAMP     dateTimeField     mapDateTime
#define SQL_TYPE_DATE      91
#define SQL_TYPE_TIME      92
#define SQL_TYPE_TIMESTAMP 93

  SQL_LONGVARCHAR   (-1) SQL_C_CHAR          stringField       mapString
  SQL_BINARY        (-2) -
  SQL_VARBINARY     (-3) -
  SQL_LONGVARBINARY (-4) -
  SQL_BIGINT        (-5) SQL_C_LONG            longField       mapLong
  SQL_TINYINT       (-6) SQL_C_SHORT           shortField      mapShort
  SQL_BIT           (-7) SQL_C_BIT             boolField       mapBool
  ========================================================================
                      ( - means not handled here! )
*/
struct odbcMap
{
  sql_to_c_types data;
  odbcMap()
  {
    data.insert(make_pair(SQL_CHAR,new mapString));
    data.insert(make_pair(SQL_NUMERIC,new mapNumeric));
    data.insert(make_pair(SQL_DECIMAL,new mapNumeric));
    data.insert(make_pair(SQL_INTEGER,new mapLong));
    data.insert(make_pair(SQL_SMALLINT,new mapShort));
    data.insert(make_pair(SQL_FLOAT,new mapFloat));
    data.insert(make_pair(SQL_REAL,new mapFloat));
    data.insert(make_pair(SQL_DOUBLE,new mapDouble));
    data.insert(make_pair(SQL_DATETIME,new mapDateTime));
    data.insert(make_pair(SQL_VARCHAR,new mapString));
    data.insert(make_pair(SQL_DATE,new mapDateTime));
    data.insert(make_pair(SQL_TIME,new mapDateTime));
    data.insert(make_pair(SQL_TIMESTAMP,new mapDateTime));
    data.insert(make_pair(SQL_TYPE_DATE,new mapDateTime));
    data.insert(make_pair(SQL_TYPE_TIME,new mapDateTime));
    data.insert(make_pair(SQL_TYPE_TIMESTAMP,new mapDateTime));
    data.insert(make_pair(SQL_LONGVARCHAR,new mapString));
    data.insert(make_pair(SQL_BIGINT,new mapLong));
    data.insert(make_pair(SQL_TINYINT,new mapShort));
    data.insert(make_pair(SQL_BIT,new mapBool));
  }
  ~odbcMap()
  {
    sql_to_c_types::iterator pos;

    for( pos = data.begin(); pos != data.end(); ++pos )
    {
      delete pos->second;
    }
  }
};
#endif
