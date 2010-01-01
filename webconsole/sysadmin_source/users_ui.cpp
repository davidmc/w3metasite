/*

  Users Form
  ===================================
  CGI User Interface for users
            
*/
#include "metaconnectstring"
#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "list_base.hpp"
#include "cgiTemplates.h"
#include "users.hpp"
#include "openLogin.h"
openLogin oLogin;

/*
  users Form 
    - Glues the business object to the form object through 
      multiple inheritance.      
                  
*/                 
class users_Form:  public users, public forms_base
{
public: 
  // Constructor
  users_Form(cgiScript & script):users(),forms_base(script){setKey(*this);}  
  ~users_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
  
    llongFXfer( "group_id", group_id );
    stringFXfer( "first", first );
    stringFXfer( "last", last );
    stringFXfer( "login", login );
    stringFXfer( "password", password );
    stringFXfer( "phone_number", phone_number );
    stringFXfer( "email", email );
    if( group_id != 0 ) stringFXfer( "role_id",  user_roles);
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
    ocString group_sql = "select id, name from groups";
    if( !oLogin.SiteAdmin() )
    {
      group_sql += " where id = ";
      group_sql += oLogin.GroupId();
    }
    group_sql += " order by name";
    script << endl << "<!--" << group_sql << "-->" << endl;
    script << makeTop("users_ui.cgi", "USERS")
           << makeStaticBox("Id","id",id,"12" )
           << makeComboBox("Group", "group_id", group_id, 
                            group_sql)                
           << makeTextBox("First","first",first,"24")
           << makeTextBox("Last","last",last,"24")
           << makeTextBox("Login","login",login,"24")
           << makeTextBox("Password","password",password,"12")
           << makeTextBox("Phone Number","phone_number",phone_number,"16")
           << makeTextBox("Email","email",email,"64");
    if( group_id != 0 )
    {
      ocString select = "select r.id, r.name from roles r inner join group_roles gr on "
                        "r.id = gr.role_id where gr.group_id = ";
      select.append(group_id);                   
      script
           << makeComboBox("Roles","role_id",user_roles,
                           select.c_str(), 
                           "", " multiple='multiple' size='6'" );  
    }                                    
    script << makeButtons( key() )
           << makeBottom( m_result ) << endl; 
    return breturn;
  }      
};

class users_List:  public list_base
{
public: 
  // Constructor
  users_List(cgiScript&sc):list_base(sc)
  {
    ;                   
  }  
  ~users_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","users_ui.cgi");
    hotCol=1;    
    
    emitFilter( "users_ui.cgi",  // program name
                "&nbsp;&nbsp;&nbsp;&nbsp;<B>FILTER</B>" ); // Label
    
    emitHeadings("Id|User Name");
    if( !oLogin.SiteAdmin() )
    {  
       addedCriteria = " group_id = ";
       addedCriteria += oLogin.GroupId();
    } 
    getFilteredData( "id, concat( first, ' ', last) as name", 
                     "users", 
                     "last like '$filter$%'", 
                     "last, first" );     
    emitData();
    
    emitNavigation("users_ui.cgi");
    
    emitEnd();
    return breturn;
  }
  void derived_data_event( void )
  {  
      
  }
        
};

int main( int argc, char ** argv )
{  
  cgiScript script( "text/html", false );
  users_Form myFrm(script); 
  users_List mylist(script); // so you can set cookies 
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
            .replaceAll("$heading$","USERS");

    myFrm.loadControlTemplates("Templates/form.htmp");  
    myFrm.form_action();  
    myFrm.form_display();
    
    
    mylist.loadListTemplates("Templates/list.htmp");  
    mylist.list_display();
    
    script << pgTemplate.getParagraph("bottom");
  }
  else
  {
    script.Redirect("signIn.html"); 
  } 
}

// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"


 
