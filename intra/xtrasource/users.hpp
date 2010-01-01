/* 
  users.hpp

  Object Definition and Implementation 
  for users.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef users_HPP
#define users_HPP

#include "read_write_base.hpp"

class users_Obj: public read_write_base
{
public:
  identifier id;
  llong group_id;
  string first;
  string last;
  string login;
  string password;
  string phone_number;
  string email;
  string preferences;
  llong metasite_user_id;
  
  // a pipe delimited list of selected roles for this user
  ocString user_roles;   
    
  users_Obj():read_write_base()
  ,id(0LL)
  ,group_id(0LL)
  ,first("")
  ,last("")
  ,login("")
  ,password("")
  ,phone_number("")
  ,email("")
  ,preferences("")
  ,metasite_user_id(0LL)
  {  
    // set name
    data_name("users");
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new llongXfer("group_id", &group_id ));
    addDXMap( new stringXfer("first", &first ));
    addDXMap( new stringXfer("last", &last ));
    addDXMap( new stringXfer("login", &login ));
    addDXMap( new stringXfer("password", &password ));
    addDXMap( new stringXfer("phone_number", &phone_number ));
    addDXMap( new stringXfer("email", &email ));
    addDXMap( new stringXfer("preferences", &preferences ));
    addDXMap( new llongXfer("metasite_user_id", &metasite_user_id ));
  }
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

