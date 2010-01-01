/* 
  anyData.hpp
  This file prevents the browser from monkeying with my form data
  It needs code on the client side to encode/decode in the inverse op.
  
  (c) David McCombs W3 Systems 2006 
  
*/
#ifndef ANY_Data_Hpp
#define ANY_Data_Hpp

#include <map>
typedef map<string, int> anyFormDataArray;

class hexEncoder
{
private:

public:
  hexEncoder()
  {
  }

  string w3HexEncode( string AsciiIn )
  {
    string HexOut = "";    
    for(int i=0;i<AsciiIn.length();i++)
    {
      char ch = AsciiIn.c_str()[i];
      char buf[24];
      sprintf( buf, "%02X", ch );
      HexOut += buf; 
    }
    return HexOut;
  }
  string w3hexDecode( string HexIn )
  {
    string AsciiOut = "";
    for( int i=0;i<HexIn.length();i+=2)
    {
      string hexVal = HexIn.substr(i,2);
      long lVal = strtol(hexVal.c_str(), 0, 16);
      AsciiOut += (char) lVal;
    }
    return AsciiOut;
  }
} ; 

#endif

#ifdef  IN_T2_TESTHARNESS

// test code here
  string test = "I have had enough!";
  hexEncoder enc;
  string encoded = enc.w3HexEncode(test);
  string decoded = enc.w3hexDecode( encoded );
  cout << "   test string: " << test << endl;
  cout << "encoded string: " << encoded << endl;
  cout << "decoded string: " << decoded << endl;
  

#endif
