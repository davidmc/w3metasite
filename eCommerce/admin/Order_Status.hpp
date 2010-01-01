/* 
  Order_Status.hpp

  Object Definition and Implementation 
  for Order_Status.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Order_Status_HPP
#define Order_Status_HPP

#include "read_write_base.hpp"
class Order_Status_Obj: public read_write_base
{
public:
  identifier Id;
  string Name;
  
  Order_Status_Obj():read_write_base()
  ,Id(0LL)
  ,Name("")
  {  
    // set name
    data_name("Order_Status");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new stringXfer("Name", &Name ));
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

