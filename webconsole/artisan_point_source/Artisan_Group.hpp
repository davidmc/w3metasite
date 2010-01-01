/*
  base classes:
  read_write_base
      
*/
#ifndef AP_Artisan_Group
#define AP_Artisan_Group

#include "read_write_base.hpp"

/* 
  Artisan Group Business Object
*/  
    
class Artisan_Group:  public read_write_base
{
public:
  long long Id;
  string Name;
  long long Site_Id;
  long long Primary_Contact_Id;
  bool Primary_Is_Default_Handler;
  Artisan_Group()
  :read_write_base(),Id(0LL),Site_Id(0LL)
  ,Primary_Contact_Id(0LL),Primary_Is_Default_Handler(false)
  {
    data_name("Artisan_Group");
    addDXMap( new llongXfer("Id", &Id) );
    addDXMap( new stringXfer("Name", &Name) );
    addDXMap( new llongXfer("Site_Id", &Site_Id) );
    addDXMap( new llongXfer("Primary_Contact_Id", &Primary_Contact_Id) );
    addDXMap( new boolXfer("Primary_Is_Default_Handler", &Primary_Is_Default_Handler) );
  }   
};


#endif
