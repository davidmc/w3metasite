#include "hstring.h"
#include<ctype.h>


hString::hString()
:psza((char*)0L)
,size(0)
{
 ;
}

hString::hString( char* input )
:psza((char*)0L)
,size(0)
{
 if( input )
 {
  int len = strlen( input );
  size = len+1;
  psza = new char[size];
  strcpy(psza,input);
 }   
}

hString & hString::operator = (  char * input )
{
 if( input )
 {

  if( psza )
  {
    delete [] psza;
  }
  int len = strlen( input );
  size = len+1;
  psza = new char[size];
  strcpy(psza,input);
   
 }   
 return *this;
}

hString & hString::operator += (  char * input )
{
 if( input )
 {

  int len1 = strlen( input );
  int len2 = 0;
  if( psza )
  {
    len2 = strlen(psza);
  }

  size = len1+len2+1;
  char * temp = new char[size];

  if( len2 )
  {
    strcpy(temp,psza);
    delete[]psza;

  }

  if ( len1 )
  {
    strcat(temp,input);
  }
  psza = temp;

 }
 return *this;
}

hString & hString::upperS(void)
{
 if( psza )
  *psza = toupper( *psza );
 return *this;
}

hString & hString::lowerS(void)
{
 if( psza )
  *psza = tolower( *psza );
 return *this;
}

hString & hString::searchAndReplaceWith( const char * find, char * replace )
{

  if( find && replace )
  {

    int lenFind = strlen( find );
    int lenReplace = strlen( replace );
    char * cursor = psza;

    while( cursor )
    {

      cursor = strstr( cursor, find );

      if( cursor && *cursor )
      {
	 int newSize = size + lenFind + lenReplace;
	 char * newPsza = new char [ newSize ];
	 strcpy( newPsza, psza );
	 char * tempCursor = strstr( newPsza, find );
	 if( tempCursor )
	 {
	   cursor += lenFind;
	   strncpy( tempCursor, replace, lenReplace );
	   tempCursor+=lenReplace;
	   strcpy(tempCursor, cursor );
	   delete [] psza;
	   psza = newPsza;
	   size = newSize;
	   cursor = tempCursor;
	 }
      }

    } // end while

  } // end if

  return *this;

}


hString & hString::trim( void )
{

  int pos = size - 2;
  char * cursor = psza + pos;


  while( (*cursor == ' '|| *cursor == '\0') && pos )
  {

    *cursor = '\0';
    cursor--;

  } // end while 

  return *this;

}


Boolean hString::matchIn( const char * input )
{

  Boolean ret = false;

  if( input )
  {
    int len = strlen( input );

    char * cursor = psza; 

    for( int i = 0; i < size-len; i++, cursor++ )
    {

      if( !strncmp( cursor, input, len ) )
      {
        ret = true; 
      }

    } // end for 

  } // end if

  return ret;

}

Boolean hString::match( const char * input )
{

  Boolean ret = false;

  if( input )
  {

   if( !strcmp( psza, input ) )
   {
     ret = true; 
   }

  } // end if

  return ret;

}



//
// A stand alone trim function
//
void trim( char * psza )
{

  int pos = strlen(psza);
  char * cursor = psza + pos;


  while( (*cursor == ' '|| *cursor == '\0') && pos )
  {

    *cursor = '\0';
    cursor--;

  } // end while 


}

