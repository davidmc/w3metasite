/*
  ls_usersUI.cpp

  User Interface Definition and Implementation 
  for users.
  
  Copyright (c) 2005 by D.K. McCombs.
  
  davidmc@w3sys.com
  
  W3 Systems Design Inc.  
  
*/
#include <string>
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
using namespace std;
#include "ls_users.hpp"

// one global
string baseUrl;

#include "w3intranet.h"

class users_List:  public list_base
{
public: 
  // Constructor
  users_List(cgiScript&sc,Session_Obj & session):list_base(sc,session){;}   
  ~users_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;

    hotCol=-2;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","ls_usersUI.meta");
    emitFilter( "ls_usersUI.meta",
                "    <B>Search (by Last Name)</B>" );
    string heading =
                     "<a class='sortcol' href='ls_usersUI.meta?sort=id'>id</a>|"                     
                     "<a class='sortcol' href='ls_usersUI.meta?sort=first'>first</a>|"                     
                     "<a class='sortcol' href='ls_usersUI.meta?sort=last'>last</a>|"                     
                     "<a class='sortcol' href='ls_usersUI.meta?sort=login'>login</a>|"                     
                     "<a class='sortcol' href='ls_usersUI.meta?sort=password'>password</a>|"                     
                     "<a class='sortcol' href='ls_usersUI.meta?sort=phone_number'>phone number</a>|"                     
                     "<a class='sortcol' href='ls_usersUI.meta?sort=email'>email</a>"
                    ;
    emitHeadings(heading);
    getFilteredData(
                     "id, " 
                     "first, " 
                     "last, " 
                     "login, " 
                     "password, " 
                     "phone, " 
                     "email " 
                     ,"users",                     
                     "last like '$filter$%'" );
    emitData();
    emitNavigation("ls_usersUI.meta");         
    emitEnd();
    return breturn;
  }      
}; 

class users_form:  public users_Obj, public forms_base
{
public: 
  users_form(cgiScript & script):users_Obj(),forms_base(script){setKey(*this);} 
  virtual ~users_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    stringFXfer( "first", first);
    stringFXfer( "last", last);
    stringFXfer( "login", login);
    stringFXfer( "password", password);
    stringFXfer( "phone", phone);
    stringFXfer( "email", email);
    stringFXfer( "mobile", mobile);
    stringFXfer( "fax", fax);
    
    stringFXfer( "role_id",  user_roles); 
  } 
  bool isupplemental( void )
  {
    return setRoles();
  }
  
  // event fired after update
  bool usupplemental( changeMap & changes )
  { 
    bool bRet = deleteRoles();
    if(bRet)  bRet = setRoles();
    return bRet; 
  }
  
  // event fired after delete
  bool dsupplemental( void )
  { 
    return deleteRoles(); 
  }
  // event fired after gettring data
  bool ssupplemental( void )
  {
    return retrieveRoles();
  } 
    
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    ocString sql; // for combo boxes
    
    script << makeTop("ls_usersUI.meta", "users")
           << makeHiddenBox("group_id", 0 )
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("id", "id", id ,"8"); 
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");

    script << makeTextBox("first", "first", first ,"125","25");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("last", "last", last ,"125","25");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("login", "login", login ,"12");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("password", "password", password ,"12");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("email", "email", email ,"125","25");
    script << "<br class='clearall'>" << endl;    
    script << makeTextBox("phone", "phone", phone ,"125","25");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("mobile", "mobile", mobile ,"125","25");
    script << "<br class='clearall'>" << endl;     
    script << makeTextBox("fax", "cax", fax ,"125","25");
    script << "<br class='clearall'>" << endl;     
     
    ocString select = "select r.id, r.name from roles r ";

    script << makeComboBox("Roles","role_id",user_roles,
                            select.c_str(), 
                            "", " multiple='multiple' size='6'" );     
    script << "<br class='clearall'>" << endl;  

    script << makeButtons( key() );        
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};

bool intraMain(page & pg ,cgiScript & script)
{

  users_form myFrm(script); 
  users_List mylist(script,oLogin.Session());  

  script.closeHeader();

  myFrm.loadControlTemplates("Templates/divform.htmp");  
  myFrm.form_action();  
  myFrm.form_display();

  mylist.loadListTemplates("Templates/navlist.htmp");  
  mylist.list_display();

  return true;
}

// Standard main function
int main(int argc, char ** argv)
{  
  baseUrl = "ls_usersUI.meta";  // this is my compiled name!
  // because we are checking
  cgiScript script("text/html",false);
  bool isSignedOn = check(script);
  script.closeHeader();
  
  /* Replace intranet.meta with anything you please, or not,
     make sure an associated page entry exists in the metasite database. */
  script.ScriptName() =
    ocString( script.ScriptName().c_str()).replace("ls_usersUI.meta","services").c_str();  
    
  page pg(script);  
  
  writelog( "load page" );  
  if( pg.load() )
  {    
    writelog( "instance of page control" );
    page_control ctrl(pg);     
    ctrl.addOp ( "servicelist",  new servicelist_functor(pg,script,isSignedOn) );    
    ctrl.addOp ( "userinterface",  new form_functor(pg,script,isSignedOn) );    
    writelog( "page control emit" );
    ctrl.emit();    
  }
  else
  {
    script << "<html><head><title>Page Unavailable</title></head>" << endl
           << "<body><h1>Sorry</h1>" << endl
           << "<p>The page you requested is not available</p></body></html>";
  } 
  return 0;
}

// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"

