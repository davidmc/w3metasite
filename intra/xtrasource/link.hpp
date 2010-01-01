/*
  link.hpp 
  
  hyperlink object abstraction

  Copyright (late) 2004  by D.K. McCombs.
  ======================================
  www.w3sys.com
    
*/
#ifndef link_hpp
#define link_hpp

#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "openLogger.h"

using namespace std;
/*
  links database table...
  
  CREATE TABLE links
  (
    id bigint not null primary key auto_increment,
    site_id bigint NOT NULL,
    name    text,
    url     text,
    target  text
  );
  
*/     
class link: public read_write_base
{
public:
  identifier id;
  long long site_id;
  string name;
  string url;
  string target;
  
  link():read_write_base(),id(0LL),site_id(0LL)
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
  bool dsupplemental( void )
  {
    ocString sql = "delete from menu_items where link_id = ";
    sql.append(id);
    cmd.execute(sql);
    return true;
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
