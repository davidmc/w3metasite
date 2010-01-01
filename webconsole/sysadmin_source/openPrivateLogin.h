/*
  openLogin.h   (private) version
  Security over admin panels.
*/
#ifndef OPEN_LOGIN_H
#define OPEN_LOGIN_H

#include "cgiCookie.h"
#include "openDB.hpp"
#include "connectstring"
#include "read_base.hpp"
#include <map>
#include <vector>
// for parsing preferences
#include "ocXML.h"
#include "Session.hpp"

class openService
{
  public:
  
  // serviceItemMap serviceItems;
  
  string cat_name; // to satisfy w3intranet in reports
  string id,service_name,sequence,tree_depth,menu_name,
         uri,xml_params,xml_param1,xml_param2;
  bool local_uri, secure;

  openService()
  {;}
    virtual ~openService()
  {;}
  openService(const openService&in):cat_name(in.cat_name),id(in.id), sequence(in.sequence),
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
public:
protected:
  
  string id,group_id,first,last,login,password,phone_number,email,preferences;
  bool site_admin;
  string sql;
  string lastError;
  serviceMap services;
  serviceVector serviceNames;
  string fieldTemp;
  quickQuery qqry;
  openRS & rs;
  Session_Obj session;
  
  void setCookieValue( cgiCookie & cookie, const char * name, string & value )
  {
    if( value.length() == 0 )
    {
      // set the timeout to 'Thu, 01-Jan-1970 00:00:00 GMT'
      // so the cookie will expire NOW.
      string & date = cookie.setTimeout( 1, 1, 1970, 0, 0 );
      cookie.set( name, value.c_str() ); 
      lastError = date;     
      date = ""; // set the date to null fo the next cookie setting.
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
    login=rs.getField(4).format();
    password=rs.getField(5).format();
    phone_number=rs.getField(6).format();
    email=rs.getField(7).format();
    site_admin = dynamic_cast<longField&>(rs.getField(8)).get();
    preferences = rs.getField(9).format();
  }
public:
  // now part of the interface. not used yet
  string source;
  
  openLogin(bool inCharge=true):qqry(),rs(qqry.getRS()),site_admin(false),session(inCharge){;}
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
            "u.login,u.password,u.phone_number,u.email,g.site_admin,u.preferences " // 4,5,6,7,8
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
        lastError += rs.getErrors();
        lastError += " no such user on this site, please try again. ";
      }
    } // end if user and password
    else
    {
      lastError += " both the login and password must be entered. ";
    }

    // End Temp Code
    setCookieValue( cookie, "lastError", lastError );
    return bRet;
  }

  virtual bool testLoginStatus( void )
  {
    bool bRet = false;
    cgiCookie cookie;
    string & rId = id;    
    
    if( rId.length() == 0 || rId == "0") rId = cookie.get("token");
    
    if( rId.length() > 0 )
    {
      // Check to see if a real user is here
      sql = "Select u.id,u.group_id,u.first,u.last,"
            "u.login,u.password,u.phone_number,u.email,g.site_admin,u.preferences "
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
        lastError += " cn: ";
        lastError += CONNECTSTRING;
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
          "s.xml_params, gs.xml_param1, gs.xml_param2 " // 8,9,10
          "from services s "
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
    sql += " order by s.sequence, s.tree_depth";

    for( open = rs.open(sql); open; open = rs.next() )
    {
      bRet = true;
      openService service;
      service.id =    rs.getField(0).format();
      service.service_name =  rs.getField(1).format();
      service.menu_name =     rs.getField(2).format();
      service.cat_name=service.service_name;
      service.uri =           rs.getField(3).format();
      service.xml_params =    rs.getField(8).format();
      service.xml_param1 =    rs.getField(9).format();
      service.xml_param2 =    rs.getField(10).format();
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
  // devolves in this case to id
  string & myStaff( void )
  {
    return id;
  }  
  Session_Obj & Session( void )
  {
    return session;
  }  
  // new preference stuff
  string getPreference( string Name )
  {
    string ret;
    xmlParser parser( preferences );
    parser.parse();
    node_map & nodes = parser.states.nodemap;
    node_map::iterator iter;
    iter = nodes.lower_bound(Name);
    if(iter != nodes.upper_bound(Name))
    {
      ret = parser.nodeList()[iter->second].data;    
    }
    return ret;
  }
  
  bool setPreference( string Name, string Value )
  {    
    xmlParser parser( preferences );
    parser.parse();
    node_map & nodes = parser.states.nodemap; 
    node_map::iterator pos = nodes.lower_bound(Name);
    if(pos != nodes.upper_bound(Name))
    {
      parser.nodeList()[pos->second].data = Value;    
    }
    else
    {
      xmlNode aNode;
      aNode.name = Name;
      aNode.data = Value;
      parser.addNode(aNode);
    }
    preferences = parser.emit();
    
    return synch();
  }
  bool synch()
  {
    bool isGood = false;
    if( id.length() )
    {
      openDbFactory DbMaker(PROVIDER,CONNECTSTRING);
      openDB & db = DbMaker.db();
      openCmdFactory CmdMaker(db);   
      openCMD & cmd = CmdMaker.cmd();
      sql = "update users set preferences = '";
      sql += preferences;
      sql += "' where id = ";
      sql += id;
      isGood = cmd.execute(sql);           
    } 
    return isGood;
  } 
   
};



#endif
