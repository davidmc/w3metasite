/*
  Product UI
  =============
  List and Form for AP Products

*/
#include "connectstring"
#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "cgiTemplates.h"
#include "Products.hpp"
#include "nav_menu.h"
#include "list_base.hpp"

/* 
  Product select List
*/

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
    hotCol=1;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","art_product_ui.cgi");
    string heading = " Id | Product Title | Artisan ";
    emitFilter( "art_product_ui.cgi",  // program name
                "&nbsp;&nbsp;&nbsp;&nbsp;<B>FILTER (by Artist Last Name)</B>" );  // Label
    emitHeadings(heading);
    getFilteredData( "p.Id, concat(p.Title,' * '), concat( a.First_Name, ' ', a.Last_Name )", 
                     "Products p inner join Person a on p.Artisan_Id = a.Id ", 
                     "a.Last_Name like '$filter$%'", 
                     "p.Title, a.Last_Name, a.First_Name" );
    emitData();
    emitNavigation("art_product_ui.cgi");
    emitEnd();
    return breturn;
  }      
}; 

/*
  Product Form - Glues the business object to the form object through 
                 multiple inheritance.                  
*/ 

class Product_Form:  public Products, public forms_base
{
public:
 
  // Constructor
  Product_Form(cgiScript & in):Products(),forms_base(in){setKey(*this);}  
  ~Product_Form(){;}
  
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  
  void form_data_transfer( void )
  {
    // Product 
    llongFXfer( "Category_Id", Category_Id );
    llongFXfer( "Sub_Category_id", Sub_Category_id );
    llongFXfer( "Artisan_Id",  Artisan_Id);
    llongFXfer( "Handler_Id", Handler_Id );
    llongFXfer( "Group_Id", Group_Id );    
    stringFXfer( "Title",  Title);
    stringFXfer( "Description", Description );
    stringFXfer( "Dimensions", Dimensions );
    stringFXfer( "Weight", Weight );
    moneyFXfer( "Price", Price );
    stringFXfer( "List_Picture", List_Picture );
    stringFXfer( "Detail_Picture", Detail_Picture );
    stringFXfer( "Keywords", Keywords ); 
    stringFXfer( "Stock_Number", Stock_Number ); 
    llongFXfer( "Stock_Count", Stock_Count );
  }
   
  bool dbf_action( string mode, changeMap & changes )
  {  
    if(Sub_Category_id>1)
    {
      ocString sqry ="select Category_Id from Sub_Category where id = ";
      sqry.append(Sub_Category_id);
      if( rs.open( sqry ) )
      {
        Category_Id = atoll( rs.getField(0).format().c_str() );
        rs.close();
        changes["Category_Id"]="Category_Id";
        script << "Cat Is " << Category_Id << endl;
      }
    }      
    return db_action( mode, changes );
  }
   
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script  << makeTop( "art_product_ui.cgi", "Product" )
            << makeStaticBox("Id","Id",Id,"12")            
            << makeComboBox("Category","Sub_Category_id"
                            ,Sub_Category_id
                            ,"select sc.Id, concat(c.Name, ' - ', sc.name) from Category c "
                            "inner join Sub_Category sc on c.Id=sc.Category_Id "
                            " order by c.Name, sc.name "
                            ,"Select Product Category");
    script  << makeComboBox("Artisan","Artisan_Id",Artisan_Id,
                            "select Id, concat(First_Name,' ', Last_Name) "
                            " from Person p inner join Artisan a"
                            " on p.Id = a.Person_Id" 
                            " order by Last_Name, First_Name" )    
            << makeComboBox("Product Handler","Handler_Id", Handler_Id,
                            "select Id, concat(First_Name,' ', Last_Name) "
                            " from Person p inner join Artisan a"
                            " on p.Id = a.Person_Id"   
                            " order by Last_Name, First_Name" );                           
    script  << makeComboBox("Artisan Group","Group_Id"
                            ,Group_Id
                            ,"select Id, Name from Artisan_Group order by Name"
                            ,"Select Group (if any)")                                                      
            << makeTextBox("Title","Title",Title,"35")
            << makeTextArea("Description","Description",Description,"5","80");
    script  << makeTextBox("Dimensions","Dimensions",Dimensions,"35")
            << makeTextBox("Weight","Weight",Weight,"35")
            << makeTextBox("Price","Price",Price,"35")
            << makeImageBox("List Picture","List_Picture",List_Picture,"35")
            << makeImageBox("Detail Picture","Detail_Picture",Detail_Picture,"35")
            << makeTextBox("Keywords","Keywords",Keywords,"35")
            << makeTextBox("Product Stock ID#","Stock_Number",Stock_Number,"35")
            << makeTextBox("Amount in Stock","Stock_Count",Stock_Count,"10")
            << makeButtons( key() )
            << makeBottom( m_result ) << endl; 
    return breturn;
  }      
};


int main( int argc, char ** argv )
{
  cgiScript script("text/html",false);
  Product_Form  prod(script); 
  Products_List  mylist(script);  
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();  
    cgiTemplates pgTemplate;
    pgTemplate.load("Templates/adminPane.htmp");
    script << ocString(pgTemplate.getParagraph("top"))
              .replaceAll("$heading$","Product Editor")
              .replace("$menu$",getNavMenu().c_str())
              .replace("$instructions$",
                    "You may insert, update or delete an "
                    "Product here.");  

    prod.loadControlTemplates("Templates/form.htmp"); 

    prod.form_action();
    prod.form_display();
    
    mylist.loadListTemplates("Templates/list.htmp");  
    mylist.list_display();
    
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

