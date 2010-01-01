/* 
  group_services.hpp

  Object Definition and Implementation 
  for group_services.
  
  Copyright (C) 2005 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef group_services_HPP
#define group_services_HPP

#include "read_write_base.hpp"

class group_services_Obj: public read_write_base
{
public:
  identifier id;
  llong service_id;
  llong group_id;
  short sequence;
  string xml_param1;
  string xml_param2;
  
  group_services_Obj():read_write_base()
  ,id(0LL)
  ,service_id(0LL)
  ,group_id(0LL)
  ,sequence(0)
  ,xml_param1("")
  ,xml_param2("")
  {  
    // set name
    data_name("group_services");
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new llongXfer("service_id", &service_id ));
    addDXMap( new llongXfer("group_id", &group_id ));
    addDXMap( new shortXfer("sequence", &sequence ));
    addDXMap( new stringXfer("xml_param1", &xml_param1 ));
    addDXMap( new stringXfer("xml_param2", &xml_param2 ));
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



/*
  group_services.hpp
  base classes:
  read_write_base
  Group Services Business Object    



                 
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

*/

 
