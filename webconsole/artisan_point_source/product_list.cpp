/*
  Products List
  ===================================
  CGI List for Products Point Category  
    
*/
#include "connectstring"
#include "list_base.hpp"
#include "nav_menu.h"

class Products_List:  public list_base
{
public: 
  // Constructor
  Products_List(cgiScript&sc):list_base(sc){;}  
  ~Products_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    string sql = "select p.Id, p.Title, concat( a.First_Name, ' ', a.Last_Name ) from Products p"
                 " inner join Person a on p.Artisan_Id = a.Id "
                 " order by p.Title, a.Last_Name, a.First_Name";
    editLink = "<a href='product_form.cgi?Id=$key$'>Edit</a>";
    string heading = " [Id] | Product Title | Artisan | <a href='product_form.cgi'>New</a>";
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
  
    Products_List mylist(sc);  
    cgiTemplates pgTemplate;
    pgTemplate.load("Templates/adminPane.htmp");
    sc << ocString(pgTemplate.getParagraph("top"))
            .replaceAll("$heading$","Products List")
            .replace("$menu$",getNavMenu().c_str())
            .replace("$instructions$",
                    "Please add and/or edit Products here.");                 
    mylist.loadListTemplates("Templates/list.htmp");  
    mylist.list_display();
    sc << pgTemplate.getParagraph("list-bottom");
  }
  else
  {    
    showSignOnForm (sc);    
  }
};
