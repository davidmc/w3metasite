/*
  Document.hpp

  Object Definition and Implementation 
  for Document.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Document_HPP
#define Document_HPP

#include "read_write_base.hpp"

class Document_Obj: public read_write_base
{
public:
  identifier Id;
  string Category;
  string Name;
  string Description;
  string Filename;
  bool Visible;
  int Sequence;
  Document_Obj():read_write_base()
  ,Id(0LL)
  ,Category("")
  ,Name("")
  ,Description("")
  ,Filename("")
  ,Visible(false)
  ,Sequence(0)
  {
    // set name
    data_name("Document");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new stringXfer("Category", &Category ));
    addDXMap( new stringXfer("Name", &Name ));
    addDXMap( new stringXfer("Description", &Description ));
    addDXMap( new stringXfer("Filename", &Filename ));
    addDXMap( new boolXfer("Visible", &Visible ));
    addDXMap( new intXfer("Sequence", &Sequence ));
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



