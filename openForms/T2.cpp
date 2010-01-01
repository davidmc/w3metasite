/*

  HARNESS T2.cpp
  =======================
  The compile is easy:
  it is done in the makefile
  just type make

*/

#include <iostream>
#include <iomanip>

// STEP 1). 
// Initial include of all headers under test
// needed to define classes

//#include "ocTypes.h"
//#include "ocString.h"
//#include "cgiClass.h"


//#include "connectstring.h"
//const char * CONNECTSTRING = getConnectstring();
//#define PROVIDER MySQL

#include <string>
#include <map>
using namespace std;

//#include "forms.h"
#include "anyData.hpp"

#define IN_T2_TESTHARNESS

using namespace std;

int main( int argc, char ** argv )
{
//  cgiScript script;
// STEP 2).
// Second include of files under test for actual test code.

#include "anyData.hpp"

//#include "forms.h"

return 0;

}
#undef IN_T2_TESTHARNESS






