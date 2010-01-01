#ifndef HSTRING_H
#define HSTRING_H

#include<string.h>
typedef enum Boolean { false, true };


class hString
{

private:

 char * psza;
 int size;

public:

 hString();

 hString( char* );

 ~hString(){ if(psza) delete[]psza; }

 char * psz() { return psza; }

 operator char * () { return psza; } 

 hString & operator = (  char * input );

 hString & operator += (  char * input );

 int max( void ) { return size; }

 hString & searchAndReplaceWith( const char * find, char * replace );

 hString & upperS(void);

 hString & lowerS(void);
 
 hString & trim(void);

 Boolean matchIn( const char * input );

 Boolean match( const char * input );


};

// A stand alone trim function
//
void trim( char * );

#endif
