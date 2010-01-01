/*
  base classes:
  read_write_base
      
*/
#ifndef AP_Sub_Category
#define AP_Sub_Category
#include "read_write_base.hpp"

/* 
  Sub Category Business Object
*/

class Sub_Category:  public read_write_base
{
public:

  identifier Id;
  long long Category_Id;
  string Name;
  long long Sub_Order;
  long long Personalization_Owner;
  long long Personalization_Group;
  
// Constructor
  Sub_Category()
  :read_write_base(),Id(0LL),Category_Id(0LL),Sub_Order(0LL),
  Personalization_Owner(0LL),Personalization_Group(0LL)
  {    
    data_name("Sub_Category");       
    // ALWAYS make the key be first, so we can skip it in insert logic
    addDXMap( new llongXfer("Id", &Id) );
    addDXMap( new llongXfer("Category_Id", &Category_Id) );    
    addDXMap( new stringXfer("Name", &Name) );
    addDXMap( new llongXfer("Sub_Order", &Sub_Order) );
    addDXMap( new llongXfer("Personalization_Owner", &Personalization_Owner) );    
    addDXMap( new llongXfer("Personalization_Group", &Personalization_Group) );
  }   
};

#endif
