/*
  base classes:
  read_write_base
  PayDrop Category Business Object  
  
*/

#ifndef PD_Category
#define PD_Category
#include "read_write_base.hpp"

/* 
  Category Business Object
*/  
                 
class Category:  public read_write_base
{
public:
  identifier CategoryID;
  string CategoryName;
  
  // Constructor
  Category():read_write_base(),CategoryID(0LL)
  {    
    data_name("Category");       
    // ALWAYS make the key be first, so we can skip it in insert logic
    addDXMap( new llongXfer("CategoryID", &CategoryID) );
    addDXMap( new stringXfer("CategoryName", &CategoryName) );
    
  }
  virtual ~Category(){;}  
};
#endif


 
