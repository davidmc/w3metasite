/*
  Scheme_FieldUI.cpp

  User Interface Definition and Implementation 
  for Scheme_Field.
  
  Copyright (c) 2005 by D.K. McCombs.
  
  davidmc@w3sys.com
  
  W3 Systems Design Inc.  
  
*/
#include <string>
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
using namespace std;
 // added to resolve compatibility issues where this may be a symbolic link to another project
#include "compatible.hpp"

#include "connectstring"

#include "w3intranet.h"
#include "Scheme_Field.hpp"

class Scheme_Field_List:  public list_base
{
public: 
  // Constructor
  Scheme_Field_List(cgiScript&sc,Session_Obj & session):list_base(sc,session){;}   
  ~Scheme_Field_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    hotCol=-2;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","Scheme_FieldUI.meta");
    // TODO:  Make sure you set the filter by name
    emitFilter( "Scheme_FieldUI.meta",
                "    <B>FILTER (by Table Name)</B>" );  
    string heading =
                     "<a class='sortcol' href='Scheme_FieldUI.meta?sort=f.Id'>Id</a>|"                     
                     "<a class='sortcol' href='Scheme_FieldUI.meta?sort=t.Name'>Table</a>|"                     
                     "<a class='sortcol' href='Scheme_FieldUI.meta?sort=f.Name'>Field</a>|"                     
                     "<a class='sortcol' href='Scheme_FieldUI.meta?sort=f.Col_Type'>Col_Type</a>"                     

                    ;
    emitHeadings(heading);
    getFilteredData(
                     "f.Id, " 
                     "t.Name, " 
                     "f.Name, " 
                     "f.Col_Type " 
                     ," Scheme_Field f inner join Scheme_Table t on t.Id = f.Scheme_Table ", 
                     // TODO: Make sure you set the filter by field                    
                     "t.Name like '$filter$%'" );   
    emitData();
    emitNavigation("Scheme_FieldUI.meta");         
    emitEnd();
    return breturn;
  }      
}; 

class Scheme_Field_form:  public Scheme_Field_Obj, public forms_base
{
public: 
  Scheme_Field_form(cgiScript & script):Scheme_Field_Obj(),forms_base(script){setKey(*this);} 
  virtual ~Scheme_Field_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "Scheme_Table", Scheme_Table);
    stringFXfer( "Name", Name);
    stringFXfer( "Col_Type", Col_Type);
    llongFXfer( "Links_To", Links_To);
 
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
    // TODO: Set the sql for combos, replace any fields with special controls, properly set lengths.
    script << makeTop("Scheme_FieldUI.meta", "Scheme_Field")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("Id", "Id", Id ,"8");
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");
    sql = " select Id, Name from Scheme_Table order by Name";
    script << makeComboBox("Table", "Scheme_Table", Scheme_Table ,sql);
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Name", "Name", Name ,"125","35");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Col Type", "Col_Type", Col_Type ,"125","35");
    script << "<br class='clearall'>" << endl; 
    sql = " select f.Id, concat(t.Name, ', ', f.Name) " 
            "from Scheme_Field f inner join Scheme_Table t on t.Id = f.Scheme_Table "
            "order by t.Name, f.Name";
    script << makeComboBox("Links To", "Links_To", Links_To ,sql, "None");
    script << "<br class='clearall'>" << endl; 
  
    script << makeButtons( key() );        
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};

bool intraMain(page & pg ,cgiScript & script)
{

  Scheme_Field_form myFrm(script); 
  Scheme_Field_List mylist(script,oLogin.Session());  

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
  baseUrl = "Scheme_FieldUI.meta";  // this is my compiled name!
  // because we are checking
  cgiScript script("text/html",false);
  bool isSignedOn = check(script);
  script.closeHeader();
  
  /* Replace intranet.meta with anything you please, or not,
     make sure an associated page entry exists in the metasite database. */
  script.ScriptName() =
    ocString( script.ScriptName().c_str()).replace("Scheme_FieldUI.meta","reports").c_str();    
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
    // try to be a regular cgi
    if( oLogin.testLoginStatus() )
    {
      script.closeHeader();
      cgiTemplates pgTemplate;    
      pgTemplate.load("Templates/adminPane.htmp");
      
      script << ocString(pgTemplate.getParagraph("top"))
                      .replaceAll("$heading$","Report");
  
      intraMain(pg ,script);
          
      ocString end = pgTemplate.getParagraph("bottom");
      script << end;          
    }
    else
    {
      script.Redirect("signIn.html"); 
    }
  }
  return 0;
}

// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"

