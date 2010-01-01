/*
  base classes:
  read_write_base
  Services Business Object    
*/

#ifndef PD_SERVICES
#define PD_SERVICES
#include "read_write_base.hpp"

/* 
 services Business Object
  
 CREATE TABLE services (
  id bigint not null primary key auto_increment,
  sequence integer NOT NULL,         -- order of apearance in a list
  tree_depth smallint NOT NULL,      -- order and tree depth determine tree position without recursion
  enabled int NOT NULL,              -- permits turning off of service for web site maintenance
  name varchar(32) NOT NULL,         -- name (for header)
  menu_name varchar(32) NOT NULL,    -- name (for menu item)
  uri varchar(64) NOT NULL,          -- location of resource
  local_uri int NOT NULL,            -- local service flag
  secure int NOT NULL,               -- determines if url should use https protocol
  xml_params text NOT NULL           -- generic parameters for service
); 
*/  
                 
class services:  public read_write_base
{
public:
  identifier id;
  int sequence;
  short tree_depth;
  bool enabled;
  string name;
  string menu_name;  
  string uri;
  bool local_uri;
  bool secure;
  string xml_params;
  
  // Constructor
  services():read_write_base(),id(0LL),sequence(0L),tree_depth(0L),enabled(true),secure(false)
  {    
    data_name("services");       
    // ALWAYS make the key be first, so we can skip it in insert logic
    addDXMap( new llongXfer("id", &id) );
    addDXMap( new intXfer("sequence", &sequence) );
    addDXMap( new shortXfer("tree_depth", &tree_depth) ); 
    addDXMap( new boolXfer("enabled", &enabled) );
    addDXMap( new stringXfer("name", &name) );
    addDXMap( new stringXfer("menu_name", &menu_name) ); 
    addDXMap( new stringXfer("uri", &uri) );
    addDXMap( new boolXfer("local_uri", &local_uri) );
    addDXMap( new boolXfer("secure", &secure) );
    addDXMap( new stringXfer("xml_params", &xml_params) );
                
  }
  virtual ~services(){;} 
  /*
  Hooks for business logic
  =========================
  // done before any action, false return prevents action
  virtual bool ivalidate( void  ) { return true; }
  virtual bool uvalidate( void  ) { return true; }
  virtual bool dvalidate( void ) { return true; }
  
  // done after any action - false return set warning flag
  virtual bool isupplemental( void ){ return true; }
  virtual bool usupplemental( changeMap & changes ){ return true; }
  virtual bool dsupplemental( void ){ return true; }
  virtual bool ssupplemental( void ){ return true; } 
  */
  
  // business object equivalent of cascade on delete
  virtual bool dsupplemental( void )
  { 
    bool bret = false;
    ocString sql = "delete from group_services where service_id = ";
    sql.append(id);
    bret =cmd.execute(sql);
    sql = "delete from role_services where service_id = ";
    sql.append(id);
    bret &= cmd.execute(sql);
    return bret;    
  }   
};
#endif


 
