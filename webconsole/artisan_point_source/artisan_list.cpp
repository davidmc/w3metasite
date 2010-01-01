/*
  Artisan List
  ===================================
  CGI List for Artisan Point Category  
    
*/
#include "connectstring"
#include "list_base.hpp"
#include "nav_menu.h"

class Artisan_List:  public list_base
{
public: 
  // Constructor
  Artisan_List(cgiScript&sc):list_base(sc){;}  
  ~Artisan_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    string sql = "select Id, First_Name, Last_Name from Person p inner join Artisan a"
                 " on p.Id = a.Person_Id";   
    sql += " order by Last_Name, First_Name";
    editLink = "<a href='artisan_form.cgi?Id=$key$'>Edit</a>";
    string heading = " [Id] | First| Last | <a href='artisan_form.cgi'>New</a>";
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
    // Category = everything
    Artisan_List mylist(sc);  
    cgiTemplates pgTemplate;
    pgTemplate.load("Templates/adminPane.htmp");
    sc << ocString(pgTemplate.getParagraph("top"))
            .replaceAll("$heading$","Artisan List")
            .replace("$menu$",getNavMenu().c_str())
            .replace("$instructions$",
                    "Please add and/or edit Artisans here.");                 
    mylist.loadListTemplates("Templates/list.htmp");  
    mylist.list_display();
    sc << pgTemplate.getParagraph("list-bottom");
  }
  else
  {    
    showSignOnForm (sc);    
  }
};
