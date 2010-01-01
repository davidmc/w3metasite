/*
  Deletion.hpp

  Object Definition and Implementation 
  for Deletion.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Deletion_HPP
#define Deletion_HPP

#include "read_write_base.hpp"

class Deletion_Obj: public read_write_base
{
public:
  identifier Id;
  llong auditTable;
  string Name;
  string Value;
  
  Deletion_Obj():read_write_base()
  ,Id(0LL)
  ,auditTable(0LL)
  ,Name("")
  ,Value("")
  {  
    // set name
    data_name("w3audit.Deletion");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new llongXfer("auditTable", &auditTable ));
    addDXMap( new stringXfer("Name", &Name ));
    addDXMap( new stringXfer("Value", &Value ));
  }
  bool uvalidate( changeMap & changes  ) { return false; }
  bool dvalidate( void ) { return false; }

};
#endif



