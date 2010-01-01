/* 
  Ordered_Item.hpp

  Object Definition and Implementation 
  for Ordered_Item.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Ordered_Item_HPP
#define Ordered_Item_HPP

#include "read_write_base.hpp"
class Ordered_Item_Obj: public read_write_base
{
public:
  identifier Id;
  llong Product_Id;
  llong Order_Id;
  int Product_Count;
  money Total_Price;
  money Shipping;
  string Internal_Id;
  string External_Id;
  
  Ordered_Item_Obj():read_write_base()
  ,Id(0LL)
  ,Product_Id(0LL)
  ,Order_Id(0LL)
  ,Product_Count(0)
  ,Total_Price(0.0)
  ,Shipping(0.0)
  ,Internal_Id("")
  ,External_Id("")
  {  
    // set name
    data_name("Ordered_Item");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new llongXfer("Product_Id", &Product_Id ));
    addDXMap( new llongXfer("Order_Id", &Order_Id ));
    addDXMap( new intXfer("Product_Count", &Product_Count ));
    addDXMap( new moneyXfer("Total_Price", &Total_Price ));
    addDXMap( new moneyXfer("Shipping", &Shipping ));
    addDXMap( new stringXfer("Internal_Id", &Internal_Id ));
    addDXMap( new stringXfer("External_Id", &External_Id ));
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

