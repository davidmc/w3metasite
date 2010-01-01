/*
  Artisan-Group Form
  ===================================
  CGI Form for Artisan Point Artisan-Groups  
      
*/
#include "connectstring"
#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "cgiTemplates.h"
#include "Artisan_Group.hpp"
#include "list_base.hpp"
#include "nav_menu.h"

class Artisan_Group_List:  public list_base
{
public: 
  // Constructor
  Artisan_Group_List(cgiScript&sc):list_base(sc){;}  
  ~Artisan_Group_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;

    hotCol=1;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","artisan_group_ui.cgi");
    emitFilter( "artisan_group_ui.cgi",  // program name
                "&nbsp;&nbsp;&nbsp;&nbsp;<B>FILTER (by Name)</B>" );  // Label        
    string heading = " Id | Name ";
    emitHeadings(heading);
    getFilteredData( "Id, Name", 
                     "Artisan_Group ", 
                     "Name like '$filter$%'", 
                     "Name" );   
    emitData();
    emitNavigation("artisan_group_ui.cgi");         
    emitEnd();
    return breturn;
  }      
}; 
                 
class Artisan_Group_Form:  public Artisan_Group, public forms_base
{
public:
 
  // Constructor
  Artisan_Group_Form(cgiScript & in):Artisan_Group(),forms_base(in){setKey(*this);}  
  ~Artisan_Group_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    
    stringFXfer( "Name", Name );
    llongFXfer( "Site_Id", Site_Id );
    llongFXfer( "Primary_Contact_Id", Primary_Contact_Id );
    boolFXfer( "Primary_Is_Default_Handler", Primary_Is_Default_Handler );    
  } 
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    string defaultSite = "";
    if( script.ClientArguments().count("Site_Id") )
    {
      defaultSite += script.ClientArguments()["Site_Id"].c_str();
    }
    script  << makeTop("artisan_group_ui.cgi", "Artisan Group" )
            << makeTextBox("Id","Id",Id,"12")
            << makeTextBox("Name","Name",Name,"56")
            << makeComboBox("Site","Site_Id",Site_Id,
                            "select id, name from sites",
                            defaultSite)
            << makeComboBox("Primary Group Contact","Primary_Contact_Id"
                            ,Primary_Contact_Id
                            ,"select Id, concat(First_Name,' ', Last_Name) "
                            " from Person p inner join Artisan a"
                            " on p.Id = a.Person_Id" 
                            " order by Last_Name, First_Name" 
                            ,"Select for Primary Contact in Group") 
            << makeBoolBox( "Does the <b>Primary Contact</b> handle shipping?",
                            "Primary_Is_Default_Handler"
                            ,Primary_Is_Default_Handler )
            << makeButtons( key() )
            << makeBottom(  m_result  ) << endl; 
    return breturn;
  }      
};


int main( int argc, char ** argv )
{ 
  cgiScript script("text/html",false);
  Artisan_Group_Form  agform(script); 
  Artisan_Group_List mylist(script);  
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();  
    cgiTemplates pgTemplate;
    pgTemplate.load("Templates/adminPane.htmp");
    script << ocString(pgTemplate.getParagraph("top"))
              .replaceAll("$heading$","Artisan Group Editor")
              .replace("$menu$",getNavMenu().c_str())
              .replace("$instructions$",
                      "You may insert, update or delete a "
                      "group here.");

    agform.loadControlTemplates("Templates/form.htmp");  
    agform.form_action();  
    agform.form_display();
    
    mylist.loadListTemplates("Templates/list.htmp");  
    mylist.list_display();
    
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


 
