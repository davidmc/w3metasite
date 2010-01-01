/* 
  Payment_Service.hpp

  Object Definition and Implementation 
  for Payment_Service.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Payment_Service_HPP
#define Payment_Service_HPP

#include "read_write_base.hpp"
class Payment_Service_Obj: public read_write_base
{
public:
  identifier Id;
  llong Type;
  string Name;
  string Label;
  string Instructions;
  
  Payment_Service_Obj():read_write_base()
  ,Id(0LL)
  ,Type(0LL)
  ,Name("")
  ,Label("")
  ,Instructions("")
  {  
    // set name
    data_name("Payment_Service");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new llongXfer("Type", &Type ));
    addDXMap( new stringXfer("Name", &Name ));
    addDXMap( new stringXfer("Label", &Label ));
    addDXMap( new stringXfer("Instructions", &Instructions ));
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

