/*
  global_content_obj.hpp

  Object Definition and Implementation 
  for global_content.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef global_content_HPP
#define global_content_HPP

#include "read_write_base.hpp"

class global_content_Obj: public read_write_base
{
public:
  identifier id;
  llong site_id;
  string name;
  string content;
  global_content_Obj():read_write_base()
  ,id(0LL)
  ,site_id(0LL)
  ,name("")
  ,content("")
  {
    // set name
    data_name("global_content");
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new llongXfer("site_id", &site_id ));
    addDXMap( new stringXfer("name", &name ));
    addDXMap( new stringXfer("content", &content ));
  }
  /*
   bool ivalidate( void  ) 
  { 
    name = ocString(name).remove("|");
    content = htmlDecode( content );
    return true; 
  }
  bool uvalidate( changeMap & changes  ) 
  { 
    name = ocString(name).remove("|");
    content = htmlDecode( content );
    return true; 
  }
  
  bool ssupplemental( void )
  { 
    name = ocString(name).remove("|");
    content = htmlFixup( content );
    return true; 
  }  

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



