/*
  siteLimit.h
  ------------
  copyright 2003 by David McCombs.
  davidmc@w3sys.com
  
*/

#ifndef Site_Limit_H
#define Site_Limit_H

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
string currentSite(void)
{
  string site;
  ocString domain = getenv("SERVER_NAME");
  ocString relpath = getenv("SCRIPT_NAME");

  // DBG  cout  << endl << endl <<  "domain: " <<    domain <<  " relpath: " <<  relpath << endl;
  
  string::size_type pos = relpath.find_last_of('/');
  relpath = relpath.substr(0,pos+1);
  quickQuery qqry;
  string getSql = "select id from sites where domain = '";
  getSql += domain + "' and url = '";
  getSql += relpath + "'";
  openRS & rs = qqry.getData(getSql);
  // DBG  cout  << endl << endl << getSql << " ... " << endl;
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

string siteFocus( void )
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

#endif
