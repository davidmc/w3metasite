#include<stdio.h>
#include<iostream.h>
#include<iomanip.h>
#include<fstream.h>
#include<string.h>
#include "vector.h"
#include "hstring.h"


typedef Vector < hString > strArray;

class classMember
{

public:
 int      iType;      // Ex: 1 - n
 hString  type;       // Ex: 'unsigned long'
 hString  name;       // Ex: 'LineCount'
 strArray AccessDefs; // Ex: 'void set<NAME>( double input );'
 strArray AccessImps; // Ex: 'float<CLASS>::<NAME>(void)const{return<NAME>;}'
 strArray Init;       // Ex: '<NAME>(0.0) // double'
 strArray Destroy;    // Ex: '  if( <NAME> ) delete [] <NAME>;'
  
 classMember():iType(0){;}
 ~classMember(){;}

};

typedef Vector < classMember > memArray;

class ClassDef
{

  int openErr;

public:

  // any include statements - other pre-processor statments are ignored
  //
  strArray includes;

  // The eventual name of the class
  //
  hString className;

  // The files to produce...
  //
  hString cppFile;
  hString hppFile;

  // provides for multiple inheritance
  //
  strArray privateParents;
  strArray publicParents;

  // supports private and public.
  // 
  memArray privateMembers;
  memArray publicMembers;
  memArray sizeMembers;

  strArray privateMethods;
  strArray publicMethods;

  // functions
  //
  ClassDef( char * );
  ~ClassDef(){;}

  int  parseMember( classMember & cls, char * classDefLine );
  void findType( classMember & cls );
  void accessDefs( classMember & cls );
  void accessImps( classMember & cls );
  void initAndDestroy( classMember & cls );

  void WriteCpp( void );
  void WriteHpp( void );

};
  
  
