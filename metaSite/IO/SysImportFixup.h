#ifndef SYSIMPORT_KEYS
#define SYSIMPORT_KEYS

#include "read_write_base.hpp"

class SysImport_Keys : public read_write_base
{
public:
  string Table_Name;
  long long Imported_Id;
  long long Current_Id;
  string errors;
  SysImport_Keys():Imported_Id(0LL),Current_Id(0LL)
  {
    data_name("SysImport_Keys");    
    addDXMap( new llongXfer("Imported_Id", &Imported_Id) );
    addDXMap( new llongXfer("Current_Id", &Current_Id) );
    addDXMap( new stringXfer("Table_Name", &Table_Name) );
    actDerived = true;
  }
  ~SysImport_Keys(){;}

  bool get( string Table_Name, long long Imported_Id )
  {
    ocString where = "Table_Name = '";
    where += Table_Name;
    where += "' and Imported_Id = ";
    where.append(Imported_Id);
    return get_data(where);    
  }
  bool get( string Table_Name, string Column_Name, string Column_Ret, string Lookup_Value )
  {
    bool ret = false;
    ocString where = "select ";
    where += Column_Ret;
    where += " from ";
    where += Table_Name;
    where += " where ";
    where += Column_Name;
    where += " ='";
    where += Lookup_Value;
    where += "'"; 
    ret = rs.open(where);       
    if( ret )
    {
      Current_Id = atoll( rs.getField(0).format().c_str() );
      rs.close();
    }
    else
    {
      errors = "Open Error! SQL: ";
      errors += where;
      errors += " message: ";
      errors += rs.getErrors(); 
    }   
    return ret;
  }
  bool insert( void )
  {
     return db_insert();
  }      
};

#endif
