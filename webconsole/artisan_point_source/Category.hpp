/*
  base classes:
  read_write_base
      
*/
#ifndef AP_Category
#define AP_Category
#include "read_write_base.hpp"

/* 
  Category Business Object
*/  
                 
class Category:  public read_write_base
{
public:
  identifier Id;
  string Name;
  long  long Cat_Order;
  long  long Personalization_Owner;
  long  long Personalization_Group;
  
  // Constructor
  Category()
  :read_write_base(),Id(0LL),Cat_Order(0LL),
  Personalization_Owner(0LL),Personalization_Group(0LL)
  {    
    data_name("Category");       
    // ALWAYS make the key be first, so we can skip it in insert logic
    addDXMap( new llongXfer("Id", &Id) );
    addDXMap( new stringXfer("Name", &Name) );
    addDXMap( new llongXfer("Cat_Order", &Cat_Order) );
    addDXMap( new llongXfer("Personalization_Owner", &Personalization_Owner) );    
    addDXMap( new llongXfer("Personalization_Group", &Personalization_Group) );
  }  
};
#endif


 
