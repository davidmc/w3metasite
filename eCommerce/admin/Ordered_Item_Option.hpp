/* 
  Ordered_Item_Option.hpp

  Object Definition and Implementation 
  for Ordered_Item_Option.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Ordered_Item_Option_HPP
#define Ordered_Item_Option_HPP

#include "read_write_base.hpp"

class Ordered_Item_Option_Obj: public read_write_base
{
public:
  identifier Id;
  llong Ordered_Item_Id;
  string Name;
  string Value;
  
  Ordered_Item_Option_Obj():read_write_base()
  ,Id(0LL)
  ,Ordered_Item_Id(0LL)
  ,Name("")
  ,Value("")
  {  
    // set name
    data_name("Ordered_Item_Option");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new llongXfer("Ordered_Item_Id", &Ordered_Item_Id ));
    addDXMap( new stringXfer("Name", &Name ));
    addDXMap( new stringXfer("Value", &Value ));
  }
  
  /*
  Event hooks for business logic
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

