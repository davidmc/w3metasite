/* 
  Project.hpp

  Object Definition and Implementation 
  for Project.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Project_HPP
#define Project_HPP

#include "read_write_base.hpp"

class Project_Obj: public read_write_base
{
public:
  identifier Id;
  string Name;
  llong Company;
  time_date Estimated_Start;
  time_date Actual_Start;
  long Estimated_Time;
  long Actual_Time;
  long Phase;
  llong Parent_Project;
  string Notes;
  llong Status;
  money Rate;
  string PO;
  money Estimate;
  
  Project_Obj():read_write_base()
  ,Id(0LL)
  ,Name("")
  ,Company(0LL)
  ,Estimated_Start( )
  ,Actual_Start( )
  ,Estimated_Time(0L)
  ,Actual_Time(0L)
  ,Phase(0L)
  ,Parent_Project(0LL)
  ,Notes("")
  ,Status(0LL)
  ,Rate(0.0)
  ,PO("")
  ,Estimate(0.0)
  {  
    // set name
    data_name("Project");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new stringXfer("Name", &Name ));
    addDXMap( new llongXfer("Company", &Company ));
    addDXMap( new time_dateXfer("Estimated_Start", &Estimated_Start ));
    addDXMap( new time_dateXfer("Actual_Start", &Actual_Start ));
    addDXMap( new longXfer("Estimated_Time", &Estimated_Time ));
    addDXMap( new longXfer("Actual_Time", &Actual_Time ));
    addDXMap( new longXfer("Phase", &Phase ));
    addDXMap( new llongXfer("Parent_Project", &Parent_Project ));
    addDXMap( new stringXfer("Notes", &Notes ));
    addDXMap( new llongXfer("Status", &Status ));
    addDXMap( new moneyXfer("Rate", &Rate ));
    addDXMap( new stringXfer("PO", &PO ));
    addDXMap( new moneyXfer("Estimate", &Estimate ));
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



