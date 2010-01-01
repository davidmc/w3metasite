/*
  ocEnc.h
*/

#ifndef OC_INC_H
#define OC_INC_H

#include <cstdlib>
#include <string>

using namespace std;

// A very simple and straightforward string encoder/decoder
class ocEnc : public string  
{  

  int lmt;
  int mySecret;

public:

  ocEnc():string(),lmt(8),mySecret(129038){;}
  ocEnc( const char * in ) : string (in),lmt(8),mySecret(129038){;}
  ocEnc( const string & in ) : string (in),lmt(8),mySecret(129038){;}

  ocEnc & encode( int seed )
  {
    srand(seed+mySecret);        
    for( size_type i=0; i< length(); ++i )
    {
      operator[](i) = operator[](i) - (rand()%lmt);
    }
    return * this;
  }

  ocEnc & decode( int seed )
  {
    srand(seed+mySecret);    
    for( size_type i=0; i< length(); ++i )
    {
      operator[](i) = operator[](i) + (rand()%lmt);
    }
    return * this;
  }

  ocEnc & shake( int sec, int lmt=8 )
  {
    this->mySecret=sec;
    this->lmt=lmt;
    return *this;
  }

};

#endif

#ifdef IN_T2_TESTHARNESS
  string ary[3]= {
  "host=localhost;uid=GOOFYadmin;pwd=12gOOF;db=mig",
  "host=localhost;uid=plutoadmin;pwd=34pLUTO;db=boeing",
  "host=localhost;uid=dumbouser;pwd=mydeal99;db=airbus"
  };

  ocEnc encoder = "A-Z,0-9,!@#$%^&*()_+|\\=-`,a-z  /?'\";:.>,<my brother 123 is a BRAT 4567890! Zoo Breath is his name.";

  cout << "Fresh  : " << encoder << endl;
  encoder.encode(22);
  cout << "Encoded: " << encoder << endl;
  encoder.decode(22);
  cout << "Decoded: " << encoder << endl;
    
  for( int x=0;x<3;x++ ) 
  {
    cout << endl << "========" << endl << endl;
    encoder = ary[x];
    cout << "Fresh   : " << encoder << endl;
    encoder.encode(232);
    cout << "Encoded : " << encoder << endl;
    encoder.decode(232);
    cout << "Decoded : " << encoder << endl;
    
  }
  #ifdef OC_STRING_H
  ocString brash = "davidmc@w3sys.com";
  int val = brash.makeHash();
  cout << endl << "HASH = " <<  val << endl;  
  
  for( int x=0;x<3;x++ ) 
  {
    cout << endl << "HASH KEYED========" << endl << endl;
    encoder = ary[x];
    encoder.shake( val, 22 );
    cout << "Fresh   : " << encoder << endl;
    encoder.encode(232);
    cout << "Encoded : " << encoder << endl;
    encoder.decode(232);
    cout << "Decoded : " << encoder << endl;
    
  }
  
  #endif
  
#endif
