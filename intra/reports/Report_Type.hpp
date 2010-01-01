/* 
  Report_Type.hpp

  Object Definition and Implementation 
  for Report_Type.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Report_Type_HPP
#define Report_Type_HPP

#include "read_write_base.hpp"

class Report_Type_Obj: public read_write_base
{
public:
  identifier Id;
  string Name;
  string File_Extension;
    
  Report_Type_Obj():read_write_base()
  ,Id(0LL)
  ,Name("")
  ,File_Extension("")  
  {  
    // set name
    data_name("Report_Type");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new stringXfer("Name", &Name ));
    addDXMap( new stringXfer("File_Extension", &File_Extension ));    
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

