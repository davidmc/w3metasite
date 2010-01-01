#ifndef open_fields_h
#define open_fields_h

#include <time.h> // for struct tm
#include <vector> // for direct ordinal access
#include <map>    // for key map associative access
#include <string>

/*
  openFields.h
  Defines interface to reusable field class template.
*/
using namespace std;
/*
  The folowing typenames are supported:

  Numeric: short, long, double, float

  String:  string

  Money:   currency - defined type to eleviate comparison problems with double

  Date & Time: tm struct from time.h

*/

enum fieldTypes
{
  shortType, longType,  longlongType, floatType, doubleType,
  stringType, currencyType, dateTimeType, boolType, noType = 10000
};

struct currency
{
  double amount;
};

// this is that class returned by the recordset
class basicField
{
protected:
  bool       m_bIsNull;
  bool       m_bIsEditable;
  string     m_strTable;
  string     m_strName;
  string     m_strFormatMask;
  string     m_strServerMask;
  string     m_strFormatted;
  fieldTypes m_fieldType;
  int        m_iSize;

public:

  basicField();
  basicField( bool isNull, bool isEditable,  string name, string table );
  virtual ~basicField();

    // assignment
  basicField & operator = ( basicField & input );

  // Sets
  basicField & setNull( bool bNull = true );
  basicField & setIsEditable( bool bEditable = true );
  basicField & setName( string strName );
  basicField & setTable( string strTable );
  basicField & setFormatMask( string strFormatMask );
  basicField & setSvrFormatMask( string strFormatMask );
  basicField & setSize( int iSize );

  // Gets
  bool isNull( void );
  bool isEditable( void );
  int  getSize( void );
  string & getName( void );
  string & getTable( void );
  fieldTypes getType( void );
  virtual string & format();
  virtual string & svrFormat();
};

// typedef a vector of fields
typedef vector< basicField * > fieldVector;

// typedef a map of fields
typedef map< string, basicField * > fieldMap;


// shortType
class shortField: public basicField
{
  short m_field;

public:
  // construction / destruction
  shortField( short value, bool isNull, bool isEditable,
              string name, string table );
  ~shortField();
  // assignment
  shortField & operator = ( shortField & input );

  // access
  short & get( void );
  void set( short );

  // formatting
  string & format();
  string & svrFormat();
};

// longType
class longField: public basicField
{
  long  m_field;

public:
  // construction / destruction
  longField( long value, bool isNull, bool isEditable,
              string name, string table );
  ~longField();

  // assignment
  longField & operator = ( longField & input );

  // access
  long & get( void );
  void set( long );

  // formatting
  string & format();
  string & svrFormat();
};

// longType
class longlongField: public basicField
{
  long long  m_field;

public:
  // construction / destruction
  longlongField( long long value, bool isNull, bool isEditable,
              string name, string table );
  ~longlongField();

  // assignment
  longlongField & operator = ( longlongField & input );

  // access
  long long & get( void );
  void set( long long );

  // formatting
  string & format();
  string & svrFormat();
};


// doubleType
class doubleField: public basicField
{
  double  m_field;

public:
  // construction / destruction
  doubleField( double value, bool isNull, bool isEditable,
               string name, string table );
  ~doubleField();
  // assignment
  doubleField & operator = ( doubleField & input );

  // access
  double & get( void );
  void set( double );

  // formatting
  string & format();
  string & svrFormat();
};

// floatType
class floatField: public basicField
{
  float  m_field;

public:
  // construction / destruction
  floatField( float value, bool isNull, bool isEditable,
               string name, string table );
  ~floatField();
  // assignment
  floatField & operator = ( floatField & input );

  // access
  float & get( void );
  void set( float );
  // formatting
  string & format();
  string & svrFormat();
};

// stringType
class stringField: public basicField
{
  string   m_field;

public:
  // construction / destruction
  stringField( string value, bool isNull, bool isEditable,
               string name, string table );
  ~stringField();
  // assignment
  stringField & operator = ( stringField & input );

  // access
  string & get( void );
  void set( string );

  // formatting
  string & format();
  string & svrFormat();
};

class currencyField: public basicField
{
  currency  m_field;

public:
  // construction / destruction
  currencyField( currency value, bool isNull, bool isEditable,
               string name, string table );
  ~currencyField();
  // assignment
  currencyField & operator = ( currencyField & input );

  // Gets
  currency & get( void );
  void set( currency );

  // formatting
  string & format();
  string & svrFormat();
};

class dateTimeField: public basicField
{
  tm  m_field;

public:
  // construction / destruction
  dateTimeField( tm value, bool isNull, bool isEditable,
                 string name, string table );
  ~dateTimeField();

  // assignment
  dateTimeField & operator = ( dateTimeField & input );

  // access
  tm & get( void );
  void set( tm  );

  // formatting
  string & format();
  string & svrFormat();
};

class boolField: public basicField
{
  bool  m_field;

public:
  // construction / destruction
  boolField( bool value, bool isNull, bool isEditable,
               string name, string table );
  ~boolField();

  // assignment
  boolField & operator = ( boolField & input );

  // access
  bool & get( void );
  void set( bool );

  // formatting
  string & format();
  string & svrFormat();
};

#endif

