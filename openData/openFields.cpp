/*
  Open Field impleentation file: openFields.cpp

*/
#include "openFields.h"
#include <cstdio> // for sprintf
#include "ocString.h"
/*
class basicField
properties:
  bool       m_bIsNull;
  bool       m_bIsEditable;
  string     m_strTable;
  string     m_strName;
  string     m_strFormatMask;
  string     m_strServerMask;
  string     m_strFormatted;
  fieldTypes m_fieldType;
  int        m_iSize;
*/
basicField::basicField()
:m_bIsNull(false),m_bIsEditable(false),m_strTable(""),
m_strName(""),m_strFormatMask(""),m_strServerMask(""),
m_strFormatted(""),m_fieldType(noType),m_iSize(0)
{
  ;
}
basicField::basicField
( bool isNull, bool isEditable,  string name, string table )
:m_bIsNull(isNull),m_bIsEditable(isEditable),m_strTable(table),
m_strName(name),m_strFormatMask(""),m_strServerMask(""),
m_strFormatted(""),m_fieldType(noType),m_iSize(0)
{
  ;
}


basicField::~basicField()
{
  ;
}
    // assignment
basicField & basicField::operator = ( basicField & input )
{
  m_bIsNull=input.m_bIsNull;
  m_bIsEditable=input.m_bIsEditable;
  m_strTable=input.m_strTable;
  m_strName=input.m_strName;
  m_strFormatMask=input.m_strFormatMask;
  m_strServerMask = input.m_strServerMask;
  m_fieldType=input.m_fieldType;
  m_iSize = input.m_iSize;
  return *this;
}
  // Sets
basicField & basicField::setNull( bool bNull )
{
  m_bIsNull=bNull;
  return *this;
}
basicField & basicField::setIsEditable( bool bEditable )
{
  m_bIsEditable=bEditable;
  return *this;
}
basicField & basicField::setName( string strName )
{
  m_strName=strName;
  return *this;
}
basicField & basicField::setTable( string strTable )
{
  m_strTable=strTable;
  return *this;
}
basicField & basicField::setFormatMask( string strFormatMask )
{
  m_strFormatMask=strFormatMask;
  return *this;
}
basicField & basicField::setSvrFormatMask( string strFormatMask )
{
  m_strServerMask=strFormatMask;
  return *this;
}
basicField & basicField::setSize( int iSize )
{
  m_iSize = iSize;
  return *this;
}
  // Gets
bool basicField::isNull( void )
{
  return m_bIsNull;
}
bool basicField::isEditable( void )
{
  return m_bIsEditable;
}
int  basicField::getSize( void )
{
  return m_iSize;
}
string & basicField::getName( void )
{
  return m_strName;
}
string & basicField::getTable( void )
{
  return m_strTable;
}
fieldTypes basicField::getType( void )
{
  return m_fieldType;
}

string & basicField::format()
{
  return m_strFormatMask;
}
string & basicField::svrFormat()
{
  return m_strServerMask;
}


// shortType
/*
class shortField:
properties:
  bool       m_bIsNull;
  bool       m_bIsEditable;
  string     m_strTable;
  string     m_strName;
  string     m_strFormatMask;
  fieldTypes m_fieldType;
  short m_field;
*/

// construction / destruction
shortField::shortField
( short value, bool isNull, bool isEditable,  string name, string table )
:basicField(isNull, isEditable, name, table),m_field(value)
{
  m_strFormatMask = "%d";
  m_strServerMask = "%d";
  m_fieldType = shortType;
}
shortField:: ~shortField(){;}

// assignment
shortField & shortField::operator = ( shortField & input )
{
  basicField::operator = (input);
  m_field = input.get();
  return *this;
}

// access
short & shortField::get( void )
{
  return m_field;
}
void shortField::set( short input )
{
  m_field = input;
  m_bIsNull = false;
}

// formatting
string & shortField::format()
{
  // temporary buffer big enough to hold formatted value
  char temp[64];
  // use old c formatting function
  sprintf( temp, m_strFormatMask.c_str(), m_field);
  m_strFormatted = temp;
  return  m_strFormatted;
}
string & shortField::svrFormat()
{
  // temporary buffer big enough to hold formatted value
  char temp[64];
  // use old c formatting function
  sprintf( temp, m_strServerMask.c_str(), m_field);
  m_strFormatted = temp;
  return  m_strFormatted;
}

// longType
/*
class longField:

properties:
  bool       m_bIsNull;
  bool       m_bIsEditable;
  string     m_strTable;
  string     m_strName;
  string     m_strFormatMask;
  fieldTypes m_fieldType;
  long       m_field;

*/

// construction / destruction
longField::longField
( long value, bool isNull, bool isEditable, string name, string table )
:basicField(isNull, isEditable, name, table),m_field(value)
{
  m_strFormatMask = "%ld";
  m_strServerMask = "%ld";
  m_fieldType = longType;
}

longField::~longField(){;}


// assignment
longField & longField::operator = ( longField & input )
{
  basicField::operator = (input);
  m_field = input.get();
  return *this;
}

// access
long & longField::get( void )
{
  return m_field;
}
void longField::set( long input )
{
  m_field = input;
  m_bIsNull = false;
}

// formatting
string & longField::format()
{
  if( m_bIsNull )
  {
    m_strFormatted="";
  }
  else
  {
    // temporary buffer big enough to hold formatted value
    char temp[64];
    // use old c formatting function
    sprintf( temp, m_strFormatMask.c_str(), m_field);
    m_strFormatted = temp;
  }
  return  m_strFormatted;
}
string & longField::svrFormat()
{
  if( m_bIsNull )
  {
    m_strFormatted="NULL";
  }
  else
  {
    // temporary buffer big enough to hold formatted value
    char temp[64];
    // use old c formatting function
    sprintf( temp, m_strServerMask.c_str(), m_field);
    m_strFormatted = temp;
  }
  return  m_strFormatted;
}

// longlongType
/*
class longlongField:

properties:
  bool       m_bIsNull;
  bool       m_bIsEditable;
  string     m_strTable;
  string     m_strName;
  string     m_strFormatMask;
  fieldTypes m_fieldType;
  long       m_field;

*/

// construction / destruction
longlongField::longlongField
( long long value, bool isNull, bool isEditable, string name, string table )
:basicField(isNull, isEditable, name, table),m_field(value)
{
  m_strFormatMask = "%lld";
  m_strServerMask = "%lld";
  m_fieldType = longlongType;
}

longlongField::~longlongField(){;}


// assignment
longlongField & longlongField::operator = ( longlongField & input )
{
  basicField::operator = (input);
  m_field = input.get();
  return *this;
}

// access
long long & longlongField::get( void )
{
  return m_field;
}
void longlongField::set( long long input )
{
  m_field = input;
  m_bIsNull = false;
}

// formatting
string & longlongField::format()
{
  if( m_bIsNull )
  {
    m_strFormatted="";
  }
  else
  {
    // temporary buffer big enough to hold formatted value
    char temp[64];
    // use old c formatting function
    sprintf( temp, m_strFormatMask.c_str(), m_field);
    m_strFormatted = temp;
  }
  return  m_strFormatted;
}
string & longlongField::svrFormat()
{
  if( m_bIsNull )
  {
    m_strFormatted="NULL";
  }
  else
  {
    // temporary buffer big enough to hold formatted value
    char temp[64];
    // use old c formatting function
    sprintf( temp, m_strServerMask.c_str(), m_field);
    m_strFormatted = temp;
  }
  return  m_strFormatted;
}






// doubleType
/*

class doubleField:
properties:
  bool       m_bIsNull;
  bool       m_bIsEditable;
  string     m_strTable;
  string     m_strName;
  string     m_strFormatMask;
  fieldTypes m_fieldType;
  double  m_field;

*/

// construction / destruction
doubleField::doubleField( double value, bool isNull, bool isEditable, string 
name, string table )
:basicField(isNull, isEditable, name, table),m_field(value)
{
  m_strFormatMask = "%g";
  m_strServerMask = "%f";
  m_fieldType = doubleType;
}
doubleField::~doubleField(){;}

// assignment
doubleField & doubleField::operator = ( doubleField & input )
{
  basicField::operator = (input);
  m_field = input.get();
  return *this;
}

// access
double & doubleField::get( void )
{
  return m_field;
}
void doubleField::set( double input )
{
  m_field = input;
  m_bIsNull = false;
}

// formatting
string & doubleField::format()
{
  if( m_bIsNull )
  {
    m_strFormatted="";
  }
  else
  {
    // temporary buffer big enough to hold formatted value
    char temp[64];
    // use old c formatting function
    sprintf( temp, m_strFormatMask.c_str(), m_field);
    m_strFormatted = temp;
  }
  return  m_strFormatted;
}
string & doubleField::svrFormat()
{
  if( m_bIsNull )
  {
    m_strFormatted="NULL";
  }
  else
  {
    // temporary buffer big enough to hold formatted value
    char temp[64];
    // use old c formatting function
    sprintf( temp, m_strServerMask.c_str(), m_field);
    m_strFormatted = temp;
  }
  return  m_strFormatted;
}

// floatType
/*
  class floatField:

  float  m_field;

*/

  // construction / destruction
floatField::floatField( float value, bool isNull, bool isEditable,  string 
name, string table )
:basicField(isNull, isEditable, name, table),m_field(value)
{
  m_strFormatMask = "%f";
  m_strServerMask = "%g";
  m_fieldType = floatType;
}

floatField::~floatField(){;}

// assignment
floatField & floatField::operator = ( floatField & input )
{
  basicField::operator = (input);
  m_field = input.get();
  return *this;
}

// access
float & floatField::get( void )
{
  return m_field;
}

void floatField::set( float input )
{
  m_field = input;
  m_bIsNull = false;
}

// formatting
string & floatField::format()
{
  if( m_bIsNull )
  {
    m_strFormatted="";
  }
  else
  {
    // temporary buffer big enough to hold formatted value
    char temp[64];
    // use old c formatting function
    sprintf( temp, m_strFormatMask.c_str(), m_field);
    m_strFormatted = temp;
  }
  return  m_strFormatted;
}
string & floatField::svrFormat()
{
  if( m_bIsNull )
  {
    m_strFormatted="NULL";
  }
  else
  {
    // temporary buffer big enough to hold formatted value
    char temp[64];
    // use old c formatting function
    sprintf( temp, m_strServerMask.c_str(), m_field);
    m_strFormatted = temp;
  }
  return  m_strFormatted;
}

// stringType
/*
  class stringField:
  string   m_field;
*/

  // construction / destruction
stringField::stringField( string value, bool isNull, bool isEditable, string
name, string table )
:basicField(isNull, isEditable, name, table),m_field(value)
{
  m_strFormatMask = "%s";
  m_strServerMask = "'%s'";
  m_fieldType = stringType;
}

stringField::~stringField(){;}

// assignment
stringField & stringField::operator = ( stringField & input )
{
  basicField::operator = (input);
  m_field = input.get();
  return *this;
}

// access
string & stringField::get( void )
{
  return m_field;
}
void stringField::set( string input )
{
  m_field = input;
  m_bIsNull = false;
}

// formatting
string & stringField::format()
{
  if( m_bIsNull )
  {
    m_strFormatted="";
  }
  else
  {
    // temporary buffer big enough to hold formatted value
    char * temp = new char[ m_field.size() + m_strFormatMask.size() + 1 ];
    // use old c formatting function
    sprintf( temp, m_strFormatMask.c_str(), m_field.c_str() );
    m_strFormatted = temp;
    delete [] temp;
  }
  return  m_strFormatted;
}
string & stringField::svrFormat()
{
  if( m_bIsNull )
  {
    m_strFormatted="NULL";
  }
  else
  {
    int fieldLen = m_field.size();
    string strTemp("");
    // double all single ticks in the string
    for( int i=0; i<fieldLen; i++ )
    {
      char ch = m_field[i];
      strTemp += ch;
      if( ch == '\'' )
      {
        strTemp += "'";
      }
    }
    fieldLen = strTemp.size();
    // temporary buffer big enough to hold formatted value
    char * temp = new char[ fieldLen + m_strFormatMask.size() + 1 ];
    // use old c formatting function
    sprintf( temp, m_strServerMask.c_str(), strTemp.c_str() );
    m_strFormatted = temp;
    delete [] temp;
  }
  return  m_strFormatted;
}
/*
class currencyField:

 currency  m_field;

*/

// construction / destruction
currencyField::currencyField( currency value, bool isNull, bool isEditable,
string name, string table )
:basicField(isNull, isEditable, name, table),m_field(value)
{
  m_strFormatMask = "%.2f";
  m_strServerMask = "%.2f";
  m_fieldType = currencyType;
}

currencyField::~currencyField(){;}

// assignment
currencyField & currencyField::operator = ( currencyField & input )
{
  basicField::operator = (input);
  m_field.amount = input.get().amount;
  return *this;
}
// Gets
currency & currencyField::get( void )
{
  return m_field;
}
void currencyField::set( currency input)
{
  m_field.amount = input.amount;
  m_bIsNull = false;
}
// formatting
string & currencyField::format()
{
  if( m_bIsNull )
  {
    m_strFormatted="";
  }
  else
  {
    // temporary buffer big enough to hold formatted value
    char temp[64];
    // use old c formatting function
    sprintf( temp, m_strFormatMask.c_str(), m_field);
    m_strFormatted = temp;
  }
  return  m_strFormatted;
}
string & currencyField::svrFormat()
{
  if( m_bIsNull )
  {
    m_strFormatted="NULL";
  }
  else
  {
    // temporary buffer big enough to hold formatted value
    char temp[64];
    // use old c formatting function
    sprintf( temp, m_strServerMask.c_str(), m_field);
    m_strFormatted = temp;
  }
  return  m_strFormatted;
}

/*
  class dateTimeField:

  tm  m_field;
    int tm_sec;    Seconds. [0-60] (1 leap second)
    int tm_min;    Minutes. [0-59]
    int tm_hour;   Hours. [0-23]
    int tm_mday;   Day.  [1-31]
    int tm_mon;    Month. [0-11]
    int tm_year;   Year - 1900.
    int tm_wday;   Day of week. [0-6]
    int tm_yday;   Days in year.[0-365]
    int tm_isdst;  DST.  [-1/0/1]
*/

// construction / destruction
dateTimeField::dateTimeField( tm value, bool isNull, bool isEditable,
            string name, string table )
:basicField(isNull, isEditable, name, table),m_field(value)
{
  m_strFormatMask = "%m/%d/%Y %H:%M:%S";
  m_strServerMask = "%m/%d/%Y %H:%M:%S";
  m_fieldType = dateTimeType;
}
dateTimeField::~dateTimeField(){;}

// assignment
dateTimeField & dateTimeField::operator = ( dateTimeField & input )
{
  basicField::operator = (input);
  m_field = input.get();
  return *this;
}

// access
tm & dateTimeField::get( void )
{
  return m_field;
}
void dateTimeField::set( tm input )
{
  m_field = input;
  m_bIsNull = false;
}

// formatting
string & dateTimeField::format()
{
  if( m_bIsNull )
  {
    m_strFormatted="";
  }
  else
  {
    // temporary buffer big enough to hold formatted value
    char temp[64];
    // use old c formatting function
    strftime( temp, sizeof(temp), m_strFormatMask.c_str(), &m_field );
    m_strFormatted = temp;
  }
  return  m_strFormatted;
}
string & dateTimeField::svrFormat()
{
  if( m_bIsNull )
  {
    m_strFormatted="NULL";
  }
  else
  {
    // temporary buffer big enough to hold formatted value
    char temp[64];
    // use old c formatting function
    strftime( temp, sizeof(temp), m_strServerMask.c_str(), &m_field );
    m_strFormatted = "'";
    m_strFormatted += temp;
    m_strFormatted += "'";
  }
  return  m_strFormatted;
}
/*
class boolField
*/
boolField::boolField
( bool value, bool isNull, bool isEditable,
  string name, string table )
:basicField(isNull, isEditable, name, table),m_field(value)
{
  m_strFormatMask = "Yes|No";
  m_strServerMask = "true|false";
  m_fieldType = boolType;
}
boolField::~boolField(){;}

// assignment
boolField & boolField::operator = ( boolField & input )
{
  basicField::operator = (input);
  m_field = input.get();
  return *this;
}
// access
bool & boolField::get( void )
{
  return m_field;
}
void boolField::set( bool input )
{
  m_field = input;
  m_bIsNull = false;
}
// formatting
string & boolField::format()
{
  if( m_bIsNull )
  {
    m_strFormatted="";
  }
  else
  {
    // temporary buffer big enough to hold formatted value
    string strTrue, strFalse;
    string::size_type idx = m_strFormatMask.find('|');
    if( idx != string::npos ) // found |
    {
      strTrue = m_strFormatMask.substr(0, idx);
      strFalse = m_strFormatMask.substr(idx+1);
      m_strFormatted = m_field?strTrue:strFalse;
    }
  }
  return  m_strFormatted;
}
string & boolField::svrFormat()
{
  if( m_bIsNull )
  {
    m_strFormatted="NULL";
  }
  else
  {
    // temporary buffer big enough to hold formatted value
    string strTrue, strFalse;
    string::size_type idx = m_strServerMask.find('|');
    if( idx != string::npos ) // found |
    {
      strTrue = m_strServerMask.substr(0, idx);
      strFalse = m_strServerMask.substr(idx+1);
      m_strFormatted = m_field?strTrue:strFalse;
    }
  }
  return  m_strFormatted;
}


