/*

  userLogin.h
  Generic Security for the web.  

*/
#ifndef USER_LOGIN_H
#define USER_LOGIN_H


#include "openDB.hpp"
#include "connectstring"
#include "read_base.hpp"
#include <map>
#include <vector>
#include "cgiCookie.h"


class openService
{
  public:

  string id,service_name,sequence,tree_depth,menu_name,
         uri,xml_params,xml_param1,xml_param2;
  bool local_uri, secure;

  openService()
  {;}
  virtual ~openService()
  {;}
  openService(const openService&in):id(in.id), sequence(in.sequence),
         tree_depth(in.tree_depth), service_name(in.service_name),  menu_name(in.menu_name),
         uri(in.uri), local_uri(in.local_uri), xml_params(in.xml_params), xml_param1(in.xml_param1),
         xml_param2(in.xml_param2)
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
    secure=in.secure;
    return *this;
  }
};

typedef map < string, openService > serviceMap;
typedef vector < string > serviceVector;

class userLogin
{  
public:
  // defaulted to auction Bidder, 
  // changes must be set before validation checkUser or testLoginStatus
  string userCols; // must reflect id, first, last, login, password, phone_number, email prperties
  string userTable;
  string loginName;
  string passwordName;
  
  // must be set before calling services
  string role_id;
    
  // all have this
  string id, first, last, login, password, phone_number, email;
  
  string sql;
  string lastError;
  serviceMap services;
  serviceVector serviceNames;  
  string fieldTemp;
  quickQuery qqry;
  openRS & rs;
  
protected:
  
  void setVars(void)
  {
    id=rs.getField(0).format();
    first=rs.getField(1).format();
    last=rs.getField(2).format();
    login=rs.getField(3).format();
    password=rs.getField(4).format();
    phone_number=rs.getField(5).format();
    email=rs.getField(6).format();
  }
  
public:

  userLogin(string userCols,string userTable,string loginName,string passwordName)
  :qqry(),rs(qqry.getRS()),
  userCols(userCols),userTable(userTable),loginName(loginName),passwordName(passwordName)
  {;}
  
  // defaulted values
  userLogin():qqry(),rs(qqry.getRS()),
  userCols(" Id, First_Name , Last_Name , EMail , Password, Day_Phone, EMail "),
  userTable("Bidder"),loginName("EMail"),passwordName("Password")
  {;}
  
  virtual ~userLogin(){;}
  
  // The name in these context is typically related to the site  
  string & getSiteRole( string roleName )
  {
    sql = "select id from metasite.roles where name = '" + roleName + "'";    
    
    if( rs.open(sql) )
    {
      role_id = rs.getField(0).format();
      rs.close();
    }
    else 
    {
      lastError += rs.getErrors();
      lastError += " - " + sql;
    }
    return role_id;
  }
  virtual bool checkUser( const char * user, const char * password )
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
      sql = "Select ";
      sql += userCols;
      sql += " from " ;            
      sql += userTable;
      sql += " where " + loginName + " = '";
      sql += user;
      sql += "' and " + passwordName += " = '";
      sql += password;
      sql += "'";

      if( rs.open(sql) )
      {
        // set the data vars
        setVars();

        // error? what error?
        lastError = "";
        // return true
        bRet = true;
        rs.close();
      } // end if query returned data
      else
      {
        lastError += rs.getErrors();
        lastError += " no such user on this site, please try again. ";
      }
    } // end if user and password
    else
    {
      lastError += " both the login and password must be entered. ";
    }
    return bRet;
  }

  virtual bool testLoginStatus( void )
  {
    bool bRet = false;
    cgiCookie cookie;
    string & rId = id;    
    
    if( rId.length() > 0 )
    {
      // Check to see if a real user is here
      sql = "select ";
      sql += userCols;
      sql += " from " ;            
      sql += userTable; 
      sql += " where ";
      // physical id is always the first field of userCols      
      ocString pcols = userCols;
      sql += pcols.parse(",");
      sql += " = ";
      sql += rId;
      if( rs.open(sql) )
      {
        // set the data vars
        setVars();

        bRet = true;
        rs.close();
      } // end if query returned data
      else
      {
        lastError += "Could not fetch your login!\n";
        lastError += "<!--";
        lastError += sql;
        lastError += "-->";
      }
    } // end if Id has length
    return bRet;
  }
            
  bool fetchServices( string criteria = "" )
  {
    bool bRet = false;
    bool open = false;

    // get the role services - tie back to both roles and groups
    sql = "select distinct s.id, s.name, s.menu_name, s.uri, "// 0,1,2,3
          "s.local_uri, s.secure, " // 4,5
          "s.sequence, s.tree_depth, " // 6,7
          "s.xml_params " // 8
          "from metasite.services s "
          "inner join metasite.role_services rs on rs.service_id = s.id "
          "where rs.role_id = ";
    sql += role_id;
    if( criteria.length() )
    {
      sql += " and ";
      sql += criteria;
    }
    sql += " order by s.sequence, s.tree_depth";

    for( open = rs.open(sql); open; open = rs.next() )
    {
      bRet = true;
      openService service;
      service.id =    rs.getField(0).format();
      service.service_name =  rs.getField(1).format();
      service.menu_name =     rs.getField(2).format();
      service.uri =           rs.getField(3).format();
      service.xml_params =    rs.getField(8).format();
      service.local_uri =  dynamic_cast<longField&>(rs.getField(4)).get();
      service.secure =     dynamic_cast<longField&>(rs.getField(5)).get();
      service.sequence =      rs.getField(6).format();
      service.tree_depth =    rs.getField(7).format();
      string key = service.service_name;
      services.insert(make_pair(key,service));
      serviceNames.push_back(key);
    }
    rs.close();
    if( !bRet )
    {
      lastError += " no services for ";
      lastError += first;
      lastError += " ";
      lastError += last;
      lastError += " are enabled. <!--";
      lastError += sql;
      lastError += " | ";
      lastError += rs.getErrors();
      lastError += "-->";
    }
    return bRet;

  }  // end fetch services method

  serviceMap & Services( void )
  {
    return services;
  }
  
  serviceVector & ServiceNames( void )
  {
    return serviceNames;
  }
  
  int serviceCount( void )
  {
    return serviceNames.size();
  }
  openService & getService(int idx)
  {
    return services[serviceNames[idx]];
  }
  
  openService & getService(string serviceName)
  {
    return services[serviceName];
  }
 

};



#endif
