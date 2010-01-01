/*

  Product Form
  =============
  CGI Form for Product Point Product

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
    return db_action( mode, changes );
  } 
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script  << makeTop( "product_form.cgi", "Product" )
            << makeStaticBox("Id","Id",Id,"12")
            << makeComboBox("Category","Category_Id"
                            ,Category_Id
                            ,"select Id, Name from Category"
                            ,"Select Product Category") 
            << makeComboBox("Sub-Category","Sub_Category_id"
                            ,Sub_Category_id
                            ,"select sc.Id, concat(c.Name, ' - ', sc.name) from Category c "
                            "inner join Sub_Category sc on c.Id=sc.Category_Id"
                            ,"Select Product Sub-Category");
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
            << makeTextArea("Description","Description",Description,"15","80");
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

