#include "cgiExtend.h"
#include "cgiCookie.h"
#include "cgiTemplates.h"

using namespace std;
#define standard_cgi

// function declarations
bool templateTests(cgiScript & script);
bool cgiTests(cgiScript & script);
bool cookieTesting ( void );

// Standard main function
int main(int argc, char ** argv)
{
  cgiScript script("text/html",true,"uploads");
  // templateTests( script );
  cgiTests(script);

  return 0;
}

bool cgiTests(cgiScript & script)
{
  // create a new html instance in its own scope
  cgiHtml html;
  {
    // create a new head tag
    cgiHead head;
    {
      cgiCan title("title");
      title << "testHarness";
    }
    {
      cgiCan style("style");
      style << ".odd{ color: darkblue; background: linen;}\n"
            << ".even{ color: linen; background: darkblue;}\n"
            << ".cookie{ color: green; background: tan; }\n";
    }

  }
  {
    // create a body tag
    cgiBody body;
    script << "<h1 style=\"color:red;\">What is this environment?</h1>" << endl;
    script << "<h2>Extra: </h2>" <<   endl;
    
    script << getenv("DOCUMENT_ROOT") << "<br>" << endl;
    
    script << "<h2>Expected: </h2>" <<   endl;
    script.DebugString();
  } // end of body
  return true;
}
bool templateTests(cgiScript & script)
{
  cgiTemplates content;
  string path=script.ClientArguments()["template"].c_str();
  if( content.load( path.c_str() ) != true )
  {
    script << "Failed to load content! " << path << endl;
    return false;
  }
  paragraphMap & tmap = content.getParagraphs();
  paragraphMap::iterator pos;
  for ( pos=tmap.begin(); pos != tmap.end(); ++pos )
  {
    script << "<br><br><h1 style=\"color:red;\">" << pos->first << "</h1>" << pos->second << "<br>" << endl;
  }
  script.DebugString();
  return true;
}


bool cookieTesting ( void )
{
  // testing cookies
  // cookie sets must be after script but befor any other output
  cgiCookie cookie;
  cookie.setTimeout( 12, 31, 2002, 12, 0 );
  cookie.setPath( "/" );
  cookie.set("Oid","1234");
  return true;
}

