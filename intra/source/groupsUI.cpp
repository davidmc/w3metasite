/*
  groupsUI.cpp

  User Interface Definition and Implementation 
  for groups.
  
  Copyright (c) 2005 by D.K. McCombs.
  
  davidmc@w3sys.com
  
  W3 Systems Design Inc.  
  
*/
#include <string>
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
using namespace std;
#include "groups.hpp"

// one global
string baseUrl;

#include "w3intranet.h"
#include "sublist_base.hpp"

class services_List:  public sublist_base
{
public: 
  long long group_id;
  
  // Constructor
  services_List(cgiScript&sc, long long id):sublist_base(sc),group_id(id)
  {
    ;                   
  }  
  ~services_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","group_services_ui.cgi");
    hotCol=1;    
    ocString sql= "select sp.id, s.cat_name, s.menu_name from group_services sp "
                "inner join services s on s.id = sp.service_id where sp.group_id = "; 
    sql.append(group_id);
    emitHeadings("Id|Category|Service Name"); 
    getData(sql);
    emitData();    
    emitEnd();
    return breturn;
  }
  void derived_complete_event( void )
  {  
    ocString loco = "group_services_ui.cgi";
    if( group_id)
    {
      loco += "?group_id=";
      loco.append(group_id);
    }
    ocString adder = listTemplate.getParagraph("list_new");
    webIO << adder.replace("$cols","2")
                  .replace("$location",loco.c_str())
                  .replace("$item","Service");
  } 
  
  // override this to get the group_id in the link
  virtual void sendHotField( int iField, ocString & td )
  {
    ocString linkId = rs.getField(0).format();
    if( group_id)
    {
      linkId += "&group_id=";
      linkId.append(group_id);
    }
    webIO <<  td.replace( "$data$",
                    editLink.replaceAll( "$key$", linkId.c_str() )
                            .replaceAll( "$col$", rs.getField(iField).format().c_str() ).c_str());
  }
};
class groups_List:  public list_base
{
public: 
  // Constructor
  groups_List(cgiScript&sc,Session_Obj & session):list_base(sc,session){;}   
  ~groups_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;

    hotCol=-2;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","groupsUI.meta");
    emitFilter( "groupsUI.meta",
                "    <B>FILTER (by Name)</B>" );  
    string heading =
                     "<a class='sortcol' href='groupsUI.meta?sort=id'>id</a>|"                     
                     "<a class='sortcol' href='groupsUI.meta?sort=name'>name</a>|"                     
                     "<a class='sortcol' href='groupsUI.meta?sort=site_admin'>site_admin</a>"                     
                    ;
    emitHeadings(heading);
    getFilteredData(
                     "id, " 
                     "name, " 
                     "site_admin " 
                     ,"groups",                     
                     "name like '$filter$%'" );   
    emitData();
    emitNavigation("groupsUI.meta");         
    emitEnd();
    return breturn;
  }      
}; 

class groups_form:  public groups_Obj, public forms_base
{
public: 
  groups_form(cgiScript & script):groups_Obj(),forms_base(script){setKey(*this);} 
  virtual ~groups_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    stringFXfer( "name", name);
    boolFXfer( "site_admin", site_admin);
    // get all the group role id's
    stringFXfer( "role_id",  group_roles);
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
    
    script << makeTop("groupsUI.meta", "groups")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("id", "id", id ,"8");
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");
    
    script << makeTextBox("name", "name", name ,"25");
    script << "<br class='clearall'>" << endl; 
    script << makeBoolBox("site_admin", "site_admin", site_admin );
    script << "<br class='clearall'>" << endl; 
    script << makeComboBox("Roles","role_id",group_roles,
                           "select id,  name from roles", 
                           "", " multiple='multiple' size='6'" );  
    script << "<br class='clearall'>" << endl;                          
    
    if ( id != 0 )
    {
      script << "<div class='ctrlgroup'>"
                "<div class='lable'>GROUP Services:</div><div class='control'>";
      services_List svclist(script,id);
      svclist.loadListTemplates("Templates/poplist.htmp");  
      svclist.list_display();
      script << "</div></div>";
    }   
    script << "<br class='clearall'>" << endl; 
    script << makeButtons( key() );            
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};

bool intraMain(page & pg ,cgiScript & script)
{

  groups_form myFrm(script); 
  groups_List mylist(script,oLogin.Session());  

  script.closeHeader();
  cgiTemplates pgTemplate;    
  pgTemplate.load("Templates/adminPane.htmp");

  script << ocString(pgTemplate.getParagraph("top"))
                  .replaceAll("$heading$","groups");

  myFrm.loadControlTemplates("Templates/divform.htmp");  
  myFrm.form_action();  
  myFrm.form_display();

  mylist.loadListTemplates("Templates/navlist.htmp");  
  mylist.list_display();

  ocString end = pgTemplate.getParagraph("bottom");
  script << end;          
 
  return true;
}

// Standard main function
int main(int argc, char ** argv)
{  
  baseUrl = "groupsUI.meta";  // this is my compiled name!
  // because we are checking
  cgiScript script("text/html",false);
  bool isSignedOn = check(script);
  script.closeHeader();
  
  /* Replace intranet.meta with anything you please, or not,
     make sure an associated page entry exists in the metasite database. */
  script.ScriptName() =
    ocString( script.ScriptName().c_str()).replace("groupsUI.meta","services").c_str();  
    
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

