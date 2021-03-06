/*
  module_basis.cpp
  ===================================
  CGI for now  
    
*/
#include "connectstring"
#include "cgiExtend.h"
#include "cgiTemplates.h"
#include "openLogin.h"

// don't mangle me!
extern "C"
{
  int ordinal( void );
  string label( void );
  void presentation( ostream & client );
}
using namespace std;

int ordinal( void )
{
  return 0;
}

string label ( void )
{
  return "All";
}

void presentation( ostream & client )
{
  cgiEnvironment envir; 
  openLogin login(false);
  string menuLabel;
  if( login.testLoginStatus() ) menuLabel = "Sign Off";
  else menuLabel =  "Sign On";     
  cgiTemplates tmplts;
  tmplts.load("./Templates/menu.htmp");  
  ocString tmplt = tmplts.getParagraph("listitem");
  cout << tmplt.replace("$location", "signIn.html").replace("$label",menuLabel.c_str()) << endl;      
}

#include "read_write_base.cpp"