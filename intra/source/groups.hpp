/* 
  groups.hpp

  Object Definition and Implementation 
  for groups.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef groups_HPP
#define groups_HPP

#include "read_write_base.hpp"

class groups_Obj: public read_write_base
{
public:
  identifier id;
  string name;
  bool site_admin;
  // a pipe delimited list of selected roles for this group
  ocString group_roles;  
  groups_Obj():read_write_base()
  ,id(0LL)
  ,name("")
  ,site_admin(false)
  {  
    // set name
    data_name("groups");
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new stringXfer("name", &name ));
    addDXMap( new boolXfer("site_admin", &site_admin ));
  }
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

