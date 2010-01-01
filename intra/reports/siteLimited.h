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
  return "";
}

string siteLimits( openLogin & login )
{ 
  return "";  
}



// Completely different implementation 
// figures out which site to work on based on from where it's called.
string currentSite( void )
{
  string site;    
  char * envptr = NULL;
  ocString domain;  
  ocString relpath;
  
  envptr = getenv("SERVER_NAME");
  if(envptr) domain = envptr;
  envptr = getenv("SCRIPT_NAME");    
  if(envptr) relpath = envptr;
  
  relpath.parse("filePick");  
  quickQuery qqry;
  string getSql = "select id from metasite.sites where domain = '";
  getSql += domain + "' and url = '";
  getSql += relpath + "'";
  openRS & rs = qqry.getData(getSql);
  if( qqry.opened )
  {
    site = rs.getField(0).format();
  }
  else
  {
    cout << "Error finding site - " << getSql << endl;
  }     
 
  return site;
}

string siteFocus( openLogin & login )
{


  string site_id = currentSite();
  string sql;
  // check for cookie
  if( site_id.length() )
  {
    sql = " = "; 
    sql +=  site_id;
  }  
  return sql;  
}

