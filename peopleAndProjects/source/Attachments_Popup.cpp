/*
  Attachments_Popup.cpp

  User Interface Definition and Popup Implementation for Pop up Dialogs with Data Lists 
  for Attachments.
  
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
#include "Attachments.hpp"
#include "openLogin.h" 
#include "diaButtons.h"

using namespace std;
openLogin oLogin;
 // Include file
#include "sublist_base.hpp"
#include "InfoPoints.hpp"
// Definition

class Attachments_List:  public sublist_base
{

  public:
    ocString Message_Id;
    ocString Task_Id;
    
  // Constructor
    Attachments_List(cgiScript&sc):sublist_base(sc){;}
    ~Attachments_List(){;}
  
    bool list_display( void )
    {
      bool breturn = true;
      string sql =  "select Id, "
          "URL, Title, Description, "
          "concat('<a href=\"./attachments/',URL,'\" target=\"_blank\">**View ',Title,'**</a>') "
          "from Attachments where Task = " ;
      sql +=  Task_Id;
      sql += " ";
      if(  Message_Id.length() && Message_Id != "0" && Message_Id != "-1" )
      {
        sql += " and  Message = ";
        sql += Message_Id;
        sql += " ";
      } 
      sql += "order by Title";

      hotCol=-2;
      editLink = listTemplate.getParagraph("peerlink");
      editLink = editLink.replace("$prog$","Attachments_Popup.cgi");
      string heading =
            "Id| "
            "URL| "
            "Title| "
            "Description| View "
            ;
      emitHeadings(heading);

      getData( sql );
      emitData();
      ocString list_new = listTemplate.getParagraph("peernew");
      ocString url = "Attachments_Popup.cgi?Task=";
      url += Task_Id;
      url += "&Message=";
      url += Message_Id;

      webIO << list_new
           .replace("$location",url.c_str())
           .replace("$cols","4")
           .replace("Item","Attachment");
      emitEnd();
      return breturn;
    }
};
class Attachments_form:  public Attachments_Obj, public forms_base
{
  public:
    Attachments_form(cgiScript & script):Attachments_Obj(),forms_base(script)
    {
      setKey(*this);
    }
    virtual ~Attachments_form(){;}
  
    void form_id_transfer( void )
    {
      llongFXfer( "Id", Id );
    }
    void form_data_transfer( void )
    {
      llongFXfer( "Message", Message);
      llongFXfer( "Task", Task);
      stringFXfer( "URL", URL);
      if( URL.length() == 0 )  stringFXfer( "URL2", URL);
      stringFXfer( "Title", Title);
      stringFXfer( "Description", Description);
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
      script << makeTop("Attachments_Popup.cgi", "Attachments")
             << formTemplate.getParagraph("advanced_begin");

      script << makeStaticBox("Id", "Id", Id ,"8");
      script << "<br class='clearall'>" << endl;

      script << formTemplate.getParagraph("advanced_end");
      
      script << "<br class='clearall'>" << endl;
      script << makeTextBox("Title", "Title", Title ,"125","35");
       script << "<br class='clearall'>" << endl;
       
      sql = "select Id, Name  from Task where Id = ";
      sql.append(Task);
      script << makeComboBox("Task", "Task", Task ,sql,"Please Choose");

      sql = "select Id, Subject from Messages where Task = ";
      sql.append(Task); 
      script << makeComboBox("Message", "Message", Message ,sql,"Please Choose");
      script << "<br class='clearall'>" << endl;
      
      script << makeFileBox( "URL", "URL", URL, "30","255", "id='URL'")
             << makeHiddenBox( "URL2", URL ) << "<br class='clearall'>" << endl;



      script << "<br class='clearall'>" << endl;
      script << makeTextArea("Description", "Description", Description ,"2","80");

      script << "<br class='clearall'>" << endl;

      script << diaButtons( key() ,
                            " Insert Attachments ",
                            " Update Attachments ",
                            " Delete Attachments ",
                            "",  // reset
                            ""); // reset, but keep current data

      script << makeBottom( m_result ) << endl;
      return breturn;
    }
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false , "./attachments" );
  Attachments_form myFrm(script);
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
        .replaceAll("$heading$","Attachments");

    myFrm.loadControlTemplates("Templates/childdivform.htmp");
    myFrm.form_action();
    myFrm.form_display();
    if( myFrm.Task )
    {
      ocString Message_Id;
      ocString Task_Id;
      Attachments_List AttachmentsList(script);
      AttachmentsList.Task_Id.append(myFrm.Task);
      AttachmentsList.Message_Id.append(myFrm.Message);
      AttachmentsList.loadListTemplates("Templates/spawninglist.htmp");
      script << "<div class=\"ui\">" << endl;
      script << "<div class='heading'>Attachments List:</div>" << endl;
      AttachmentsList.list_display();
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


