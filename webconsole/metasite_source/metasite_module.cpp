/*
  metasite_module.cpp
  ===================================
  Dynamic Linking Module for Metasite
    
*/
#include "connectstring"
#include "openLogin.h"
#include "cgiTemplates.h"
#include "cgiExtend.h"
#include "siteLimit.h"

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
  return 2;
}

string label( void )
{
  return "Meta-Site-Admin";
}

void showSites( ostream & client, cgiTemplates & tmplts, openLogin & login  )
{
  quickQuery QQ;
  openRS & rs =  QQ.getRS();
  string siteLimitation = siteLimits(login);
  string sql = "select id, name from metasite.sites";
  if( siteLimitation.length() )
  {
    sql += " where id ";
    sql += siteLimitation;
  }  
  sql += " order by name";
  client << "<!--To get sites:  " << sql << "-->" << endl;
  for( bool b = rs.open(sql);
       b;
       b= rs.next() )
  {
    ocString loco = "site_ui.cgi?id=";
    loco += rs.getField(0).format();
    client << ocString(tmplts.getParagraph("listitem"))
    .replace( "$location",  loco.c_str())
    .replace( "$label", rs.getField(1).format().c_str() ) << endl;
  }
}

void showTemplates( ostream & client, cgiTemplates & tmplts, openLogin & login  )
{
  quickQuery QQ;
  openRS & rs =  QQ.getRS();
  string siteLimitation = siteFocus(login);
  string sql = "select id, name from metasite.templates";
  if( siteLimitation.length() )
  {
    sql += " where site_id ";
    sql += siteLimitation;
  } 
  sql += " order by name";
  client << "<!--To get templates:  " << sql << "-->" << endl;
  for( bool b = rs.open(sql);
       b;
       b= rs.next() )
  {
    ocString loco = "template_ui.cgi?id=";
    loco += rs.getField(0).format();
    client << ocString(tmplts.getParagraph("listitem"))
    .replace( "$location",  loco.c_str())
    .replace( "$label", rs.getField(1).format().c_str() ) << endl;
  }
}
void showMenus( ostream & client, cgiTemplates & tmplts, openLogin & login  )
{
  quickQuery QQ;
  openRS & rs =  QQ.getRS();
  string siteLimitation = siteFocus(login);
  string sql = "select id, name from metasite.menus";
  if( siteLimitation.length() )
  {
    sql += " where site_id ";
    sql += siteLimitation;
  } 
  sql += " order by name";
  client << "<!--To get menus:  " << sql << "-->" << endl;
  for( bool b = rs.open(sql);
       b;
       b= rs.next() )
  {
    ocString loco = "menu_ui.cgi?id=";
    loco += rs.getField(0).format();
    client << ocString(tmplts.getParagraph("listitem"))
    .replace( "$location",  loco.c_str())
    .replace( "$label", rs.getField(1).format().c_str() ) << endl;
  }
}
void showPages( ostream & client, cgiTemplates & tmplts, openLogin & login  )
{
  quickQuery QQ;
  openRS & rs =  QQ.getRS();
  string siteLimitation = siteFocus(login);
  string sql = "select id, name from metasite.pages";
  if( siteLimitation.length() )
  {
    sql += " where site_id ";
    sql += siteLimitation;
  } 
  sql += " order by name";
  client << "<!--To get pages:  " << sql << "-->" << endl;
  for( bool b = rs.open(sql);
       b;
       b= rs.next() )
  {
    ocString loco = "page_ui.cgi?id=";
    loco += rs.getField(0).format();
    client << ocString(tmplts.getParagraph("listitem"))
    .replace( "$location",  loco.c_str())
    .replace( "$label", rs.getField(1).format().c_str() ) << endl;
  }
}
void runService( ostream & client, string serviceName, cgiTemplates & tmplts, openLogin & login  )
{
      
  if( serviceName == "Meta-Site Sites" )
  {
    
    showSites( client, tmplts, login );
  }  
  else if ( serviceName == "Meta-Site Templates" )
  {
    showTemplates( client, tmplts, login );
  }  
  else if ( serviceName == "Meta-Site Menus" )
  {
    showMenus( client, tmplts, login  );
  }
  
  else if ( serviceName == "Meta-Site Pages" )
  {
    showPages( client, tmplts, login  );
  }
}

void presentation( ostream & client )
{
  cgiEnvironment envir;  
  cgiTemplates tmplts;
  tmplts.load("./Templates/menu.htmp");
  openLogin login(false);  
 
  ocString expanded = tmplts.getParagraph("expanded"); 
  ocString collapsed = tmplts.getParagraph("collapsed");    
  ocString subitem = tmplts.getParagraph("subitem");
  
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
        login.fetchServices( " s.tree_depth >= 2 && s.xml_params like '<actor>site author</actor>%'" ) )
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
          client << subitem
                    .replace("$location",service.uri.c_str())
                    .replace("$label",service.menu_name.c_str()) << endl;
          runService( client, service.service_name, tmplts, login );          
        }
      }
    }
    else
    {
      client << login.getLastError() << endl;
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
#include "read_write_base.cpp"

