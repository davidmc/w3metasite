/*
  base classes:
  read_write_base
  XXXX Business Object  
  
*/

#ifndef PD_XXXX
#define PD_XXXX
#include "read_write_base.hpp"

/* 
  XXXX Business Object
*/  
                 
class XXXX:  public read_write_base
{
public:
  identifier ;
  string ;
  
  // Constructor
  XXXX():read_write_base(),(0LL)
  {    
    data_name("XXXX");       
    // ALWAYS make the key be first, so we can skip it in insert logic
    addDXMap( new llongXfer("", &) );
    addDXMap( new llongXfer("", &) );
    addDXMap( new llongXfer("", &) );
    
    addDXMap( new stringXfer("", &) );                   
    addDXMap( new stringXfer("", &) );
    addDXMap( new stringXfer("", &) );
    addDXMap( new stringXfer("", &) );
    addDXMap( new stringXfer("", &) );
    
    addDXMap( new longXfer("", &) );
    addDXMap( new longXfer("", &) );
    addDXMap( new longXfer("", &) );
    addDXMap( new longXfer("", &) );
    
    
    addDXMap( new boolXfer("", &) );
    addDXMap( new boolXfer("", &) );        
    addDXMap( new boolXfer("", &) );
    addDXMap( new boolXfer("", &) );
    addDXMap( new boolXfer("", &) );    
    
    
    addDXMap( new oc_dateXfer("", &) );
    addDXMap( new oc_dateXfer("", &) );
    addDXMap( new oc_dateXfer("", &) );
    addDXMap( new oc_dateXfer("", &) );
            
    addDXMap( new time_dateXfer("", &) );
    addDXMap( new time_dateXfer("", &) );
    addDXMap( new time_dateXfer("", &) );
    addDXMap( new time_dateXfer("", &) );        
  }
  virtual ~XXXX(){;}  
};
#endif


 
