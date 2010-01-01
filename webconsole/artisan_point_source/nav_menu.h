/*
  nav_menu.h

*/
#include "openLogin.h"
#include "ocString.h"
#include "cgiTemplates.h"


class artisanLogin: public openLogin
{
public:
  string Id, First_Name, Last_Name;
  string Login, Password, Address, City, State;
  string Zip, Phone, EMail;
  string Artisan_Group_Id;  
  void setArtVars(void)
  {
    Id=rs.getField(0).format();
    First_Name=rs.getField(1).format();
    Last_Name=rs.getField(2).format();
    Login=rs.getField(3).format();
    Password=rs.getField(4).format();
    Address=rs.getField(5).format();
    City=rs.getField(6).format();
    State=rs.getField(7).format();
    Zip=rs.getField(8).format();
    Phone=rs.getField(9).format();
    EMail=rs.getField(10).format();
    id=rs.getField(11).format();
    Artisan_Group_Id=rs.getField(12).format(); 
  }
  string selectClause( void )
  {
    return "select p.Id, p.First_Name, p.Last_Name, "
           "p.Login, p.Password, p.Address, p.City, p.State, "
           "p.Zip, p.Phone, p.EMail, a.User_Id, a.Artisan_Group_Id "
           "from Person p inner join Artisan a on p.Id=a.Person_Id ";
  }
  string whereIdIsClause( void )
  {
    string ret = " where p.Id = ";
    ret += Id;
    return ret;
  }
  string whereLoginIsClause( void )
  {
    string ret = " where p.Login = '";
    ret += Login;
    ret += "' and  p.Password = '";    
    ret += Password;
    ret += "'";
    return ret;
  }
  void logout( void )
  {
    cgiCookie cookie;  
    cookie.setPath("/");  
    string empty = "";
    setCookieValue( cookie, "arttoken", empty );
    setCookieValue( cookie, "token", empty );
  } 
  bool testLoginStatus( void )
  {
    bool bRet = openLogin::testLoginStatus();false;    
    cgiCookie cookie;    
    Id = cookie.get("arttoken");
    if( Id.length() > 0 )
    {
      sql = selectClause();
      sql += whereIdIsClause();
      if( rs.open(sql) )
      {
        // set the data vars
        setArtVars();
        // they can do some limited things here
        if( !bRet ) bRet = true;
                         
        rs.close();             
      } // end if query returned data
      else
      {
        lastError += "Could not fetch login: ";
        lastError += sql;        
      }
    }
    else
    {
      lastError += "Not logged in!";
    }
    return bRet;
  }
  bool checkUser( const char * user, const char * password )
  {   
    bool bRet = false;    
    lastError = "Bad Login / Password combination: ";    
    cgiCookie cookie;    
    cookie.setPath("/");
    if( user ) Login = user;
    if( password ) Password = password; 
    if( Login.length() && Password.length() )
    {
      // check validity of user & password
      sql = selectClause();
      sql += whereLoginIsClause();

      if( rs.open(sql) )
      {
        // set the data vars
        setArtVars();
        rs.close();        
        // error? what error?
        lastError = "";        
        // set session cookies
        setCookieValue( cookie, "arttoken", Id );
        if( id.length() && id != "0" )
        { 
          setCookieValue( cookie, "token", id );          
          bRet = openLogin::testLoginStatus(); 
        }       
        
      } // end if query returned data
      else
      {
        lastError += rs.getErrors();
        lastError += " no such user on this site, please try again. ";
        lastError += sql;
      }
    } // end if user and password
    else
    {
      lastError += " both the login and password must be entered. ";
    }
    setCookieValue( cookie, "lastError", lastError );
    return bRet;  
  }    
  artisanLogin ():openLogin(){;}
  ~artisanLogin(){;}

};

artisanLogin oLogin;

string getNavMenu( void )
{  
  return "";
}


void showSignOnForm ( cgiScript & sc )
{
  sc.closeHeader(); 
  sc << oLogin.getLastError();
  
  // sc.Redirect("signIn.html"); 
       
}
