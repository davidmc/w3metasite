/*
  ProjectUI.cpp

  User Interface Definition and Implementation 
  for Project.

  Copyright (c) 2006 by D.K. McCombs.

  davidmc@w3sys.com

  W3 Systems Design Inc.  

*/
#include <string>
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
using namespace std;

#include "w3intranet.h"
#include "Project.hpp"
#include "sublist_base.hpp"

// Definition

class Task_List:  public sublist_base
{
  
public: 
  ocString Parent_Id;
  // Constructor
Task_List(cgiScript&sc):sublist_base(sc){;}
  ~Task_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    string sql =  "select t.Id, "
      "t.Name,  "
      "t.Estimated_Time,  "
      "t.Actual_Time,  "
      "t.Description,  "
      "t.Start,  "
      "s.Name,  "
      "t.Percent_Complete,  "
      "t.Estimate  "
      "from Task t left join Status s on s.Id = t.Status where Project = " ;
    sql +=  Parent_Id;
    sql += " order by  Task_Order";
    hotCol=-2;
    editLink = listTemplate.getParagraph("jopenspecial");
    editLink = editLink.replace("$prog$","Task_Popup.cgi");
    string heading = "Id|"
      "Name|"
      "Estimated Time|"
      "Actual Time|"
      "Description|"
      "Start|"
      "Status|"
      "% Complete|"
      "Estimate"
      ;
             
    emitHeadings(heading);
    // webIO << sql;
    getData( sql );
    emitData();
    ocString list_new = listTemplate.getParagraph("peernew");
    ocString url = "javascript:jopen('Task_Popup.cgi?Project=";
    url += Parent_Id;
    url += "','resizable,width=600,height=400,scrollbars','Tasks')";
    webIO << list_new.replace("$location",url.c_str()).replace("$cols","9");
    emitEnd();
    return breturn;
  }
}; 
class Project_List:  public list_base
{
public: 
  // Constructor
  Project_List(cgiScript&sc,Session_Obj & session):list_base(sc,session){;}   
  ~Project_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;

    hotCol=-2;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","ProjectUI.meta");
    // TODO:  Make sure you set the filter by name
    emitFilter( "ProjectUI.meta",
                "    <B>FILTER (by Name)</B>" );  
    string heading =
      "<a class='sortcol' href='ProjectUI.meta?sort=p.Id'>Id</a>|"
      "<a class='sortcol' href='ProjectUI.meta?sort='Name'>Name</a>"
                    ;
    emitHeadings(heading);
    getFilteredData(
                     "p.Id, "
                     "concat(c.Name, ' - ', p.Name ) as 'Name' "
                     ,"Project p inner join Company c on c.Id = p.Company ", 
                     "concat(c.Name, ' - ', p.Name ) like '$filter$%'", "p.Status, c.Name, p.Name " );
    emitData();
    emitNavigation("ProjectUI.meta");         
    emitEnd();
    return breturn;
  }      
}; 

class Project_form:  public Project_Obj, public forms_base
{
public: 
  Project_form(cgiScript & script):Project_Obj(),forms_base(script){setKey(*this);} 
  virtual ~Project_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    stringFXfer( "Name", Name);
    llongFXfer( "Company", Company);
    dateFXfer( "Estimated_Start", Estimated_Start);
    dateFXfer( "Actual_Start", Actual_Start);
    longFXfer( "Estimated_Time", Estimated_Time);
    longFXfer( "Actual_Time", Actual_Time);
    longFXfer( "Phase", Phase);
    llongFXfer( "Parent_Project", Parent_Project);
    stringFXfer( "Notes", Notes);
    llongFXfer( "Status", Status);
    moneyFXfer( "Rate", Rate);
    stringFXfer( "PO", PO);
    moneyFXfer( "Estimate", Estimate);
 
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
    
    script << makeTop("ProjectUI.meta", "Project")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("Id", "Id", Id ,"8");
    script << "<br class='clearall'>" << endl;
    sql = "select Id, Name from Project order by Name ";
    script << makeComboBox("Parent_Project", "Parent_Project", Parent_Project ,sql,"None");
    script << "<br class='clearall'>" << endl;
    
    script << formTemplate.getParagraph("advanced_end");
    
    script << makeTextBox("Name", "Name", Name ,"125","35");
    script << "<br class='clearall'>" << endl; 
    sql = "select Id, Name from Company order by Name ";
    script << makeComboBox("Company", "Company", Company ,sql);
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Estimated_Start", "Estimated_Start", Estimated_Start ,"24","24");
    script << makeTextBox("Actual_Start", "Actual_Start", Actual_Start ,"24","24");
    script << "<br class='clearall'>" << endl;
    
    script << ocString(makeTextBox("Estimated_Time", "Estimated_Time", Estimated_Time ,"8","8"))
        .replace("<!--eogc-->"," hours " );
    script << ocString(makeTextBox("Actual_Time", "Actual_Time", Actual_Time ,"8","8"))
        .replace("<!--eogc-->",   " hours " );
    script << "<br class='clearall'>" << endl;
;
    script << makeTextBox("Rate", "Rate", Rate ,"16","16");
    script << makeTextBox("Estimate", "Estimate", Estimate ,"16","16");
    script << "<br class='clearall'>" << endl;
    
    script << makeTextBox("Phase", "Phase", Phase ,"8","8");
    script << "<br class='clearall'>" << endl;
    
    script << makeTextArea("Notes", "Notes", Notes ,"2","45");
    script << "<br class='clearall'>" << endl;
    
    sql = "select Id, Name from Status where Class='Project' order by Sequence ";
    ocString links = "";
    if( Status > 0 )
    { 
      links = " &nbsp; <a href='javascript:jopen(\"Status_Popup.cgi?Id=";
      links.append(Status);
      links += "\",\"resizable,width=400,height=300\",\"status\")'>Edit</a>";
    }
    links += " &nbsp; <a  href='javascript:jopen(\"Status_Popup.cgi?Class=Project\","
      "\"resizable,width=400,height=300\",\"status\")'>Add</a>";
    
    script << ocString(makeComboBox("Status", "Status", Status ,sql))
      .replace("<!--eogc-->",links);


    script << makeTextBox("PO", "PO", PO ,"125","35");
    script << "<br class='clearall'>" << endl; 
  
    script << makeButtons( key() );
    if( key() )
    {
      script << "<br class='clearall'>" << endl;
      Task_List TaskList(script);
      TaskList.Parent_Id.append(key());
      TaskList.loadListTemplates("Templates/spawninglist.htmp");
      script << "<div class='label'>Tasks:</div>" << endl;
      TaskList.list_display();
    }  
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};

bool intraMain(page & pg ,cgiScript & script)
{

  Project_form myFrm(script); 
  Project_List mylist(script,oLogin.Session());  

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
  baseUrl = "ProjectUI.meta";  // this is my compiled name!
  // because we are checking
  cgiScript script("text/html",false);
  isSignedOn = check(script);
  script.closeHeader();
  
  /* TODO: Optionally replace intranet.meta with anything you please, or not,
     make sure an associated page entry exists in the metasite database. 
  script.ScriptName() =
    ocString( script.ScriptName().c_str()).replace("ProjectUI.meta","intranet.meta").c_str();  
  */  
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

