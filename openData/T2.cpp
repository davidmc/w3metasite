/*
  NEW TEST HARNESS T2.cpp    
  =======================
  The compile is easy:
  For pgsql stuff
  g++ T2.cpp -lpq -o T2 -I../openCore
  For mysql stuff
  g++ T2.cpp -lmysqlclient -o T2 -I../openCore -I/usr/include/mysql/
     
*/
#include <iostream>
#include <iomanip>

// STEP 1). 
// Initial include of all headers under test
// needed to define classes

#include "connectstring.h"
#include "openCatalog.h"


// Forces inclusion of test code
#define IN_T2_TESTHARNESS

using namespace std;

int main( int argc, char ** argv )
{
// STEP 2).
// Second include of files under test for actual test code.

#include "openCatalog.h"

return 0;
}

#undef IN_T2_TESTHARNESS

// STEP 3).
// include .cpp files, if any here  
//  ../objects/fmtTime.o -o T2
#include "openFields.cpp"
#include "openDB.cpp"
#include "fmtTime.cpp"

//#include "openDBPG.cpp"
//#include "openPgSqlCatalog.cpp"

#include "openMyDB.cpp"
#include "openMySqlCatalog.cpp"

