/*
  services_liteUI.cpp

  User Interface Definition and Implementation 
  for services.

  Copyright (c) 2005 - 2007 by D.K. McCombs.

  davidmc@w3sys.com

  W3 Systems Design Inc.
*/
#include <string>
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
using namespace std;
#include "services_lite.hpp"


#include "w3intranet.h"

class services_List:  public list_base
{
public: 
  // Constructor
  services_List(cgiScript&sc,Session_Obj & session):list_base(sc,session){;}  
  ~services_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;

    hotCol=-2;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","services_liteUI.meta");
    emitFilter( "services_liteUI.meta",
                "<B>Search by Menu Name:</B>" );
    string heading =
                     "<a class='sortcol' href='services_liteUI.meta?sort=id'>id</a>|"
                     "<a class='sortcol' href='services_liteUI.meta?sort=sequence'>sequence</a>|"
                     "<a class='sortcol' href='services_liteUI.meta?sort=tree_depth'>tree_depth</a>|"
                     "<a class='sortcol' href='services_liteUI.meta?sort=enabled'>enabled</a>|"
                     "<a class='sortcol' href='services_liteUI.meta?sort=cat_name'>cat_name</a>|"
                     "<a class='sortcol' href='services_liteUI.meta?sort=menu_name'>menu_name</a>|"
                     "<a class='sortcol' href='services_liteUI.meta?sort=uri'>uri</a>"
                    ;
    emitHeadings(heading);
    getFilteredData(
                     "id, " 
                     "sequence, " 
                     "tree_depth, " 
                     "enabled, " 
                     "cat_name, " 
                     "menu_name, " 
                     "uri "
                     ,"services",
                     "menu_name like '$filter$%'" );   
    emitData();
    emitNavigation("services_liteUI.meta");         
    emitEnd();
    return breturn;
  }
};

class services_form:  public services_Obj, public forms_base
{
public: 
  services_form(cgiScript & script):services_Obj(),forms_base(script){setKey(*this);} 
  virtual ~services_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    intFXfer( "sequence", sequence);
    intFXfer( "tree_depth", tree_depth);
    boolFXfer( "enabled", enabled);
    stringFXfer( "cat_name", cat_name);
    stringFXfer( "menu_name", menu_name);
    stringFXfer( "uri", uri);
    stringFXfer( "target", target);
    intFXfer( "prog_enum", prog_enum);
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

    script << makeTop("services_liteUI.meta", "Services")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("id", "id", id ,"8");
    script << "<br class='clearall'>" << endl;
    script << makeTextBox("Target", "target", target ,"125","35");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Program enumeration", "prog_enum", prog_enum ,"8","8");
    script << "<br class='clearall'>" << endl;
    
    script << formTemplate.getParagraph("advanced_end");
    script << makeTextBox("sequence", "sequence", sequence ,"8","8");
    // script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Tree depth", "tree_depth", tree_depth ,"4","4");
    script << "<br class='clearall'>" << endl; 
    script << makeBoolBox("Enabled?", "enabled", enabled );
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Cat name", "cat_name", cat_name ,"255", "35");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Menu name", "menu_name", menu_name ,"255","35");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("URL", "uri", uri ,"512" ,"55");
    script << "<br class='clearall'>" << endl;

    script << makeButtons( key() );        
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};

bool intraMain(page & pg ,cgiScript & script)
{

  services_form myFrm(script); 
  services_List mylist(script,oLogin.Session());  

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
  baseUrl = "services_liteUI.meta";  // this is my compiled name!
  // because we are checking
  cgiScript script("text/html",false);
  isSignedOn = check(script);
  script.closeHeader();  
  page pg(script);  
  script.ScriptName() =
    ocString( script.ScriptName().c_str()).replace("services_liteUI.meta","Services").c_str();
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
           << "<p>The page you requested "
           << script.ScriptName()
           << " is not available</p></body></html>";
  } 
  return 0;
}

// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"
