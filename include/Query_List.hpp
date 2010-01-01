/*
  Handy query viewer
*/
#ifndef Query_Result_List_Hpp
#define Query_Result_List_Hpp

#include "Composite_Query.hpp"

class Query_Result_List:  public list_base
{
public: 
  ocString Query_Id;
  
  int fieldCount;
  // Constructor
  Query_Result_List(cgiScript&sc, Session_Obj & session):list_base(sc,session){;}  
  ~Query_Result_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    hotCol=-1; // no heat
    string sql;
    
    Composite_Query_Obj qry;
    
    qry.key(atoll(Query_Id.c_str()));
    if( qry.get_data() )
    {
      
      sql = qry.asSQL(false);

      if( sql.find("limit") == string::npos )
        sql += " limit 2 ";       
      getData( sql ); 
      emitHeadings();      
      emitData();   
    }
    else
    {
      webIO << "No data in Query - Key: " << Query_Id << endl;
    }    

     
    emitEnd();
    return breturn;
  } 
  void emitHeadings( void )
  {
    ocString heading;
    fieldCount = rs.getFieldCount();
    for( int i=0; i<fieldCount; i++ )
    {
      heading.append(i);
      heading += "<br>";         
      heading += rs.getField(i).getName();
      heading += "|";
    }
    list_base::emitHeadings(heading);     
  }  
}; 
#endif
