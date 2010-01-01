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
  string first;
  string last;
  string login;
  string password;
  string phone;
  string mobile;
  string fax;
  string email;
  string pager;
  string xml_preferences;
  llong group_id;
  
  // a pipe delimited list of selected roles for this user
  ocString user_roles;   
    
  users_Obj():read_write_base()
  ,id(0LL)
  ,first("")
  ,last("")
  ,login("")
  ,password("")
  ,phone("")
  ,email("")
  ,fax("")
  ,pager("")
  ,xml_preferences("")
  ,group_id(0LL)
  {  
    // set name
    data_name("users");
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new stringXfer("first", &first ));
    addDXMap( new stringXfer("last", &last ));
    addDXMap( new stringXfer("login", &login ));
    addDXMap( new stringXfer("password", &password ));
    addDXMap( new stringXfer("phone", &phone ));
    addDXMap( new stringXfer("mobile", &mobile ));
    addDXMap( new stringXfer("email", &email ));
    addDXMap( new stringXfer("fax", &fax ));
    addDXMap( new stringXfer("pager", &pager ));
    addDXMap( new stringXfer("xml_preferences", &xml_preferences ));
    addDXMap( new llongXfer("group_id", &group_id ));
    // cerr <<  "llongXfer(group_id: " << group_id << endl;

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

