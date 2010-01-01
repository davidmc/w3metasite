/* 
  Scheme_Table.hpp

  Object Definition and Implementation 
  for Scheme_Table.
  
  Copyright (C) 2005 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Scheme_Table_HPP
#define Scheme_Table_HPP

#include "read_write_base.hpp"

class Scheme_Table_Obj: public read_write_base
{
public:
  identifier Id;
  string Name;
  string Abbr;
  
  Scheme_Table_Obj():read_write_base()
  ,Id(0LL)
  ,Name("")
  ,Abbr("")
  {  
    // set name
    data_name("Scheme_Table");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new stringXfer("Name", &Name ));
    addDXMap( new stringXfer("Abbr", &Abbr ));
  }  
  virtual bool dsupplemental( void )
  { 
    ocString sql = "delete from Scheme_Field where Scheme_Table = ";
    sql.append(Id);
    cmd.execute(sql);
    return true; 
  }
};
#endif



