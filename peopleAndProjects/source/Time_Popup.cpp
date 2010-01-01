/*
  Time_Popup.cpp

  User Interface Definition and Implementation 
  for Time.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  
  
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
#include "Time.hpp"
#include "openLogin.h" 


using namespace std;
openLogin oLogin;

#include "InfoPoints.hpp"

class Time_form:  public Time_Obj, public forms_base
{
public: 
  Time_form(cgiScript & script):Time_Obj(),forms_base(script)
  {
    setKey(*this);
  } 
  virtual ~Time_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  
  void form_data_transfer( void )
  {
    llongFXfer( "Task", Task);
    dateFXfer( "Start", Start);
    dateFXfer( "Stop", Stop);
    stringFXfer( "Description", Description);
    doubleFXfer( "Hours", Hours);
    llongFXfer( "Status", Status);
    llongFXfer( "Invoice", Invoice);
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  { 
    bool breturn = true;    
    ocString sql;
    script << makeTop("Time_Popup.cgi", "Time")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("Id", "Id", Id ,"8");
    script << makeStaticBox("Task", "Task", Task, "8" ); 
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");
    script << makeTextBox("Start", "Start", Start ,"24","24");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Stop", "Stop", Stop ,"24","24");
    script << "<br class='clearall'>" << endl; 
    script << makeTextArea( "Description", "Description", Description, "3", "50" );
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Hours", "Hours", Hours ,"16","16");
    script << "<br class='clearall'>" << endl;
    sql = "select Id, Name from Status where Class='Time' order by Sequence ";
    ocString links = "";
    if( Status > 0 )
    {
      links = " &nbsp; <a href='javascript:jopen(\"Status_Popup.cgi?Id=";
      links.append(Status);
      links += "\",\"resizable,width=400,height=300\",\"status\")'>Edit</a>";
    }
    links += " &nbsp; <a  href='javascript:jopen(\"Status_Popup.cgi?Class=Time\","
      "\"resizable,width=400,height=300\",\"status\")'>Add</a>";
    script << ocString(makeComboBox("Status", "Status", Status ,sql))
      .replace("<!--eogc-->",links);
    script << "<br class='clearall'>" << endl;
    sql = "select Id, Invoice_Date from Invoice order by Invoice_Date desc ";
    script << makeComboBox("Invoice", "Invoice", Invoice ,sql,"Please Choose");
    script << "<br class='clearall'>" << endl; 
    script << makeButtons( key() );
    script << makeBottom( m_result ) << endl;
    return breturn;
  }  
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  Time_form myFrm(script); 
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
    pgTemplate.load("Templates/childPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
                     .replaceAll("$heading$","Time");

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

