/*
  customSQL.hpp

  --
  This is for finding sql from a configuration file
  uses ocXML



*/
#include <map>
#include <string>
#include <fstream>
#include <iomanip>
#include "ocXML.h"

ocString getSQL( string tag, string configFile )
{
  string qry;
  ifstream templateFile( configFile.c_str() );
  xmlParser parser( &templateFile );
  parser.parse();
  node_vector::iterator & it = parser.findFirstNodeByName( tag );
  if( it !=  parser.nodeList().end() )
  {
    xmlNode & node = dynamic_cast<xmlNode &>(*it);
    qry = node.data;
  }
  return qry;
}
