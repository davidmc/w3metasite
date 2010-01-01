/*
  siteLimit.h
  ------------
  copyright 2003 by David McCombs.
  davidmc@w3sys.com
      
*/
#include "connectstring"
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
  string sql = " = ";  
  
  // establish cookies
  cgiCookie cookie;
  // cookies valid for entire site
  cookie.setPath("/"); 
  // check for cookie
  sql +=  cookie.get("site_id");
    
  return sql;  
}
string currentSite( cgiInput & args )
{
  string site;
  
  // establish cookies
  cgiCookie cookie;
  // cookies valid for entire site
  cookie.setPath("/"); 
  // check for cookie
  site =  cookie.get("site_id");
  if( args.count("changeSite") && args.count("site_id") )
  { 
    site = args["site_id"].c_str();
    cookie.set( "site_id", site.c_str() );    
  }  
  return site;
}

string templateFromId( string template_id )
{
  string ret;
  quickQuery qqry;  
  string getSql = "select path from templates where id = ";
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
  string getSql = "select template_id from pages where id = ";  
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
