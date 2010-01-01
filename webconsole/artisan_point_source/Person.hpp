/*
  base classes:
  read_write_base
        
*/
#ifndef AP_Person
#define AP_Person
#include "read_write_base.hpp"
class Person:  public read_write_base
{
public:
  identifier Id;
  string First_Name;
  string Last_Name;
  string Login;
  string Password;
  string Address;
  string Address2;  
  string City;
  string State;
  string Zip;
  string Phone;
  string EMail;
  // Constructor
  Person()
  :read_write_base(),Id(0LL)
  {
    data_name("Person"); 
    addDXMap( new llongXfer("Id", &Id) );
    addDXMap( new stringXfer("First_Name", &First_Name) );
    addDXMap( new stringXfer("Last_Name", &Last_Name) );
    addDXMap( new stringXfer("Login", &Login) );
    addDXMap( new stringXfer("Password", &Password) );
    addDXMap( new stringXfer("Address", &Address) );
    addDXMap( new stringXfer("Address2", &Address2) );    
    addDXMap( new stringXfer("City", &City) );
    addDXMap( new stringXfer("State", &State) );
    addDXMap( new stringXfer("Zip", &Zip) );
    addDXMap( new stringXfer("Phone", &Phone) );
    addDXMap( new stringXfer("EMail", &EMail) );
  }
  
  // No two persons may have the same login
  bool ivalidate( void  ) 
  { 
    bool bret = false;
    m_result = "The login ";
    m_result += Login;
    m_result += " is already in the system.";
    if( actDerived == true)
    {
      bret = true;
      m_result = "";
    }
    else
    {
      // see if a user already exists with this login
      string sql = "select count(*) from Person where Login = '";
      sql += Login;
      sql += "'";

      if( rs.open(sql) )
      {
        if( rs.getField(0).format() == "0" ) 
        {
          bret = true;
          m_result = "";
        }            
        rs.close(); 
      }
    }
    return bret; 
  }
};

#endif
