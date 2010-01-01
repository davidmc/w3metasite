/*
  base classes:
  read_write_base
  PayDrop Config Business Object  
  
*/

#ifndef PD_Config
#define PD_Config
#include "read_write_base.hpp"

/* 
  Config Business Object
*/  
     
class  Config :  public read_write_base
{
public:
  identifier ConfigID;
  string ConfigVariable;
  string ConfigValue; 
  
  // Constructor
  Config():read_write_base(),ConfigID(0LL)
  {    
    data_name("Config");       
    // ALWAYS make the key be first, so we can skip it in insert logic
    addDXMap( new llongXfer("ConfigID", &ConfigID) );
    addDXMap( new stringXfer("ConfigVariable", &ConfigVariable) );
    addDXMap( new stringXfer("ConfigValue", &ConfigValue) );    
  }
  virtual ~Config(){;}    
};

#endif
