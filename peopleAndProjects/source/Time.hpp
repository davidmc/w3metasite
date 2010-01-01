
/* 
  Time.hpp

  Object Definition and Implementation 
  for Time.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Time_HPP
#define Time_HPP

#include "read_write_base.hpp"

class Time_Obj: public read_write_base
{
  public:
    identifier Id;
    llong Task;
    time_date Start;
    time_date Stop;
    string Description;
    double Hours;
    llong Status;
    llong Invoice;
    llong Performed_By;
    void setHours( void )
    {
    // figure the elapsed time.
      time_date temp;
      temp.parse( Stop.format("%m/%d/%Y  %H:%M").c_str(), "%m/%d/%Y  %H:%M" );

      temp.normalize();

      time_t elapsed_seconds =  temp.normalize();
      temp.parse(Start.format("%m/%d/%Y  %H:%M").c_str(), "%m/%d/%Y  %H:%M" );

      temp.normalize();
      elapsed_seconds -= temp.normalize();
      Hours = (double) ( (int) elapsed_seconds/3600 ) + (double) ( ((double)(elapsed_seconds%3600))/3600.0 );
    }
    Time_Obj():read_write_base()
    ,Id(0LL)
    ,Task(0LL)
    ,Start( )
    ,Stop( )
    ,Description("")
    ,Hours(0.0)
    ,Status(0LL)
    ,Invoice(0LL)
    ,Performed_By(0LL)
    {
// set name
      data_name("Time");
// add fields
      addDXMap( new llongXfer("Id", &Id ));
      addDXMap( new llongXfer("Task", &Task ));
      addDXMap( new time_dateXfer("Start", &Start ));
      addDXMap( new time_dateXfer("Stop", &Stop ));
      addDXMap( new stringXfer("Description", &Description ));
      addDXMap( new doubleXfer("Hours", &Hours ));
      addDXMap( new llongXfer("Status", &Status ));
      addDXMap( new llongXfer("Invoice", &Invoice ));
      addDXMap( new llongXfer("Performed_By", &Performed_By ));
      Start.normalize();
      Stop.normalize();
    }
    virtual bool ivalidate( void  )
    {
    // Set the stop time to Now
      Stop.now();
    // figure the elapsed time
      setHours();
      return true;
    }
    virtual bool isupplemental( void )
    {

      return true;
    }
    virtual bool uvalidate( changeMap & changes  )
    {
    // if start or stop time were changed
      if( changes.find("Stop") != changes.end() ||
          changes.find("Start") != changes.end() )
      {
        setHours();
        changes["Hours"]="Hours";
      }
      return true;
    }

};
#endif

