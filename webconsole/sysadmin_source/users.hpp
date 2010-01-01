/*
  base classes:
  read_write_base
  users Business Object  
  
*/

#ifndef PD_users
#define PD_users
#include "read_write_base.hpp"

/* 
  users Business Object
CREATE TABLE users (
  id bigint not null primary key auto_increment,
  group_id bigint NOT NULL,               -- link to groups
  first varchar(24) NOT NULL,             -- first name
  last varchar(24) NOT NULL,              -- last name
  login varchar(36) NOT NULL,             -- login
  password varchar(12) NOT NULL,          -- users password (encrypted)
  phone_number varchar(16) NOT NULL,      -- user phone number
  email varchar(64) NOT NULL              -- user email
);  
*/  
                 
class users:  public read_write_base
{
public:
  identifier id;
  long long group_id;
  string first;
  string last;
  string login;
  string password;
  string phone_number;
  string email;
  
  // a pipe delimited list of selected roles for this user
  ocString user_roles;   
  
  // Constructor
  users():read_write_base(),id(0LL),group_id(0LL)
  {    
    data_name("users");       
    // ALWAYS make the key be first, so we can skip it in insert logic
    addDXMap( new llongXfer("id", &id) );
    addDXMap( new llongXfer("group_id", &group_id) );    
    addDXMap( new stringXfer("first", &first) );                   
    addDXMap( new stringXfer("last", &last) );
    addDXMap( new stringXfer("login", &login) );
    addDXMap( new stringXfer("password", &password) );
    addDXMap( new stringXfer("email", &email) );     
  }
  virtual ~users(){;} 
  
  // on Event of form submission, update
  virtual bool setRoles( void )
  {
    bool bRet = false;
    if( user_roles.length() )
    {
      ocString sql;
      ocString sqlStart = "insert into user_roles (user_id,role_id,enabled) values (";
      sqlStart.append(id);
      sqlStart += ",";
      user_roles.parseInit();
      while( !user_roles.endOfParse() )
      {
        sql = sqlStart;
        sql += user_roles.parse("|");
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
    ocString sql = "delete from user_roles where user_id = ";
    sql.append(id);
    bRet = cmd.execute(sql);
    return bRet;
  }
  
  // on event of getting data
  virtual bool retrieveRoles( void )
  {
    bool bRet = false; 
    user_roles = "";   
    ocString sql = "select role_id from user_roles where user_id = ";
    sql.append(id);
    if( rs.open(sql) )
    {
      bRet = true;
      bool moreData = true;
      do
      {
        user_roles += rs.getField(0).format();
        moreData = rs.next();
        if( moreData ) user_roles += "|";
      }
      while( moreData );
    }  
    return bRet;
  }      
};
#endif


 
