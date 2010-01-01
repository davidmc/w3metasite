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
#include "Subcategory.hpp"
#include "nav_menu.h"

/*
  Subcategory Form - Glues the business object to the form object through 
                     multiple inheritance.
*/
class Subcategory_Form:  public Subcategory, public forms_base
{
public: 
  // Constructor
  Subcategory_Form(cgiScript & in):Subcategory(),forms_base(in){setKey(*this);}  
  ~Subcategory_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "SubcategoryID", SubcategoryID );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "CategoryID", CategoryID );
    stringFXfer( "SubcategoryName", SubcategoryName );
  } 
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script << makeTop( "subcategory_ui.cgi", "Sub Categories" )
           << makeTextBox( "Subcategory ID","Id",SubcategoryID,"12" )
           << makeComboBox( "Category ID", "CategoryID", CategoryID, 
                            "select CategoryID, CategoryName from Category" )                       
           << makeTextBox("Subcategory Name","SubcategoryName",SubcategoryName,"56")          
           << makeButtons( key() )
           << makeBottom( m_result ) << endl; 
    return breturn;
  }      
};

class Subcategory_List:  public list_base
{
public: 
  // Constructor
  Subcategory_List(cgiScript&sc):list_base(sc)
  {
    ;                   
  }  
  ~Subcategory_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","subcategory_ui.cgi");
    hotCol=2;    
    
    emitFilter( "subcategory_ui.cgi",  // program name
                "Subcategory" );       // Label
    
    emitHeadings("Id|Category|Subcategory"); 
            
    getFilteredData( "SubcategoryID, CategoryName, SubcategoryName", 
                     "Subcategory sc inner join Category c on sc.CategoryID = c.CategoryID", 
                     "SubcategoryName like '$filter$%'", "CategoryName,SubcategoryName" );     
    emitData();
    
    emitNavigation("subcategory_ui.cgi");
    
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
  Subcategory_Form  scat(script); 
  Subcategory_List mylist(script); // so you can set cookies 
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << pgTemplate.getParagraph("top");

    scat.loadControlTemplates("Templates/form.htmp");  
    scat.form_action();  
    scat.form_display();
    
    
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


 
