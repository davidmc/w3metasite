/*
  Attachments.hpp

  Object Definition and Implementation 
  for Attachments.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Attachments_HPP
#define Attachments_HPP

#include "read_write_base.hpp"

class Attachments_Obj: public read_write_base
{
public:
  identifier Id;
  llong Message;
  llong Task;
  string URL;
  string Title;
  string Description;
  Attachments_Obj():read_write_base()
  ,Id(0LL)
  ,Message(0LL)
  ,Task(0LL)
  ,URL("")
  ,Title("")
  ,Description("")
  {
    // set name
    data_name("Attachments");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new llongXfer("Message", &Message ));
    addDXMap( new llongXfer("Task", &Task ));
    addDXMap( new stringXfer("URL", &URL ));
    addDXMap( new stringXfer("Title", &Title ));
    addDXMap( new stringXfer("Description", &Description ));
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



