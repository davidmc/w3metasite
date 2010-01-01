/* 
  lookup.hpp
  
  ----------  
  One function call to lookup data in a table 
  
*/

#ifndef LOOKUP_HPP
#define LOOKUP_HPP 

#include "read_base.hpp"
/*
  Used to lookup a value based on an id in a lookup table
*/
string tableLookup ( string sql )
{
  string ret;
  quickQuery qqry;
  openRS & rs = qqry.getData(sql);
  if( qqry.opened )
  {
    ret = rs.getField(0).format();
    rs.close();
  }  
  return ret;
}
#endif

