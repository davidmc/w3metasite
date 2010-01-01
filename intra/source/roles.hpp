/* 
  roles.hpp

  Object Definition and Implementation 
  for roles.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef roles_HPP
#define roles_HPP

#include "read_write_base.hpp"

class roles_Obj: public read_write_base
{
public:
  identifier id;
  string name;
  string comment;

    
  roles_Obj():read_write_base()
  ,id(0LL)
  ,name("")
  ,comment("")
  {  
    // set name
    data_name("roles");
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new stringXfer("name", &name ));
    addDXMap( new stringXfer("comment", &comment ));
  }

  
  // on Event of form submission, delete
  virtual bool deleteServices( void )
  {
    bool bRet = false;
    ocString sql = "delete from role_services where role_id = ";
    sql.append(id);
    bRet = cmd.execute(sql);
    return bRet;
  }
  

};
#endif

