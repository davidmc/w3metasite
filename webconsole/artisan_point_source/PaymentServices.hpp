/*

  Payment_Services.hpp
  --------------------  
  Class definitions for Artisan Point Payment Services  
  Payment_Type_Parameter table entries for trust commerce;    

*/

#ifndef ARTISAN_PAYMENT_SERVICES
#define ARTISAN_PAYMENT_SERVICES

#include "paymentEnumerations.h"

class Payment_Type_Parameter
{
public:
  identifier Id;
  long long Service_Type;
  long   Value_Location;
  long   Param_Type;
  long   Interface_Type;
  string Name;
  string Machine_Name;
  string Value;
  string Instructions;
  
  Payment_Type_Parameter()
  :Id(0LL),Service_Type(0LL),Value_Location(0LL),Param_Type(0LL)
  {;}
  ~Payment_Type_Parameter(){;}
  Payment_Type_Parameter & operator = ( const Payment_Type_Parameter & in )
  {
    Id=Id;
    Service_Type=in.Service_Type;
    Value_Location=in.Value_Location;
    Param_Type=in.Param_Type;
    Interface_Type=in.Interface_Type;
    Name=in.Name;
    Machine_Name=in.Machine_Name;
    Value=in.Value;
    Instructions=in.Instructions;
  }  
};

typedef Payment_Type_Parameter Payment_Type_Parameter_Obj;

// map by param type
typedef multimap < long, Payment_Type_Parameter > Payment_Type_Parameters;    

class Patron_Payment_Parameter_Datum
{
public:
  // Patron_Payment_Service_Parameters part
  identifier Id;
  long long Patron_Pmt_Service_Id;
  long long Pmt_Type_Param_Id; 
  string Name; 
  string Machine_Name;
  string Value; 
  Patron_Payment_Parameter_Datum():Id(0LL),Patron_Pmt_Service_Id(0LL),Pmt_Type_Param_Id(0LL)  
  {
  }
  virtual ~Patron_Payment_Parameter_Datum(){;}  
  Patron_Payment_Parameter_Datum & operator = ( const Patron_Payment_Parameter_Datum & in )
  {
    Id=in.Id;
    Patron_Pmt_Service_Id=in.Patron_Pmt_Service_Id;
    Pmt_Type_Param_Id=in.Pmt_Type_Param_Id; 
    Name=in.Name;   
    Machine_Name=in.Machine_Name;
    Value=in.Value;    
    return *this;
  }  
};

typedef vector< Patron_Payment_Parameter_Datum > Patron_Payment_Parameters;

class Patron_Payment_Parameter :  public Patron_Payment_Parameter_Datum, public read_write_base
{
public:

  // ctor dtor and assign op.
  Patron_Payment_Parameter():Patron_Payment_Parameter_Datum()  
  {
    data_name("Patron_Payment_Parameters");
    addDXMap( new llongXfer("Id", &Id) );
    addDXMap( new llongXfer("Patron_Pmt_Service_Id", &Patron_Pmt_Service_Id) );
    addDXMap( new llongXfer("Pmt_Type_Param_Id", &Pmt_Type_Param_Id) );    
    addDXMap( new stringXfer("Machine_Name", &Machine_Name) );
    addDXMap( new stringXfer("Value", &Value) );
  }  
  ~Patron_Payment_Parameter(){;}      
};
 
// read only - this is set up by a batch sql file
class Payment_Service:  public read_write_base
{
public:
  identifier Id;
  long long  Type;
  string Name;
  string Label;
  string Instructions;
  
  Payment_Type_Parameters sys_params;
  Payment_Type_Parameters usr_params;
  Payment_Type_Parameters dyn_params; 
  
  // Constructor
  Payment_Service():read_write_base(),Id(0LL),Type(0L)  
  {
    data_name("Payment_Service");    
    addDXMap( new llongXfer("Id", &Id) );
    addDXMap( new llongXfer("Type", &Type) );
    addDXMap( new stringXfer("Name", &Name) );
    addDXMap( new stringXfer("Label", &Label) );
    addDXMap( new stringXfer("Instructions", &Instructions) ); 
  }
    
  bool getPaymentServiceParameters(void)  
  {
    bool bReturn = false;
    ocString type;
    type.append(Type);
    string sql;  
    // collect upay, upreauthorize, upostauthorize, user
    sql = "select Id, "
          " Service_Type, " 
          " Value_Location, "
          " Param_Type, "   
          " Interface_Type, "
          " Name, "
          " Machine_Name, "
          " Value, "        
          " Instructions from Payment_Type_Parameter where "
          " Service_Type = ";              
    sql += type;
        

    if( rs.open(sql) )
    {
      bReturn = true;
      do
      {
        addParameter();                         
      } while(rs.next());
    }
    return bReturn;
  }
  
  void addParameter(void)
  {
    Payment_Type_Parameter temp;
    setParameter(temp);
    if(temp.Value_Location==pml_parm)
      sys_params.insert(make_pair(temp.Param_Type,temp));
    else if(temp.Value_Location==pml_user_parm)
      usr_params.insert(make_pair(temp.Param_Type,temp));
    else 
      dyn_params.insert(make_pair(temp.Param_Type,temp));
  }
  
  void setParameter( Payment_Type_Parameter & output )
  {   
    output.Id=atoll(rs.getField(0).format().c_str());
    output.Service_Type=atol(rs.getField(1).format().c_str());
    output.Value_Location=atol(rs.getField(2).format().c_str());
    output.Param_Type=atol(rs.getField(3).format().c_str());
    output.Interface_Type=atol(rs.getField(4).format().c_str());
    output.Name=rs.getField(5).format().c_str();
    output.Machine_Name=rs.getField(6).format().c_str();
    output.Value=rs.getField(7).format().c_str();
    output.Instructions=rs.getField(8).format().c_str();
  }
    
};

// Patron_Payment_Service
class Patron_Payment_Service: public read_write_base
{
public:
  identifier Id;
  long long Patron_Id;
  long long Payment_Service_Id;
  bool Is_Default_Service;
  bool Do_Cache_Data;  

  // vector of user parms;
  Patron_Payment_Parameters params;
  
  // Constructor
  Patron_Payment_Service()
  :Id(0LL),Patron_Id(0LL)
  ,Payment_Service_Id(0LL)
  ,Is_Default_Service(true)  
  ,Do_Cache_Data(true)
  {
    data_name("Patron_Payment_Service");    
    addDXMap( new llongXfer("Id", &Id) );
    addDXMap( new llongXfer("Patron_Id", &Patron_Id) );
    addDXMap( new llongXfer("Payment_Service_Id", &Payment_Service_Id) );
    addDXMap( new boolXfer("Is_Default_Service", &Is_Default_Service) );
    addDXMap( new boolXfer("Do_Cache_Data", &Do_Cache_Data) );
  } 
  virtual bool ivalidate( void  ) 
  { 
    // see if we have what we need...
    if( !Patron_Id || !Payment_Service_Id )
    {
      return false;
    }
    
    // all is good if we got here - destroy old payment info and start fresh    
    ocString list = "select Id from Patron_Payment_Service where Patron_Id = ";
    list.append(Patron_Id);
    
    if( rs.open(list) )
    {
      ocString ids;
      ids += rs.getField(0).format();
      while( rs.next() )
      {
        ids += ",";
        ids += rs.getField(0).format();
      } 
      rs.close();      
      ocString del = "delete from Patron_Payment_Parameters where Patron_Pmt_Service_Id in ( ";
      del += ids;
      del += ")";
      cmd.execute(del);
      del = "delete from Patron_Payment_Service where Patron_Id = ";
      del.append(Patron_Id);
      cmd.execute(del);
    }
    return true; 
  }
  
  virtual bool uvalidate( void  ) 
  { 
    // see if we have what we need...
    if( !Patron_Id )
    {
      m_result += " No Patron_Id ";
      return false;
    }
    if( !Payment_Service_Id )
    {
      m_result += " No Payment_Service_Id ";
      return false;
    }
    // all is good if we got here - destroy old payment info and start fresh    
    ocString list = "select Id from Patron_Payment_Service where Patron_Id = ";
    list.append(Patron_Id);
    if( rs.open(list) )
    {
      ocString ids;      
      ids += rs.getField(0).format();
      Id = atoll(ids.c_str());
      while( rs.next() )
      {
        ids += ",";
        ids += rs.getField(0).format();
      } 
      rs.close();      
      ocString del = "delete from Patron_Payment_Parameters where Patron_Pmt_Service_Id in ( ";
      del += ids;
      del += ")";
      cmd.execute(del);
    }
    return true; 
  } 
  
  bool getParameters(void)
  {  
    bool bReturn = false;
    ocString sql
     = "select Id,Patron_Pmt_Service_Id,Pmt_Type_Param_Id,Machine_Name,Value "
       "from Patron_Payment_Parameters where Patron_Pmt_Service_Id = ";
    sql.append(Id);
    if( rs.open(sql) )
    {
      bReturn = true;
      do
      {
        addParameter();                         
      } while(rs.next());
    }
    else
    {
      m_result += "Get Parameter Failed - sql: ";
      m_result +=sql;
    }
    
    return bReturn;
  }
  
  void addParameter(void)
  {
    Patron_Payment_Parameter_Datum datum;
    setParameter(datum);
    params.push_back(datum);
  }
  
  void setParameter( Patron_Payment_Parameter_Datum & datum )
  {
    datum.Id=atoll(rs.getField(0).format().c_str());
    datum.Patron_Pmt_Service_Id=atoll(rs.getField(1).format().c_str());
    datum.Pmt_Type_Param_Id=atoll(rs.getField(2).format().c_str());  
    datum.Machine_Name=rs.getField(3).format().c_str();
    datum.Value=rs.getField(4).format().c_str();   
  }      
};

#endif

