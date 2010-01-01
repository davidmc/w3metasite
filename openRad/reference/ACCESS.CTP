//
// Set and Get methods for <CLASS>::<LNAME>
//

double <CLASS>::<UNAME>( void ) const
{
  return <LNAME>;
} 
void <CLASS>::set<UNAME>( double input )
{
  <LNAME> = input;
}

 
-
//
// Set and Get methods for <CLASS>::<LNAME>
//

const double* <CLASS>::<UNAME>( void ) const
{
  return <LNAME>;
} 

void <CLASS>::set<UNAME>( const double* input, unsigned long size )
{
  if( input && size )
  {
    if( <LNAME> )
    {
      delete [] <LNAME>;
    }
    size<UNAME> = size;
    <LNAME> = new double [size];
    memcpy( <LNAME>, input, size*sizeof(double) );
  } 
} 


-
//
// Set and Get methods for <CLASS>::<LNAME>
//

float <CLASS>::<UNAME>( void ) const
{
  return <LNAME>;
} 
void <CLASS>::set<UNAME>( float input )
{
  <LNAME> = input;
}


-
//
// Set and Get methods for <CLASS>::<LNAME>
//

long <CLASS>::<UNAME>( void ) const
{
  return <LNAME>;
} 
void <CLASS>::set<UNAME>( long input )
{
  <LNAME> = input;
}


-
//
// Set and Get methods for <CLASS>::<LNAME>
//

unsigned long <CLASS>::<UNAME>( void ) const
{
  return <LNAME>;
} 
void <CLASS>::set<UNAME>( unsigned long input )
{
  <LNAME> = input;
}


-
//
// Set and Get methods for <CLASS>::<LNAME>
//

int <CLASS>::<UNAME>( void ) const
{
  return <LNAME>;
} 
void <CLASS>::set<UNAME>( int input )
{
  <LNAME> = input;
}


-
//
// Set and Get methods for <CLASS>::<LNAME>
//

unsigned int <CLASS>::<UNAME>( void ) const
{
  return <LNAME>;
} 
void <CLASS>::set<UNAME>( unsigned int input )
{
  <LNAME> = input;
}


-
//
// Set and Get methods for <CLASS>::<LNAME>
//

short <CLASS>::<UNAME>( void ) const
{
  return <LNAME>;
} 
void <CLASS>::set<UNAME>( short input )
{
  <LNAME> = input;
}


-
//
// Set and Get methods for <CLASS>::<LNAME>
//

unsigned short <CLASS>::<UNAME>( void ) const
{
  return <LNAME>;
} 
void <CLASS>::set<UNAME>( unsigned short input )
{
  <LNAME> = input;
}


-
//
// Set and Get methods for <CLASS>::<LNAME>
//

const char* <CLASS>::<UNAME>( void ) const
{
  return <LNAME>;
} 
void <CLASS>::set<UNAME>(const char* input, unsigned long size )
{
  if( input && size )
  {
    if( <LNAME> )
    {
      delete [] <LNAME>;
    }
    size<UNAME> = size;
    <LNAME> = new char [size];
    memcpy( <LNAME>, input, size );
  } 
}

-
//
// Set and Get methods for <CLASS>::<LNAME>
//

char <CLASS>::<UNAME>( void ) const
{
  return <LNAME>;
} 
void <CLASS>::set<UNAME>( char input )
{
  <LNAME> = input;
}


-
//
// Set and Get methods for <CLASS>::<LNAME>
//

unsigned char <CLASS>::<UNAME>( void ) const
{
  return <LNAME>;
} 
void <CLASS>::set<UNAME>( unsigned char input )
{
  <LNAME> = input;
}


-
//
// Set and Get methods for <CLASS>::<LNAME>
//

byte <CLASS>::<UNAME>( void ) const
{
  return <LNAME>;
} 
void <CLASS>::set<UNAME>( byte input )
{
  <LNAME> = input;
}


-
//
// Set and Get methods for <CLASS>::<LNAME>
//

const byte* <CLASS>::<UNAME>( void ) const
{
  return <LNAME>;
} 
void <CLASS>::set<UNAME>( const byte * input, unsigned long size )
{
  if( input && size )
  {
    if( <LNAME> )
    {
      delete [] <LNAME>;
    }
    size<UNAME> = size;
    <LNAME> = new byte [size];
    memcpy( <LNAME>, input, size*sizeof(byte) );
  } 
} 
