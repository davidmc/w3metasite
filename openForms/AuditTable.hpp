/*
  AuditTable.hpp

  Object Definition and Implementation 
  for AuditTable.

  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/
#ifndef AuditTable_HPP
#define AuditTable_HPP

#include "read_write_base.hpp"
#include "lookup.hpp"

class AuditTable_Obj: public read_write_base
{
public:
  identifier Id;
  string aDatabase;
  string aTable;
  llong aRow;
  llong Actor;
  time_date Timestamp;
  int auditType;
  
  AuditTable_Obj():read_write_base()
  ,Id(0LL)
  ,aDatabase("")
  ,aTable("")
  ,aRow(0LL)
  ,Actor(0LL)
  ,Timestamp( )
  ,auditType(0)
  {  
    // set name
    data_name("w3audit.AuditTable");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new stringXfer("aDatabase", &aDatabase ));
    addDXMap( new stringXfer("aTable", &aTable ));
    addDXMap( new llongXfer("aRow", &aRow ));
    addDXMap( new llongXfer("Actor", &Actor ));
    addDXMap( new time_dateXfer("Timestamp", &Timestamp ));
    addDXMap( new intXfer("auditType", &auditType ));
  }

  string AuditType( void )
  {
   string ret = "unknown";

    switch(auditType)
    {
      case 1:
      ret = "insert";
      break;
      case 2:
      ret = "update";
      break;
      case 3:
      ret = "delete";
      break;
    }
    return ret;
  }
  
  string Author( ocString sql="select concat( first,' ',last) from user where id = " )
  {
    string ret = "unknown";
    if( Actor )
    {
      sql.append( Actor );
      ret = tableLookup ( sql );
    }
    return ret;
  }
  
  bool uvalidate( changeMap & changes  ) { return false; }
  bool dvalidate( void ) { return false; }

};
#endif



