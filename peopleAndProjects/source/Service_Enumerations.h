/*
  Service_Enumerations.h
  Prog_Enum id's for Status table
*/

#ifndef Service_Enumerations_h
#define Service_Enumerations_h

#include "ocString.h"
#include "lookup.hpp"


// put service enumerations here for permissions
enum Service_Enumeration{
  svcenm_none = -1,
  scvenm_project = 100,
  svcenm_task=500
};

string Service_Id( Service_Enumeration enm )
{
  ocString sql = "select Id from Services where Prog_Enum = ";
  sql.append(enm);
  return tableLookup (sql);
}

llong ll_Service_Id( Service_Enumeration enm )
{
  llong statId = 0;
  string stat_Id = Service_Id(enm);
  if( stat_Id.length() )
  {
    statId = atoll(stat_Id.c_str());
  }
  return statId;
}

string Service_Enum( llong id )
{
  ocString sql = "select Prog_Enum from Services where Id = ";
  sql.append(id);
  return tableLookup (sql);
}

#endif
