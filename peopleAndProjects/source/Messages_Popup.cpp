/*
  Messages_Popup.cpp

  User Interface Definition and Popup Implementation for Pop up Dialogs with Data Lists 
  for Messages.

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
#include "Messages.hpp"
#include "openLogin.h" 
#include "diaButtons.h"


using namespace std;
openLogin oLogin;
 // Include file
#include "sublist_base.hpp"
#include "InfoPoints.hpp"

// Definition
class Messages_List:  public sublist_base
{

  public:
    llong Task;
    llong Project;
  // Constructor
    Messages_List(cgiScript&sc):sublist_base(sc),Task(0LL),Project(0LL){;}
    ~Messages_List(){;}
  
    bool list_display( void )
    {
      bool breturn = true;
      ocString sql =  "select "
          "m.Id, "
          "m.Subject, "
          "concat(f.first, ' ',  f.last), "
          "concat(t.first, ' ',  t.last) "
          "from Messages m "
          " inner join users f on f.id = m.From_Id "
          " inner join users t on t.id = m.To_Id "
          "where Task = " ;
      sql.append(Task);
      hotCol=-2;
      editLink = listTemplate.getParagraph("peerlink");
      editLink = editLink.replace("$prog$","Messages_Popup.cgi");
      string heading =  "Id |Subject| From| To ";
      emitHeadings(heading);

      getData( sql );
      emitData();
      ocString list_new = listTemplate.getParagraph("peernew");
      ocString url;
      url = "Messages_Popup.cgi?Task=";
      url.append(Task);
      webIO << list_new.replace("$location",url.c_str()).replace("$cols","4").replace("Item","Messages");
      emitEnd();
      return breturn;
    }
};
class Messages_form:  public Messages_Obj, public forms_base
{
  public:
    Messages_form(cgiScript & script):Messages_Obj(),forms_base(script)
    {
      setKey(*this);
    }
    virtual ~Messages_form(){;}
  
    void form_id_transfer( void )
    {
      llongFXfer( "Id", Id );
    }
    void form_data_transfer( void )
    {
      stringFXfer( "Subject", Subject);
      llongFXfer( "From", From_Id);
      llongFXfer( "To", To_Id);
      intFXfer( "Importance", Importance);
      stringFXfer( "Content", Content);
      llongFXfer( "Task", Task);
      /*
      stringFXfer( "From_String", From_String);
      stringFXfer( "To_String", To_String);
      llongFXfer( "Company", Company);
      llongFXfer( "Project", Project);
      llongFXfer( "Author", Author);
      dateFXfer( "Created", Created);
      */
    }
  
    bool dbf_action( string mode, changeMap & changes )
    {
      Author = atoll(oLogin.Id().c_str());
      if(From_Id<1) From_Id = atoll(oLogin.Id().c_str()); 
      return db_action( mode, changes );
    }
  
  // implement pure virtual of form display
    bool form_display( void )
    {
      bool breturn = true;
      ocString sql;
      string Title = "Messages about ";
      sql = "select Name from Task where Id = ";
      sql.append(Task);
      if(rs.open(sql))
      {
        Title += rs.getField(0).format();
        rs.close();
      }
      script << makeTop("Messages_Popup.cgi", Title)
             << formTemplate.getParagraph("advanced_begin");
      script << makeHiddenBox("Task", Task);
 
      script << makeStaticBox("Id", "Id", Id ,"8");
      script << "<br class='clearall'>" << endl;
      sql = "select id, concat(first, ' ',  last) from users ";
      script << makeComboBox("Author", "Author", Author ,sql,"Please Choose");
      script << "<br class='clearall'>" << endl;
      script << makeTextBox("Created", "Created", Created ,"24","24");
      script << "<br class='clearall'>" << endl;
      script << formTemplate.getParagraph("advanced_end");

      script << makeTextBox("Subject", "Subject", Subject ,"125","35");
      script << "<br class='clearall'>" << endl;
      
      script << makeComboBox("From", "From", From_Id ,sql,"Please Choose");
      sql = "select id, concat(first, ' ',  last) from users ";
      script << makeComboBox("To", "To", To_Id ,sql,"Please Choose");
      script << "<br class='clearall'>" << endl;

      script << makeManualComboBox("Importance", "Importance", Importance ,"Informational=1,Low=2,Important=3,High=4,Critical=5","");
      script << "<br class='clearall'>" << endl;

      script << makeTextArea("Content", "Content", Content ,"5","120");
      script << "<br class='clearall'>" << endl;

      /*
      script << makeTextBox("From_String", "From_String", From_String ,"125","35");
      script << "<br class='clearall'>" << endl;

      script << makeTextBox("To_String", "To_String", To_String ,"125","35");
      script << "<br class='clearall'>" << endl;

      script << makeComboBox("Company", "Company", Company ,sql,"Please Choose");
      script << "<br class='clearall'>" << endl;

      script << makeComboBox("Project", "Project", Project ,sql,"Please Choose");
      script << "<br class='clearall'>" << endl;

      script << makeComboBox("Task", "Task", Task ,sql,"Please Choose");
      script << "<br class='clearall'>" << endl;

      script << makeComboBox("Author", "Author", Author ,sql,"Please Choose");
      script << "<br class='clearall'>" << endl;

      script << makeTextBox("Created", "Created", Created ,"24","24");
      script << "<br class='clearall'>" << endl;
      */  
      script << diaButtons( key() ,
                            " Insert Message ",
                            " Update Message ",
                            " Delete Message ",
                            "",  // reset
                            ""); // reset, but keep current data
      script << makeBottom( m_result ) << endl;
      script << "<br class='clearall'>" << endl; 
      return breturn;
    }
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  Messages_form myFrm(script);
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
        .replaceAll("$heading$","Messages");

    myFrm.loadControlTemplates("Templates/childdivform.htmp");
    myFrm.form_action();
    myFrm.form_display();
    if( myFrm.Task )
    {
      Messages_List MessagesList(script);
      MessagesList.Task = myFrm.Task;
      MessagesList.loadListTemplates("Templates/spawninglist.htmp");
      script << "<div class=\"ui\" style=\"text-align:center;\">" << endl;
      script << "<div class='heading'>Message List:</div>" << endl;
      MessagesList.list_display();
      script << "</div>" << endl;
    }    
    ocString end = pgTemplate.getParagraph("bottom");
    script << end;
  }
  else
  {
    script << "<html><body>"
           << "<a href='javascript:window.close()'><h1>You are not logged in</h1></a>"
           << "</body></html>" << endl;
  }
}
// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"


