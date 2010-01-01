/*
  servicesUI.cpp

  User Interface Definition and Implementation 
  for services.
  
  Copyright (c) 2005 by D.K. McCombs.
  
  davidmc@w3sys.com
  
  W3 Systems Design Inc.
*/
#include <string>
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
using namespace std;
#include "services.hpp"


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
    editLink = editLink.replace("$prog$","servicesUI.meta");
    emitFilter( "servicesUI.meta",
                "    <B>FILTER (by menu_name)</B>" );  
    string heading =
                     "<a class='sortcol' href='servicesUI.meta?sort=id'>id</a>|"                     
                     "<a class='sortcol' href='servicesUI.meta?sort=sequence'>sequence</a>|"                     
                     "<a class='sortcol' href='servicesUI.meta?sort=tree_depth'>tree_depth</a>|"                     
                     "<a class='sortcol' href='servicesUI.meta?sort=enabled'>enabled</a>|"                     
                     "<a class='sortcol' href='servicesUI.meta?sort=cat_name'>cat_name</a>|"                     
                     "<a class='sortcol' href='servicesUI.meta?sort=menu_name'>menu_name</a>|"                     
                     "<a class='sortcol' href='servicesUI.meta?sort=uri'>uri</a>|"                     
                     "<a class='sortcol' href='servicesUI.meta?sort=local_uri'>local_uri</a>|"                     
                     "<a class='sortcol' href='servicesUI.meta?sort=secure'>secure</a>|"                     
                     "<a class='sortcol' href='servicesUI.meta?sort=xml_params'>xml_params</a>"                     
                    ;
    emitHeadings(heading);
    getFilteredData(
                     "id, " 
                     "sequence, " 
                     "tree_depth, " 
                     "enabled, " 
                     "cat_name, " 
                     "menu_name, " 
                     "uri, " 
                     "local_uri, " 
                     "secure, " 
                     "xml_params " 
                     ,"services",                     
                     "menu_name like '$filter$%'" );   
    emitData();
    emitNavigation("servicesUI.meta");         
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
    shortFXfer( "tree_depth", tree_depth);
    boolFXfer( "enabled", enabled);
    stringFXfer( "cat_name", cat_name);
    stringFXfer( "menu_name", menu_name);
    stringFXfer( "uri", uri);
    boolFXfer( "local_uri", local_uri);
    boolFXfer( "secure", secure);
    stringFXfer( "xml_params", xml_params);
 
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
    
    script << makeTop("servicesUI.meta", "services")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("id", "id", id ,"8");
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");
    
    script << makeTextBox("sequence", "sequence", sequence ,"8","8");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("tree_depth", "tree_depth", tree_depth ,"4","4");
    script << "<br class='clearall'>" << endl; 
    script << makeBoolBox("enabled", "enabled", enabled );
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("cat_name", "cat_name", cat_name ,"25");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("menu_name", "menu_name", menu_name ,"25");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("uri", "uri", uri ,"225" ,"25");
    script << "<br class='clearall'>" << endl; 
    script << makeBoolBox("local_uri", "local_uri", local_uri );
    script << "<br class='clearall'>" << endl; 
    script << makeBoolBox("secure", "secure", secure );
    script << "<br class='clearall'>" << endl; 
    
    script << makeTextArea( "xml_params", "xml_params", xml_params, "4", "50" );
    // makeTextBox("xml_params", "xml_params", xml_params ,"25");
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
  baseUrl = "servicesUI.meta";  // this is my compiled name!
  // because we are checking
  cgiScript script("text/html",false);
  isSignedOn = check(script);
  script.closeHeader();  
  page pg(script);  
  script.ScriptName() =
    ocString( script.ScriptName().c_str()).replace("servicesUI.meta","services").c_str();
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
