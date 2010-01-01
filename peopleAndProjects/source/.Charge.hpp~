/*
  Charge.hpp

  Object Definition and Implementation 
  for Charge.
  
  Copyright (C) 2008 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Charge_HPP
#define Charge_HPP

#include "read_write_base.hpp"

class Charge_Obj: public read_write_base
{
public:
  identifier Id;
  string Name;
  string Description;
  llong Company;
  llong Project;
  llong Invoice;
  money MyCost;
  money Charge;
  Charge_Obj():read_write_base()
  ,Id(0LL)
  ,Name("")
  ,Description("")
  ,Company(0LL)
  ,Project(0LL)
  ,Invoice(0LL)
  ,MyCost(0.0)
  ,Charge(0.0)
  {
    // set name
    data_name("Charge");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new stringXfer("Name", &Name ));
    addDXMap( new stringXfer("Description", &Description ));
    addDXMap( new llongXfer("Company", &Company ));
    addDXMap( new llongXfer("Project", &Project ));
    addDXMap( new llongXfer("Invoice", &Invoice ));
    addDXMap( new moneyXfer("MyCost", &MyCost ));
    addDXMap( new moneyXfer("Charge", &Charge ));
  }

  /*
  TODO: Add Event hooks for business logic
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



