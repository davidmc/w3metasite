/*
  Status_Enumerations.h
  Prog_Enum id's for Status table

  +----+-------------------+---------+----------+-----------+
| Id | Name              | Class   | Sequence | Prog_Enum |
+----+-------------------+---------+----------+-----------+
|  1 | Awaiting Approval | Project |        0 |         0 |
|  2 | High Level Design | Project |        1 |         0 |
|  3 | Planned           | Task    |        1 |        10 |
|  4 | In Progress       | Task    |        2 |        20 |
|  5 | Complete          | Task    |        2 |        30 |
|  6 | Developing        | Project |        3 |         0 |
|  7 | Invoiced          | Task    |        3 |        40 |
|  8 | Pre Invoice       | Time    |        0 |       110 |
|  9 | Invoiced          | Time    |        1 |       120 |
| 10 | Paid              | Time    |        2 |       130 |
| 11 | Prospect          | Company |        0 |         0 |
| 12 | Active            | Company |        1 |         0 |
| 13 | Paid              | Task    |        5 |        50 |
| 14 | Estimate          | Task    |        0 |         5 |
| 15 | Former            | Company |        0 |         0 |
| 16 | Complete          | Project |     2000 |         0 |
| 17 | No Charge         | Task    |        6 |        60 |
| 18 | No Charge         | Time    |        3 |       140 |
| 19 | Need to Estimate  | Task    |       -1 |         2 |
| 20 | Archived          | Project |     3000 |      3000 |
+----+-------------------+---------+----------+-----------+


*/

#ifndef Status_Enumerations_h
#define Status_Enumerations_h
#include "ocString.h"
#include "lookup.hpp"

enum Status_Enumeration 
{
  Undefined_Status = 0,
  Task_Need_To_Estimate = 2,
  Task_Estimate = 5,
  Task_Planned = 10,
  Task_In_Progress = 20,
  Task_Complete = 30,
  Task_Invoiced = 40,
  Task_Paid = 50,
  Task_No_Charge = 60,
  Time_Pre_Invoice = 110,
  Time_Invoiced = 120,
  Time_Paid  = 130,
  Time_No_Charge = 140,
};

string Status_Id( Status_Enumeration enm )
{
  ocString sql = "select Id from Status where Prog_Enum = ";
  sql.append(enm);
  return tableLookup (sql);
}

llong ll_Status_Id( Status_Enumeration enm )
{
  llong statId = 0;
  string stat_Id = Status_Id(enm);
  if( stat_Id.length() )
  {
    statId = atoll(stat_Id.c_str());
  }
  return statId;
}

string Status_Enum( llong id )
{
  ocString sql = "select Prog_Enum from Status where Id = ";
  sql.append(id);
  return tableLookup (sql);
}

long l_Status_Enum( llong id )
{
  long statEnum = 0;
  string strStatEnum =  Status_Enum( id );
  if(  strStatEnum.length() )
  {
    statEnum = atol(strStatEnum.c_str());
  }
  return statEnum;
}
#endif
