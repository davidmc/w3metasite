/*

  Report_Templates_ui.cpp

  User Interface Definition and Implementation 
  for Report_Templates.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 System Design  
  
*/
#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "cgiTemplates.h"
#include "openLogin.h"
#include "Report_Templates.hpp"
#include "list_base.hpp"
#include "forms_base.hpp"
#include "siteLimit.h"

using namespace std;
openLogin oLogin;

class Report_Templates_List:  public list_base
{
public: 
  // Constructor
  Report_Templates_List(cgiScript&sc):list_base(sc){;}  
  ~Report_Templates_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    string siteLimitation = siteFocus(oLogin);
    if( siteLimitation.length() != 0 )
    {
      addedCriteria = " Site_Id ";
      addedCriteria += siteLimitation;
    } 
    hotCol=-2;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","Report_Templates_ui.cgi");
    emitFilter( "Report_Templates_ui.cgi",
                "    <B>FILTER (by Name)</B>" );  
    string heading =
                     "<a class='sortcol' href='Report_Templates_ui.cgi?sort=Id'>Id</a>|"                     
                     "<a class='sortcol' href='Report_Templates_ui.cgi?sort=Name'>Name</a>|"                     
                     "<a class='sortcol' href='Report_Templates_ui.cgi?sort=Path'>Path</a>|"                     
                    ;
    emitHeadings(heading);
    getFilteredData(
                     "Id, " 
                     "Name, " 
                     "Path " 
                     ,"Report_Templates",                     
                     "Name like '$filter$%'" );   
    emitData();
    emitNavigation("Report_Templates_ui.cgi");         
    emitEnd();
    return breturn;
  }      
}; 

class Report_Templates_form:  public Report_Templates_Obj, public forms_base
{
public: 
  Report_Templates_form(cgiScript & script):Report_Templates_Obj(),forms_base(script){setKey(*this);} 
  virtual ~Report_Templates_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "Site_Id", Site_Id);
    stringFXfer( "Name", Name);
    stringFXfer( "Path", Path);
 
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    ocString sql= "select id, name from metasite.sites"; // for combo boxes
    string siteLimitation = siteFocus(oLogin);
    if( siteLimitation.length() != 0 )
    {
      sql += " where id ";
      sql += siteLimitation;
    }
    sql += " order by name";
    script << makeTop("Report_Templates_ui.cgi", "Report_Templates")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("Id", "Id", Id ,"8");
    script << "<br class='clearall'>" << endl; 
    
    script << makeComboBox("Site_Id", "Site_Id", Site_Id ,sql);
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end"); 
    script << makeTextBox("Name", "Name", Name ,"25");
    script << "<br class='clearall'>" << endl; 
    script << makeFileBox( "Path", "Path", Path, "55" );
    script << "<br class='clearall'>" << endl; 
    
    script << makeButtons( key() );        
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  Report_Templates_form myFrm(script); 
  Report_Templates_List mylist(script);  
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))                     
                     .replace("filePick.cgi","repTemplatePick.cgi");

    myFrm.loadControlTemplates("Templates/divform.htmp");  
    myFrm.form_action();  
    myFrm.form_display();
    
    mylist.loadListTemplates("Templates/list.htmp");  
    mylist.list_display();
        
    ocString end = pgTemplate.getParagraph("bottom");
    script << end;          
  }
  else
  {
    script.Redirect("signIn.html"); 
  } 
}
// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"

