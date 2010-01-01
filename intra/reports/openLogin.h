/*

  openLogin.h
  
  Security over INTRANET admin panels.

*/
#ifndef OPEN_LOGIN_H
#define OPEN_LOGIN_H


#include "openDB.hpp"
#include "connectstring"
#include "read_base.hpp"
#include <map>
#include <vector>
#include "Session.hpp"

// hacked for mgdist, need to kill this and get siltanium on xtranet base!

 string baseUrl;
class openService
{
  public:
  // serviceItemMap serviceItems;

  string id,cat_name,sequence,tree_depth,menu_name,
         uri,xml_params,xml_param1,xml_param2;
  bool local_uri, secure;

  openService()
  {;}
    virtual ~openService()
  {;}
  openService(const openService&in):id(in.id), sequence(in.sequence),
         tree_depth(in.tree_depth), cat_name(in.cat_name),  menu_name(in.menu_name),
         uri(in.uri), local_uri(in.local_uri), xml_params(in.xml_params), xml_param1(in.xml_param1),
         xml_param2(in.xml_param2)
  {;}
  openService & operator = ( const openService & in )
  {
    id=in.id;
    sequence=in.sequence;
    tree_depth=in.tree_depth;
    cat_name=in.cat_name;
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

class openLogin
{
protected:
  
  string id,group_id,first,last,login,password,phone_number,email;
  bool site_admin;
  string sql;
  string lastError;
  serviceMap services;
  serviceVector serviceNames;
  string fieldTemp;
  quickQuery qqry;
  openRS & rs;
  Session_Obj session;
  
  void setCookieValue( cgiCookie & cookie, string name, string & value )
  {
    if( value.length() == 0 )
    {
      // set the timeout to 'Thu, 01-Jan-1970 00:00:00 GMT'
      // so the cookie will expire NOW.
      string & date = cookie.setTimeout( 1, 1, 1970, 0, 0 );
      cookie.set( name.c_str(), value.c_str() ); 
      lastError = date;     
      date = ""; // set the date to null for the next cookie setting.
    }
    else
    {
      cookie.set( name.c_str(), value.c_str() );
    }
  }
  void setVars(void)
  {
    id=rs.getField(0).format();
    group_id=rs.getField(1).format();
    first=rs.getField(2).format();
    last=rs.getField(3).format();
    login=rs.getField(4).format();
    password=rs.getField(5).format();
    phone_number=rs.getField(6).format();
    email=rs.getField(7).format();
    site_admin = dynamic_cast<longField&>(rs.getField(8)).get();
 #ifdef BOOTSTRAP 
   site_admin = true;
 #endif   
  }
  
  // a fairly flat recursive function
  string subordinate(string bosses, string listed)
  {
    // want to get all users that respond to the bosses
    bool gotMoreStaff = false;
    string staff; // staff is always only the current level
    string sql = "select id from users where reports_to in (";
    sql += bosses;
    sql += " ) and  id not in (";
    sql += listed;
    sql += ")";
    
    if( rs.open(sql) )
    {
      do
      {
        if( staff.length() ) staff += ",";
        staff += rs.getField(0).format();
      } while(rs.next());
      rs.close();
      if( listed.length() ) listed += ",";
      listed += staff;  // listed is cumulative    
      string substaff = subordinate(staff,listed);   
      if( substaff.length() )
      {
        if( staff.length() ) staff += ",";
        staff += substaff;
      }
    }
    return staff;
  }
  
public:
  string token;
  openLogin():qqry(),rs(qqry.getRS()),site_admin(false),session(),token("intraToken"){;}
  virtual ~openLogin(){;}

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
      sql = "Select u.id,u.group_id,u.first,u.last," // 0,1,2,3
            "u.login,u.password,u.phone_number,u.email,g.site_admin " // 4,5,6,7,8
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
        setCookieValue( cookie, token, id );

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
    setCookieValue( cookie, "lastError", lastError );
    return bRet;
  }

  virtual bool testLoginStatus( void )
  {
    bool bRet = false;
    cgiCookie cookie;
    string & rId = id;    
    
    if( rId.length() == 0 || rId == "0") rId = cookie.get(token.c_str());
    
    if( rId.length() > 0 )
    {
      // Check to see if a real user is here
      sql = "Select u.id,u.group_id,u.first,u.last,"
            "u.login,u.password,u.phone_number,u.email,g.site_admin "
            " from users u inner join groups g on g.id=u.group_id where u.id = ";
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
  
  string myStaff( void )
  {
    // of course, I am part of my staff
    string sIds = id;
    if( sIds.length() )
    {
      string staff = subordinate(sIds,sIds);
      if(  staff.length() )  sIds += ",";    
      sIds += staff;
    }
    return sIds;    
  }
  
  // a fairly flat recursive function
  string myRegions( void )
  {
    string regions;
    string sql = "select Region from User_Regions where User = ";
    sql += id;    
    if( rs.open(sql) )
    {
      do
      {
        if( regions.length() ) regions += ",";
        regions += rs.getField(0).format();
      } while(rs.next());
      rs.close();      
    }
    return regions;
  }
            
  bool fetchServices( string criteria = "" )
  {
    bool bRet = false; 
    bool open = false;
    services.clear();
    serviceNames.clear();
    // get the role services - tie back to both roles and groups
    sql = "select distinct s.id, s.cat_name, s.menu_name, s.uri, "// 0,1,2,3
          "s.local_uri, s.secure, " // 4,5
          "s.sequence, s.tree_depth, " // 6,7
          "s.xml_params"
#ifndef BOOTSTRAP          
          ", gs.xml_param1, gs.xml_param2 " // 8,9,10
#endif           
          " from services s "
#ifndef BOOTSTRAP            
          "inner join group_services gs on gs.service_id = s.id "
          "inner join role_services rs on rs.service_id = gs.service_id "
          "inner join roles r on r.id = rs.role_id "
          "inner join user_roles ur on ur.role_id = r.id "
          "where ur.user_id = ";
    sql += id;
    sql += " and ur.enabled = 1";
    sql += " and rs.enabled = 1";
    sql += " and gs.group_id = ";
    sql += group_id;

    if( criteria.length() )
    {
      sql += " and ";
      sql += criteria;
    }
#else
  ;    
#endif 

    sql += " order by s.sequence, s.tree_depth";
    
    for( open = rs.open(sql); open; open = rs.next() )
    {
      bRet = true;
      openService service;
      service.id =    rs.getField(0).format();
      service.cat_name =  rs.getField(1).format();
      service.menu_name =     rs.getField(2).format();
      service.uri =           rs.getField(3).format();
      service.local_uri =  dynamic_cast<longField&>(rs.getField(4)).get();
      service.secure =     dynamic_cast<longField&>(rs.getField(5)).get();
      service.sequence =      rs.getField(6).format();
      service.tree_depth =    rs.getField(7).format();      
      service.xml_params =    rs.getField(8).format();
#ifndef BOOTSTRAP                  
      service.xml_param1 =    rs.getField(9).format();
      service.xml_param2 =    rs.getField(10).format();
#endif
      string key = service.uri;
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
  string FullName( void )
  {
    return first + " " + last;
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
  Session_Obj & Session( void )
  {
    return session;
  }
};



#endif
