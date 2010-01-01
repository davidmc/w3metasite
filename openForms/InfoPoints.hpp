/*
  InfoPoints.hpp
  part Of openForms base code for sign in and connectivity configuration
  ------------------
  (C) W3systems 2007
  David McCombs  davidmc@w3sys.com
*/
#include "ocFileSys.h"
#include "ocXML.h"
class infoPoints {
public:
  string idToken;
  string authPoint;
  string cmsPoint;
  string appPoint;
  infoPoints(string accessPointsFileName = "accessPoints.xml")
  {
     // see if we have an accessPoints.xml file
     ocFileSys ocFS;
     if( ocFS.is( accessPointsFileName ) )
     {
       ifstream  accessPointsFile( accessPointsFileName.c_str() );
       if( accessPointsFile ) 
       {
         xmlParser parser( &accessPointsFile );
         parser.parse();
         node_map::iterator it;
         it = parser.states.nodemap.find("idToken");
         if( it != parser.states.nodemap.end() )
         { // Allows totally different sign-in mechanisms across different directory paths
           idToken = parser.states.nodes[it->second].data;
         }
         it = parser.states.nodemap.find("authPoint");
         if( it != parser.states.nodemap.end() )
         { // id, name ordered SQL for the list of authors
           authPoint = parser.states.nodes[it->second].data;
         }
         it = parser.states.nodemap.find("cmsPoint");
         if( it != parser.states.nodemap.end() )
         {
           cmsPoint = parser.states.nodes[it->second].data;
         }
         it = parser.states.nodemap.find("appPoint");
         if( it != parser.states.nodemap.end() )
         { // Starting path (or absolute URL) of the application
           appPoint = parser.states.nodes[it->second].data;
         }
       }
     }
  }
};
