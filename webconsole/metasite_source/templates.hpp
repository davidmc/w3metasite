#ifndef TEMPLATES_HPP
#define TEMPLATES_HPP

#include "read_write_base.hpp"
/*
  templates.hpp
  
  CREATE TABLE templates 
  (
    id bigint not null primary key auto_increment,
    site_id bigint NOT NULL,  -- link to site
    name text,       -- name (for menu item)
    path text        -- path to file (usually just the filename)
  );

*/
using namespace std;
class Template: public read_write_base
{
public:
  identifier id;
  long long site_id;  
  string name;
  string path;
  
  Template():read_write_base(),id(0LL)
  {  
    // set name
    data_name("metasite.templates");
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new llongXfer("site_id", &site_id ));    
    addDXMap( new stringXfer("name", &name ));
    addDXMap( new stringXfer("path", &path ));
  }
};

#endif
