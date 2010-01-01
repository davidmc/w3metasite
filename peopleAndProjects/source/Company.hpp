/* 
  Company.hpp

  Object Definition and Implementation 
  for Company.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Company_HPP
#define Company_HPP

#include "read_write_base.hpp"

class Company_Obj: public read_write_base
{
public:
  identifier Id;
  string Name;
  llong Key_Contact;
  string Address1;
  string Address2;
  string City;
  string State;
  string Zip;
  string Country;
  string Notes;
  time_date Last_Contacted;
  llong Status;
  string Open_PO;
  
  Company_Obj():read_write_base()
  ,Id(0LL)
  ,Name("")
  ,Key_Contact(0LL)
  ,Address1("")
  ,Address2("")
  ,City("")
  ,State("")
  ,Zip("")
  ,Country("")
  ,Notes("")
  ,Last_Contacted( )
  ,Status(0LL)
  ,Open_PO("")
  {  
    // set name
    data_name("Company");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new stringXfer("Name", &Name ));
    addDXMap( new llongXfer("Key_Contact", &Key_Contact ));
    addDXMap( new stringXfer("Address1", &Address1 ));
    addDXMap( new stringXfer("Address2", &Address2 ));
    addDXMap( new stringXfer("City", &City ));
    addDXMap( new stringXfer("State", &State ));
    addDXMap( new stringXfer("Zip", &Zip ));
    addDXMap( new stringXfer("Country", &Country ));
    addDXMap( new stringXfer("Notes", &Notes ));
    addDXMap( new time_dateXfer("Last_Contacted", &Last_Contacted ));
    addDXMap( new llongXfer("Status", &Status ));
    addDXMap( new stringXfer("Open_PO", &Open_PO ));
  }
  bool uvalidate( changeMap & changes  )
  {
    changes["Key_Contact"] = "Key_Contact";
    return true;
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



