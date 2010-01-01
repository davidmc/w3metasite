/*
  Report_Parameters_ui.cpp

  User Interface Definition and Implementation 
  for Report_Parameters.
  
  Copyright (c) 2005 by D.K. McCombs.
  
  davidmc@w3sys.com
  
  W3 Systems Design Inc.  
  
*/
#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "cgiTemplates.h"
     
#include "read_write_base.hpp"
#include "list_base.hpp"
#include "forms_base.hpp"
#include "Report_Parameters.hpp"
#include "openLogin.h" // open-login has own connectstring
#include "connectstring" //re-assert connect
using namespace std;
openLogin oLogin;

class Report_Parameters_List:  public list_base
{
public: 
  // Constructor
  Report_Parameters_List(cgiScript&sc):list_base(sc){;}  
  ~Report_Parameters_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;

    hotCol=-2;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","Report_Parameters_ui.cgi");
    emitFilter( "Report_Parameters_ui.cgi",
                "    <B>FILTER (by Display_Label)</B>" );  
    string heading =
                     "<a class='sortcol' href='Report_Parameters_ui.cgi?sort=Id'>Id</a>|" 
                     "<a class='sortcol' href='Report_Parameters_ui.cgi?sort=Display_Label'>Display_Label</a>|"                     
                     "<a class='sortcol' href='Report_Parameters_ui.cgi?sort=Data_Element'>Data_Element</a>|"                     
                     "<a class='sortcol' href='Report_Parameters_ui.cgi?sort=Tag_Name'>Tag_Name</a>|"                     
                     "<a class='sortcol' href='Report_Parameters_ui.cgi?sort=Choices_Query'>Choices_Query</a>|"                     
                    ;
    emitHeadings(heading);
    getFilteredData(
                     "Id, " 
                     "Display_Label, " 
                     "Data_Element, " 
                     "Tag_Name, " 
                     "Choices_Query " 
                     ,"Report_Parameters",                     
                     "Display_Label like '$filter$%'" );   
    emitData();
    emitNavigation("Report_Parameters_ui.cgi");         
    emitEnd();
    return breturn;
  }      
}; 

class Report_Parameters_form:  public Report_Parameters_Obj, public forms_base
{
  string Parameter_Types;
public: 
  Report_Parameters_form(cgiScript & script):Report_Parameters_Obj(),forms_base(script)
  {
    setKey(*this);
    Parameter_Types = "STRING=STRING,DATE=DATE,SELECT=SELECT";
  } 
  virtual ~Report_Parameters_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "Report_Id", Report_Id);
    stringFXfer( "Display_Label", Display_Label);
    stringFXfer( "Data_Element", Data_Element);
    stringFXfer( "Tag_Name", Tag_Name);
    stringFXfer( "Choices_Query", Choices_Query);
    stringFXfer( "Parameter_Type", Parameter_Type);
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
    
    script << makeTop("Report_Parameters_ui.cgi", "Report_Parameters")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("Id", "Id", Id ,"8");
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");
    
    sql = "select Id, Name from Report order by Name";
    
    script << makeComboBox("Report_Id", "Report_Id", Report_Id ,sql);
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Display_Label", "Display_Label", Display_Label ,"25");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Data_Element", "Data_Element", Data_Element ,"25");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Tag_Name", "Tag_Name", Tag_Name ,"25");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Choices_Query", "Choices_Query", Choices_Query ,"25");    
    script << "(For combo boxes) <br class='clearall'>" << endl; 
    script << makeManualComboBox("Parameter_Type", "Parameter_Type", Parameter_Type ,Parameter_Types.c_str() );
    script << "<br class='clearall'>" << endl;     
    /*    
      Special Purpose alternative field controls:
      string makeManualComboBox( string label, string name, valType value, 
                        ocString labelValuePair = "", string defaulted = "", 
                        string attributes = "");
      string makeHiddenBox( string name, valType value );
      string makeStaticBox( string label, string name, valType value, 
                          string size, string length = "");
      template<class valType> 
      string makeDateBox( string label, string name, valType value, string size);
      string makeEmailBox( string label, string name, string value, 
                        string size, string length = "");
      string makePasswordBox( string label, string name, 
                            string value, string size); 
      string makeZipBox( string label, string name, string value, string size);
      string makeImageBox( string label, string name, string value, string size); 
      string makeFileBox( string label, string name, string value, string size);                                         
      string makeTextArea( string label, string name, 
                        string value, string rows="12", string cols="80"); 
      string makeStateBox( string label, string name, string value);
    */   
    script << makeButtons( key() );        
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  Report_Parameters_form myFrm(script); 
  Report_Parameters_List mylist(script);  
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
                     .replaceAll("$heading$","Report_Parameters");

    myFrm.loadControlTemplates("Templates/divform.htmp");  
    myFrm.form_action();  
    myFrm.form_display();
    
    mylist.loadListTemplates("Templates/list.htmp");  
    mylist.list_display();
        
    ocString end = pgTemplate.getParagraph("bottom");
    script << end;          
  }
  else
  {
    script.Redirect("signIn.html"); 
  } 
}
// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"

