/*
  Scheme_Table_Popup.cpp

  User Interface Definition and Implementation 
  for Scheme_Table.
  
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
#include "cgiTemplates.h"
#include "Scheme_Table.hpp"
#include "openLogin.h" 
#include "../../openData/openCatalog.h"
#include "Scheme_Field.hpp"
#include <set>

using namespace std;
openLogin oLogin;

class Scheme_Table_form:  public Scheme_Table_Obj, public forms_base
{
public: 
  Scheme_Table_form(cgiScript & script):Scheme_Table_Obj(),forms_base(script)
  {    
  } 
  virtual ~Scheme_Table_form(){;}  
  void form_id_transfer( void )
  {
  }
  void form_data_transfer( void )
  {
  }  
  bool dbf_action( string mode, changeMap & changes )
  {
    return false;
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  { 
    bool breturn = true;    
    ocString sql;
    script << makeTop("Scheme_Table_Popup.cgi", "Schema")
           << formTemplate.getParagraph("advanced_begin");
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");
    bool importing = false;
    bool tablesSpecified = false;
    ocString specifiedTables;
    set <string> tableSet;
    if(script.RequestMethod() == "POST")
    {
      importing = true;
      if( script.ClientArguments().count("addedTable") > 0 )
      {
        tablesSpecified = true;
        specifiedTables = script.ClientArguments()["addedTable"].c_str();
        string specTable = specifiedTables.parse("|");
        while( specTable.length() )
        {
           tableSet.insert(specTable);
           specTable = specifiedTables.parse("|"); 
        }
      }
      script << "<h2>DOING IMPORT!</h2>" << endl;
      if( tablesSpecified == false )
      {
        sql = "delete from Scheme_Table";
        cmd.execute(sql);
        sql = "delete from Scheme_Field";
        cmd.execute(sql);
      }
    }
    openCatalog cat(CONNECTSTRING);
    if( cat.open() )
    {
      cout << "<pre>" << endl;
      do
      {
        if(  script.ClientArguments().count("viewOnly") == 0 )
        { cout << "<input type='checkbox' name='addedTable' value='" << cat.name() << "'>";
        }
        cout << " &nbsp; Table: " << cat.name() <<  endl;
        if( importing && ( tablesSpecified == false || tableSet.find(cat.name()) != tableSet.end() ) )
        {
          key(0);
          Name = cat.name();
          db_insert();
        }
        openFields & lst = cat.fieldsList();
        for( int i=0; i<lst.size(); i++)
        {
          cout << "    Field: " << lst[i].name() << " type: " << lst[i].type() << endl;
          if( importing && ( tablesSpecified == false || tableSet.find(cat.name()) != tableSet.end() ) )
          { Scheme_Field_Obj fld;
            fld.key(0);
            fld.Scheme_Table = key();
            fld.Name = lst[i].name();
            fld.Col_Type = lst[i].type();
            if( !fld.db_insert() ) cout << " Error: " << fld.last_result() << endl ;

          }
        }
      }
      while( cat.next() );
      if(  script.ClientArguments().count("viewOnly") == 0 )
      {
        cout << "</pre>" << endl <<
                "<div class=\"buttons\"><input name=\"action\" "
                "class=\"action\" value=\"&nbsp;Import&nbsp;\" "
                "type=\"submit\"></div>" << endl;
      }
    }
    
    script << makeBottom( m_result ) << endl;     
    return breturn;
  }  
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  Scheme_Table_form myFrm(script); 
   
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/childPane.htmp");    
    script << ocString(pgTemplate.getParagraph("top")).replaceAll("$heading$","Hi Mom!");
    myFrm.loadControlTemplates("Templates/childdivform.htmp");    
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

#ifdef OPEN_MYDB_H
  #include "../../openData/openMySqlCatalog.cpp"
#else
  #include "../../openData/openPgSqlCatalog.cpp"
#endif
