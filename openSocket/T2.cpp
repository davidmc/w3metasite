/*
  NEW TEST HARNESS T2.cpp    
  =======================
  The compile is easy:
  g++ T2.cpp -o T2
     
*/
#include <iostream>
#include <iomanip>

// STEP 1). 
// Initial include of all headers under test
// needed to define classes

#include "oc_SMTP.h"


// Forces inclusion of test code
#define IN_T2_TESTHARNESS

using namespace std;

int main( int argc, char ** argv )
{
// STEP 2).
// Second include of files under test for actual test code.

#include "oc_SMTP.h"

return 0;
}

// STEP 3).
// include .cpp files, if any here

