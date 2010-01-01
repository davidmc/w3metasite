/*
  Task_Popup.cpp

  User Interface Definition and Implementation 
  for Task.
 
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design
  
   Templates/spawninglist.htmp
   Templates/childEditingPane.htmp
   Templates/childdivform.htmp
*/

#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "cgiTemplates.h"
#include "Task.hpp"
#include "sublist_base.hpp"
#include "permittedActions.hpp"

using namespace std;
openLogin oLogin;
#include "sublist_base.hpp"
#include "InfoPoints.hpp"

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
          "date_format(t.Start, '%m/%d/%Y'), "
          "t.Description,  "
          "t.Hours, s.Name  "
          "from Time t left join Status s on s.Id = t.Status where Task = " ;
      sql +=  Parent_Id;
      hotCol=-2;
      skipCol=0;
      editLink = listTemplate.getParagraph("hotcolumn");
      editLink = editLink.replace("$prog$","Time_Popup.cgi");
      string heading = 
          "Start|"
          "Description|"
          "Hours|Status"
          ;
    
      emitHeadings(heading);
    
      getData( sql );
      emitData();
    
      webIO << "<tr><th colspan='3'>TOTAL: </th><td>";
      webIO << total;
      webIO << " hours</td></tr>" << endl;
     
      ocString list_new = listTemplate.getParagraph("list_new");
// TODO: Use correct Parent ID name 
      ocString url = "Time_Popup.cgi?Task=";
      url += Parent_Id;
      webIO << list_new.replace("$location",url.c_str()).replace("$cols","4");
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

class Task_form:  public Task_Obj, public forms_base
{
  // what can they do in this dialog?
  permittedActions actions;
  public:
  Task_form(cgiScript & script):Task_Obj(),forms_base(script)
  {
    setKey(*this);

  }
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
     Requestor = atoll(oLogin.Id().c_str());
     
     llongFXfer( "Requestor", Requestor);
     // dateFXfer( "Requested", Requested);
     // many to many fields
     stringFXfer( "Task_Assignment", Task_Assignment );
     
     // due date
     dateFXfer( "Estimated_End", Estimated_End);
     dateFXfer( "Actual_End", Actual_End); 
}

  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  }

  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    
    if( oLogin.testLoginStatus() )
    {
       oLogin.fetchServices();
       actions.loadByEnum(svcenm_task,oLogin.Services());
     }  
    if(  actions.May("Detail","show") )
    {
      ocString sql;
      script << makeTop("Task_Popup.cgi", "Task")
            << formTemplate.getParagraph("advanced_begin");

      script << makeTextBox("Id", "Id", Id ,"8","8");
      script << "<br class='clearall'>" << endl;

      sql = "select Id, Name from Task where Project = ";
      sql.append(Project);
      sql += " order by Name ";

      script << makeComboBox("Parent Task", "Parent_Task", Parent_Task ,sql);
      script << "<br class='clearall'>" << endl;

      script << makeTextBox("Hierarchy", "Hierarchy", Hierarchy ,"8","8");
      script << "<br class='clearall'>" << endl;
      script << makeTextBox("Estimate", "Estimate", Estimate ,"16","16");
      script << "<br class='clearall'>" << endl;
      script << formTemplate.getParagraph("advanced_end");

      sql = "select p.Id, concat(c.Name,  ' - ', p.Name) from Project p inner join Company c on c.Id = p.Company order by c.Name, p.Name ";

      script << makeComboBox("Project", "Project", Project ,sql,"Please Choose");
      script << "<br class='clearall'>" << endl; 
      if(  actions.May("Name","edit") )
      {
        script << makeTextBox("Name", "Name", Name ,"125","80");
      }
      else
      {
        script << makeStaticBox("Name", "Name", Name ,"80");
      }
      script << "<br class='clearall'>" << endl; 
      script << makeTextArea("Description", "Description", Description ,"5","120");
      script << "<br class='clearall'>" << endl; 

      script << makeTextBox("Estimated Time", "Estimated_Time", Estimated_Time ,"16","16");
      script << makeTextBox("Actual Time", "Actual_Time", Actual_Time ,"16","16");
      script << "<br class='clearall'>" << endl; 



      script << makeTextBox("Started", "Start", Start ,"24","24");
      if( actions.May("Task_Order","edit") )
      {
        script << makeTextBox("Priority", "Task_Order", Task_Order ,"5","8");
      }
      else
      {
        script << makeStaticBox("Priority", "Task_Order", Task_Order ,"5");
      }
      script << "<br class='clearall'>" << endl; 

      script << makeDateBox("Due Date", "Estimated_End", Estimated_End ,"10");
      script << makeDateBox("Actual Date Completed", "Actual_End", Actual_End ,"10");
      script << "<br class='clearall'>" << endl; 

      sql = "select Id, Name from Status where Class='Task' ";
      if( !actions.May("Status","edit") )
      {
          // limit to current value
        sql += " and Id = ";
        sql.append(Status);
      }
      sql += " order by Sequence ";
      ocString links = "";
      if( Status > 0 )
      {
        links = " &nbsp; <a href='javascript:jopen(\"Status_Popup.cgi?Id=";
        links.append(Status);
        links += "\",\"resizable,width=400,height=300\",\"status\")'>Edit</a>";
      }
      links += " &nbsp; <a  href='javascript:jopen(\"Status_Popup.cgi?Class=Task\","
          "\"resizable,width=500,height=400\",\"status\")'>Add</a>";

      script << ocString(makeComboBox("Status", "Status", Status ,sql))
          .replace("<!--eogc-->",links);

      script << "<br class='clearall'>" << endl;
      ocString vals;
      if( actions.May("Percent_Complete","edit") )
      {
        vals = "0%=0,10%=10,20%=20,30%=30,"
            "40%=40,50%=50,60%=60,"
            "70%=70,80%=80,90%=90,100%=100";
      }
      else
      {
        vals.append(Percent_Complete);
        vals += "%=";
        vals.append(Percent_Complete);
      }
      // script << makeTextBox("Percent_Complete", "Percent_Complete", Percent_Complete, "8","8");
      script << makeManualComboBox( "% Complete", "Percent_Complete", Percent_Complete,
                          "0%=0,10%=10,20%=20,30%=30,"
                              "40%=40,50%=50,60%=60,"
                              "70%=70,80%=80,90%=90,100%=100",
                          "", ""
                                  ) << endl; 
      script << "<br class='clearall'>" << endl;

      sql = "select id, concat(first, ' ',  last) from users ";
      if( Requestor )  {
        sql += "where id = ";
        sql.append( Requestor );
      }

      script << makeComboBox("Requestor", "Requestor", Requestor ,sql,"Please Choose");

      script << makeDateBox("Requested", "Requested", Requested ,"10");
      script << "<br class='clearall'>" << endl;

      // Multi Choice
      sql = "select id, concat(first, ' ',  last) from users order by last";
      script <<
          makeComboBox( "Assigned To","Task_Assignment",Task_Assignment,
                        sql.c_str(), "", " multiple='multiple' size='5'" );
            
      script << "<br class='clearall'>" << endl;
      script << makeButtons( key() );
      
      if(  actions.May("Time","enter") )
      {
        if( key() )
        {
          Time_List TimeList(script);
          TimeList.Parent_Id.append(key());
          TimeList.loadListTemplates("Templates/spawninglist.htmp");
          script << "<br class='clearall'><div class='ctrlgroup'> <div class='label'>Time Entry:</div>" << endl;
          script << "<br class='clearall'>" << endl; 
          TimeList.list_display();
          script << "</div>";
        }
      }


      script << makeBottom( m_result ) << endl;
    }
    else
    {
       script << "svc_id: " << oLogin.Session().GetData("svc_id") << endl;
    }
    return breturn;
  }
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  Task_form myFrm(script); 
  // New: need to be able to override where to get the data and what cookie to set for id
  infoPoints iPoints;
  if( iPoints.idToken.length() )
  {
    oLogin.token = iPoints.idToken;
  }
  // end of new 
  if( oLogin.testLoginStatus() )
  {
      script.closeHeader();
      cgiTemplates pgTemplate;    
      pgTemplate.load("Templates/childEditingPane.htmp");
      
      script << ocString(pgTemplate.getParagraph("top"))
                      .replaceAll("$heading$","Task");
  
      myFrm.loadControlTemplates("Templates/childdivform.htmp");  
      myFrm.form_action();  
      myFrm.form_display();
          
      ocString end = pgTemplate.getParagraph("bottom");
      script << end;          
  }
  else
  {
      script.Redirect("/");
  }
}
// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"

