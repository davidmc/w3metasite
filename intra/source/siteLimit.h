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
string currentSite( char * path="filePick" )
{
  string site;
  ocString domain = getenv("SERVER_NAME");
  ocString relpath = getenv("SCRIPT_NAME");
  string::size_type pos = relpath.find_last_of('/');
  relpath = relpath.substr(0,pos+1);
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
    cout << endl << endl << "Error finding site - " << getSql << endl;
  }
  return site;
}

string siteFocus( openLogin & login,  char * path="filePick" )
{


  string site_id = currentSite(path);
  string sql;
  // check for cookie
  if( site_id.length() )
  {
    sql = " = "; 
    sql +=  site_id;
  }  
  return sql;  
}

