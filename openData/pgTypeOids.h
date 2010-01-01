#ifndef PGTYPE_OIDS_H
#define PGTYPE_OIDS_H
#include "fmtTime.h"
#include <ctype.h>
/*
Lost one day struggling with the BINARY CURSOR and gave up:
- After reverse engineering postgres source (there is NO documentation on this!):
   For all types it's pretty clear what the BE gives back,
   BUT with the Timestamp value (double) it really wasn't clear what the value means.
Postgres really should provide FE functions to turn the Timestamp into a tm struct.
It should also provide an FE function to unpack the packed values of the
Numeric and Decimal (synonomous) types.
I have decided to give in and use the character return data, only to convert it to the native type.
The major issue for me with this is that I KNOW that precision is going to be lost on the
floating point and double values.  I have already seen this in the return string
representation of the data.
Also there is the performance issue of formatting to strings, converting to a native type, then
re-formatting again for a web page (or XML stream.)

David McCombs 12/14/2001
NOTE!!!!:  NEED TO ADD TEXT FIELD TYPE HANDLER! DKMc 10/20/2002
DONE! , Also implemented longlongField use. DKMc 7/30/03

*/
/*
For reference - this is the openFields enumeration
enum fieldTypes
{
  shortType, longType,  floatType, doubleType,
  stringType, currencyType, dateTimeType, boolType
};
SEE /usr/include/pgsql/server/catalog for type defs
*/
enum pgTypeOids
{
  pgBooleanOid       =16, //BOOLOID        - probably char or short loot at size
  pgLongOid          =20, //INT8OID        -  long long
  pgShortOid         =21, //INT2OID        -  short
  pgIntegerOid       =23, //INT4OID        -  int
  pgTextOid          =25,  // TEXT               
  pgFloatOid         =700, //FLOAT4OID     - float
  pgDoubleOid        =701, //FLOAT8OID     - double
  pgCharOid          =1042, //BPCHAROID    - char *
  pgCharArrayOid     =1042, //BPCHAROID    - char *
  pgVarCharArrayOid  =1043, //VARCHAROID   - char *
  pgTimestampOid = 1114,   // TIMESTAMPOID	1114
  pgDateOid          =1082, //DATEOID      - Timestamp
  pgTimeOid          =1083, //TIMEOID      - Timestamp  
  pgTimestampTzOid     =1184, //TIMESTAMPTZOID	- Timestamp  
  pgDecimalOid       =1700, //NUMERICOID   - packed decimal
  pgNumericOid       =1700 //NUMERICOID    - packed decimal
};

#define iDateFMT "%Y-%m-%d"
#define iTimeFMT "%H:%M:%S"
#define iDateTimeFMT "%Y-%m-%d %H:%M:%S"
#define oDateFMT "%m-%d-%Y"
#define oTimeFMT "%I:%M %p"
#define oDateTimeFMT "%m-%d-%Y %H:%M:%S"

#ifdef JUST_PG_FIELD_INTERFACES
basicField * createField( pgTypeOids oid, bool isNull, string value, string  name, bool isClient=false );
basicField & setField( basicField & rBF, pgTypeOids oid, bool isNull, string value, bool isClient=false );
#else
basicField * createField( pgTypeOids oid, bool isNull, string value, string  name, bool isClient=false )
{
  basicField * pBF = NULL;
  tm t;
  char chVal = '\0';
  bool bVal = false;
  short sVal = 0;
  long lVal = 0l;
  long long llVal = 0l;
  float fVal = 0.0;
  double dVal = 0.0;
  currency cVal;
  cVal.amount = 0.0;
  currentTime(&t);

  switch( oid )
  {
    case pgBooleanOid:
      bVal = false;
      chVal = toupper( value[0] );
      // T,Y,1 is true  F,N,0 is false
      if ( chVal == 'T' || chVal == 'Y' || chVal == '1' ) bVal = true;
      pBF = new boolField( bVal, isNull, true, name, "" );
      break;

    case pgShortOid:
      sVal = atoi( value.c_str());
      pBF = new shortField( sVal, isNull, true, name, "" );
      break;

    case pgIntegerOid:
      lVal = atol( value.c_str());
      pBF = new longField( lVal, isNull, true, name, "" );
      break;
    case pgLongOid:
      llVal = atoll( value.c_str());
      pBF = new longlongField( llVal, isNull, true, name, "" );
      break;

    case pgFloatOid:
      fVal = (float) atof( value.c_str());
      pBF = new floatField( fVal, isNull, true, name, "" );
      break;

    case pgDoubleOid:
      dVal = atof( value.c_str());
      pBF = new doubleField( dVal, isNull, true, name, "" );
      break;

    // case pgCharOid:
    case pgCharArrayOid:
    case pgVarCharArrayOid:
    case pgTextOid:
      pBF = new stringField( value, isNull, true, name, "" );
      break;

    case pgDateOid:
      if( value.length() == 0 )  isNull = true;
      else parseTime(value.c_str(), isClient?oDateFMT:iDateFMT, &t);
      pBF = new dateTimeField( t, isNull, true, name, "" );
      pBF->setFormatMask(string(oDateFMT));
      pBF->setSvrFormatMask(string(iDateFMT));
      break;

    case pgTimeOid:
      if( value.length() == 0 )  isNull = true;
      else parseTime(value.c_str(), isClient?oTimeFMT:iTimeFMT, &t);
      pBF = new dateTimeField( t, isNull, true, name, "" );
      pBF->setFormatMask(oTimeFMT);
      pBF->setSvrFormatMask(string(iTimeFMT));
      break;

    case pgTimestampOid:
      if( value.length() == 0 )  isNull = true;
      else parseTime(value.c_str(), isClient?oDateTimeFMT:iDateTimeFMT, &t);
      pBF = new dateTimeField( t, isNull, true, name, "" );
      pBF->setFormatMask(oDateTimeFMT);
      pBF->setSvrFormatMask(string(iDateTimeFMT));
      break;

    //case pgDecimalOid:
    case pgNumericOid:
      cVal.amount = atof( value.c_str());
      pBF = new currencyField( cVal, isNull, true, name, "" );
      break;

    default:
      // fprintf( stderr, " Do not know about this type: %d\n", oid );
    break;

  }
  return pBF;
};
basicField & setField( basicField & rBF, pgTypeOids oid, bool isNull, string value, bool isClient=false )
{
  tm t;
  bool bVal;
  char chVal;
  short sVal;
  long lVal;
  long long llVal = 0l;
  float fVal;
  double dVal;
  currency cVal;

  switch( oid )
  {
    case pgBooleanOid:
    {
      bVal = false;
      chVal = toupper( value[0] );
      // T,Y,1 is true  F,N,0 is false
      if ( chVal == 'T' || chVal == 'Y' || chVal == '1' ) bVal = true;
      boolField & rboolF = dynamic_cast<boolField&>(rBF);
      rboolF.set(bVal);
      rboolF.setNull(isNull);
      break;
    }
    case pgShortOid:
    {
      sVal = atoi( value.c_str());
      shortField & rshortF = dynamic_cast<shortField&>(rBF);
      rshortF.set(sVal);
      rshortF.setNull(isNull);
      break;
    }
    case pgIntegerOid:
    {
      lVal = atol( value.c_str());
      longField & rlongF = dynamic_cast<longField&>(rBF);
      rlongF.set(lVal);
      rlongF.setNull(isNull);
      break;
    }
    case pgLongOid:
    {
      llVal = atoll( value.c_str());
      longlongField & rlongF = dynamic_cast<longlongField&>(rBF);
      rlongF.set(llVal);
      rlongF.setNull(isNull);
      break;
    }
    case pgFloatOid:
    {
      fVal = (float) atof( value.c_str());
      floatField & rfloatF = dynamic_cast<floatField&>(rBF);
      rfloatF.set(fVal);
      rfloatF.setNull(isNull);
      break;
    }
    case pgDoubleOid:
    {
      dVal = atof( value.c_str());
      doubleField & rdoubleF = dynamic_cast<doubleField&>(rBF);
      rdoubleF.set(dVal);
      rdoubleF.setNull(isNull);
      break;
    }
    // case pgCharOid:
    case pgTextOid:
    case pgCharArrayOid:
    case pgVarCharArrayOid:
    {
      stringField & rstringF = dynamic_cast<stringField&>(rBF);
      rstringF.set( value);
      rstringF.setNull(isNull);
      break;
    }
    case pgDateOid:
    {
      parseTime(value.c_str(), isClient?oDateFMT:iDateFMT, &t);
      dateTimeField & rdateF = dynamic_cast<dateTimeField&>(rBF);
      rdateF.set(t);
      rdateF.setNull(isNull);
      break;
    }
    case pgTimeOid:
    {
      parseTime(value.c_str(), isClient?oTimeFMT:iTimeFMT, &t);
      dateTimeField & rtimeF = dynamic_cast<dateTimeField&>(rBF);
      rtimeF.set(t);
      rtimeF.setNull(isNull);
      break;
    }
    case pgTimestampOid:
    {
      parseTime(value.c_str(), isClient?oDateTimeFMT:iDateTimeFMT, &t);
      dateTimeField & rdatetimeBF = dynamic_cast<dateTimeField&>(rBF);
      rdatetimeBF.set(t);
      rdatetimeBF.setNull(isNull);
      break;
    }
    //case pgDecimalOid:
    case pgNumericOid:
    {
      cVal.amount = atof( value.c_str());
      currencyField & currencyF = dynamic_cast<currencyField&>(rBF);
      currencyF.set(cVal);
      currencyF.setNull(isNull);
      break;
    }
    default:
    break;

  }
  return rBF;
};
#endif
#endif
