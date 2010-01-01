/*
  base classes:
  read_write_base
  roles Business Object    
*/

#ifndef PD_ROLES
#define PD_ROLES
#include "read_write_base.hpp"

/* 
  role Business Object

   CREATE TABLE roles (
    id bigint not null primary key auto_increment,
    name varchar(24) NOT NULL,  -- role name
    comment varchar(255)
  );

*/  
                 
class roles:  public read_write_base
{
public:
  identifier id;
  string name;
  string comment;
  
  // a pipe delimited list of selected roles for this group
  ocString role_services;   
  
  // Constructor
  roles():read_write_base(),id(0LL)
  {    
    data_name("roles");       
    // ALWAYS make the key be first, so we can skip it in insert logic
    addDXMap( new llongXfer("id", &id) );
    addDXMap( new stringXfer("name", &name) );
    addDXMap( new stringXfer("comment", &comment) );
                
  }
  virtual ~roles(){;}  
  
  // on Event of form submission, update
  virtual bool setServices( void )
  {
    bool bRet = false;

    if( role_services.length() )
    {
      ocString sql;
      ocString sqlStart = "insert into role_services(role_id,service_id,enabled) values (";
      sqlStart.append(id);
      sqlStart += ",";
      role_services.parseInit();
      while( !role_services.endOfParse() )
      {
        sql = sqlStart;
        sql += role_services.parse("|");
        sql += ",1)";     

        bRet = cmd.execute(sql);
        if(!bRet) break;
      }      
    }
    
    return bRet;
  }
  
  // on Event of form submission, delete
  virtual bool deleteServices( void )
  {
    bool bRet = false;
    ocString sql = "delete from role_services where role_id = ";
    sql.append(id);
    bRet = cmd.execute(sql);
    return bRet;
  }
  
  // on event of getting data
  virtual bool retrieveServices( void )
  {
    bool bRet = false; 
    role_services = "";   
    ocString sql = "select service_id from role_services where role_id = ";
    sql.append(id);
    if( rs.open(sql) )
    {
      bRet = true;
      bool moreData = true;
      do
      {
        role_services += rs.getField(0).format();
        moreData = rs.next();
        if( moreData ) role_services += "|";
      }
      while( moreData );
    }  
    return bRet;
  }     
  
};
#endif


 
