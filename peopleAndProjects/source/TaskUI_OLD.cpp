/*
  TaskUI.cpp

  User Interface Definition and Implementation 
  for Task.
  
  Copyright (c) 2006 by D.K. McCombs.
  
  davidmc@w3sys.com
  
  W3 Systems Design Inc.  
  
*/
#include <string>
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
using namespace std;

// One global
string baseUrl;

#include "w3intranet.h"
#include "Task.hpp"
#include "sublist_base.hpp"

// Definition
class Time_List:  public sublist_base
{
  
public: 
  ocString Parent_Id;
  double total;
  // Constructor
  Time_List(cgiScript&sc):sublist_base(sc),total(0){;}
  ~Time_List(){;}
  

  bool list_display( void )
  {
    bool breturn = true;
    string sql =  "select t.Id, "
      "t.Start,  "
      "t.Description,  "
      "t.Hours, s.Name  "
      "from Time t left join Status s on s.Id = t.Status where Task = " ;
    sql +=  Parent_Id;             
    hotCol=-2;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","Time_Popup.cgi");        
    string heading = "Id|"
      "Start|" 
      "Description|"
      "Hours|Status" 
      ;
    
    emitHeadings(heading);
    
    getData( sql );   
    emitData();
    
    webIO << "<tr><th colspan='4'>TOTAL: </th><td>";
    webIO << total;
    webIO << " hours</td></tr>" << endl;
     
    ocString list_new = listTemplate.getParagraph("list_new");
// TODO: Use correct Parent ID name 
    ocString url = "Time_Popup.cgi?Task=";
    url += Parent_Id;
    webIO << list_new.replace("$location",url.c_str()).replace("$cols","5");
    emitEnd();
    return breturn;
  } 
  virtual void sendHotField( int iField, ocString & td )
  {
    if( iField == 3 )
    {
      total += atof(rs.getField(iField).format().c_str());
    }
    webIO <<  td.replace( "$data$",
                          editLink.replaceAll( "$key$",rs.getField(0).format().c_str())
                          .replaceAll( "$col$",rs.getField(iField).format().c_str()).c_str());
  } 
}; 

class Task_List:  public list_base
{
public: 
  // Constructor
  Task_List(cgiScript&sc,Session_Obj & session):list_base(sc,session){;}   
  ~Task_List(){;}

  bool list_display( void )
  {
    bool breturn = true;

    hotCol=-2;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","TaskUI.meta");
    // TODOne:  Make sure you set the filter by name
    emitFilter( "TaskUI.meta",
                "    <B>FILTER (by Name)</B>" );  
    string heading =
      "<a class='sortcol' href='TaskUI.meta?sort=t.Id'>Id</a>|"                     
      "<a class='sortcol' href='TaskUI.meta?sort=p.Name'>Project</a>|"                     
      "<a class='sortcol' href='TaskUI.meta?sort=t.Name'>Name</a>|"                     
      "<a class='sortcol' href='TaskUI.meta?sort=t.Estimated_Time'>Estimated_Time</a>|"
      "<a class='sortcol' href='TaskUI.meta?sort=t.Actual_Time'>Actual_Time</a>|"
      "<a class='sortcol' href='TaskUI.meta?sort=t.Start'>Start</a>|"
      "<a class='sortcol' href='TaskUI.meta?sort=t.Task_Order'>Task Order</a>|"
      "<a class='sortcol' href='TaskUI.meta?sort=s.Name'>Status</a>|"
      "<a class='sortcol' href='TaskUI.meta?sort=t.Percent_Complete'>Percent_Complete</a>"                     
                    ;
    emitHeadings(heading);
    getFilteredData(
                     "t.Id, " 
                     "p.Name, " 
                     "t.Name, t.Estimated_Time, t.Actual_Time,"
                     "t.Start,  t.Task_Order, s.Name, " 
                     "t.Percent_Complete "
                     ,"Task t inner join Project p on t.Project = p.Id "
                      "inner join Status s on s.Id = t.Status " ,
                     // TODOne: Make sure you set the filter by field                    
                     "t.Name like '$filter$%'","p.Name, t.Task_Order" );   
    emitData();
    emitNavigation("TaskUI.meta");
    emitEnd();
    return breturn;
  }
};

class Task_form:  public Task_Obj, public forms_base
{
public: 
  Task_form(cgiScript & script):Task_Obj(),forms_base(script){setKey(*this);} 
  virtual ~Task_form(){;}

  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "Project", Project);
    stringFXfer( "Name", Name);
    doubleFXfer( "Estimated_Time", Estimated_Time);
    doubleFXfer( "Actual_Time", Actual_Time);
    stringFXfer( "Description", Description);
    dateFXfer( "Start", Start);
    longFXfer( "Task_Order", Task_Order);
    longFXfer( "Hierarchy", Hierarchy);
    llongFXfer( "Status", Status);
    longFXfer( "Percent_Complete", Percent_Complete);
    llongFXfer( "Parent_Task", Parent_Task);
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
    
    script << makeTop("TaskUI.meta", "Task")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("Id", "Id", Id ,"8");
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");



    ocString links = "";
    if(Project)
    {
      links = " &nbsp; <a href='ProjectUI.meta?Id=";
      links.append(Project);
      links += "'>Back to Project</a>";
    }
    sql = "select Id, Name from Project order by Name ";
    script << ocString(makeComboBox("Project", "Project", Project ,sql)).replace("<!--eogc-->",links.c_str());


    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Name", "Name", Name ,"125","35");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Estimated_Time", "Estimated_Time", Estimated_Time ,"8","8");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Actual_Time", "Actual_Time", Actual_Time ,"8","8");
    script << "<br class='clearall'>" << endl; 
    script << makeTextArea("Description", "Description", Description ,"5","100");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Start", "Start", Start ,"24","24");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Task_Order", "Task_Order", Task_Order ,"8","8");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Hierarchy", "Hierarchy", Hierarchy ,"8","8");
    script << "<br class='clearall'>" << endl; 
    
    
    sql = "select Id, Name from Status where Class='Task' order by Sequence ";
    links = "";
    if( Status > 0 )
    { 
      links = " &nbsp; <a href='javascript:jopen(\"Status_Popup.cgi?Id=";
      links.append(Status);
      links += "\",\"resizable,width=400,height=300\",\"status\")'>Edit</a>";
    }
    links += " &nbsp; <a  href='javascript:jopen(\"Status_Popup.cgi?Class=Task\","
      "\"resizable,width=400,height=300\",\"status\")'>Add</a>";
    
    script << ocString(makeComboBox("Status", "Status", Status ,sql))
      .replace("<!--eogc-->",links);
    
    
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Percent_Complete", "Percent_Complete", Percent_Complete ,"8","8");
    script << "<br class='clearall'>" << endl; 
    sql = "select Id, Name from Task where Project = ";
    sql.append(Project);
    sql += " order by Name ";
    script << makeComboBox("Parent_Task", "Parent_Task", Parent_Task ,sql,"None");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Estimate", "Estimate", Estimate ,"16","16");
    script << "<br class='clearall'>" << endl; 
  
    script << makeButtons( key() );
    
    if( key() )
    {
      Time_List TimeList(script);
      TimeList.Parent_Id.append(key()); 
      TimeList.loadListTemplates("Templates/spawninglist.htmp"); 
      script << "<br class='clearall'><div class='ctrlgroup'> <div class='label'>Time Entry:</div>" << endl; 
      TimeList.list_display();
      script << "</div>";
    } 
    
    
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};

bool intraMain(page & pg ,cgiScript & script)
{

  Task_form myFrm(script); 
  Task_List mylist(script,oLogin.Session());  

  script.closeHeader();
  
  myFrm.loadControlTemplates("Templates/divform.htmp");  
  myFrm.form_action();  
  myFrm.form_display();
  if( myFrm.Project > 0 )
  {
    mylist.addedCriteria = "t.Project = ";
    ocString pid;
    pid.append(myFrm.Project);
    mylist.addedCriteria += pid;
  }
  mylist.loadListTemplates("Templates/navlist.htmp");  
  mylist.list_display();
 
  return true;
}

// Standard main function
int main(int argc, char ** argv)
{  
  baseUrl = "TaskUI.meta";  // this is my compiled name!
  // because we are checking
  cgiScript script("text/html",false);
  bool isSignedOn = check(script);
  script.closeHeader();
  
  /* TODO: Optionally replace intranet.meta with anything you please, or not,
     make sure an associated page entry exists in the metasite database. 
  script.ScriptName() =
    ocString( script.ScriptName().c_str()).replace("TaskUI.meta","intranet.meta").c_str();  
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

