/*
  PaymentServices.hpp
  --------------------  
  Class definitions for Payment Services  
  Payment_Type_Parameter table entries for trust commerce;    

*/

#ifndef PAYMENT_SERVICES
#define PAYMENT_SERVICES

#include "paymentEnumerations.h"
#include "ocEnc.h"
#include "ocCoder.h"

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

// map by param type
typedef multimap < long, Payment_Type_Parameter > Payment_Type_Parameters;    

class Customer_Payment_Parameter_Datum
{
public:
  // Customer_Payment_Service_Parameters part
  identifier Id;
  long long Customer_Pmt_Service_Id;
  long long Pmt_Type_Param_Id; 
  string Name; 
  string Machine_Name;
  string Value; 
  Customer_Payment_Parameter_Datum():Id(0LL),Customer_Pmt_Service_Id(0LL),Pmt_Type_Param_Id(0LL)  
  {
  }
  virtual ~Customer_Payment_Parameter_Datum(){;}
  
  Customer_Payment_Parameter_Datum & operator = ( const Customer_Payment_Parameter_Datum & in )
  {
    Id=in.Id;
    Customer_Pmt_Service_Id=in.Customer_Pmt_Service_Id;
    Pmt_Type_Param_Id=in.Pmt_Type_Param_Id; 
    Name=in.Name;   
    Machine_Name=in.Machine_Name;
    Value=in.Value;    
    return *this;
  }
  
  // All of this is of course without comment whatsoever
  void encode(void)
  {
    int keyVal = Customer_Pmt_Service_Id;
    ocString toHash =  Machine_Name;
    keyVal += toHash.makeHash();
    ocEnc enc =  Value;
    Value = enc.encode(keyVal);
    ocCoder coder;
    Value = coder.base64encode( (const unsigned char *) Value.c_str(),Value.length());
  }
  void decode(void)
  {
    int keyVal = Customer_Pmt_Service_Id;
    ocString toHash =  Machine_Name;
    keyVal += toHash.makeHash();
    ocCoder coder;
    bins & binval = coder.base64decode(Value);
    ocEnc enc = "";
    for(int i=0; i<binval.size(); ++i)
    {
      enc += (char) binval[i];
    }
    Value = enc.decode(keyVal);
  }
};

typedef vector< Customer_Payment_Parameter_Datum > Customer_Payment_Parameters;

class Customer_Payment_Parameter :  public Customer_Payment_Parameter_Datum, public read_write_base
{
public:

  // ctor dtor and assign op.
  Customer_Payment_Parameter():Customer_Payment_Parameter_Datum()  
  {
    data_name("Customer_Payment_Parameter");
    addDXMap( new llongXfer("Id", &Id) );
    addDXMap( new llongXfer("Customer_Pmt_Service_Id", &Customer_Pmt_Service_Id) );
    addDXMap( new llongXfer("Pmt_Type_Param_Id", &Pmt_Type_Param_Id) );    
    addDXMap( new stringXfer("Machine_Name", &Machine_Name) );
    addDXMap( new stringXfer("Value", &Value) );
  }  
  ~Customer_Payment_Parameter(){;}

};
 
// Just for viewing in this case
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
    
  bool getPaymentServiceParameters(llong Param_Type=pt_pay)  
  {
    bool bReturn = false;
    
    ocString sql;
    
    // collect pay, preauthorize, postauthorize params
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
    sql.append(Type);
    sql+= " and Param_Type in ( 0, ";
    sql.append(Param_Type);
    sql+= ")";      
    
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

//
// Customer_Payment_Service with added stuff for simple ecommerce
//
class Customer_Payment_Service: public read_write_base
{
public:
  identifier Id;
  long long Customer_Id;
  long long Payment_Service_Id;


  // vector of user parms;
  Customer_Payment_Parameters params;
  
  // Constructor
  Customer_Payment_Service()
  :Id(0LL),Customer_Id(0LL)
  ,Payment_Service_Id(0LL) 
  {
    data_name("Customer_Payment_Service");    
    addDXMap( new llongXfer("Id", &Id) );
    addDXMap( new llongXfer("Customer_Id", &Customer_Id) );
    addDXMap( new llongXfer("Payment_Service_Id", &Payment_Service_Id) );

  } 
  virtual bool ivalidate( void  ) 
  { 
    // see if we have what we need...
    if( !Customer_Id )
    {
      m_result = "No Customer ID";

      return false;
    }
    if( !Customer_Id || !Payment_Service_Id )
    {
      m_result = "No Payment Service Id";

      return false;
    }    
    // all is good if we got here - destroy old payment info and start fresh    
    ocString list = "select Id from Customer_Payment_Service where Customer_Id = ";
    list.append(Customer_Id);
    
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
      ocString del = "delete from Customer_Payment_Parameter where Customer_Pmt_Service_Id in ( ";
      del += ids;
      del += ")";
      cmd.execute(del);
      // delete all other payment setups for thios customer
      del = "delete from Customer_Payment_Service where Customer_Id = ";
      del.append(Customer_Id);
      cmd.execute(del);
    }
    return true; 
  }
  
  virtual bool uvalidate( void  ) 
  { 
    // see if we have what we need...
    if( !Customer_Id )
    {
      m_result += " No Customer_Id ";
      return false;
    }
    if( !Payment_Service_Id )
    {
      m_result += " No Payment_Service_Id ";
      return false;
    }
    // all is good if we got here - destroy old payment info and start fresh    
    ocString list = "select Id from Customer_Payment_Service where Customer_Id = ";
    list.append(Customer_Id);
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
      ocString del = "delete from Customer_Payment_Parameter where Customer_Pmt_Service_Id in ( ";
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
     = "select Id,Customer_Pmt_Service_Id,Pmt_Type_Param_Id,Machine_Name,Value "
       "from Customer_Payment_Parameter where Customer_Pmt_Service_Id = ";
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
    Customer_Payment_Parameter_Datum datum;
    setParameter(datum);
    params.push_back(datum);
  }
  
  void setParameter( Customer_Payment_Parameter_Datum & datum )
  {
    datum.Id=atoll(rs.getField(0).format().c_str());
    datum.Customer_Pmt_Service_Id=atoll(rs.getField(1).format().c_str());
    datum.Pmt_Type_Param_Id=atoll(rs.getField(2).format().c_str());  
    datum.Machine_Name=rs.getField(3).format().c_str();
    datum.Value=rs.getField(4).format().c_str();   
  }      
};

#endif

