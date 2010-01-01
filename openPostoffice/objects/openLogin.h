#ifndef OPEN_LOGIN_H
#define OPEN_LOGIN_H

#include "cgiCookie.h"
#include "openDB.hpp"
#include "pconnectstring"
#include <map>

/*

class openServiceItem
{
  public:
  string id,service_id, sequence,name;
  boolean enabled;
  openServiceItem(){;}
  virtual ~openServiceItem()
  {;}
  openServiceItem(const openService&in):id(in.id),service_id(in.service_id),
                  sequence(in.sequence),name(in.name),enabled(in.enabled)
  {;}
  openServiceItem & operator = ( const openServiceItem & in )
  {
    id=in.id;
    service_id=in.service_id;
    sequence=in.sequence;
    name=in.name;
    enabled=in.enabled;
    return * this;
  }
}

// map of name to service item - names should be unique
typedef map < string, openServiceItem > serviceItemMap;

*/


class openService
{
  public:
  // serviceItemMap serviceItems;

  string id,service_name,sequence,tree_depth,menu_name,
         uri,xml_params,xml_param1,xml_param2,service_items;
  bool local_uri, secure;

  openService()
  {;}
    virtual ~openService()
  {;}
  openService(const openService&in):id(in.id), sequence(in.sequence),
         tree_depth(in.tree_depth), service_name(in.service_name),  menu_name(in.menu_name),
         uri(in.uri), local_uri(in.local_uri), xml_params(in.xml_params), xml_param1(in.xml_param1),
         xml_param2(in.xml_param2), service_items(in.service_items)
  {;}
  openService & operator = ( const openService & in )
  {
    id=in.id;
    sequence=in.sequence;
    tree_depth=in.tree_depth;
    service_name=in.service_name;
    menu_name=in.menu_name;
    uri=in.uri;
    local_uri=in.local_uri;
    xml_params=in.xml_params;
    xml_param1=in.xml_param1;
    xml_param2=in.xml_param2;
    service_items=in.service_items;
    secure=in.secure;
    return *this;
  }

  bool fetchServiceItems( void )
  {
    bool bRet = false;
    // not implemented yet
    return bRet;
  }
};

typedef map < string, openService > serviceMap;

class openLogin
{
  string  id,group_id,first,last,group_name,login,password,phone_number,email,domain,url;
  bool site_admin;
  string sql;
  string lastError;
  serviceMap services;
  postgresDB db;
  postgresRS rs;
  string fieldTemp;

  void setCookieValue( cgiCookie & cookie, const char * name, string & value )
  {
    if( value.length() == 0 )
    {
      // set the timeout to 'Thu, 01-Jan-1970 00:00:00 GMT'
      // so the cookie will expire NOW.
      string & date = cookie.setTimeout( 1, 1, 1970, 0, 0 );
      cookie.set( name, value.c_str() );
    }
    else
    {
      cookie.set( name, value.c_str() );
    }
  }
  void setVars(void)
  {
    id=rs.getField(0).format();
    group_id=rs.getField(1).format();
    first=rs.getField(2).format();
    last=rs.getField(3).format();
    group_name= rs.getField(4).format();
    login=rs.getField(5).format();
    password=rs.getField(6).format();
    phone_number=rs.getField(7).format();
    email=rs.getField(8).format();
    site_admin = dynamic_cast<boolField&>(rs.getField(9)).get();
    domain=rs.getField(10).format();
    url=rs.getField(11).format();
  }
public:
  openLogin():db(CONNECTSTRING),rs(db){;}
  ~openLogin(){;}

  bool checkUser( const char * user, const char * password )
  {
    // be pessimistic about the return code
    bool bRet = false;
    // be pessimistic about the error string
    lastError = "Bad Login / Password combination: ";
    // establish cookies
    cgiCookie cookie;
    // cookies valid for entire site
    cookie.setPath("/");

    if( user && password && strlen(user) && strlen(password) )
    {
      // check validity of user & password
      sql = "Select u.id,u.group_id,u.first,u.last,g.name as group_name,"
            "u.login,u.password,u.phone_number,u.email,g.site_admin, g.domain, g.url "
            " from users u inner join groups g on g.id=u.group_id ";
      sql += " where u.login = '";
      sql += user;
      sql += "' and u.password = '";
      sql += password;
      sql += "'";


      if( rs.open(sql) )
      {
        // set the data vars
        setVars();

        // set sesson cookies
        setCookieValue( cookie, "token", id );

        // error? what error?
        lastError = "";
        // return true
        bRet = true;
        rs.close();
      } // end if query returned data
      else
      {
        lastError += " no such user on this site, please try again. ";
      }
    } // end if user and password
    else
    {
      lastError += " both the login and password must be entered. ";
    }
    setCookieValue( cookie, "lastError", lastError );
    return bRet;
  }
  void logout( void )
  {
    string value = "0";
    cgiCookie cookie;
    cookie.setPath("/");
    setCookieValue( cookie, "token", value );
  }
  bool testLoginStatus( void )
  {
    bool bRet = false;
    cgiCookie cookie;
    string & rId = cookie.get("token");

    if( rId.length() > 0 )
    {
      // Check to see if a real user is here
      sql = "Select u.id,u.group_id,u.first,u.last,g.name as group_name,"
            "u.login,u.password,u.phone_number,u.email,g.site_admin, g.domain, g.url  "
            " from users u inner join groups g on g.id=u.group_id where u.id = ";
      sql += rId;
      if( rs.open(sql) )
      {
        // set the data vars
        setVars();

        bRet = true;
        rs.close();
      } // end if query returned data
    } // end if Id has length
    return bRet;
  }
            
  bool fetchServices( void )
  {
    bool bRet = false;
    bool open = false;

    // get the role services
    sql = "select service_id,service_name,sequence,tree_depth,menu_name,"
          "uri,xml_params,xml_param1,xml_param2,service_items,local_uri,secure "
          "from vw_role_services where service_enabled = true and role_id in ("
          "select role_id from user_roles where user_id = ";
    sql += id;
    sql += " and user_roles.enabled = true) order by sequence";

    for( open = rs.open(sql); open; open = rs.next() )
    {
      bRet = true;
      openService service;
      service.id =    rs.getField(0).format();
      service.service_name =  rs.getField(1).format();
      service.sequence =      rs.getField(2).format();
      service.tree_depth =    rs.getField(3).format();
      service.menu_name =     rs.getField(4).format();
      service.uri =           rs.getField(5).format();
      service.xml_params =    rs.getField(6).format();
      service.xml_param1 =    rs.getField(7).format();
      service.xml_param2 =    rs.getField(8).format();
      service.service_items = rs.getField(9).format();
      service.local_uri =  dynamic_cast<boolField&>(rs.getField(10)).get();
      service.secure =     dynamic_cast<boolField&>(rs.getField(11)).get();
      services.insert(make_pair(service.service_name,service));
    }
    rs.close();
    if( !bRet )  lastError += " no services for this user are enabled. ";
    return bRet;

  }  // end fetch services method

  serviceMap & Services( void )
  {
    return services;
  }

  string & Id( void )
  {
    return id;
  }
  string & Last( void )
  {
    return last;
  }
  string & First( void )
  {
    return first;
  }
  string & GroupName( void )
  {
    return group_name;
  }
  string & Email ( void )
  {
    return email;
  }
  string & PhoneNumber ( void )
  {
    return phone_number;
  }
  string & Login ( void )
  {
    return login;
  }
  string & Password ( void )
  {
    return password;
  }
  string & getSql( void )
  {
    return sql;
  }
  string & getLastError( void )
  {
    return lastError;
  }

  //  Added group_id  site_admin
  string & GroupId( void )
  {
    return group_id;
  }
  bool SiteAdmin( void )
  {
    return site_admin;
  }

  // Added domain and url
  string & Domain( void )
  {
    return domain;
  }
  string & Url( void )
  {
    return url;
  }

};

#endif
