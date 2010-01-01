#ifndef GLOBAL_CONTENT_H
#define GLOBAL_CONTENT_H

#include "read_write_base.hpp"


/*
  CREATE TABLE global_content (
    id bigint not null primary key auto_increment,
    site_id bigint NOT NULL,    -- site  
    name text NOT NULL,    -- name
    content text NOT NULL  -- content   
  );
*/
class global_content: public read_write_base
{
public:
  identifier id;
  long long site_id;
  string name;
  string content;
  
  global_content():read_write_base(),id(0LL),name("top")
  {
    // set name
    data_name("metasite.global_content");
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new llongXfer("site_id", &site_id ));    
    addDXMap( new stringXfer("name", &name ));
    addDXMap( new stringXfer("content", &content ));
  }
};

#endif
