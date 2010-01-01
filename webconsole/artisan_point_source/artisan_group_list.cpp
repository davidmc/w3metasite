/*
  Artisan List
  ===================================
  CGI List for Artisan Point Category  
    
*/
#include "connectstring"
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
    string sql = "select Id, Name from Artisan_Group";   
    sql += " order by Name";
    editLink = "<a href='artisan_group_form.cgi?Id=$key$'>Edit</a>";
    string heading = " [Id] | Name | <a href='artisan_group_form.cgi'>New</a>";
    emitHeadings(heading);
    getData( sql );
    emitData();
    emitEnd();
    return breturn;
  }      
}; 

int main( int argc, char ** argv )
{
  cgiScript sc("text/html", false);
  if( oLogin.testLoginStatus() )
  {
    sc.closeHeader();  
    Artisan_Group_List mylist(sc);  
    cgiTemplates pgTemplate;
    pgTemplate.load("Templates/adminPane.htmp");
    sc << ocString(pgTemplate.getParagraph("top"))
            .replaceAll("$heading$","Artisan Group List")
            .replace("$menu$",getNavMenu().c_str())
            .replace("$instructions$",
                    "Please add and/or edit Artisan Groups here.");                 
    mylist.loadListTemplates("Templates/list.htmp");  
    mylist.list_display();
    sc << pgTemplate.getParagraph("list-bottom");
  }
  else
  {    
    showSignOnForm (sc);    
  }
};
