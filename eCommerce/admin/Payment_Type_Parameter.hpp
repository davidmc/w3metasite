/* 
  Payment_Type_Parameter.hpp

  Object Definition and Implementation 
  for Payment_Type_Parameter.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Payment_Type_Parameter_HPP
#define Payment_Type_Parameter_HPP

#include "read_write_base.hpp"
#include "paymentEnumerations.h"

class Payment_Type_Parameter_Obj: public read_write_base
{
public:
  identifier Id;
  llong Service_Type;
  long Value_Location;
  long Param_Type;
  long Interface_Type;
  string Name;
  string Machine_Name;
  string Value;
  string Instructions;
  
  Payment_Type_Parameter_Obj():read_write_base()
  ,Id(0LL)
  ,Service_Type(0LL)
  ,Value_Location(0L)
  ,Param_Type(0L)
  ,Interface_Type(0L)
  ,Name("")
  ,Machine_Name("")
  ,Value("")
  ,Instructions("")
  {  
    // set name
    data_name("Payment_Type_Parameter");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new llongXfer("Service_Type", &Service_Type ));
    addDXMap( new longXfer("Value_Location", &Value_Location ));
    addDXMap( new longXfer("Param_Type", &Param_Type ));
    addDXMap( new longXfer("Interface_Type", &Interface_Type ));
    addDXMap( new stringXfer("Name", &Name ));
    addDXMap( new stringXfer("Machine_Name", &Machine_Name ));
    addDXMap( new stringXfer("Value", &Value ));
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

