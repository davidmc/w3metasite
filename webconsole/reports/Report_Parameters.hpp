/* 
  Report_Parameters.hpp

  Object Definition and Implementation 
  for Report_Parameters.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Report_Parameters_HPP
#define Report_Parameters_HPP

#include "read_write_base.hpp"

class Report_Parameters_Obj: public read_write_base
{
public:
  identifier Id;
  llong Report_Id;
  string Display_Label;
  string Data_Element;
  string Tag_Name;
  string Choices_Query;
  string Parameter_Type;
  
  Report_Parameters_Obj():read_write_base()
  ,Id(0LL)
  ,Report_Id(0LL)
  ,Display_Label("")
  ,Data_Element("")
  ,Tag_Name("")
  ,Choices_Query("")
  ,Parameter_Type("")  
  {  
    // set name
    data_name("Report_Parameters");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new llongXfer("Report_Id", &Report_Id ));
    addDXMap( new stringXfer("Display_Label", &Display_Label ));
    addDXMap( new stringXfer("Data_Element", &Data_Element ));
    addDXMap( new stringXfer("Tag_Name", &Tag_Name ));
    addDXMap( new stringXfer("Choices_Query", &Choices_Query ));
    addDXMap( new stringXfer("Parameter_Type", &Parameter_Type ));    
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

