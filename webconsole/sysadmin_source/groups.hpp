/*
  base classes:
  read_write_base
  groups Business Object  
  
*/

#ifndef PD_groups
#define PD_groups
#include "read_write_base.hpp"

/* 
  groups Business Object
  
CREATE TABLE groups (
  id bigint not null primary key auto_increment,
  name varchar(24) NOT NULL,     -- group name
  domain varchar(255) NOT NULL,  -- domain for email
  url varchar(255) NOT NULL,     -- web address
  site_admin  int NOT NULL       -- admin indicator
);  
*/  
                 
class groups:  public read_write_base
{
public:
  identifier id;
  string name;
  // string domain;
  // string url;
  bool site_admin;
  
  // a pipe delimited list of selected roles for this group
  ocString group_roles;  
  
  // Constructor
  groups():read_write_base(),id(0LL),site_admin(false)
  {    
    data_name("groups");       
    // ALWAYS make the key be first, so we can skip it in insert logic
    addDXMap( new llongXfer("id", &id) );
    
    addDXMap( new stringXfer("name", &name) );                   
    // addDXMap( new stringXfer("domain", &domain) );
    // addDXMap( new stringXfer("url", &url) );

    addDXMap( new boolXfer("site_admin", &site_admin) );    
     
  }
  virtual ~groups(){;} 
  
  // on Event of form submission, update
  virtual bool setRoles( void )
  {
    bool bRet = false;
    if( group_roles.length() )
    {
      ocString sql;
      ocString sqlStart = "insert into group_roles (group_id,role_id,enabled) values (";
      sqlStart.append(id);
      sqlStart += ",";
      group_roles.parseInit();
      while( !group_roles.endOfParse() )
      {
        sql = sqlStart;
        sql += group_roles.parse("|");
        sql += ",1)";        
        bRet = cmd.execute(sql);
        if(!bRet) break;
      }      
    }
    
    return bRet;
  }
  
  // on Event of form submission, delete
  virtual bool deleteRoles( void )
  {
    bool bRet = false;
    ocString sql = "delete from group_roles where group_id = ";
    sql.append(id);
    bRet = cmd.execute(sql);
    return bRet;
  }
  
  // on event of getting data
  virtual bool retrieveRoles( void )
  {
    bool bRet = false; 
    group_roles = "";   
    ocString sql = "select role_id from group_roles where group_id = ";
    sql.append(id);
    if( rs.open(sql) )
    {
      bRet = true;
      bool moreData = true;
      do
      {
        group_roles += rs.getField(0).format();
        moreData = rs.next();
        if( moreData ) group_roles += "|";
      }
      while( moreData );
    }  
    return bRet;
  }     
};
#endif


 
