/*
  aString.cpp  general purpose string class.  

  Description:
  Provides the implementation to 
  the class definition described in aString.h

  copyright (c) 1996 - 2001 
  
  David McCombs davidmc@newcottage.com

  Nuclear Core Class Library

  freely re-usable, redistributable.
*/


#include<ctype.h>
#include<iostream>
#include<stdlib.h>
#include "Astring.h"

//  default constructor uses initializers
//  instead of assignment.
//
aString::aString()
:buffer(NULL)
,endDelimiter('\0')
,size(0)
,pchToken(NULL)
,tokened(NULL)
,tokenleft(NULL)
{
 ;
}

// constructor with passed char string.
//
aString::aString( const char* input )
:buffer(NULL)
,endDelimiter('\0')
,size(0)
,pchToken(NULL)
,tokened(NULL)
,tokenleft(NULL)
{
 if( input )
 {
  allocate(strlen( input )+1);
  strcpy(buffer,input);
 }
}

// STL string compatibility
aString::aString( const string & input )
:buffer(NULL)
,endDelimiter('\0')
,size(0)
,pchToken(NULL)
,tokened(NULL)
,tokenleft(NULL)
{
 if( input.length() )
 {
  allocate( input.length()+1);
  strcpy(buffer,input.c_str());
 }
}

// Copy constructor.
//
aString::aString( const aString & input )
:buffer(NULL)
,endDelimiter('\0')
,size(0)
,pchToken(NULL)
,tokened(NULL)
,tokenleft(NULL)
{
 if( input.length() )
 {
  allocate(input.length()+1);
  strcpy(buffer,input.str());
 }
}

// Assignment operator using char string.
//
aString & aString::operator = ( char * input )
{
 return *this = (const char *) input;
}

// const char string input version of assignment operator
//
aString & aString::operator = (  const char * input )
{
 if( input )
 {
  allocate(strlen( input )+1);
  strcpy(buffer,input);
 }
 else
 {
   delete [] buffer;
   buffer = NULL;
   size = 0;
 }
 tokenReset();
 return *this;
}

// compatibility with STL string
aString & aString::operator = ( const string & input )
{
  (*this) = input.c_str();
  return *this;
}

// assigment operator using aString reference input
//
aString & aString::operator = ( const aString & input )
{
 tokenReset();
 if( input.length() )
 { 
  allocate(input.length()+1);
  strcpy(buffer,input.str());
 }
 else
 {
   delete [] buffer;
   buffer = NULL;
   size = 0;
 }
 
 return *this;
}

// determine if the buffer is a number
bool aString::isNumber( void )
{
  bool bRet = false; // set false for the empty string case
  char numerical[]="0123456789.";
  char * chCursor = buffer;
  char * chNum = numerical;
  while( chCursor && *chCursor)
  {
    bRet = false;
    while( chNum && *chNum )
    {
      if( *chNum == *chCursor )
        bRet = true;
      chNum++;
    }
    if( !bRet )
      break;
    chCursor++;
  }
  return bRet;
}

// attach method
void aString::attach( char * input )
{
  if( input )
  {
    size = strlen(input)+1;
    delete [] buffer;
    buffer = input;
  }
}



// += operator performs string concatenation.
//
aString & aString::operator += (  const char * input )
{

  if( input )
  {
    int len1 = strlen( input );
    int len2 = 0;

    if( buffer )
    {
      len2 = strlen(buffer);
    }

    // allocated buffer must be the length of both
    // strings plus a null, copy the original back.
    //
    allocate( len1+len2+1, true );
    
    if ( len1 )
    {
      strcat(buffer,input);
    }
    
  }
  return *this;
}

aString & aString::operator += (  char * input )
{
  return *this += ( const char * ) input;
}

aString & aString::operator += (  char input )
{
  char temp[2];
  temp[0]=input;
  temp[1]='\0';
  return *this += ( const char * ) temp;
}

aString & aString::upperS(void)
{
 if( buffer )
  *buffer = static_cast<char>(toupper( *buffer ));
 return *this;
}

aString & aString::lowerS(void)
{
 if( buffer )
  *buffer = static_cast<char>(tolower( *buffer ));
 return *this;
}

aString & aString::upper(void)
{
 if( buffer )
 {
   for( int i=0; i<size; i++ )
   {
     buffer[i] = static_cast<char>(toupper( buffer[i] ));
   }
 }
 return *this;
}

aString & aString::lower(void)
{
 if( buffer )
 {
   for( int i=0; i<size; i++ )
   {
      buffer[i] = static_cast<char>(tolower( buffer[i] ));
   }
 } 
 return *this;
}

// subString methods
aString &  aString::subString( const char * input, int start )
{
  if( input && 
      strlen( input ) >= (unsigned) start )
  {
     const char * adj = input+start;
     *this=adj;   
  }
  return *this;
}

aString &  aString::subString( const char * input, int start, int end )
{
  if( input && 
      end >= start &&
      strlen( input ) >= (unsigned) end )
  {
     const char * adj = input+start;
     if( allocate( 2+end-start ) )
     {
       strncpy( buffer, adj, 1+end-start );
       buffer[1+end-start]='\0';
     }
  }
  return *this;
}


// Sets the buffer to a new size. 
aString & aString::setSize( int newSize )
{
  allocate( newSize, true );
  return *this;
}


// Replaces any chars in 'find' with the replacement char 'replace'
// defaulted parameter provides for searching in alternate string, though
// replacement still occurs in the internal buffer.
//
aString & aString::replaceFoundWith( const char * find, const char replace, const char * search )
{

  if ( find )
  {
    int findLen = strlen(find);

    int targetLen = strlen(buffer);

    int sourceLen = search? strlen(search): targetLen;

    const char * pszSource = search? 
                             search: 
                             buffer;

    // index should be the smaller of source and target
    int idx = sourceLen < targetLen? sourceLen: targetLen;

    for( int i=0; i < idx; i++ )
    {
      for( int j=0; j<findLen; j++ )
      {
        if(  pszSource[i]==find[j] )
        {
          buffer[i] = replace;
          break;
        } 
      }
    } 
  }
  return *this;   
}

aString & aString::replaceFoundWith( const char * find, const char * replace )
{
  if( find && replace )
  {
    bool changed = false;
    int lenFind = strlen( find );
    int lenReplace = strlen( replace );
    int i = 0, j = 0;
    if( lenFind < size && lenFind > 0 )
    {
      int newBufferSize=size;

      if( lenFind < lenReplace )
      {    
        // calculate the worst possible case 
        // for the new buffer size
        int xFoundCouldBeReplaced = size/lenFind;
        // account for divisor
        newBufferSize = xFoundCouldBeReplaced * lenReplace; 
        // account for modulus and null terminator
        newBufferSize += (size%lenFind) + 1;
      }
      
      // allocate the new buffer
      char * newBuffer = new char [newBufferSize];
      if (newBuffer) do 
      {
        // optimism
        bool matches = true;
        for ( int k=0; k<lenFind; k++ )
        {
          if( find[k] != buffer[i+k] )
          {
            // reality
            matches=false;
            break;
          } // end if matches
        } // end for find string
        if( matches )
        {
          memcpy(&(newBuffer[j]), replace, lenReplace );
          j+=lenReplace;
          i+=lenFind;
          changed=true;
        }
        newBuffer[j] = buffer[i];
        i++; 
        j++;
      } while ( buffer[i] );
	    if( changed )
      {
        newBuffer[j]=0;
        allocate(strlen(newBuffer) + 1); 
        strcpy( buffer, newBuffer );
      }
      delete [] newBuffer;
    } // end if lenFind < size && lenFind > 0 
  } // end if find && replace
  return *this;
}

// replace at the zero based position with the passed replacement text.
//
aString & aString::replaceAt( unsigned int pos, char * replace )
{
  if( replace && strlen( replace ) )
  {
    int replaceLen = strlen( replace );
    if( replaceLen && ( pos+replaceLen < (unsigned) size ) )
    { 
      for( char * pszSource = replace, * pszTarget = buffer+pos; 
           *pszSource ; 
           pszSource++, pszTarget++ )
      {
        *pszTarget = *pszSource;
      }
    } 
  }
  return *this;
}

// removes any instance of the characters found in
// the character list 'find'.
//
aString & aString::removeChars( const char * find )
{

  if( buffer )
  {
    char * buf = new char [ size ];
    strcpy( buf, buffer );
    char * ptr = strtok(  buf, find ); 
    memset( buffer, '\0', size );
    while( ptr )
    {
      strcat( buffer, ptr );
      ptr = strtok( NULL, find );      
    }
    delete [] buf;

  } // end if buffer

  return *this;
}

aString & aString::encapsulateIn( const char * boundary, const char * end )
{
  if( buffer && boundary )
  {
    int lenHead = strlen( boundary );
    int lenTail = end? strlen( end ): lenHead;
    char * buf = new char[ lenHead + size + lenTail ];
    strcpy( buf, boundary );
    strcat( buf, buffer );
    strcat( buf, end? end: boundary );
    size += lenHead+lenTail;   
    delete [] buffer;
    buffer = buf; 
  } // end if the strings exist
  return *this;
}

aString & aString::trim( aString::end end )
{

  if( buffer )
  {
      char * cursor;
      
      if( end == left || end == both )
      {
          cursor = buffer;
          while( *cursor &&
                 ( *cursor == ' ' || 
                   *cursor == '\n' ||
                   *cursor == '\r' ||
                   *cursor == '\t' ) )
          {
      
            *cursor = '\0';
            cursor++;
      
          } // end while
          int newLen = strlen( cursor );
          if( newLen && cursor != buffer )
          {
              memmove( buffer, cursor, newLen+1 );
          }
      }
      
      if( end == right || end == both )
      {
          // the magic number '2' comes from 
          // the addition of zero base positioning and
          // the probable null at end of the string.
          int pos = size - 2;
      
          cursor = buffer + pos;
      
          while( ( *cursor == ' ' || 
                   *cursor == '\0' ||
                   *cursor == '\n' ||
                   *cursor == '\r' ||
                   *cursor == '\t'  ) && cursor != buffer )
          {
      
            *cursor = '\0';
            cursor--;
      
          } // end while 
      }
      
  } // end if buffer

  return *this;

}

char * aString::find( const char * input ) const
{
  char * strRet = NULL;
  if( input )
  {
    int len = strlen( input );

    char * cursor = buffer; 

    for( int pos = 0; 
         pos < size-len && strRet == NULL; 
         pos++, cursor++ )
    {

      if( !strncmp( cursor, input, len ) )
      {
        // found it!
        strRet = cursor;
      }

    } // end for 

  } // end if
  return strRet;
}
bool aString::matchIn( const char * input ) const
{

  bool ret = false;

  if( input )
  {
    int len = strlen( input );

    char * cursor = buffer; 

    for( int pos = 0; pos < size-len && ret == false; pos++, cursor++ )
    {

      if( !strncmp( cursor, input, len ) )
      {
        // found it!
        ret = true;
      }

    } // end for 

  } // end if

  return ret;

}

bool aString::matchesStartOf( const char * input ) const
{

  bool ret = false;

  if( input )
  {
    int len = strlen( input );

    if( len > size )
    {
      int myLen = length();
      if( strncmp( buffer, input, myLen ) == 0 )
      {
        // compares!
        ret = true;
      }
    }
  } // end if

  return ret;

}

bool aString::startOfMatches( const char * input ) const
{

  bool ret = false;

  if( input )
  {
    int len = strlen( input );

    if( len <= length() )
    {
      if( strncmp( buffer, input, len ) == 0 )
      {
        // compares!
        ret = true;
      }
    }
  } // end if

  return ret;

}



bool aString::match( const char * input ) const
{

  bool ret = false;

  if( buffer && input )
  {

   if( !strcmp( buffer, input ) )
   {
     ret = true; 
   }

  } // end if

  return ret;

}

//
// Internal function for aStrings use only.
//
int aString::oneToken( const char * separator, aString * delimiterHolder )
{
  enum tokenStates { start, leadingDelimiter, adding, trailingDelimiter } state;
  bool charIsDelimiter;

  int iStartDelimiterCount = 0;

  // Set beginning state.
  state = start; 

  while( *tokenleft )
  {
    const char * sepCursor = separator;
    endDelimiter = '\0';
    charIsDelimiter = false;

    while( *sepCursor && *tokenleft )
    {
      if( *sepCursor == *tokenleft )
      {
        endDelimiter = *tokenleft;
        charIsDelimiter = true;
        break;
      }
      sepCursor++;

    } // end inner while on separators

    // handle states in context of current state   
    //
    switch( state )
    {
    case start:
    case leadingDelimiter:
        if( charIsDelimiter )
        {
            if(delimiterHolder)
            {
              (*delimiterHolder) += endDelimiter;
            }
            state = leadingDelimiter;
            iStartDelimiterCount++;
        }
        else
        {
            state = adding;
        }
        break;
    case adding:
        if( charIsDelimiter )
        {
            *tokenleft='\0';
            state = trailingDelimiter;
        }
        break;

    } // end switch
        

    // if the current character is  '\0' and
    // termAdded is true, then we must be on 
    // a delimiter.
    //
    if( tokenleft && *tokenleft == '\0' && state == trailingDelimiter )
    {
      // move to the next token and break
      // out of the while loop.
      //
      tokenleft++;
      break;
    }

    if( tokenleft )
    {
      tokenleft++;
    }

  } // end while

  return iStartDelimiterCount;

}

// Slides the requested amount over the tokenized buffer
// default is 1 character slide to move tokenleft to the next 
// position in string.  Can slide a specified 
// amount past the current position.  always returns the 
// current position.
//
char * aString::slide( int iAmount )
{  
  // detect end of tokens
  if( tokenleft && *tokenleft == '\0' )
  { 
    tokenReset();
  }
  // If this is a new token operation,
  // allocate and set a new and increment tokenLeft for return.
  else
  {
    if ( tokened == NULL )
    {
      if( (  pchToken = tokenleft = tokened = new char [ size ] ) != NULL )
      {
        strcpy( tokened, buffer );
      } // end if token allocation worked
    }
    for( int x=0; x<iAmount && *tokenleft; x++)
    {
      // increment tokenleft for return.
      tokenleft++;  
    }
    pchToken = tokenleft-1;
  }
  if( pchToken )
  {
    allTokens.add( pchToken );
  }
  return pchToken;
}



char * aString::token( const char * separator, aString * delimiterHolder )
{
  if( buffer )
  {
    // Tokened holds the new allocated memory buffur. 
    // token is for return of the current token.
    // tokenleft hold the position to the remaining tokened string.
    // 
    int iJump = 0;

    // Are we at the end of our string?
    //
    if( tokenleft && *tokenleft == '\0' )
    { 
      tokenReset();
    }

    // Are we performing a new token operation?
    //
    else if ( tokened == NULL )
    {
      if( ( pchToken = tokenleft = tokened = new char [ size ] ) != NULL )
      {

        strcpy( tokened, buffer );
        iJump = oneToken( separator, delimiterHolder );

      } // end if token alloction worked

    } // end if a new token operation is happening. 

    // We must be in an ongoing token operation.
    //
    else
    {
      pchToken = tokenleft;
      iJump = oneToken( separator, delimiterHolder );
    }

    pchToken+=iJump;  
  }

  if( pchToken )
  {
    allTokens.add( pchToken );
  }
  return pchToken;
}

int aString::remainderPosition( void )
{
  int iRet = -1;
  if( tokenleft && tokened && tokenleft >= tokened )
  {
    iRet = tokenleft - tokened;
  }
  return iRet;    
}

int aString::tokenPosition( void )
{
  int iRet = -1;
  if( pchToken && tokened && pchToken >= tokened )
  {
    iRet = pchToken - tokened;
  }
  return iRet;    
}

char aString::startDelimiter( void )
{
  char chRet='\0';
  int iTokPos=tokenPosition();
  if( iTokPos > 0 && buffer && length() > iTokPos-1)
  {
    chRet = buffer[iTokPos-1];
  }
  return chRet;
}


char * aString::backTrack()
{  
  int tokCount = allTokens.Size();
  int tokPos = tokenPosition();
  int tokEnd = tokPos + (pchToken?strlen( pchToken ):0);
  if( tokCount > 1 )
  {
    // get last tok pos.
    char * pchLast = allTokens[tokCount-2];

    if( pchLast )
    {
      // determine if last was a slide op
      // by looking for a null beween the last 
      // token and the current one
      bool slideLast = true;
      char * testChar = pchLast;
      while( testChar < pchToken )
      {
         if( *testChar == '\0' )
         {
           slideLast=false;
           break;
         }
         testChar++;
      }
      if( slideLast==true )
      {
        // simple position moves
        pchToken = pchLast;
        tokenleft = pchLast+1;
      }
      else 
      {
        // undo last delimiter
        tokened[tokEnd]=buffer[tokEnd];

        // move the remainder
        tokenleft = pchLast + strlen(pchLast) + 1;

        // move token back
        pchToken = pchLast;
      }
    }
  }
  else 
  {
    // either ONE or NO tokens, 
    // resetting is appropriate here
    tokenReset();
  }
  if( tokCount )
  {
    allTokens.remove(tokCount-1);
  }
  return pchToken;
}

char * aString::currentToken( void )
{
  return pchToken;
}


void aString::tokenReset( void )
{
  delete [] tokened;
  pchToken = tokenleft = tokened = NULL;
  if( allTokens.Size() > 0 )
    allTokens.removeAll();
}


aString & aString::exclusive( char * input )
{
  // Be very defensive
  //
  if( input && *input && size && buffer && *buffer )
  {

    char * source;
    char * target;
    for( source = target = buffer; *source; source++ )
    {
      for( int i = 0; input[i]; i++ )
      {
        if( *source == input[i] )
        {
          *target = *source;
          target++;
          break; 
        } // end if match found

      } // end for i

    } // end for source

    *target='\0';

  } // end if input 

  return *this;
}

aString & aString::exclusive( char low, char high )
{
  if( size && buffer && *buffer )
  {
    char * source;
    char * target;
    for( source = target = buffer; *source; source++ )
    {
      if( *source >= low && *source <= high )
      {
        *target = *source;
        target++;
        break;
      } // end if match found

    } // end for source

    *target='\0';
  } // end if input
  return *this;
}


// Added this to manage memory a little 
//  better than the original string class
bool aString::allocate(int sizeRequested, bool bCopy )
{
  bool bRet = false;
  if( sizeRequested > size )
  {
    size = (1+sizeRequested/incSize)*incSize;
    char * temp = buffer;
    buffer = new char [size];
    if( buffer ) 
    {
      bRet = true;
      memset( buffer, 0, size );
      if( bCopy && temp )
      {
        strcpy( buffer, temp ); 
      }
    }
    delete [] temp;
  }
  else
  {
    bRet = true;
  }
  tokenReset();
  return bRet;
}

// This method converts the string from a hexadecimal escape to the actual character,
// input is the starting token that signifies the begining of the hex escape.
// two characters are read after the escape, since that is all that is needed to represent
// a character in a hex.
//
aString & aString::deHexify( char hexToken )
{
  if( size && strchr( buffer, hexToken ) ) 
  {
    // continue operation
    
    int iChar;
    char hexVal[5];
    char * curPos; // position in the current buffer
    char * curNew; // position in the new buffer
    char * end;
    char * newBuffer = new char [ size ];
    memset( newBuffer, 0, size );
    for( curNew=newBuffer, curPos=buffer; *curPos; curPos++, curNew++ )
    {
      if( *curPos==hexToken &&   // Token char was found
          strlen(curPos) >= 3 && // Enough length to dehex
          isxdigit(curPos[1]) &&    // Both chars after
          isxdigit(curPos[2]) )     // escape are decimal. (12/07/2006:DKMc)
      {
        curPos++;
        strcpy( hexVal, "0x" );
        memcpy(hexVal+2, curPos, 2);
        hexVal[4]='\0';
        iChar = (int) strtol( hexVal, &end, 16 );
        *curNew = iChar;
      
        curPos++;
      }
      else
      {
        *curNew=*curPos;
      }        
        
    } // end for 

    delete [] buffer;
    buffer = newBuffer;
  }
  return *this; 
}

std::ostream & operator << (std::ostream & outStream, const aString & inString )
{
  return outStream << inString.str();
}

