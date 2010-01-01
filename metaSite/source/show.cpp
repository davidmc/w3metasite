/*
 
  show.cpp    
  main file for show.meta
  
*/
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
#include "connectstring"
#include "cgiClass.h"
// one global
string baseUrl;
#include "page.hpp"
#include "show_control.hpp"

using namespace std;

string buildSlideMatchString( string & control )
{ 
  string matches;  
  // get the slide and detail nodes, get the items, and item atributes
  xmlParser ctrlParse(control);
  ctrlParse.parse();
  string detail="detail",slide="slides";
  node_vector & xnodes = ctrlParse.nodeList();
  node_map & nodekeys = ctrlParse.states.nodemap;
  node_map::iterator mnx = nodekeys.lower_bound(detail);
  while( mnx!=nodekeys.upper_bound(detail) )
  {
    size_t key = mnx->second;    
    xmlNode & dtlNode = xnodes[key];
    node_attr::iterator x = dtlNode.attr.find("item");
    // backward compat...
    if( x==dtlNode.attr.end() ) x = dtlNode.attr.find("items");
    if( x!=dtlNode.attr.end() )
    {
      matches=x->second;
    }
    ++mnx;
  }
  mnx = nodekeys.lower_bound(slide);
  while( mnx!=nodekeys.upper_bound(slide) )
  {  
    size_t key = mnx->second; 
    xmlNode & sldNode = xnodes[key];
    node_attr::iterator x = sldNode.attr.find("items");
    if( x!=sldNode.attr.end() )
    {
      matches+=",";
      matches+=x->second;
    }
    ++mnx;
  }
  return matches;
}
// Standard main function
int main(int argc, char ** argv)
{  
  baseUrl = "show.meta";  // this is my compiled name!  
  cgiScript script;  
  show_page pg(script);

  if( pg.load() )
  { 
    page_control ctrl(pg);    
    ctrl.addOp ( "detail",  new detail_functor(pg,script) );    
    ctrl.addOp ( "slides",  new slides_functor(pg,script) );
    // Get the control string
    string control = pg.loaded_control_string();
    // Create a match string
    string matches = buildSlideMatchString(control);
    // load the valid slide and detail paragraph selectors
    pg.loadSlides( matches );
    ctrl.emit (); 
  }
  else if( script.ClientArguments().count("DBG") )
  {
    script << "<html><head><title>Error</title></head>" << endl    
           << "<body><h1>Error</h1><p>Unable to load the requested page<br>" << endl;
           
    script << "</p></body></html>";
  }
  else
  {
    script << "<html><head><title>Page Unavailable</title></head>" << endl
           << "<body><h1>Sorry</h1>" << endl
           << "<p>The page you requested is not available</p></body></html>";
  }  
 
  return 0;
}
