/*
  Report_Conditioning_Popup.cpp

  User Interface Definition and Implementation 
  for Report_Conditioning.
  
  Copyright (C) 2005 by D.K. McCombs.
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
#include "list_base.hpp"
#include "cgiTemplates.h"
#include "Report_Conditioning.hpp"
#include "list_base.hpp"
#include "openLogin.h" 


using namespace std;
openLogin oLogin;

#include "InfoPoints.hpp"

class Report_Conditioning_form:  public Report_Conditioning_Obj, public forms_base
{
  string colHandlers;
  string directions;
  string formats;
  string spawningEvents;
public: 
  Report_Conditioning_form(cgiScript & script):Report_Conditioning_Obj(),forms_base(script)
  {
    setKey(*this);
    colHandlers = "NONE=NONE,SUM=SUM,AVG=AVG,GROUP=GROUP,SPAWN REPORT=SPAWN,LOOKUP=LOOKUP";
    directions = "NONE=NONE,ASC=ASC,DESC=DESC";
    formats = "STRING=STRING,CURRENCY=CURRENCY,DECIMAL=DECIMAL,INTEGER=INTEGER,TIME=TIME,DATE=DATE,DATETIME=DATETIME";
    //   commence_event, prerow_event, field_event, postfield_event, postrow_event, complete_event
    spawningEvents = "Commence=0,Pre-Row=1,Field=2,Post-Field=3,Post-Row=4,Complete=5";
  } 
  virtual ~Report_Conditioning_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "Report_Id", Report_Id);
    intFXfer( "Col", Col);
    // intFXfer( "Sort_Pos", Sort_Pos);
    stringFXfer( "Col_Handler", Col_Handler);
    stringFXfer( "Direction", Direction);
    stringFXfer( "Format", Format);
    stringFXfer( "Tuple_Section", Tuple_Section);
    llongFXfer( "spawnedReport", spawnedReport);
    stringFXfer( "spawnedTuplePretext", spawnedTuplePretext);
    stringFXfer( "spawnedTuplePosttext", spawnedTuplePosttext);
    intFXfer( "spawningEvent", spawningEvent);       
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
    script << makeTop("Report_Conditioning_Popup.cgi", "Report_Conditioning");
    script << makeHiddenBox("Id", Id );
    script << makeHiddenBox("Report_Id", Report_Id );

    
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Col", "Col", Col , "4" );
    script << "<br class='clearall'>" << endl;
    
    script << makeManualComboBox("Col Handler", "Col_Handler", Col_Handler ,colHandlers.c_str() );
    script << "<br class='clearall'>" << endl; 

    script << makeManualComboBox("Format", "Format", Format ,formats.c_str() );
    script << "<br class='clearall'>" << endl;
    
    script << makeTextBox("Tuple Section", "Tuple_Section", Tuple_Section ,"25");
    script << "<br class='clearall'>" << endl;
    sql = "select Id, Name from Report order by Name";  
    script << makeComboBox("Spawned Report<br>Only for Spawn Report Handler", "spawnedReport", spawnedReport ,sql);
    script << "<br class='clearall'>" << endl;
    script << makeTextArea("Spawned Report Tuple Pre-text<br>(Or id, name SQL lookup query for LOOKUP)", "spawnedTuplePretext", spawnedTuplePretext ,"5","55");
    script << "<br class='clearall'>" << endl;
    script << makeTextArea("spawned Tuple Post-text", "spawnedTuplePosttext", spawnedTuplePosttext ,"5","55");
    script << "<br class='clearall'>" << endl;
    script << makeManualComboBox("spawningEvent", "spawningEvent", spawningEvent,
                                 spawningEvents );
    script << makeButtons( key() );
    script << "<br><ul>\
        <li>commence_event - event just before any data is displayed \
        <li>prerow_event -  Before any data on the current row is displayed \
        want to check for groups and sums on the previous rows \
        <li>field_event - The field on a row of data is being rendered, give the handler an opportunity to do this \
        <li>postfield_event -  After current field is displayed \
        <li>postrow_event -  After all data on the row is displayed \
        <li>complete_event - After all data is displayed \
        </ul>"  << endl;
    script << "<a style='color:#283;text-decoration:underline;'  href=\"javascript:jopen('intranet.meta/reportHelp?view=Report_Conditioning',"
        "'scrollbars,resizable,width=575,height=300','Help')\">Help</a>" << endl; 
    script << makeBottom( m_result ) << endl;     
    return breturn;
  }  
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  Report_Conditioning_form myFrm(script); 
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
                     .replaceAll("$heading$","Report_Conditioning");

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
