/*
  Hosting.hpp

  Object Definition and Implementation 
  for Hosting.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Hosting_HPP
#define Hosting_HPP

#include "read_write_base.hpp"

class Hosting_Obj: public read_write_base
{
public:
  identifier Id;
  llong Company;
  llong Invoice;
  oc_date From_Date;
  oc_date To_Date;
  string Site_URL;
  int Billing_Interval;
  money Charge_Per_Month;
  money Charge;
  Hosting_Obj():read_write_base()
  ,Id(0LL)
  ,Company(0LL)
  ,Invoice(0LL)
  ,From_Date( )
  ,To_Date( )
  ,Site_URL("")
  ,Billing_Interval(0)
  ,Charge_Per_Month(0.0)
  ,Charge(0.0)
  {
    // set name
    data_name("Hosting");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new llongXfer("Company", &Company ));
    addDXMap( new llongXfer("Invoice", &Invoice ));
    addDXMap( new oc_dateXfer("From_Date", &From_Date ));
    addDXMap( new oc_dateXfer("To_Date", &To_Date ));
    addDXMap( new stringXfer("Site_URL", &Site_URL ));
    addDXMap( new intXfer("Billing_Interval", &Billing_Interval ));
    addDXMap( new moneyXfer("Charge_Per_Month", &Charge_Per_Month ));
    addDXMap( new moneyXfer("Charge", &Charge ));
  }
  bool ivalidate( void  )
  {
    time_date temp;
    int months = (To_Date.year()-From_Date.year())*12;
    months +=  (To_Date.month() - From_Date.month());
    Charge =  Charge_Per_Month.amount() * months;
    return true;
  }

  bool uvalidate( changeMap & changes  )
  {
    changes["Charge"]="Charge";
    return ivalidate();
  }
  
  
  /*
  TODO: Add Event hooks for business logic
  =========================
  // Done before any action - a false return prevents action
  virtual
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



