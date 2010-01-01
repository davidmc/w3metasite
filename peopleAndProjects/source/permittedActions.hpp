/*

  permittedActions.hpp

  Defines a set of permitted actions
  based on XML from service

  Ex:
  
  <Task_Order>edit</Task_Order>
  <Name>edit</Name>
  <Status>edit</Status>
  <Percent_Complete>edit</Percent_Complete>
  <Detail>show</Detail>
  <Time>enter</Time>


*/

#ifndef permittedActions_HPP
#define permittedActions_HPP
#include <map>
#include "ocXML.h"
#include "openLogin.h"
#include "Service_Enumerations.h"

typedef map <string, string> permitted;


class permittedActions
{
protected:

  string m_XML;
  permitted m_Permissions;
public:
  // constructor
  permittedActions():m_XML("")
  {
    ;
  }
  // destructor
  virtual ~permittedActions(){;}
  // copy constructor 
  permittedActions( const permittedActions & in):
  m_XML(in.m_XML),
  m_Permissions(in.m_Permissions)
  {
    ;
  }

  bool May( string key, string val )
  {
     bool ret = false;
     permitted::iterator pit = m_Permissions.find(key);
     if( pit != m_Permissions.end() )
     {
       if( pit->second == val )
         ret = true;
     }
     return ret;
  }
  // assignment operator
  permittedActions & operator = ( const permittedActions & in )
  {
    m_XML = in.m_XML;
    m_Permissions = in.m_Permissions;
    return *this;
  }

  // load by enumeration
  bool loadByEnum( Service_Enumeration enm, serviceMap sMap )
  {
     serviceMap :: iterator itx = sMap.begin();
     while( itx != sMap.end() )
     {
       // DBG       cout << "<br> PROG ENUM:" << itx->second.prog_enum << "<br>" << endl;
       if( atol(itx->second.prog_enum.c_str()) == enm )
       {
         // DBG         cout << "<br> PARAMS PASSED:" << itx->second.xml_params << "<br>" << endl;
         XML( itx->second.xml_params );
         break;
       }
       ++itx;
     }
  }
  
  // member access operators  

  
  // get
  string & XML( void ) { return m_XML; }
  // set
  void XML( const string & in )
  {
    m_XML = in;
    xmlParser prsr(m_XML);
    prsr.parse();
    node_vector & xnodes = prsr.nodeList();
    for(int i=0;i<xnodes.size();i++)
    {
      xmlNode & node = xnodes[i];
      m_Permissions.insert(make_pair(node.name,node.data));
    }
  }
  
    // get
  permitted & Permissions( void ) { return m_Permissions; }
  // set
  void Permissions( const permitted & in ) { m_Permissions = in; }

};



#endif

