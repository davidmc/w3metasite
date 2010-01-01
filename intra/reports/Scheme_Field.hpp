/* 
  Scheme_Field.hpp

  Object Definition and Implementation 
  for Scheme_Field.
  
  Copyright (C) 2005 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Scheme_Field_HPP
#define Scheme_Field_HPP

#include "read_write_base.hpp"

class Scheme_Field_Obj: public read_write_base
{
public:
  identifier Id;
  llong Scheme_Table;
  string Name;
  string Col_Type;
  llong Links_To;
  
  Scheme_Field_Obj():read_write_base()
  ,Id(0LL)
  ,Scheme_Table(0LL)
  ,Name("")
  ,Col_Type("")
  ,Links_To(0LL)
  {  
    // set name
    data_name("Scheme_Field");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new llongXfer("Scheme_Table", &Scheme_Table ));
    addDXMap( new stringXfer("Name", &Name ));
    addDXMap( new stringXfer("Col_Type", &Col_Type ));
    addDXMap( new llongXfer("Links_To", &Links_To ));
  }  

};
#endif



