/*
  rolesUI.cpp

  User Interface Definition and Implementation 
  for roles.
  
  Copyright (c) 2005 by D.K. McCombs.
  
  davidmc@w3sys.com
  
  W3 Systems Design Inc.
*/
#include <string>
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
using namespace std;
#include "roles.hpp"
#include "w3intranet.h"
#include "sublist_base.hpp"


class role_services_List:  public sublist_base
{
  
public: 
  ocString Parent_Id;
  // Constructor
role_services_List(cgiScript&sc):sublist_base(sc){;}
  ~role_services_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    string sql =  "select rs.id, "
      "s.menu_name,  "
      "rs.enabled,  "
      "rs.xml_params  "
      "from role_services rs "
      "inner join roles r on r.id = rs.role_id "
      "inner join services s on s.id = rs.service_id "
      "where rs.role_id = " ;
    sql +=  Parent_Id;
    hotCol=-2;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","role_services_Popup.cgi");
    string heading = "Id|"
      "service|"
      "enabled|"
      "xml_params"
      ;
    
    emitHeadings(heading);
    
    if( getData( sql ) )
    {
      emitData();
    }
 
    ocString list_new = listTemplate.getParagraph("list_new");
    ocString url = "role_services_Popup.cgi?role_id=";
    url += Parent_Id;
    webIO << list_new.replace("$location",url.c_str()).replace("$cols","5");
    emitEnd();
    return breturn;
  }
}; 
class roles_List:  public list_base
{
public: 
  // Constructor
  roles_List(cgiScript&sc,Session_Obj & session):list_base(sc,session){;}   
  ~roles_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;

    hotCol=-2;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","rolesUI.meta");
    emitFilter( "rolesUI.meta",
                "    <B>FILTER (by Name)</B>" );
    string heading =
                     "<a class='sortcol' href='rolesUI.meta?sort=id'>id</a>|"                     
                     "<a class='sortcol' href='rolesUI.meta?sort=name'>name</a>|"                     
                     "<a class='sortcol' href='rolesUI.meta?sort=comment'>comment</a>"                     
                    ;
    emitHeadings(heading);
    getFilteredData(
                     "id, " 
                     "name, " 
                     "comment " 
                     ,"roles",                     
                     "name like '$filter$%'" );
    emitData();
    emitNavigation("rolesUI.meta");         
    emitEnd();
    return breturn;
  }      
}; 

class roles_form:  public roles_Obj, public forms_base
{
public: 
  roles_form(cgiScript & script):roles_Obj(),forms_base(script){setKey(*this);} 
  virtual ~roles_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    stringFXfer( "name", name);
    stringFXfer( "comment", comment);    
  }

  // event fired after delete
  bool dsupplemental( void )
  {
    return deleteServices();
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
    
    script << makeTop("rolesUI.meta", "Roles")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("id", "id", id ,"8");
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");
    
    script << makeTextBox("name", "name", name ,"125", "25");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("comment", "comment", comment ,"125","25");
    script << "<br class='clearall'>" << endl;
    script << makeButtons( key() );
    
    if( key() )
    { script << "<br class='clearall'>" << endl;
      role_services_List cList(script);
      cList.Parent_Id.append(key());
      cList.loadListTemplates("Templates/spawninglist.htmp");
      script << "<h5>Role services:</h5>" << endl;
      cList.list_display();
    }
    
    script << makeBottom( m_result ) << endl;
    return breturn;
  }
};

bool intraMain(page & pg ,cgiScript & script)
{

  roles_form myFrm(script); 
  roles_List mylist(script,oLogin.Session());  


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
  baseUrl = "rolesUI.meta";  // this is my compiled name!
  // because we are checking
  cgiScript script("text/html",false);
  isSignedOn = check(script);
  script.closeHeader();
  
  /* Replace intranet.meta with anything you please, or not,
     make sure an associated page entry exists in the metasite database. */
  script.ScriptName() =
    ocString( script.ScriptName().c_str()).replace("rolesUI.meta","services").c_str();  
    
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

