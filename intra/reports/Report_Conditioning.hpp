/* 
  Report_Conditioning.hpp

  Object Definition and Implementation 
  for Report_Conditioning.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Report_Conditioning_HPP
#define Report_Conditioning_HPP

#include "read_write_base.hpp"

class Report_Conditioning_Obj: public read_write_base
{
public:
  identifier Id;
  llong Report_Id;
  int Col;
  int Sort_Pos;
  string Col_Handler;
  string Direction;
  string Format;  
  string Tuple_Section;
  llong spawnedReport;
  string spawnedTuplePretext;
  string spawnedTuplePosttext;
  int spawningEvent;
  
  Report_Conditioning_Obj():read_write_base()
  ,Id(0LL)
  ,Report_Id(0LL)
  ,Col(0)
  ,Sort_Pos(0)
  ,Col_Handler("")
  ,Direction("")
  ,Format("")
  ,Tuple_Section("")
  ,spawnedReport(0LL)
  ,spawnedTuplePretext("")
  ,spawnedTuplePosttext("")
  ,spawningEvent(0)
  {  
    // set name
    data_name("Report_Conditioning");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new llongXfer("Report_Id", &Report_Id ));
    addDXMap( new intXfer("Col", &Col ));
    addDXMap( new intXfer("Sort_Pos", &Sort_Pos ));
    addDXMap( new stringXfer("Col_Handler", &Col_Handler ));
    addDXMap( new stringXfer("Direction", &Direction ));
    addDXMap( new stringXfer("Format", &Format ));
    addDXMap( new stringXfer("Tuple_Section", &Tuple_Section ));
    addDXMap( new llongXfer("spawnedReport", &spawnedReport ));
    addDXMap( new stringXfer("spawnedTuplePretext", &spawnedTuplePretext ));
    addDXMap( new stringXfer("spawnedTuplePosttext", &spawnedTuplePosttext ));
    addDXMap( new intXfer("spawningEvent", &spawningEvent ));
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

