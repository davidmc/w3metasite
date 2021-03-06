/*
  NEW TEST HARNESS T2.cpp    
  =======================
  The compile is easy:
  g++ -g T2.cpp -o T2
     
*/
#include <iostream>
#include <iomanip>
#include <string>

// STEP 1). 
// Initial include of all headers under test
// needed to define classes

//#include "ocTypes.h"
//#include "ocString.h"
//#include "cgiExtend.h"
//#include "cgiClass.h"
#include "ocFileSys.h"
// #include "ocEnc.h"
// #include "ocCoder.h"
// #include "ocXML.h"
// Forces inclusion of test code
#define IN_T2_TESTHARNESS
                                              
using namespace std;

int main( int argc, char ** argv )
{
  
 ocFileSys ocFS;
 cout << "YOU ARE HERE:  " << ocFS.getCWD() << endl;
 if( argc == 2 )
 {
    ocFS.remove(argv[1]);
 }  
// STEP 2).
// Second include of files under test for actual test code.

// #include "ocTypes.h"
//#include "ocString.h"
//#include "cgiClass.h"
// #include "ocEnc.h"
// #include "ocFileSys.h"
// #include "ocXML.h"
  /*
  // special test of enc anomoly
  string Value="3123 Langley Drive Langley Drive",
         Machine_Name = "address1";
  int   Customer_Pmt_Service_Id = 15;
  int keyVal = Customer_Pmt_Service_Id;
  ocString toHash =  Machine_Name;
  keyVal += toHash.makeHash();
  ocEnc enc =  Value;
  cout << "Value: " << Value << endl;
  Value = enc.encode(keyVal);
  ocCoder coder;
  Value = coder.base64encode( (const unsigned char *) Value.c_str(),Value.length());
  
  cout << "Encoded Value: " << Value << endl;

  bins & binval = coder.base64decode(Value);
  enc = "";
  for(int i=0; i<binval.size(); ++i)
  {
    enc += (char) binval[i];
  }
  Value = enc.decode(keyVal);

  cout << "Decoded Value: " << Value << endl;
  */
  
return 0;
}

#undef IN_T2_TESTHARNESS

// STEP 3).
// include .cpp files, if any here
//#include "Astring.cpp"
//#include "cgiClass.cpp"
