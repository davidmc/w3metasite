/*
  group_services.hpp
  base classes:
  read_write_base
  Group Services Business Object    
*/

#ifndef PD_GROUP_SERVICES
#define PD_GROUP_SERVICES
#include "read_write_base.hpp"

/*

 group_services Business Object
 
 CREATE TABLE group_services (
  id bigint not null primary key auto_increment,
  service_id     bigint NOT NULL,    -- 1,n relationship to services
  group_id       bigint NOT NULL,    -- 1,n relationship to groups
  sequence       smallint NOT NULL,  -- order of appearance in setup
  xml_param1     text,               -- two xml params for anything else
  xml_param2     text
  ); 
 
*/  
                 
class group_services:  public read_write_base
{
public:
  identifier id;
  long long service_id;
  long long group_id;
  int sequence;
  string xml_param1;
  string xml_param2;  
  
  // Constructor
  group_services()
  :read_write_base()
  ,id(0LL),service_id(0LL),group_id(0LL),sequence(0L)
  {    
    data_name("group_services");       
    // ALWAYS make the key be first, so we can skip it in insert logic
    addDXMap( new llongXfer("id", &id) );
    addDXMap( new llongXfer("service_id", &service_id) );
    addDXMap( new llongXfer("group_id", &group_id) );        
    addDXMap( new shortXfer("sequence", &sequence) );
    addDXMap( new stringXfer("xml_param1", &xml_param1) );
    addDXMap( new stringXfer("xml_param2", &xml_param2) );               
  }
  virtual ~group_services(){;}  
};
#endif


 
