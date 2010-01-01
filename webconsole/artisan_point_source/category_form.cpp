/*
  Category Form
  ===================================
  CGI Form for Artisan Point Category  
    
*/

#include "connectstring"
#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
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
  Category_Form(cgiScript & in):Category(),forms_base(in){setKey(*this);}  
  ~Category_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    stringFXfer( "Name", Name );
    llongFXfer( "Cat_Order", Cat_Order );
    llongFXfer( "Personalization_Owner", Personalization_Owner );
    llongFXfer( "Personalization_Group", Personalization_Group );
  } 
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script << makeTop( "category_form.cgi", "Category" )
           << makeTextBox("Id","Id",Id,"12")
           << makeTextBox("Name","Name",Name,"56")
           << makeTextBox("Category Order","Cat_Order",Cat_Order,"12")
           << makeComboBox("Personalization Owner","Personalization_Owner"
                          ,Personalization_Owner
                          ,"select Id, concat(First_Name,' ',Last_Name) "
                           "from Person p inner join Artisan a "
                           "on p.Id = a.Person_Id "
                           "order by Last_Name"
                          ,"Select for Special Artisan") 
           << makeComboBox("Personalization Group","Personalization_Group"
                          ,Personalization_Group
                          ,"select Id, Name from Artisan_Group"
                          ,"Select for Special Group") 
           << makeButtons( key() )
           << makeBottom( m_result ) << endl; 
    return breturn;
  }      
};


int main( int argc, char ** argv )
{  
  cgiScript script("text/html",false);
  Category_Form  cat(script); 
  if( oLogin.testLoginStatus() )
  {
    script .closeHeader();
    cgiTemplates pgTemplate;
    pgTemplate.load("Templates/adminPane.htmp");
    script << ocString(pgTemplate.getParagraph("top"))
              .replaceAll("$heading$","Category Editor")
              .replace("$menu$",getNavMenu().c_str())
              .replace("$instructions$",
                       "You may insert, update or delete a "
                       "category here.");


    cat.loadControlTemplates("Templates/form.htmp");  
    cat.form_action();  
    cat.form_display();
    script << pgTemplate.getParagraph("bottom");
  }
  else
  {
    showSignOnForm(script); 
  } 
};

// as an alternative to building more objects
#include "read_write_base.cpp"
#include "forms_base.cpp"


 
