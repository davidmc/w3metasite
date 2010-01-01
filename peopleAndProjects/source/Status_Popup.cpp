/*
  Status_Popup.cpp

  User Interface Definition and Implementation 
  for Status.
  
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
#include "Status.hpp"
#include "openLogin.h" 
#include "diaButtons.h"

using namespace std;
openLogin oLogin;
 // Include file
#include "sublist_base.hpp"

// Definition

class Status_List:  public sublist_base
{

  public:
    ocString Parent_Id;
  // Constructor
    Status_List(cgiScript&sc):sublist_base(sc){;}
    ~Status_List(){;}
  
    bool list_display( void )
    {
      bool breturn = true;
      string sql =  "select Id, "
          "Sequence,  "
          "Class,  "
          "Name,  "
          "Prog_Enum  "
          "from Status where Class = '" ;
      sql +=  Parent_Id;
      sql += "'";
      sql += "order by Sequence";
      hotCol=-2;
      editLink = listTemplate.getParagraph("peerlink");
      editLink = editLink.replace("$prog$","Status_Popup.cgi");
      string heading = "Id|"
          "Sequence|"
          "Class|"
          "Name|"
          "Prog_Enum";
    
      emitHeadings(heading);
    
      getData( sql );
      emitData();
      ocString list_new = listTemplate.getParagraph("peernew");
      ocString url = "Status_Popup.cgi?Class=";
      url += Parent_Id;
// TODO: Match the column count to the number of fields
      webIO << list_new.replace("$location",url.c_str()).replace("$cols","7").replace("Item","Status");
      emitEnd();
      return breturn;
    }
};

class Status_form:  public Status_Obj, public forms_base
{
public: 
  Status_form(cgiScript & script):Status_Obj(),forms_base(script)
  {
    setKey(*this);
  } 
  virtual ~Status_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    longFXfer( "Sequence", Sequence);
    stringFXfer( "Class", Class);
    stringFXfer( "Name", Name);
    longFXfer( "Prog_Enum", Prog_Enum);
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
    script << makeTop("Status_Popup.cgi", "Status")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("Id", "Id", Id ,"8");
    script << makeStaticBox("Class", "Class", Class ,"125","35");
    script << "<br class='clearall'>" << endl;
    script << makeTextBox("Prog_Enum", "Prog_Enum", Prog_Enum ,"8","8");
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");
    
    script << makeTextBox("Sequence", "Sequence", Sequence ,"8","8");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Name", "Name", Name ,"125","35");
    script << "<br class='clearall'>" << endl; 

    

 
    script << diaButtons( key() ,
                          "&nbsp;Insert&nbsp;",
                          "&nbsp;Update&nbsp;",
                          "&nbsp;Delete&nbsp;",
                          "","");
    script << makeBottom( m_result ) << endl;     
    return breturn;
  }  
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  Status_form myFrm(script); 
   
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/childPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
                     .replaceAll("$heading$","Status")
                     .replace("<!--Added_Header_Content-->",
                              "<SCRIPT type=\"text/javascript\" src=\"colorCtrl.js\"></SCRIPT>");
    
    myFrm.loadControlTemplates("Templates/childdivform.htmp");  
    myFrm.form_action();  
    myFrm.form_display();
    
    if( myFrm.Class.length() )
    {
      Status_List StatusList(script);
      StatusList.Parent_Id.append(myFrm.Class);
      StatusList.loadListTemplates("Templates/spawninglist.htmp");
      script << "<div class=\"ui\">" << endl;
      script << "<div class='heading'>" << myFrm.Class << " Status List:</div>" << endl;
      StatusList.list_display();
      script << "</div>" << endl;
    }
    
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

