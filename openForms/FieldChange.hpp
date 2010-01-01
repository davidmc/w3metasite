/*
  FieldChange.hpp

  Object Definition and Implementation 
  for FieldChange.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef FieldChange_HPP
#define FieldChange_HPP

#include "read_write_base.hpp"

class FieldChange_Obj: public read_write_base
{
public:
  identifier Id;
  llong auditTable;
  string Name;
  string Value;
  
  FieldChange_Obj():read_write_base()
  ,Id(0LL)
  ,auditTable(0LL)
  ,Name("")
  ,Value("")
  {  
    // set name
    data_name("w3audit.FieldChange");
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



