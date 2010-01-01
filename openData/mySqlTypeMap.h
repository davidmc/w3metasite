/*
  mySqlTypeMap.h
  ==============
  mySQL to openField conversion and fixup
*/

#ifndef MY_SQL_TYPE_MAPS_H
#define MY_SQL_TYPE_MAPS_H

#include "fmtTime.h"

#define iDateMyFMT "%Y-%m-%d"
#define iTimeMyFMT "%H:%M:%S"
#define iDateTimeMyFMT "%Y-%m-%d %H:%M:%S"
#define oDateMyFMT "%m-%d-%Y"
#define oTimeMyFMT "%H:%M:%S"
#define oDateTimeMyFMT "%m-%d-%Y %H:%M:%S"

/*
  Helper Classes
  ==============
*/

struct mysqlColInfo
{
  string ColName;
  enum enum_field_types mySqlType;
  unsigned int length;     // Width of column
  unsigned int max_length; // Max width of selected set
  unsigned int flags;      // Div flags
  unsigned int decimals;   // Number of decimals in field
  mysqlColInfo() :length(0),max_length(0),flags(0)
  {
  }
  mysqlColInfo( MYSQL_FIELD *pField )
  :length(0),max_length(0),flags(0)
  {
    if( pField )
    {
      if(pField->name) ColName = pField->name;
      mySqlType = pField->type;
      length =  pField->length;
      max_length =  pField->max_length;
      flags = pField->flags;
      decimals = pField->decimals;
    }
  }
  ~mysqlColInfo() {}
};

// col. pos. based lookup
typedef vector< mysqlColInfo * > my_col_infoVector;

struct my_mapItem
{
  // used for format from client or server (for dates esp.)
  bool isClient;
  my_mapItem():isClient(false){;}
  virtual ~my_mapItem(){;}
  virtual basicField * createField( mysqlColInfo * info ) = 0;
  virtual basicField * setField( basicField * pFld, const char * pchVal ) = 0;
};

// MySQL type based lookup
typedef map <enum enum_field_types, my_mapItem *> mysql_to_c_types;

struct my_mapString : public my_mapItem
{
  my_mapString(){;}
  virtual ~my_mapString(){;}
  virtual basicField * createField( mysqlColInfo * info )
  {
    // next create the field
    return new stringField( "", false, true, (char*) info->ColName.c_str(), "" );
  }
  virtual basicField * setField( basicField * pFld, const char * pchVal )
  {
    if( pFld )
    {
      stringField & rstringF = dynamic_cast<stringField&>(*pFld);
      if( pchVal )
      {
        rstringF.set(pchVal);
        rstringF.setNull(false);
      }
      else
      {
        rstringF.setNull(true);
      }
    }
    return pFld;
  }
};  // end of my_mapString class


struct my_mapBool  : public my_mapItem
{
  my_mapBool():my_mapItem(){;}
  virtual ~my_mapBool(){;}
  virtual basicField * createField( mysqlColInfo * info )
  {
    return new boolField( false, false, true, info->ColName, "" );
  }
  virtual basicField * setField( basicField * pFld, const char * pchVal )
  {
    if( pFld )
    {
      boolField & rboolF = dynamic_cast<boolField&>(*pFld);
      bool bVal = false;
      if( pchVal )
      {
        if ( pchVal[0] == 'T' || pchVal[0] == 'Y' || pchVal[0] == '1' ) bVal = true;

        rboolF.set(bVal);
        rboolF.setNull(false);
      }
      else
      {
        rboolF.setNull(true);
      }
    }
    return pFld;
  }
}; // end of my_mapBool class

struct my_mapShort  : public my_mapItem
{
  my_mapShort():my_mapItem(){;}
  virtual ~my_mapShort(){;}
  virtual basicField * createField( mysqlColInfo * info )
  {
    return new shortField( 0, false, true, info->ColName, "" );
  }
  virtual basicField * setField( basicField * pFld, const char * pchVal )
  {
    if( pFld )
    {
      shortField & rshortF = dynamic_cast<shortField&>(*pFld);
      if( pchVal )
      {
        short sVal = atoi(pchVal);
        rshortF.set(sVal);
        rshortF.setNull(false);
      }
      else
      {
        rshortF.setNull(true);
      }
    }
    return pFld;
  }
}; // end of my_mapShort class

struct my_mapLong  : public my_mapItem
{
  my_mapLong():my_mapItem(){;}
  virtual ~my_mapLong(){;}
  virtual basicField * createField( mysqlColInfo * info )
  {
    return new longField( 0, false, true, info->ColName, "" );
  }
  virtual basicField * setField( basicField * pFld, const char * pchVal )
  {
    if( pFld )
    {
      longField & rlongF = dynamic_cast<longField&>(*pFld);
      if( pchVal )
      {
        long lVal = atol(pchVal);
        rlongF.set(lVal);
        rlongF.setNull(false);
      }
      else
      {
        rlongF.setNull(true);
      }
    }
    return pFld;
  }
}; // end of my_mapLong class


struct my_mapLongLong  : public my_mapItem
{
  my_mapLongLong():my_mapItem(){;}
  virtual ~my_mapLongLong(){;}
  virtual basicField * createField( mysqlColInfo * info )
  {
    return new longlongField( 0, false, true, info->ColName, "" );
  }
  virtual basicField * setField( basicField * pFld, const char * pchVal )
  {
    if( pFld )
    {
      longlongField & rlongF = dynamic_cast<longlongField&>(*pFld);
      if( pchVal )
      {
        long long lVal = atoll(pchVal);
        rlongF.set(lVal);
        rlongF.setNull(false);
      }
      else
      {
        rlongF.setNull(true);
      }
    }
    return pFld;
  }
}; // end of my_mapLong class


struct my_mapFloat  : public my_mapItem
{
  my_mapFloat():my_mapItem(){;}
  virtual ~my_mapFloat(){;}
  virtual basicField * createField( mysqlColInfo * info )
  {
    return new floatField( 0.0, false, true, info->ColName, "" );
  }
  virtual basicField * setField( basicField * pFld, const char * pchVal )
  {
    if( pFld )
    {
      floatField & rfloatF = dynamic_cast<floatField&>(*pFld);
      if( pchVal )
      {
        float fVal = atof(pchVal);
        rfloatF.set(fVal);
        rfloatF.setNull(false);
      }
      else
      {
        rfloatF.setNull(true);
      }
    }
    return pFld;
  }
}; // end of my_mapFloat class

struct my_mapDouble  : public my_mapItem
{
  my_mapDouble():my_mapItem(){;}
  virtual ~my_mapDouble(){;}
  virtual basicField * createField( mysqlColInfo * info )
  {
    return new doubleField( 0.0, false, true, info->ColName, "" );
  }
  virtual basicField * setField( basicField * pFld, const char * pchVal )
  {
    if( pFld )
    {
      doubleField & rdoubleF = dynamic_cast<doubleField&>(*pFld);
      if( pchVal )
      {
        double fVal = atof(pchVal);
        rdoubleF.set(fVal);
        rdoubleF.setNull(false);
      }
      else
      {
        rdoubleF.setNull(true);
      }
    }
    return pFld;
  }
};  // end of my_mapDouble class

struct my_mapDateTime  : public my_mapItem
{
  my_mapDateTime():my_mapItem(){;}
  virtual ~my_mapDateTime(){;}
  virtual basicField * createField( mysqlColInfo * info )
  {
    tm t;
    currentTime(&t);
    dateTimeField *pDateF = new dateTimeField( t, false, true, info->ColName, "" );
    // oDateTimeMyFMT format is for the client
    pDateF->setFormatMask( oDateTimeMyFMT );
    // iDateTimeMyFMT format is for the server
    pDateF->setSvrFormatMask( iDateTimeMyFMT );
    return pDateF;

  }
  virtual basicField * setField( basicField * pFld, const char * pchVal )
  {
    if( pFld )
    {
      tm t;
      currentTime(&t);
      dateTimeField & rdateF = dynamic_cast<dateTimeField&>(*pFld);
      if( pchVal )
      {
        parseTime(pchVal, isClient?oDateTimeMyFMT:iDateTimeMyFMT, &t);
        rdateF.set(t);
        rdateF.setNull(false);
      }
      else
      {
        rdateF.setNull(true);
      }
    }
    return pFld;
  }
}; // end of my_mapDateTime class
struct my_mapDate  : public my_mapItem
{
  my_mapDate():my_mapItem(){;}
  virtual ~my_mapDate(){;}
  virtual basicField * createField( mysqlColInfo * info )
  {
    tm t;
    currentTime(&t);
    dateTimeField *pDateF = new dateTimeField( t, false, true, info->ColName, "" );
    // oDate format is for the client
    pDateF->setFormatMask( oDateMyFMT );
    // iDate format is for the server
    pDateF->setSvrFormatMask( iDateMyFMT );
    return pDateF;
  }
  virtual basicField * setField( basicField * pFld, const char * pchVal )
  {
    if( pFld )
    {
      tm t;
      currentTime(&t);
      dateTimeField & rdateF = dynamic_cast<dateTimeField&>(*pFld);
      if( pchVal )
      {
        // oDate format is for the client
        // iDate format is for the server
        parseTime(pchVal, isClient?oDateMyFMT:iDateMyFMT, &t);
        rdateF.set(t);
        rdateF.setNull(false);
      }
      else
      {
        rdateF.setNull(true);
      }
    }
    return pFld;
  }
}; // end of my_mapDate class
struct my_mapTime  : public my_mapItem
{
  my_mapTime():my_mapItem(){;}
  virtual ~my_mapTime(){;}
  virtual basicField * createField( mysqlColInfo * info )
  {
    tm t;
    currentTime(&t);
    dateTimeField *pDateF = new dateTimeField( t, false, true, info->ColName, "" );
    // oDate format is for the client
    pDateF->setFormatMask( oTimeMyFMT );
    // iDate format is for the server
    pDateF->setSvrFormatMask( iTimeMyFMT );
    return pDateF;
  }
  virtual basicField * setField( basicField * pFld, const char * pchVal )
  {
    if( pFld )
    {
      tm t;
      currentTime(&t);
      dateTimeField & rtimeF = dynamic_cast<dateTimeField&>(*pFld);
      if( pchVal )
      {
        // oDate format is for the client
        // iDate format is for the server
        parseTime(pchVal, isClient?oTimeMyFMT:iTimeMyFMT, &t);
        rtimeF.set(t);
        rtimeF.setNull(false);
      }
      else
      {
        rtimeF.setNull(true);
      }
    }
    return pFld;
  }
}; // end of my_mapTime class
struct my_mapNumeric : public my_mapItem
{
  my_mapNumeric():my_mapItem(){;}
  virtual ~my_mapNumeric(){;}
  virtual basicField * createField( mysqlColInfo * info )
  {
    currency temp;
    temp.amount=0.0;
    return new currencyField( temp, false, true, info->ColName, "" );
  }
  virtual basicField * setField( basicField * pFld, const char * pchVal )
  {
    currency cVal;
    cVal.amount = 0.0;
    if( pFld )
    {
      currencyField & currencyF = dynamic_cast<currencyField&>(*pFld);
      if( pchVal )
      {
        cVal.amount = atof(pchVal);
        currencyF.set(cVal);
        currencyF.setNull(false);
      }
      else
      {
        currencyF.setNull(true);
      }
    }
    return pFld;
  }
};  // end of my_mapNumeric class


/*
    enum enum_field_types
    {
    FIELD_TYPE_DECIMAL, FIELD_TYPE_TINY,
    FIELD_TYPE_SHORT,  FIELD_TYPE_LONG,
    FIELD_TYPE_FLOAT,  FIELD_TYPE_DOUBLE,

    FIELD_TYPE_NULL,   FIELD_TYPE_TIMESTAMP,
    FIELD_TYPE_LONGLONG,FIELD_TYPE_INT24,
    FIELD_TYPE_DATE,   FIELD_TYPE_TIME,
    FIELD_TYPE_DATETIME, FIELD_TYPE_YEAR,
    FIELD_TYPE_NEWDATE,

    FIELD_TYPE_ENUM=247,
    FIELD_TYPE_SET=248,

    FIELD_TYPE_TINY_BLOB=249,
    FIELD_TYPE_MEDIUM_BLOB=250,
    FIELD_TYPE_LONG_BLOB=251,
    FIELD_TYPE_BLOB=252,
    FIELD_TYPE_VAR_STRING=253,
    FIELD_TYPE_STRING=254
    };
*/

struct mysqlMap
{
  mysql_to_c_types data;
  mysqlMap()
  {
    data.insert(make_pair(FIELD_TYPE_DECIMAL,new my_mapNumeric));
    data.insert(make_pair(FIELD_TYPE_TINY,new my_mapShort));
    data.insert(make_pair(FIELD_TYPE_SHORT,new my_mapShort));
    data.insert(make_pair(FIELD_TYPE_LONG,new my_mapLong));
    data.insert(make_pair(FIELD_TYPE_FLOAT,new my_mapFloat));
    data.insert(make_pair(FIELD_TYPE_DOUBLE,new my_mapDouble));
    data.insert(make_pair(FIELD_TYPE_NULL,new my_mapString));
    data.insert(make_pair(FIELD_TYPE_TIMESTAMP,new my_mapDateTime));
    data.insert(make_pair(FIELD_TYPE_LONGLONG,new my_mapLongLong));
    data.insert(make_pair(FIELD_TYPE_INT24,new my_mapLongLong));
    data.insert(make_pair(FIELD_TYPE_DATE,new my_mapDate));
    data.insert(make_pair(FIELD_TYPE_TIME,new my_mapTime));
    data.insert(make_pair(FIELD_TYPE_DATETIME,new my_mapDateTime));
    data.insert(make_pair(FIELD_TYPE_TINY_BLOB,new my_mapString));
    data.insert(make_pair(FIELD_TYPE_MEDIUM_BLOB,new my_mapString));
    data.insert(make_pair(FIELD_TYPE_LONG_BLOB,new my_mapString));
    data.insert(make_pair(FIELD_TYPE_BLOB,new my_mapString));
    data.insert(make_pair(FIELD_TYPE_VAR_STRING,new my_mapString));
    data.insert(make_pair(FIELD_TYPE_STRING,new my_mapString));
  }
  ~mysqlMap()
  {
    mysql_to_c_types::iterator pos;

    for( pos = data.begin(); pos != data.end(); ++pos )
    {
      delete pos->second;
    }
  }
};

#endif
