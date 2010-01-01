/* 
  services_lite.hpp

  Object Definition and Implementation 
  for services.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef services_HPP
#define services_HPP

#include "read_write_base.hpp"

class services_Obj: public read_write_base
{
public:
  identifier id;
  int sequence;
  int tree_depth;
  bool enabled;
  string cat_name;
  string menu_name;
  string uri;
  string target;
  int prog_enum;
  
  services_Obj():read_write_base()
  ,id(0LL)
  ,sequence(0)
  ,tree_depth(0)
  ,enabled(false)
  ,cat_name("")
  ,menu_name("")
  ,uri("")
  ,target("")
  ,prog_enum(0)
  {  
    // set name
    data_name("services");
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new intXfer("sequence", &sequence ));
    addDXMap( new intXfer("tree_depth", &tree_depth ));
    addDXMap( new boolXfer("enabled", &enabled ));
    addDXMap( new stringXfer("cat_name", &cat_name ));
    addDXMap( new stringXfer("menu_name", &menu_name ));
    addDXMap( new stringXfer("uri", &uri ));
    addDXMap( new stringXfer("target", &target ));
    addDXMap( new intXfer("prog_enum", &prog_enum ));
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



