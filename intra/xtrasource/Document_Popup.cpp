/*
  Document_Popup.cpp

  User Interface Definition and Popup Implementation for Pop up Dialogs with Data Lists 
  for Document.
  
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
#include "Document.hpp"
#include "openLogin.h" 
#include "diaButtons.h"

using namespace std;
openLogin oLogin;
 // Include file
#include "sublist_base.hpp"
#include "ocFileSys.h"
// Definition

class Document_List:  public sublist_base
{

  public:
  // Constructor
    Document_List(cgiScript&sc):sublist_base(sc){;}
    ~Document_List(){;}

    string Filter;
  
    bool list_display( void )
    {
      bool breturn = true;
      string sql =  "select "
                    "Id, "
                    "Category, "
                    "Name, Sequence "
                    "from Document ";
      if( Filter.length() )
      {
        sql += "where Category like '" ;
        sql +=  Filter;
        sql += "%'";
      }
      sql += " order by Sequence, Category, Name ";
      hotCol=-2;
      editLink = listTemplate.getParagraph("peerlink");
      editLink = editLink.replace("$prog$","Document_Popup.cgi");
      string heading =  "Id| "
                        "Category| "
                        "Name| Sequence " ;
      webIO << "<form action='Document_Popup.cgi'><h3>Filter by Category: <input type='text' name='Filter' value='";
      webIO << Filter << "'> <input type='submit' value='Go'></h3></form>" << endl;
      emitHeadings(heading);
      getData( sql );
      emitData();
      ocString list_new = listTemplate.getParagraph("peernew");
      ocString url = "Document_Popup.cgi";
      if( Filter.length() )
      {
        url+="?Filter=";
        url+=Filter;
      }
      
      webIO << list_new.replace("$location",url.c_str()).replace("$cols","3").replace("Item","Document");
      emitEnd();
      return breturn;
    }
};
class Document_form:  public Document_Obj, public forms_base
{
  public:
    Document_form(cgiScript & script):Document_Obj(),forms_base(script)
    {
      setKey(*this);
    }
    virtual ~Document_form(){;}
  
    void form_id_transfer( void )
    {
      llongFXfer( "Id", Id );
    }
    void form_data_transfer( void )
    {
      stringFXfer( "Category", Category);
      stringFXfer( "Name", Name);
      stringFXfer( "Description", Description);
      stringFXfer( "Filename", Filename);
      if( Filename.length() == 0 )
        stringFXfer( "URL2", Filename);
      boolFXfer( "Visible", Visible);
      intFXfer( "Sequence", Sequence);
    }
  
    bool dbf_action( string mode, changeMap & changes )
    {
      if( mode=="d" )
      {
        ocFileSys fs;
        string remPath = "./media/" + Filename;
        fs.remove(remPath);
      }
      return db_action( mode, changes );
    }
  
  // implement pure virtual of form display
    bool form_display( void )
    {
      bool breturn = true;
      ocString sql;
      script << makeTop("Document_Popup.cgi", "Document")
          << formTemplate.getParagraph("advanced_begin");
      script << makeStaticBox("Id", "Id", Id ,"8");
      script << "<br class='clearall'>" << endl;
      script << formTemplate.getParagraph("advanced_end");

      script << makeTextBox("Category", "Category", Category ,"125","35");
      script << "<br class='clearall'>" << endl;
      
      script << makeTextBox("Name", "Name", Name ,"125","35");
      script << "<br class='clearall'>" << endl;
      
      script << makeTextArea("Description", "Description", Description ,"5","80");
      script << "<br class='clearall'>" << endl;

      script << ocString(makeFileBox( "Filename", "Filename", Filename, "30","255", "id='Filename'"))
                  .replace(" class=\"ctrlgroup\"","").replace(" class=\"control\"","")
             << makeHiddenBox( "URL2", Filename ) << "<br class='clearall'>" << endl;

      script << makeBoolBox("Visible", "Visible", Visible );
      script << "<br class='clearall'>" << endl;
      
      script << makeTextBox("Sequence", "Sequence", Sequence ,"8","8");
      script << "<br class='clearall'>" << endl;
      
      script << diaButtons( key() ,
                            " Insert Document ",
                            " Update Document ",
                            " Delete Document ",
                            "",  // reset
                            ""); // reset, but keep current data
      script << makeBottom( m_result ) << endl;
      return breturn;
    }
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false , "./documents" );
  Document_form myFrm(script);
   
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;
    pgTemplate.load("Templates/childPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
        .replaceAll("$heading$","Document");

    myFrm.loadControlTemplates("Templates/childdivform.htmp");
    myFrm.form_action();
    myFrm.form_display();

    Document_List DocumentList(script);
    DocumentList.loadListTemplates("Templates/spawninglist.htmp");
    script << "<div class=\"ui\">" << endl;
    script << "<div class='heading'>Document List:</div>" << endl;
    DocumentList.Filter = script.ClientArguments()["Filter"].c_str();
    DocumentList.list_display();
    script << "</div>" << endl;

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


