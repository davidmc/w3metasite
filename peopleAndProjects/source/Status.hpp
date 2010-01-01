/* 
  Status.hpp

  Object Definition and Implementation 
  for Status.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Status_HPP
#define Status_HPP

#include "read_write_base.hpp"

class Status_Obj: public read_write_base
{
public:
  identifier Id;
  long Sequence;
  string Class;
  string Name;
  long Prog_Enum;
  
  Status_Obj():read_write_base()
  ,Id(0LL)
  ,Sequence(0L)
  ,Class("")
  ,Name("")
  ,Prog_Enum(0L)
  {  
    // set name
    data_name("Status");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new longXfer("Sequence", &Sequence ));
    addDXMap( new stringXfer("Class", &Class ));
    addDXMap( new stringXfer("Name", &Name ));
    addDXMap( new longXfer("Prog_Enum", &Prog_Enum ));
  }
  
  /*
  TODO: Add Event hooks for business logic
  =========================
  // Done before any action - a false return prevents action
  virtual bool ivalidate( void  ) { return true; }
  virtual bool uvalidate( changeMap & changes  ) { return true; }
  virtual bool dvalidate( void ) { return true; }
  
  // Done after any action - a false return sets a warning
  virtual bool isupplemental( void ){ return true; }
  virtual bool usupplemental( changeMap & changes ){ return true; }
  virtual bool dsupplemental( void ){ return true; }
  virtual bool ssupplemental( void ){ return true; } 
  */
};
#endif



