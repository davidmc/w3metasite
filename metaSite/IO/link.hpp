/* 
  link.hpp

  Object Definition and Implementation 
  for link.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

  TODO:  This needs to be reconciled with the link.hpp class 
  in the webconsole source, had to create for expediancy to get 
  I/O routine working.
*/  
 
#ifndef links_HPP
#define links_HPP

#include "read_write_base.hpp"

class links_Obj: public read_write_base
{
public:
  identifier id;
  llong site_id;
  string name;
  string url;
  string target;
  
  links_Obj():read_write_base()
  ,id(0LL)
  ,site_id(0LL)
  ,name("")
  ,url("")
  ,target("")
  {  
    // set name
    data_name("links");
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new llongXfer("site_id", &site_id ));
    addDXMap( new stringXfer("name", &name ));
    addDXMap( new stringXfer("url", &url ));
    addDXMap( new stringXfer("target", &target ));
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

