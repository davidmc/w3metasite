#include <iostream>
#include "connectstring.h"


using namespace std;


int main ( int ac, char ** av )
{
  int ret = 0;
  if( ac != 2 )
  {
    cout << "usage: encodeConnect \"connectstring\"" << endl;
    ret = -1;
  }
  else 
  {
    setConnectstring(av[1],"gtway");
  }
  return ret;
}
