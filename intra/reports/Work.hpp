/* 
  Work.hpp

  Object Definition and Implementation for Work.
  
  These classes are built by w3sys RAD tools
  to be compatible with STL, and to be derivable
  
  Part of the Workflow system.
  
  Copyright(c)2005 - D.K. McCombs.
  W3 Systems Design - davidmc@w3sys.com
                      http://www.w3sys.com  

*/  
#ifndef Work_HPP
#define Work_HPP

#include "read_write_base.hpp"

class Work_Obj: public read_write_base
{
public:
  identifier Id;
  string Name;
  string Description;
  llong Query;
  llong User;
  llong User_Group;
  llong User_Role;
  string URL;
  string Search;
  int Sequence;
  
  Work_Obj():read_write_base()
  ,Id(0LL)
  ,Name("")
  ,Description("")
  ,Query(0LL)
  ,User(0LL)
  ,User_Group(0LL)
  ,User_Role(0LL)
  ,URL("")
  ,Search("")
  ,Sequence(0)
  {  
    // set name
    data_name("Work");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new stringXfer("Name", &Name ));
    addDXMap( new stringXfer("Description", &Description ));
    addDXMap( new llongXfer("Query", &Query ));
    addDXMap( new llongXfer("User", &User ));
    addDXMap( new llongXfer("User_Group", &User_Group ));
    addDXMap( new llongXfer("User_Role", &User_Role ));
    addDXMap( new stringXfer("URL", &URL ));
    addDXMap( new stringXfer("Search", &Search ));
    addDXMap( new intXfer("Sequence", &Sequence )); 
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


