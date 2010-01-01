/* 
  role_services.hpp

  Object Definition and Implementation 
  for role_services.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef role_services_HPP
#define role_services_HPP

#include "read_write_base.hpp"

class role_services_Obj: public read_write_base
{
public:
  identifier id;
  llong role_id;
  llong service_id;
  bool enabled;
  string xml_params;
  
  role_services_Obj():read_write_base()
  ,id(0LL)
  ,role_id(0LL)
  ,service_id(0LL)
  ,enabled(false)
  ,xml_params("")
  {  
    // set name
    data_name("role_services");
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new llongXfer("role_id", &role_id ));
    addDXMap( new llongXfer("service_id", &service_id ));
    addDXMap( new boolXfer("enabled", &enabled ));
    addDXMap( new stringXfer("xml_params", &xml_params ));
  }
  

};
#endif



