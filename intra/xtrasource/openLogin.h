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

class openService
{
  public:
  // serviceItemMap serviceItems;

  string id,cat_name,sequence,tree_depth,menu_name,
         uri,xml_params,xml_param1,xml_param2;
  bool local_uri, secure;
  string target,prog_enum;
  openService();
  virtual ~openService();
  openService(const openService&in);
  openService & operator = ( const openService & in );
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
  
  void setCookieValue( cgiCookie & cookie, string name, string & value );
  void setVars(void);

public:
  string token;
  openLogin();
  virtual ~openLogin();

  virtual bool checkUser( const char * user, const char * password );
  virtual bool testLoginStatus( void );

  // New Interface!!!
  bool getUser(string & rId);
  bool fetchServices( string criteria = "" );


  serviceMap & Services( void );
  serviceVector & ServiceNames( void );
  string & Id( void );
  string myStaff( void );
  string & Last( void );
  string & First( void );
  string & Email ( void );
  string & PhoneNumber ( void );
  string & Login ( void );
  string & Password ( void );
  string & getSql( void );
  string & getLastError( void );
  string FullName( void );
  string & GroupId( void );
  bool SiteAdmin( void );
  Session_Obj & Session( void );
};

#ifdef ALTSLOGINSOURCE
// this is for validation against a totally different source of information
#include ALTSLOGINSOURCE
#else
// include the implementation seperately
#include "openLogin.cpp"
#endif

#endif
