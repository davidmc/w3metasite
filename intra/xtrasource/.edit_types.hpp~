/*
  edit_types.hpp

  Object Definition and Implementation 
  for edit_types.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef edit_types_HPP
#define edit_types_HPP

#include "read_write_base.hpp"

class edit_types_Obj: public read_write_base
{
public:
  identifier id;
  string name;
  string handler;
  int sequence;
  edit_types_Obj():read_write_base()
  ,id(0LL)
  ,name("")
  ,handler("")
  ,sequence(0)
  {
    // set name
    data_name("edit_types");
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new stringXfer("name", &name ));
    addDXMap( new stringXfer("handler", &handler ));
    addDXMap( new intXfer("sequence", &sequence ));
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



