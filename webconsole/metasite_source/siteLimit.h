/*
  siteLimit.h
  ------------
  copyright 2003 by David McCombs.
  davidmc@w3sys.com
  
*/

#include "read_base.hpp"
#include "openLogin.h"

string siteIds( openLogin & login )
{
  string sql;
  if( login.SiteAdmin() == false )
  {
    sql += " ";
    quickQuery qqry;    
    string getSql =  "select site_id from metasite.group_sites where group_id = ";
    getSql += login.GroupId();
    openRS & rs = qqry.getData(getSql);
    bool isOpen;
    for( isOpen = qqry.opened;isOpen;isOpen=rs.next())
    {
      sql += rs.getField(0).format();
      sql += ", ";
    }
    // remove the last ", ";
    if( sql.length() > 2 )
    {
      sql.resize(sql.length()-2);
    }
    sql += " ";        
  }  
  return sql;
}

string siteLimits( openLogin & login )
{
  string sql;
  if( login.SiteAdmin() == false )
  {  
    string inList = siteIds( login );
    if( inList.length() )
    {
      sql += " in (";
      sql += inList;
      sql += ")";
    }  
  }  
  return sql;  
}

string siteFocus( openLogin & login )
{
  // establish cookies
  cgiCookie cookie;
  // cookies valid for entire site
  cookie.setPath("/");
  string site_id = cookie.get("site_id");
  string sql;
  // check for cookie
  if( site_id.length() )
  {
    sql = " = "; 
    sql +=  site_id;
  }  
  return sql;  
}

// Completely different implementation 
// figures out which site to work on based on from where it's called.
string currentSite( cgiInput & args )
{
  string site;
  // establish cookies
  cgiCookie cookie;
  // cookies valid for entire site
  cookie.setPath("/"); 
  // check for cookie
  site =  cookie.get("site_id");
  if( site.length() == 0 )
  {     
    cgiEnvironment cgiEnv;
    ocString domain = cgiEnv.ServerName().c_str();
    ocString relpath = cgiEnv.ScriptName().c_str(); 
    if( domain.length() )
    {
      relpath = relpath.parse("sys");
      quickQuery qqry;
      string getSql = "select id from metasite.sites where domain = '";
      getSql += domain + "' and url = '";
      getSql += relpath + "'";
      openRS & rs = qqry.getData(getSql);
      if( qqry.opened )
      {
      site = rs.getField(0).format();
      cookie.set( "site_id", site.c_str() ); 
      }
      else
      {
      cerr << "Error finding site - " << getSql << endl;
      }
    }
  }
  return site;
}

string templateFromId( string template_id )
{
  string ret;
  quickQuery qqry;  
  string getSql = "select path from metasite.templates where id = ";
  getSql += template_id;
  openRS & rs = qqry.getData(getSql);
  if( qqry.opened )
  {
    ret = rs.getField(0).format();
    rs.close();    
  }  
  return ret;
}

string templateFromPageId( string page_id )
{
  string ret;
  quickQuery qqry;
  string getSql = "select template_id from metasite.pages where id = ";  
  getSql += page_id;
  openRS & rs = qqry.getData(getSql);
  if( qqry.opened )
  {
    string template_id = rs.getField(0).format();
    rs.close();
    ret = templateFromId( template_id );
  }  
  return ret;
}
