/*
  group_sites.hpp
  base classes:
  read_write_base
  Group Sites Business Object    
*/

#ifndef PD_GROUP_SITES
#define PD_GROUP_SITES
#include "read_write_base.hpp"

/*

 group_sites Business Object
 CREATE TABLE group_sites (
  id bigint not null primary key auto_increment,
  group_id bigint NOT NULL,  -- group 
  site_id bigint NOT NULL    -- site   
);

 
*/  
                 
class group_sites:  public read_write_base
{
public:
  identifier id;
  long long group_id;
  long long site_id;  
  
  
  // Constructor
  group_sites()
  :read_write_base()
  ,id(0LL),group_id(0LL),site_id(0LL)
  {    
    data_name("group_sites");       
    // ALWAYS make the key be first, so we can skip it in insert logic
    addDXMap( new llongXfer("id", &id) );
    addDXMap( new llongXfer("group_id", &group_id) );        
    addDXMap( new llongXfer("site_id", &site_id) );          
  }
  virtual ~group_sites(){;}  
};
#endif


 
