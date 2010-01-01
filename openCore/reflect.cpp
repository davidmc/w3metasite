#include "cgiClass.h"
using namespace std;
#define standard_cgi

// Standard main function
int main(int argc, char ** argv)
{
  cgiScript script;
  script << "<h2>Info Sent: </h2>" <<   endl;
  queryStringMap  varMap = script.ClientArguments().TheMap();
  queryStringMap::iterator it = varMap.begin();
  script << "<dl>" << endl;
  while( it != varMap.end() )
  {    
    script << "<dt>" << it->first << "</dt><dd>" << it->second << "</dd>" <<  endl;
    it++;
  }
  script << "</dl>" << endl;
  return 0;
}


