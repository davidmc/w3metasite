/*
  Sub-Category Form
  ===================================
  CGI Form for Artisan Point Sub-Category  
      
*/
#include "connectstring"
#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "cgiTemplates.h"
#include "Sub_Category.hpp"
#include "nav_menu.h"
                 
class Sub_Category_Form:  public Sub_Category, public forms_base
{
public:
 
  // Constructor
  Sub_Category_Form(cgiScript & in):Sub_Category(),forms_base(in){setKey(*this);}  
  ~Sub_Category_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "Category_Id", Category_Id );
    stringFXfer( "Name", Name );
    llongFXfer( "Sub_Order", Sub_Order );
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
    string defaultCategory = "";
    if( script.ClientArguments().count("Category_id") )
    {
      defaultCategory+=script.ClientArguments()["Category_id"].c_str();
    }
    script  << makeTop("sub_category_form.cgi", "Sub-Category" )
            << makeTextBox("Id","Id",Id,"12")
            << makeComboBox("Parent Category","Category_Id",
                            Category_Id,"select Id, Name from Category",
                            defaultCategory)
            << makeTextBox("Name","Name",Name,"56")
            << makeTextBox("Sub-Category  Order","Sub_Order",Sub_Order,"12")
            << makeComboBox("Personalization Owner","Personalization_Owner"
                            ,Personalization_Owner
                            ,"select Id, Last_Name from Artisans"
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
  Sub_Category_Form  scat(script);
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();  
    cgiTemplates pgTemplate;
    pgTemplate.load("Templates/adminPane.htmp");
    script << ocString(pgTemplate.getParagraph("top"))
              .replaceAll("$heading$","Sub-Category Editor")
              .replace("$menu$",getNavMenu().c_str())
              .replace("$instructions$",
                       "You may insert, update or delete a "
                       "subcategory here.");

    scat.loadControlTemplates("Templates/form.htmp");  
    scat.form_action();  
    scat.form_display();
    script << pgTemplate.getParagraph("bottom");
  }
  else
  {
    showSignOnForm (script); 
  }  
    
};

// as an alternative to building more objects
#include "read_write_base.cpp"
#include "forms_base.cpp"


 
