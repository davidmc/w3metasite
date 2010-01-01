/*
  base classes:
  read_write_base
  PayDrop Category Business Object  
  
*/

#ifndef PD_Subcategory
#define PD_Subategory
#include "read_write_base.hpp"

/* 
  Subcategory Business Object
*/ 
class  Subcategory: public read_write_base
{
public:
 identifier SubcategoryID;
 identifier CategoryID;
 string SubcategoryName;   
  // Constructor
  Subcategory():read_write_base(),SubcategoryID(0),CategoryID(0LL)
  {    
    data_name("Subcategory");       
    // ALWAYS make the key be first, so we can skip it in insert logic
    addDXMap( new llongXfer("SubcategoryID", &SubcategoryID) );
    addDXMap( new llongXfer("CategoryID", &CategoryID) );
    addDXMap( new stringXfer("SubcategoryName", &SubcategoryName) );    
  } 
  virtual ~Subcategory(){;} 
};

#endif

