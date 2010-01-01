/*
  TaskUI.cpp

  User Interface Definition and Implementation
  for Task.
  Copyright (c) 2007 by D.K. McCombs.

  davidmc@w3sys.com
  W3 Systems Design Inc.

  May: 
   "Detail","show"
   "Name","edit"
   "Task_Order","edit"
   "Status","edit
   "Percent_Complete","edit"
  Ex:
   <Task_Order>edit</Task_Order>
   <Name>edit</Name>
   <Status>edit</Status>
   <Percent_Complete>edit</Percent_Complete>
   <Detail>show</Detail>
   <Time>enter</Time>

*/       
#include <string>
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
using namespace std;

#include "w3intranet.h"
#include "Task.hpp"
#include "sublist_base.hpp"
#include "diaButtons.h"
#include "permittedActions.hpp"

class Task_form:  public Task_Obj, public forms_base
{
  public:
    string prefix;
    
    Task_form(cgiScript & script):Task_Obj(),forms_base(script),prefix("Task_")
    {
      setKey(*this);
    }
    virtual ~Task_form(){;}
  
    void form_id_transfer( void ) {;}
    void form_data_transfer( void ) {;}
  
    bool dbf_action( string mode, changeMap & changes )
    {
      bool isOK = true;
      if( mode == "d" )
      {
        // This is the responsibility of the calling object!
      }
      else
      {
        cgiInput & args = script.ClientArguments();
        string key = prefix+"Id";
        long test = 0;
        ocString IdVar = key;
        IdVar.append(test);
        while( args.TheMap().find( IdVar.c_str() ) != args.TheMap().end() )
        {

          ocString NameVar = prefix+"Name";
          NameVar.append(test);
          ocString DescriptionVar = prefix+"Description";
          DescriptionVar.append(test);
          ocString StartVar = prefix+"Start";
          StartVar.append(test);
          ocString Task_OrderVar = prefix+"Task_Order";
          Task_OrderVar.append(test);
          ocString StatusVar = prefix+"Status";
          StatusVar.append(test);
          ocString Percent_CompleteVar = prefix+"Percent_Complete";
          Percent_CompleteVar.append(test);
          ocString Estimated_EndVar = prefix+"Estimated_End";
          Estimated_EndVar.append(test);
          if(
             changes.find(NameVar) != changes.end() ||
             changes.find(StartVar) != changes.end() ||
             changes.find(Task_OrderVar) != changes.end() ||
             changes.find(StatusVar) != changes.end() ||
             changes.find(Percent_CompleteVar) != changes.end() ||
             changes.find(Estimated_EndVar) != changes.end() 
            )
          {
            // Normally the form name and data name match exactly - this is not so here.
            changeMap nchanges;
            if( changes.find(NameVar) != changes.end() )  nchanges["Name"]="Name";
            if( changes.find(StartVar) != changes.end() )  nchanges["Start"]="Start";
            if( changes.find(Task_OrderVar) != changes.end() )  nchanges["Task_Order"]="Task_Order";
            if( changes.find(StatusVar) != changes.end() )  nchanges["Status"]="Status";
            if( changes.find(Percent_CompleteVar) != changes.end() )  nchanges["Percent_Complete"]="Percent_Complete";
            if( changes.find(Estimated_EndVar) != changes.end() )  nchanges["Estimated_End"]="Estimated_End";
            
            llongFXfer( IdVar, Id);
            stringFXfer( NameVar, Name);
            dateFXfer( StartVar, Start);
            longFXfer( Task_OrderVar, Task_Order);
            llongFXfer( StatusVar, Status);
            longFXfer( Percent_CompleteVar, Percent_Complete);
            dateFXfer( Estimated_EndVar, Estimated_End);
            
            if( Id > 0 && NameVar.length() == 0 )
            {
              this->key(Id);
              db_delete();
            }
            else if( Id == 0 && NameVar.length() > 0 )
            {
              this->key(Id);
              db_insert();
            }
            else if( mode == "u" )
            {
              this->key(Id);
              db_update( nchanges );
            }
          }
          test++;
          IdVar = key;
          IdVar.append(test);
        }
      }
      return isOK;
    }
  
  // implement TRIVIAL pure virtual of form display
    bool form_display( void ) { return true; }
};

class Task_List:  public list_base
{
public:
  // what can they do in this dialog?
  permittedActions actions;

  long items;
  Task_form form;
  

  
  // Constructor
  Task_List(cgiScript&sc,Session_Obj & session,string sessionName):list_base(sc,session,sessionName),items(0),form(sc)
  {
    form.loadControlTemplates("Templates/listformelements.htmp");
    itemsPerPg = 12;
    
    oLogin.fetchServices();
    // see what we are allowed to do as a task service
    actions.loadByEnum(svcenm_task,oLogin.Services());
  }
  ~Task_List(){;}

  bool list_display( void )
  {
    bool breturn = true;

    hotCol=-1;    

    string heading =
        "<a class='sortcol' href='TaskUI.meta?sort=t.Id'>ID</a>\nDTL/MSG|"
      "<a class='sortcol' href='TaskUI.meta?sort=c.Name,p.Name'>Project\n"
      "<a class='sortcol' href='TaskUI.meta?sort=t.Name'>Task</a>|"
      "<a class='sortcol' href='TaskUI.meta?sort=t.Start'>Started</a>\n"
      "<a class='sortcol' href='TaskUI.meta?sort=t.Estimated_End'>Due</a>|"
      "<a class='sortcol' href='TaskUI.meta?sort=t.Task_Order'>Priority</a>\nAttachments|"
      "<a class='sortcol' href='TaskUI.meta?sort=s.Name'>Status</a>\n"
      "<a class='sortcol' href='TaskUI.meta?sort=t.Percent_Complete'>% Complete</a>"
                    ;
    emitHeadings(heading);
    getFilteredData(
                     "distinct t.Id, " 
                      "concat(c.Name,  ' - ', p.Name), "
                     "t.Name, "
                     "t.Start,  t.Task_Order, t.Status, "
                     "t.Percent_Complete, t.Estimated_End "
                     ,"Task t left join Project p on t.Project = p.Id "
                      "left join Company c on c.Id = p.Company "
                      "left join Status s on s.Id = t.Status "
                      "left join Task_Assignment ta on ta.Task = t.Id " ,
                     "","t.Task_Order" );
    emitData();
    emitNavigation("TaskUI.meta");
    emitEnd();
    return breturn;
  }
        
  void derived_data_event( void )
  {
    items++;
  }
  string makeComboPairs ( string sql )
  {
    string ret;
    if( sql.length() )
    {
      quickQuery qq;
      openRS & rs = qq.getData(sql);
      for( bool result=qq.opened; result; result=rs.next() )
      {
        string label = "";
        for( int lx=1; lx<rs.getFieldCount(); lx++ )
        {
          if( label.length() && rs.getField(lx).format().length() ) label += " - ";
          label += rs.getField(lx).format();
        }
        if( ret.length() ) ret += ",";
        ret+=label;
        ret += "=";
        ret+= rs.getField(0).format();
      }
    }
    return ret;
  }
  virtual void sendField( int iField, ocString & td )
  {
    // editLink = listTemplate.getParagraph("hotcolumn");  
    // editLink = editLink.replace("$prog$","TaskUI.meta");
    ocString ctrlName;
    ocString ctrlSource;
    ocString sql;
    ocString links;
    
    switch( iField )
    {
        // Id Col
      case 0:
        ctrlName  = form.prefix+"Id";
        ctrlName.append(items);
        links = "<input type='hidden' name='";
        links += ctrlName;
        links += "' id='";
        links += ctrlName;
        links += "' value='";
        links += rs.getField(iField).format();
        links += "'>";
        if(  actions.May("Detail","show") )
        {
          links +="<a href=\"javascript:jopen('Task_Popup.cgi?Id=";
          links += rs.getField(iField).format();
          links += "','resizable,width=625,height=600,scrollbars','Tasks')\">Detail</a>\n";
        } 
        links += "<a href=\"javascript:jopen('Messages_Popup.cgi?Task=";
        links += rs.getField(iField).format();
        links += "','resizable,width=625,height=600,scrollbars','Messages')\">Messages</a>";
        
        webIO << td.replace( "$data$", links );
        break;
        // Project Col
      case 1:
        ctrlName  = form.prefix+"Project";
        ctrlName.append(items);
        ctrlSource = rs.getField(1).format();
        ctrlSource += "<br class='clearall'>";
        // Name Col
        ctrlName  = form.prefix+"Name";
        ctrlName.append(items);
        if(  actions.May("Name","edit") )
        {
        ctrlSource += form.makeTextBox( "Name", ctrlName,
                                        rs.getField(2).format() ,"225","35");
        }
        else
        {
          ctrlSource += form.makeStaticBox( "Name", ctrlName,
                                          rs.getField(2).format() ,"35");
        }
        webIO << td.replace( "$data$", ctrlSource  );
        break;

        // Start col
      case 3:
        ctrlName  = form.prefix+"Start";
        ctrlName.append(items);
        ctrlSource = form.makeDateBox( "Start", ctrlName, rs.getField(iField).format().c_str(),"10" );
        ctrlSource += "<br class='clearall'>";
        ctrlName  = form.prefix+"Estimated_End";
        ctrlName.append(items);
        ctrlSource += form.makeDateBox( "Due", ctrlName, rs.getField(7).format().c_str(),"10" );
        webIO << td.replace( "$data$", ctrlSource );
        break;
        // Order Col  
      case 4:
        ctrlName  = form.prefix+"Task_Order";
        ctrlName.append(items);
        if( actions.May("Task_Order","edit") )
        {
          ctrlSource = form.makeTextBox( "Task_Order", ctrlName,
                                          rs.getField(iField).format() ,"8","3");
        }
        else
        {
          ctrlSource = form.makeStaticBox( "Task_Order", ctrlName,
                                           rs.getField(iField).format() ,"3");
        }
        ctrlSource += "<br class='clearall'><a href=\"javascript:jopen('Attachments_Popup.cgi?Task=";
        ctrlSource += rs.getField(0).format();
        ctrlSource += "','resizable,width=625,height=600,scrollbars','Tasks')\">Attachments</a>";
        webIO << td.replace( "$data$", ctrlSource );
        break;
        // Status Col
      case 5:
        ctrlName  = form.prefix+"Status";
        ctrlName.append(items);
        sql = "select Id, Name from Status where Class='Task' ";
        if( !actions.May("Status","edit") )
        {
          // limit to current value
          sql += " and Id = ";
          sql += rs.getField(iField).format();
        }
        sql += " order by Sequence ";
        // DBG  webIO << sql << endl
        ctrlSource = form.makeComboBox( "Status", ctrlName,
                                         rs.getField(iField).format() ,sql);
        ctrlSource += "<br class='clearall'>";
        ctrlName  = form.prefix+"Percent_Complete";
        ctrlName.append(items);
        string vals;
        if( actions.May("Percent_Complete","edit") )
        {
          vals = "0%=0,10%=10,20%=20,30%=30,"
                 "40%=40,50%=50,60%=60,"
                 "70%=70,80%=80,90%=90,100%=100";
        }
        else
        {
          vals = rs.getField(6).format();
          vals += "%=";
          vals += rs.getField(6).format();
        }
        ctrlSource +=  form.makeManualComboBox( "Percent_Complete", ctrlName,
                                                 rs.getField(6).format(),  vals );
        webIO << td.replace( "$data$", ctrlSource );
        break;

    }
  }
}; 

class Task_search_form: public forms_base
{
  llong Project;
  string Name;
  oc_date Start;
  llong Status;
  bool showAll;
  llong Task_Assignment;
public:
  ocString addedCriteria;

  Task_search_form(cgiScript & script):forms_base(script),Project(0),Status(0),Start(),showAll(false)
  {
    Start.parse("01/01/2000");
  }
  virtual ~Task_search_form(){;}

  void form_id_transfer( void )
  {
    ;
  }
  void form_data_transfer( void )
  {
    llongFXfer( "Project", Project);
    stringFXfer( "Name", Name);
    dateFXfer( "Start", Start);
    llongFXfer( "Status", Status);
    llongFXfer( "Task_Assignment", Task_Assignment);
  }

  bool dbf_action( string mode, changeMap & changes )
  {
    Session_Obj & session = oLogin.Session();
    if( mode=="n") // co-opted for Search Criteria
    {
      if( Project > 0 )
      {
        addedCriteria = "t.Project = ";
        addedCriteria.append(Project);
      }
      if( Name.length() > 0 )
      {
        if(  addedCriteria .length() > 0 ) addedCriteria += " and ";
        addedCriteria += "t.Name like '";
        addedCriteria += Name;
        addedCriteria +="%'";
      }

      if( Start.format() != "01/01/2000" )
      {
        if(  addedCriteria .length() > 0 ) addedCriteria += " and ";
        Start.setFormat("%Y-%m-%d");
        addedCriteria += "t.Start >= '";
        addedCriteria +=  Start.format();
        addedCriteria +="'" ;
      }
      if( Status > 0 )
      {
        if(  addedCriteria .length() > 0 ) addedCriteria += " and ";
        addedCriteria += "t.Status = ";
        addedCriteria.append(Status);
      }
      if( Task_Assignment > 0  )
      {
        if(  addedCriteria .length() > 0 ) addedCriteria += " and ";
        addedCriteria += "ta.User = ";
        addedCriteria.append(Task_Assignment);
      }
      // set the session var
      session.SetData("TaskFilterCriteria",addedCriteria);
      session.Synch();
      
    }
    else if( mode=="r" )
    {
      showAll=true;
      addedCriteria = "";
      session.SetData("TaskFilterCriteria",addedCriteria);
      session.Synch();
    }
    else
    {
      // check the session var
      addedCriteria = session.GetData("TaskFilterCriteria");
    }
    return true;
  }

  // Implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    ocString sql; // for combo boxes
    ocString Title = "Task Search Criteria";
    script << makeTop("TaskUI.meta", Title);

    sql = sql = "select p.Id, concat(c.Name,  ' - ', p.Name) from Project p inner join Company c on c.Id = p.Company order by c.Name, p.Name ";
    script << makeComboBox("Project", "Project", Project ,sql,"Pick");
    script << "<br class='clearall'>" << endl;
    script << makeTextBox("Name starts with", "Name", Name ,"255","30");
    script << "<br class='clearall'>" << endl;
    sql = "select Id, Name from Status where Class='Task' order by Sequence ";
    script << makeComboBox("Status", "Status", Status ,sql,"Pick");
    sql = "select id, concat(first, ' ',  last) from users order by last";
    if(  Task_Assignment < 1 )  Task_Assignment = atoll( oLogin.Id().c_str());
    script << makeComboBox( "Assigned To","Task_Assignment",Task_Assignment,   sql, "Pick" );
    script << "<br class='clearall'>" << endl;
    script << makeDateBox("Starts on or after", "Start", Start ,"10");
    script << "<br class='clearall'>" << endl;
    script << diaButtons( -1, "",  //  i Insert
                           "",     //  u Update
                           "",                         //  d Delete
                           "",     //  r Reset New
                           "&nbsp; SEARCH &nbsp; "  //   n Keep New
                        );

    Task_List mylist(script,oLogin.Session(),"Task");
    mylist.addedCriteria = addedCriteria;

    mylist.form.dbf_action(mode,changes);

    if( showAll==true )
    {
      mylist.reset();
    }
    script << "<div class=\"heading\">Tasks &nbsp; &nbsp;<sup><a href=\"javascript:jopen('Task_Popup.cgi','resizable,width=625,height=600,scrollbars','Tasks')\">Add New Task</a></sup> &nbsp; &nbsp; &nbsp;" <<
        diaButtons( -1, "&nbsp; Update &nbsp;",  //  i Insert
                     "&nbsp; Update &nbsp;",     //  u Update
                     "",                         //  d Delete
                     "",     //  r Reset New
                     ""  //   n Keep New
                  ) << "</div>" << endl;
    mylist.loadListTemplates("Templates/ctrllist.htmp");
    mylist.list_display();
    
    script << "&nbsp; &nbsp; &nbsp; &nbsp; &nbsp;" << diaButtons( -1, "&nbsp; Update &nbsp;",  //  i Insert
                           "&nbsp; Update &nbsp;",     //  u Update
                           "",                         //  d Delete
                           "&nbsp; Show All &nbsp;",     //  r Reset New
                           ""  //   n Keep New
                        );
    // DBG   script << "Authorized: " << mylist.actions.XML() <<  endl;
    script << makeBottom( "" ) << endl;
    return breturn;
  }
};

bool intraMain(page & pg ,cgiScript & script)
{

  Task_search_form myFrm(script);
  script.closeHeader();
  myFrm.loadControlTemplates("Templates/divform.htmp");
  myFrm.form_action();  
  myFrm.form_display();
 
  return true;
}

// Standard main function
int main(int argc, char ** argv)
{  
  baseUrl = "TaskUI.meta";  // this is my compiled name!
  // because we are checking
  cgiScript script("text/html",false);
  isSignedOn = check(script);
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

