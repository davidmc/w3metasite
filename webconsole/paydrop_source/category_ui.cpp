/*
  Category Form
  ===================================
  CGI User Interface for Pay Drop Category
            
*/

#include "connectstring"
#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "list_base.hpp"
#include "cgiTemplates.h"
#include "Category.hpp"
#include "nav_menu.h"

/*
  Category Form - Glues the business object to the form object through 
                  multiple inheritance.
                  
*/                 
class Category_Form:  public Category, public forms_base
{
public: 
  // Constructor
  Category_Form(cgiScript & script):Category(),forms_base(script){setKey(*this);}  
  ~Category_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "CategoryID", CategoryID );
  }
  void form_data_transfer( void )
  {
    stringFXfer( "CategoryName", CategoryName );
  } 
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script << makeTop( "category_ui.cgi", "Categories" )
           << makeTextBox("Category ID","Id",CategoryID,"12")
           << makeTextBox("CategoryName","CategoryName",CategoryName,"56")          
           << makeButtons( key() )
           << makeBottom( m_result ) << endl; 
    return breturn;
  }      
};

class Category_List:  public list_base
{
public: 
  // Constructor
  Category_List(cgiScript&sc):list_base(sc)
  {
    ;                   
  }  
  ~Category_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","category_ui.cgi");
    hotCol=1;    

    
    emitFilter( "category_ui.cgi",  // program name
                "Category" );       // Label
    
    emitHeadings("Id|Category Name"); 
            
    getFilteredData( "CategoryID, CategoryName", "Category", 
                     "CategoryName like '$filter$%'", "CategoryName" );     
    emitData();
    
    emitNavigation("category_ui.cgi");
    
    emitEnd();
    return breturn;
  }
  void derived_data_event( void )
  {  
      
  }
        
};

int main( int argc, char ** argv )
{  
  cgiScript script( "text/html", false );
  Category_Form  cat(script); 
  Category_List mylist(script); // so you can set cookies 
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << pgTemplate.getParagraph("top");

    cat.loadControlTemplates("Templates/form.htmp");  
    cat.form_action();  
    cat.form_display();
    
    
    mylist.loadListTemplates("Templates/list.htmp");  
    mylist.list_display();
    
    script << pgTemplate.getParagraph("bottom");
  }
  else
  {
    script.Redirect("signIn.html"); 
  }  
};

// as an alternative to building more objects
#include "read_write_base.cpp"
#include "forms_base.cpp"


 
