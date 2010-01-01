/* 
  page_menus.hpp

  Object Definition and Implementation 
  for page_menus.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef page_menus_HPP
#define page_menus_HPP

#include "read_write_base.hpp"

class page_menus_Obj: public read_write_base
{
public:
  identifier id;
  llong site_id;
  llong menu_id;
  llong page_id;
  int place_order;
  bool collapsible;
  
  page_menus_Obj():read_write_base()
  ,id(0LL)
  ,site_id(0LL)
  ,menu_id(0LL)
  ,page_id(0LL)
  ,place_order(0)
  ,collapsible(false)
  {  
    // set name
    data_name("page_menus");
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new llongXfer("site_id", &site_id ));
    addDXMap( new llongXfer("menu_id", &menu_id ));
    addDXMap( new llongXfer("page_id", &page_id ));
    addDXMap( new intXfer("place_order", &place_order ));
    addDXMap( new boolXfer("collapsible", &collapsible ));
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

