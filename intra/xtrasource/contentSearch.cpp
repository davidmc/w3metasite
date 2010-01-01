/*
  contentSearch.cpp

  User Interface Definition and Implementation 
  for search and replace of paragraph content.

  Copyright (C) 2007 by D.K. McCombs.
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
#include "paragraphs.hpp"
#include "openLogin.h" 
#include "siteLimit.h"
#include "InfoPoints.hpp"
#include "sublist_base.hpp"
#include "page_template.hpp"
#include "ocXML.h"
#include "page_obj.hpp"
#include "lookup.hpp"
#include "flexButtons.hpp"
using namespace std;
openLogin oLogin;

class paragraphs_List:  public sublist_base
{
public: 
  ocString Search_Scope;
  string search, replace;
  bool doReplace;
  
  // Constructor
  paragraphs_List(cgiScript&sc):sublist_base(sc){;}  
  ~paragraphs_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    string sql =  "select id, name, content, page_id "
                  "from paragraphs where " ;
    sql +=  Search_Scope;
    sql += " order by page_id, section, place_order";
    skipCol=-1;
    hotCol=-1;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","contentSearch.cgi");
    string heading = "NEW CONTENT|Name|CONTENT|Page Id";
                           
    webIO << sql;
    emitHeadings(heading);
    getData( sql );
    emitData();

    
    emitEnd();
    return breturn;
  }
  virtual void sendField( int iField, ocString & td )
  {

    ocString data;
    if( iField == 0 )
    {
      // do search and replace action here
      if( search.length() && doReplace )
      {
        paragraphs_Obj obj;
        obj.key(atoll(rs.getField(iField).format().c_str()));
        if( obj.get_data() )
        {
          ocString fixContent = obj.content;
          obj.content = fixContent.replaceAll( search, replace );
          changeMap changes;
          changes["content"]="content";
          obj.db_update(changes);
          webIO <<  td.replace( "$data$", obj.content.c_str() );
        }
      }
      else
      {
         webIO <<  td.replace( "$data$", "*" );
      }
      
    }
    else if(iField == 2 )
    { 
      ocString Parse(rs.getField(iField).format());

      while( Parse.endOfParse() == false )
      {
        data += Parse.parse( "<" );
        data += " ";
        Parse.parse( ">" );
      }

    }
    else
    {
      data = rs.getField(iField).format(); 
    }
    if( data.length() )
    {
      webIO <<  td.replaceAll( "$data$", data.c_str() );
    }
  }
}; 



  
class paragraphs_form:  public paragraphs_Obj, public forms_base
{
  int relPos;
public:

  llong site_id, page_id;
  string search, replace;
  bool doReplace;
  
  string default_edit_handler;
 
  paragraphs_form(cgiScript & script):paragraphs_Obj(),forms_base(script),relPos(0),site_id(0),page_id(0),doReplace(false)
  {
    setKey(*this);
  } 
  virtual ~paragraphs_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "site_id", site_id);
    // if it's still 0, then get it from where we are
    if( site_id <= 0 )
    {
      site_id = atoll( currentSite().c_str() );
    }

    llongFXfer( "page_id", page_id);
    boolFXfer( "doReplace", doReplace);
    stringFXfer( "search", search);
    stringFXfer( "replace", replace);
     
  }

  bool dbf_action( string mode, changeMap & changes )
  {
    bool ret = true;
    ret = true; 
    return ret;
  }


  
  // implement pure virtual of form display
  bool form_display( void )
  { 
    bool breturn = true;
    
    ocString sql;
    script << makeTop("contentSearch.cgi", "Content Search and Replace");
    

    sql = "select id, name from sites where id = ";
    sql.append(site_id);

    
    script << makeComboBox("site_id", "site_id", site_id ,sql);
    script << "<br class='clearall'>" << endl;


    sql = "select id, name from pages where site_id = ";
    sql.append(site_id);

    script << makeComboBox("page_id", "page_id", page_id ,sql,"All");
    script << "<br class='clearall'>" << endl;


    script << makeTextBox("Search", "search", search ,"125","55");
    script << "<br class='clearall'>" << endl;


    script << makeTextBox("Replace", "replace", replace ,"125","55");
    script << "<br class='clearall'>" << endl;

    script << makeBoolBox("do Replace?", "doReplace", doReplace );
    script << "<br class='clearall'>" << endl;
    
    script << flexButtons( key(), "", " Search ","","","", false,true,false,false,false );
    
    if( site_id )
    {
      script << "<br class='clearall'>" << endl; 
      paragraphs_List paragraphsList(script);
      paragraphsList.Search_Scope = " site_id = ";
      paragraphsList.Search_Scope.append(site_id);
      if( page_id > 0 )
      {
        paragraphsList.Search_Scope += " and page_id = ";
        paragraphsList.Search_Scope.append(page_id);
      }
      
      paragraphsList.loadListTemplates("Templates/list.htmp");
      paragraphsList.search = search;
      paragraphsList.replace = replace;
      paragraphsList.doReplace = doReplace;
      paragraphsList.list_display();
    }

    
    script << makeBottom( m_result ) << endl;

    return breturn;
  }

 };

int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false  );
  paragraphs_form myFrm(script); 
   
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;
    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top")).replaceAll("$heading$","W<sup>3</sup> content Editor");

    // includes the essential scripts and styles for editing
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
