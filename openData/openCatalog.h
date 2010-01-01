#ifndef open_catalog_h
#define open_catalog_h
#include <string>
#include "openDB.h"

class openCatalog;

// open field for field descriptions collection
class openField
{
protected:
  // data members
  string m_name;
  string m_type;
public:
  // constructor
  openField():
  m_name(""),
  m_type("")
  { 
    ;
  }
  // destructor
  virtual ~openField(){;}
  // copy constructor 
  openField( const openField & in):
  m_name(in.m_name),
  m_type(in.m_type)
  { 
    ;
  }
  // assignment operator
  openField & operator = ( const openField & in )
  {
    m_name = in.m_name;
    m_type = in.m_type;
    return *this;
  }
  // member access operators  
  // get
  string & name( void ) { return m_name; }
  // set
  void name( const string & in ) { m_name = in; }
  
  // get
  string & type( void ) { return m_type; }
  // set
  void type( const string & in ) { m_type = in; }  

};


typedef vector<openField> openFields;

/*
  This openCatalog class defines the interfaces to a generic catalog
  
*/

class openCatalog
{
private: 
  openFields m_fields;
  openDB * m_pDB;
  openRS * m_pRS;
  string m_name;
  string m_connectstring;
public:
  openCatalog( string connectstring );
  virtual ~ openCatalog();  
  virtual bool open();
  virtual bool next();
  openFields & fieldsList();
  virtual bool close();
  virtual string & name();
};

#endif

#ifdef IN_T2_TESTHARNESS
// openCatalog cat("user=postgres dbname=woodhollow");
openCatalog cat(getConnectstring());
if( cat.open() )
{
  cout << "catalog opened!" << endl;
  do
  {
    cout << "Table: " << cat.name() << endl;
    openFields & lst = cat.fieldsList();
    for( int i=0; i<lst.size(); i++)
    {
      cout << "    Field: " << lst[i].name() << " type: " << lst[i].type() << endl;
    }
  }
  while( cat.next() );
}

#endif
