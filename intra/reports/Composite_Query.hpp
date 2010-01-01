/* 
  Composite_Query.hpp

  Object Definition and Implementation 
  for Composite_Query.
  
  Copyright (C) 2005 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Composite_Query_HPP
#define Composite_Query_HPP

#include "read_write_base.hpp"

class Composite_Query_Obj: public read_write_base
{
public:
  identifier Id;
  llong Site_Id;
  string Name;
  string Select_Part;
  string From_Part;
  string Where_Part;
  string Group_Part;
  string Having_Part;
  string Order_Part;
  bool Is_Workflow;
  
  Composite_Query_Obj():read_write_base()
  ,Id(0LL)
  ,Site_Id(0LL)
  ,Name("")
  ,Select_Part("")
  ,From_Part("")
  ,Where_Part("")
  ,Group_Part("")
  ,Having_Part("")
  ,Order_Part("")
  ,Is_Workflow(false)
  {  
    // set name
    data_name("Composite_Query");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new llongXfer("Site_Id", &Site_Id ));
    addDXMap( new stringXfer("Name", &Name ));
    addDXMap( new stringXfer("Select_Part", &Select_Part ));
    addDXMap( new stringXfer("From_Part", &From_Part ));
    addDXMap( new stringXfer("Where_Part", &Where_Part ));
    addDXMap( new stringXfer("Group_Part", &Group_Part ));
    addDXMap( new stringXfer("Having_Part", &Having_Part ));
    addDXMap( new stringXfer("Order_Part", &Order_Part ));
    addDXMap( new boolXfer("Is_Workflow", &Is_Workflow ));
  }
  
  void addTo_Where_Part( string toAdd )
  {
    if( Where_Part.length() ) Where_Part += " and ";
    Where_Part += toAdd;    
  }
  
  string asSQL( bool includeWhere = true )
  {
    string sql = "select " + Select_Part;
    if( From_Part.length())     sql += " from " + From_Part;
    if( includeWhere && Where_Part.length() )
      sql += " where " + Where_Part;
    if( Group_Part.length() )  
      sql += " group by " + Group_Part;
    if( Having_Part.length() )
      sql += " having " + Having_Part;
    if( Order_Part.length() )
      sql += " order by " + Order_Part;
    return sql;  
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



