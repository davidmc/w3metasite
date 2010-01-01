/*

  Reports.cpp

  User Interface Definition and Implementation 
  for a Report Run

  Also - See report listing for similar list 
         functionality in a metasite intranet app.

  Copyright (C) 2005 - 2007 by D.K. McCombs.
  W3 Systems Design www.w3sys.com
*/

#include <string>
using namespace std;
// added to resolve compatibility issues where this may be a symbolic link to another project
#include "compatible.hpp"

// interface class (list) 
#define NO_INSERT_BUTTON  
#define NO_DELETE_BUTTON
// #define  SESSIONLESS_LOGIN

#include "connectstring"
#include "w3intranet.h"
#include "Reports.hpp"
// Set static value
bool Report_Parameters_form::hasParamValues = false;

/**
  the MAIN function
**/
int main( int argcount, char ** cl_args )
{
  ReportEmitter emitter;
  emitter.run();
}

// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"

