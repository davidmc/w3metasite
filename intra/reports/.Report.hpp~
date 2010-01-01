/* 
  Report.hpp

  Object Definition and Implementation 
  for Report.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Report_HPP
#define Report_HPP

#include "read_write_base.hpp"
#include "lookup.hpp"
#include "Composite_Query.hpp"

class Report_Obj: public read_write_base
{

public:
  identifier Id;
  llong Site_Id;
  llong Template_Id;
  llong Query_Id;
  llong Composite_Query;
  string Name;
  llong Media_Type;
  string Category;
  bool Show_In_List;
  int Formatting_Modulus;
  int Items_Per_Page;
  string External_Program;
  
  Report_Obj():read_write_base()
  ,Id(0LL)
  ,Site_Id(0LL)
  ,Template_Id(0LL)
  ,Query_Id(0LL)
  ,Composite_Query(0LL)
  ,Name("")
  ,Media_Type(0LL)
  ,Category("")
  ,Show_In_List(true)
  ,Formatting_Modulus(0)
  ,Items_Per_Page(0)
  ,External_Program("")
  {
    // set name
    data_name("Report");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new llongXfer("Site_Id", &Site_Id ));
    addDXMap( new llongXfer("Template_Id", &Template_Id ));
    addDXMap( new llongXfer("Query_Id", &Query_Id ));
    addDXMap( new llongXfer("Composite_Query", &Composite_Query ));
    addDXMap( new stringXfer("Name", &Name ));
    addDXMap( new llongXfer("Media_Type", &Media_Type ));
    addDXMap( new stringXfer("Category", &Category ));
    addDXMap( new boolXfer("Show_In_List", &Show_In_List ));
    addDXMap( new intXfer("Formatting_Modulus", &Formatting_Modulus ));
    addDXMap( new intXfer("Items_Per_Page", &Items_Per_Page ));
    addDXMap( new stringXfer("External_Program", &External_Program ));
  }

  string getTemplatePath(void)
  {
    ocString q = "Select Path from Report_Templates where Id = ";
    q.append(Template_Id);
    return tableLookup ( q );
  }
  
  // deprecated - use composite query instead
  string getQuery(void)
  {
    if( Query_Id )
    {
      ocString q = "Select Query from Query where Id = ";
      q.append(Query_Id);
      return tableLookup ( q );
    }
    else
    {
      return "";
    }
    
  }
  string getMediaType(void)
  {
    ocString q = "Select Name from Report_Type where Id = ";
    q.append(Media_Type);
    return tableLookup ( q );
  }
  
  string getCompositeQuery(void)
  {
    string result;
    Composite_Query_Obj cq;
    cq.key(Composite_Query);
    if( cq.get_data() )
    {
      result = cq.asSQL();
    }
    return result;
  }

};
#endif

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
