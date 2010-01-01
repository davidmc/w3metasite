/* 
  Contact.hpp

  Object Definition and Implementation 
  for Contact.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Contact_HPP
#define Contact_HPP

#include "read_write_base.hpp"

class Contact_Obj: public read_write_base
{
public:
  identifier Id;
  string First;
  string Last;
  string Prefix;
  string Phone;
  string Mobile;
  string Email;
  string Fax;
  string Home_Phone;
  llong Company;
  string Password;
  
  Contact_Obj():read_write_base()
  ,Id(0LL)
  ,First("")
  ,Last("")
  ,Prefix("")
  ,Phone("")
  ,Mobile("")
  ,Email("")
  ,Fax("")
  ,Home_Phone("")
  ,Company(0LL)
  ,Password("")
  {  
    // set name
    data_name("Contact");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new stringXfer("First", &First ));
    addDXMap( new stringXfer("Last", &Last ));
    addDXMap( new stringXfer("Prefix", &Prefix ));
    addDXMap( new stringXfer("Phone", &Phone ));
    addDXMap( new stringXfer("Mobile", &Mobile ));
    addDXMap( new stringXfer("Email", &Email ));
    addDXMap( new stringXfer("Fax", &Fax ));
    addDXMap( new stringXfer("Home_Phone", &Home_Phone ));
    addDXMap( new llongXfer("Company", &Company ));
    addDXMap( new stringXfer("Password", &Password ));
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



