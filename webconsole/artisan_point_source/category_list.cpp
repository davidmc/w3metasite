/*
  Category List
  ===================================
  CGI List for Artisan Point Category  
    
*/
#include "connectstring"
#include "list_base.hpp"
#include "nav_menu.h"

class Sub_Category_List:  public list_base
{
public: 
  // Constructor
  Sub_Category_List(cgiScript&sc):list_base(sc){;}  
  ~Sub_Category_List(){;}
  
  bool list_display( string key )
  {
    bool breturn = true;
    string sql = "select Id, Name from Sub_Category where Category_Id = ";
    sql += key;
    sql += " order by Sub_Order";
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","sub_category_form.cgi").replace("_self","editPane");
    hotCol=1;    

    string heading = "ID|SUB-CATEGORY &nbsp; "
                     "<a target='editPane' href='sub_category_form.cgi?Category_Id=";
    heading+=key;
    heading+="'>New</a>";
    emitHeadings(heading);
    getData( sql );
    emitData();
    emitEnd();
    return breturn;
  }      
}; 
                
class Category_List:  public list_base
{
public: 
  // Constructor
  Category_List(cgiScript&sc):list_base(sc){;}  
  ~Category_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;

    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","category_form.cgi").replace("_self","editPane");
    hotCol=1;     
    emitHeadings("ID|CATEGORY &nbsp; <a target='editPane' href='category_form.cgi'>New</a>");
    getData( "select Id, Name from Category  order by Cat_Order" );
    emitData();
    emitEnd();
    return breturn;
  }
  void derived_data_event( void )
  {
    string tr = listTemplate.getParagraph("tr");    
    string end_tr = listTemplate.getParagraph("end_tr");
    webIO << tr;
    Sub_Category_List sc_list( webIO );
    sc_list.copyListTemplates(listTemplate);
    // webIO << "<td>&nbsp</td><td colspan='2' style='padding-left: 50px;'>";
    webIO << "<td colspan='2' style='padding-left: 20px;'> " << endl;
    sc_list.list_display(rs.getField(0).format());
    webIO << "</td>";
    webIO << end_tr;
  }
        
};

int main( int argc, char ** argv )
{
  cgiScript sc("text/html", false);
  if( oLogin.testLoginStatus() )
  {
    // Category = everything
    Category_List mylist(sc);  
    cgiTemplates pgTemplate;
    pgTemplate.load("Templates/adminPane.htmp");
    sc << ocString(pgTemplate.getParagraph("top"))
            .replaceAll("$heading$","Category &amp; Sub-Category Lists")
            .replace("$menu$",getNavMenu().c_str())
            .replace("$instructions$",
                    "Please add and/or edit all product categories and "
                    "sub-categories from here.");
    sc << "<div style='float: left;'>"
          "<iframe src='category_form.cgi' width='550px' height='300px' frameborder='0' name='editPane'>"
          "</iframe></div><div style='float: left; width: 250px;'>";                 
    mylist.loadListTemplates("Templates/list.htmp");  
    mylist.list_display();
    sc << "</div>" << endl;   
    sc << pgTemplate.getParagraph("list-bottom");       
    // sc << pgTemplate.getParagraph("bottom");
  }
  else
  {    
    showSignOnForm (sc);    
  }
  
};



 
