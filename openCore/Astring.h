/*

  Astring.h :  aString class definition file.

  Purpose: The aString class is a generic string class intended to be portable 
     across operating environments.  It provides generic string operations 
     such as assignment, copy construction substring, concatenation, 
     replacement, and character removal.  This class is intended to be used 
     as a component in the construction of filters, parsers etc.
     It is also intended as a base for more specialized string classes.

  copyright (c) 1996 - 2001 
  
  David McCombs davidmc@newcottage.com

  Nuclear Core Class Library

  freely re-usable, redistributable.

*/
 
#ifndef ASTRING_H
#define ASTRING_H

#include <string>
#include "Vector.h" // for backtrack pointers

class aString
{

public:
 // for trimming
 //
 enum end { left, right, both };

 // Returns the character that ended the last token operation.
 //
 inline char EndDelimiter( void ){ return endDelimiter; }
 char startDelimiter( void );


 // default constructor
 //
 aString();

 // constructor using a passed char pointer
 //
 aString( const char* );

  // for better compatibility with STL string class
 //
 aString( const string & input );

 // copy constructor 
 //
 aString( const aString & input );

 // destructor
 //
 ~aString(){ delete[]buffer; delete[]tokened; }

 // method returns the buffer
 //
 const char * str() const { return buffer?buffer:""; }

 // for better compatibility with STL string class
 //
 const char * c_str() const { return str(); }

 // method returns the remainder of
 // the string undergoing token operations.
 //
 char * remainder() const { return tokenleft; }

 // cast operator casts aString to char* 
 //
 operator char * () const { return buffer; }

 // for better compatibility with STL string class
 //
 operator string () const { return string(buffer); }

 void attach( char * input ); 

 // overloaded assignment operators.
 //
 aString & operator = ( char * input );
 aString & operator = ( const char * input );
 aString & operator = ( const aString & input );

 // for better compatibility with STL string class
 //
 aString & operator = ( const string & input );


 // subString methods
 // These methods perform the function of specialized assignment.
 //
 aString & subString( const char * input, int start );
 aString & subString( const char * input, int start, int end );

 // operator performs string concatenation.
 //
 aString & operator += (  const char * input );
 aString & operator += (  char * input );
 aString & operator += (  char input );


 // method that returns the size of the string (as set by new[]).
 //
 int Size( void ) const { return size; }
 int length( void ) const { return buffer?strlen( buffer ):0; }

 // method sets the buffer to a new size.
 // 
 aString & setSize( int newSize );

 // method replaces any chars in 'find' with the replacement char 'replace' 
 // defaulted parameter provides for searching in alternate string, though
 // replacement still occurs in the internal buffer.
 //
 aString & replaceFoundWith( const char * find, const char replace, const char * search = NULL );

 // method replaces any match of 'find' with the replacement text 'replace' 
 //
 aString & replaceFoundWith( const char * find, const char * replace );

 // method replaces the text at position 'pos' with text 'replace'.
 // 'pos' is zero based. i.e. the first character position is '0'.
 //
 aString & replaceAt( unsigned int pos, char * replace );

 // This method removes all characters found in char list 'find'
 //
 aString & removeChars( const char * find );

 // This method encapsulates buffer in 'boundary', 'boundary' by default.
 // if 'end' is specified, the buffer is encapsulated in 'boundary', 'end'
 //
 aString & encapsulateIn( const char * boundary, const char * end = NULL ); 

 // This method upper cases the first character of the string.
 //
 aString & upperS(void);

 // This method lower cases the first character of the string.
 //
 aString & lowerS(void);
 
 // This method upper cases the string.
 //
 aString & upper(void);

 // This method lower cases the string.
 //
 aString & lower(void);

 // This method trims spaces off of the end of the string.
 //
 aString & trim( aString::end end = right );

 // This method returns true if the 'input' string is found anywhere
 // in the buffer
 //
 bool matchIn( const char * input )const;

 // return the point in the buffer where the input string is found
 char * find( const char * input ) const;

 // This returns true if the buffer exactly matches the start of the 
 // input string.
 //
 bool matchesStartOf( const char * input )const;

 // This returns true if the input string exactly matches the start of the
 // buffer.
 //
 bool startOfMatches( const char * input ) const;

 // This method returns true only if the 'input' string is an exact match to
 // buffer.
 //
 bool match( const char * input )const;
 // associated comparison operators
 bool operator == ( aString & input ){ return match(input.str()); }
 bool operator == ( const char * input ){ return match(input); }

 // Slide over the copy of the buffer, one
 // or more characters at a time,
 // returning the current position.
 //
 char * slide( int iAmount = 1 );

 // This method returns tokens until the end of the buffer is reached.
 // The 'separator' string holds a list of characters that the buffer is tokenized
 // around.  The original buffer string is not changed by this  method.
 // the token method should be called until it returns NULL unless tokenReset()
 // is called.
 // Also, each token should be used before the next invocation of token().
 // previous tokens are invalid after calling token() for the current token.
 //
 char * token( const char * separator, aString * delimiterHolder = NULL );
 
 // This method resets the token buffer to NULL, insuring subsequent calls to 
 // token() start at the beginning of the string.
 //
 void tokenReset( void );

 // indicates the position of the current valid token
 int tokenPosition( void );

 // indicates the position of the remainder of the token buffer 
 int remainderPosition( void );


 // returns the current valid token
 char * currentToken( void );

 // back the token string to the previous token
 char * backTrack( void );
 
 // returns true if the string evaluates to a number
 //  ( decimal, with or without decimal point)
 bool isNumber( void );

 // the following method is an exclusive filter.
 // an input  range is an overload of the
 // method.
 //
 aString & exclusive( char * input );
 aString & exclusive( char low, char high );

// This method converts the hexadecimal escaped string to the actual characters,
// input is the starting token that signifies the begining of the hex escape.
// two characters are read after the escape, since that is all that is needed to represent
// a character in a hex number representation.
//
aString & deHexify( char hexToken );

protected:

 int oneToken( const char * separator, aString * delimiterHolder );
 bool allocate( int sizeRequseted, bool bCopy = false );

private:

 char * buffer;     // pointer to buffer
 char endDelimiter; // holds the ending delimiter of the last returned token.
 int  size;         // current size of buffer
 enum { incSize=512}; // increment size by this amount with each allocation
 char * tokened;    // pointer to new[] allocated tokenized string.
 char * tokenleft;  // string remaining after token operation.
 char * pchToken;   // the current token 
 pointerVector< char > allTokens;

};

/* stream operator for aString class */
std::ostream & operator << (std::ostream & outStream, const aString & inString );


#endif
