/*
  module_basis.cpp
  ===================================
  CGI for now      
*/
#define USE_CONSOLE_DB
#include "../../webconsole/sysadmin_source/openLogin.h" // open-login has own connectstring
#include "cgiTemplates.h"
#include "cgiExtend.h"

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
  return 100;
}

string label( void )
{
  return "Projects";
}

void presentation( ostream & client )
{
  cgiEnvironment envir;  
  cgiTemplates tmplts;
  tmplts.load("./Templates/menu.htmp");
  openLogin login;  
 
  ocString expanded = tmplts.getParagraph("expanded"); 
  ocString collapsed = tmplts.getParagraph("collapsed");    
  ocString subitem = tmplts.getParagraph("subitem");
  ocString listitem = tmplts.getParagraph("listitem");
  // Contracted modules are expanded 
  //   if there is a related module id
  ocString thisOrdinal;
  string expandedOrdinal;
  string link="menu.cgi";  
  thisOrdinal.append(ordinal());
  
  cgiInput & clientSent = envir.ClientArguments();
  if( clientSent.count("module") ) 
  {
    expandedOrdinal = clientSent["module"].c_str();
  }

  if( thisOrdinal == expandedOrdinal )  
  {   
    
    client << expanded.replace( "$location", link.c_str() )
                      .replace( "$label", label().c_str() ) << endl;
    if( login.testLoginStatus() && 
        login.fetchServices( " s.tree_depth >= 2 && s.xml_params like '<actor>pmanage</actor>%'" ) )
    {
      // Add all of the services to the menu
      serviceMap & services = login.Services();
      serviceVector & names = login.ServiceNames();  // for proper order
      serviceMap::iterator pos;
      int st;
      for(st=0; st<names.size();++st  )
      {
        pos = services.find(names[st]);
        if(pos!=services.end())
        {      
          
          openService & service = pos->second;
          ocString & tmpl = service.tree_depth=="2"?subitem:listitem;
          client << tmpl
                    .replace("$location",service.uri.c_str())
                    .replace("$label",service.menu_name.c_str()) << endl;
        }
      }
    }
  }
  else
  {
    link+="?module=";
    link+=thisOrdinal;
    client << collapsed.replace( "$location", link.c_str() )
                       .replace( "$label", label().c_str() ) << endl;
  }
}

