/*
  openLogin.cpp
  Security over INTRANET admin panels.
*/

openService::openService()
{;}
openService::~openService()
{;}
openService::openService(const openService&in):id(in.id), sequence(in.sequence),
        tree_depth(in.tree_depth), cat_name(in.cat_name),  menu_name(in.menu_name),
        uri(in.uri), local_uri(in.local_uri), xml_params(in.xml_params), xml_param1(in.xml_param1),
        xml_param2(in.xml_param2),secure(in.secure),prog_enum(in.prog_enum)
{;}
openService & openService::operator = ( const openService & in )
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
  prog_enum=in.prog_enum;
  return *this;
}


void openLogin::setCookieValue( cgiCookie & cookie, string name, string & value )
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

void openLogin::setVars(void)
{
#ifdef LOGIN_LITE_SECURITY
  id=rs.getField(0).format();
  first=rs.getField(1).format();
  last=rs.getField(2).format();
  login=rs.getField(3).format();
  password=rs.getField(4).format();
  phone_number=rs.getField(5).format();
  email=rs.getField(6).format();
  group_id=rs.getField(7).format();
  site_admin = rs.getField(8).format() != "0";
#else
  id=rs.getField(0).format();
  group_id=rs.getField(1).format();
  first=rs.getField(2).format();
  last=rs.getField(3).format();
  login=rs.getField(4).format();
  password=rs.getField(5).format();
  phone_number=rs.getField(6).format();
  email=rs.getField(7).format();
  site_admin = rs.getField(7).format() != "0";
#endif
}

openLogin::openLogin():qqry(),rs(qqry.getRS()),site_admin(false),session(),token("intraToken")
{;}
openLogin::~openLogin()
{;}

bool openLogin::checkUser( const char * user, const char * password )
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
#ifdef LOGIN_LITE_SECURITY
      sql = "Select u.id, u.first, u.last," // 0,1,2
            "u.login, u.password, u.phone, u.email, " // 3,4,5,6
            "u.group_id, coalesce(g.site_admin,0) " // 7,8
            " from users u left join groups g on g.id = u.group_id ";
      sql += " where u.login = '";
      sql += user;
      sql += "' and u.password = '";
      sql += password;
      sql += "'";
#else
    sql = "Select u.id,u.group_id,u.first,u.last," // 0,1,2,3
          "u.login,u.password,u.phone_number,u.email,g.site_admin " // 4,5,6,7,8
          " from users u inner join groups g on g.id = u.group_id ";
    sql += " where u.login = '";
    sql += user;
    sql += "' and u.password = '";
    sql += password;
    sql += "'";
#endif
    // DBG cerr << sql << endl;
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
      lastError += sql;
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

bool openLogin::testLoginStatus( void )
{
  bool bRet = false;
  cgiCookie cookie;
  string & rId = id;

  if( rId.length() == 0 || rId == "0") rId = cookie.get(token.c_str());

  if( rId.length() > 0 )
  {
      bRet = getUser(rId);
  } // end if Id has length
  return bRet;
}

// New Interface!!!
bool openLogin::getUser(string & rId)
{
  bool bRet = false;
  
    // Check to see if a real user is here
#ifdef LOGIN_LITE_SECURITY
      sql = "Select u.id,u.first,u.last," // 0,1,2
            "u.login,u.password,u.phone,u.email," //
            "u.group_id, coalesce(g.site_admin,0) " // 3,4,5,6,7
            " from users u ";
      sql += " left join groups g on g.id=u.group_id where u.id = ";
#else
      sql = "Select u.id,u.group_id,u.first,u.last,"
            "u.login,u.password,u.phone_number,u.email,g.site_admin "
            " from users u inner join groups g on g.id=u.group_id where u.id = ";
#endif
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
    lastError += "Could not fetch user!\n";
    lastError += "<!--";
    lastError += sql;
    lastError += "-->";
  }
  return bRet;
}

bool openLogin::fetchServices( string criteria )
{
  bool bRet = false;
  bool open = false;
#ifdef LOGIN_LITE_SECURITY
     // get the role services (sans les groups) - tie back to both roles and groups
    sql = "select distinct s.id, s.cat_name, s.menu_name, s.uri, "// 0,1,2,3
      "s.sequence, s.tree_depth, " // 4, 5
      "rs.xml_params, s.target,  s.prog_enum, rs.id  " // 6 , 7 , 8  , 9
      " from services s "
      "inner join role_services rs on rs.service_id = s.id "
      "inner join roles r on r.id = rs.role_id "
      "inner join user_roles ur on ur.role_id = r.id "
      "where ur.user_id = ";
    sql += id;
    sql += " and s.enabled = 1";
    sql += " and ur.enabled = 1";
    sql += " and rs.enabled = 1";
#else
  // get the role services - tie back to both roles and groups
  sql = "select distinct s.id, s.cat_name, s.menu_name, s.uri, "// 0,1,2,3
        "s.local_uri, s.secure, " // 4,5
        "s.sequence, s.tree_depth, " // 6,7
        "s.xml_params"
        ", gs.xml_param1, gs.xml_param2, rs.id " // 8,9,10,11
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
#endif
  if( criteria.length() )
  {
    sql += " and ";
    sql += criteria;
  }

  sql += " order by s.sequence, s.tree_depth";
  // cout <<  "<!-- " << sql << " -->" << endl;
  for( open = rs.open(sql); open; open = rs.next() )
  {
    bRet = true;
    openService service;
#ifdef LOGIN_LITE_SECURITY
    service.id =    rs.getField(0).format();
    service.cat_name =  rs.getField(1).format();
    service.menu_name =     rs.getField(2).format();
    service.uri =           rs.getField(3).format();
    service.sequence =      rs.getField(4).format();
    service.tree_depth =    rs.getField(5).format();
    service.xml_params =    rs.getField(6).format();
    service.xml_param1 = service.xml_param2 = service.xml_params;
    service.secure = false; // Has no meaning in lite context
    service.target = rs.getField(7).format();
    service.prog_enum = rs.getField(8).format();

    
    
    if( service.prog_enum == "0" ) service.prog_enum = rs.getField(9).format();
#else    
    service.id =    rs.getField(0).format();
    service.cat_name =  rs.getField(1).format();
    service.menu_name =     rs.getField(2).format();
    service.uri =           rs.getField(3).format();
    service.local_uri =  dynamic_cast<longField&>(rs.getField(4)).get();
    service.secure =     dynamic_cast<longField&>(rs.getField(5)).get();
    service.sequence =      rs.getField(6).format();
    service.tree_depth =    rs.getField(7).format();
    service.xml_params =    rs.getField(8).format();
    service.xml_param1 =    rs.getField(9).format();
    service.xml_param2 =    rs.getField(10).format();
    service.prog_enum = rs.getField(11).format();
#endif    
    string key = service.uri;
    // prog_enum is a stronger key
    if( service.prog_enum != "0" ) key = service.prog_enum;
    if( services.find(key) == services.end() )  // it ain't, so be it
    {
      services.insert(make_pair(key,service));
      serviceNames.push_back(key);
    }
    // DBG  cout << "PROG ENUM SET TO: " <<  service.prog_enum << "<br>" << endl;
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

serviceMap & openLogin::Services( void )
{
  return services;
}
serviceVector & openLogin::ServiceNames( void )
{
  return serviceNames;
}

string & openLogin::Id( void )
{
  return id;
}
string openLogin::myStaff( void )
{
  // of course, I am part of my staff
  return id;
}
string & openLogin::Last( void )
{
  return last;
}
string & openLogin::First( void )
{
  return first;
}
string & openLogin::Email ( void )
{
  return email;
}
string & openLogin::PhoneNumber ( void )
{
  return phone_number;
}
string & openLogin::Login ( void )
{
  return login;
}
string & openLogin::Password ( void )
{
  return password;
}
string & openLogin::getSql( void )
{
  return sql;
}
string & openLogin::getLastError( void )
{
  return lastError;
}
string openLogin::FullName( void )
{
  return first + " " + last;
}

//  Added group_id  site_admin
string & openLogin::GroupId( void )
{
  return group_id;
}
bool openLogin::SiteAdmin( void )
{
  return site_admin;
}
Session_Obj & openLogin::Session( void )
{
  return session;
}

