/* 
  Query.hpp

  Object Definition and Implementation 
  for Query.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Query_HPP
#define Query_HPP

#include "read_write_base.hpp"

class Query_Obj: public read_write_base
{
public:
  identifier Id;
  llong Site_Id;
  string Name;
  string Query;
  
  Query_Obj():read_write_base()
  ,Id(0LL)
  ,Site_Id(0LL)
  ,Name("")
  ,Query("")
  {  
    // set name
    data_name("Query");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new llongXfer("Site_Id", &Site_Id ));
    addDXMap( new stringXfer("Name", &Name ));
    addDXMap( new stringXfer("Query", &Query ));
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

