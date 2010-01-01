/*
  Store Form
  ===================================
  CGI User Interface for Pay Drop Store
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
#include "Store.hpp"
#include "nav_menu.h"

/*
  Store Form - Glues the business object to the form object through 
                     multiple inheritance.
*/
class Store_Form:  public Store, public forms_base
{
public: 
  // Constructor
  Store_Form(cgiScript & in):Store(),forms_base(in){setKey(*this);}  
  ~Store_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "StoreID", StoreID );
  }
  void form_data_transfer( void )
  {    
    llongFXfer( "MarketerID", MarketerID );
    stringFXfer( "MarketerStoreID", MarketerStoreID );
    stringFXfer( "StoreName", StoreName );
    stringFXfer( "StoreLink", StoreLink );
    stringFXfer( "StoreInfo",StoreInfo);
    // Not used yet - may use for image/thumbnamil differentiation 
    longFXfer( "StoreTemplateID", StoreTemplateID );
    stringFXfer( "StoreImage", StoreImage );
    stringFXfer( "CommissionType", CommissionType );
    doubleFXfer( "Commission", Commission );
    doubleFXfer( "Discount", Discount );
    stringFXfer( "Footnote", Footnote );
    boolFXfer( "Featured", Featured );
    boolFXfer( "Enabled", Enabled );    
    // li'l cheater, 
    // sends pipe delimited multi select item to string var.
    stringFXfer( "SubcategoryID", SubcategoryIDs );
    
  }
  bool isupplemental( void )
  {
    return setSubCategories();
  }
  
  // event fired after update
  bool usupplemental( changeMap & changes )
  { 
    bool bRet = deleteSubCategories();
    if(bRet) bRet = setSubCategories();
    return bRet; 
  }
  
  // event fired after delete
  bool dsupplemental( void )
  { 
    return deleteSubCategories(); 
  }
  // event fired after gettring data
  bool ssupplemental( void )
  {
    return retrieveSubCategories();
  }
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script << makeTop( "store_ui.cgi", "Stores" )
           << makeStaticBox(  "Store ID","Id",StoreID,"12" ) 
           << makeComboBox( "Marketer*:","MarketerID",MarketerID,
                            "select MarketerID, MarketerName from Marketer" )
           << makeComboBox( "Sub-Categories*:","SubcategoryID",SubcategoryIDs,
                            "select SubcategoryID, concat(CategoryName, ' - ',  SubcategoryName) "
                            "from Subcategory s inner join Category c on s.CategoryID = "
                            "c.CategoryID", "", " multiple='multiple' size='6'" )  
           << makeTextBox(  "Marketer Store ID","MarketerStoreID",MarketerStoreID,"55" )                                                   
           << makeTextBox(  "Store Name*:","StoreName",StoreName,"55" )
           << makeTextBox(  "Store Link*:","StoreLink",StoreLink,"512","55" )           
           << makeTextArea( "Store Info*:","StoreInfo",StoreInfo ) 
           << makeComboBox( "Store Template:","StoreTemplateID", StoreTemplateID,
                            "select StoreTemplateID, StoreTemplateName from StoreTemplate" )                              
           << makeImageBox( "Store Image*:","StoreImage", StoreImage,"35" ) 
           << makeManualComboBox("Commission Type*:","CommissionType", 
                                 CommissionType, "Dollar ($)=$,Percentage (%)=%" ) 
           << makeTextBox( "Commission*:","Commission",Commission,"8" ) 
           << makeTextBox( "Discount*:","Discount",Discount,"8" ) 
           << makeTextBox( "Footnote: ","Footnote",Footnote,"55" ) 
           << makeBoolBox(  "Featured Store?:","Featured", Featured) 
           << makeBoolBox(  "Store Enabled?:","Enabled", Enabled)                   
           << makeButtons( key() )
           << makeBottom( m_result ) << endl; 
    return breturn;
  }      
};

class Store_List:  public list_base
{
public: 
  // Constructor
  Store_List(cgiScript&sc):list_base(sc)
  {
    ;                   
  }  
  ~Store_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","store_ui.cgi");
    hotCol=2;    
    
    emitFilter( "store_ui.cgi",  // program name
                "Store" );       // Label
    
    emitHeadings("Id|<a style='color:blue;' href='store_ui.cgi?sort=MarketerName'>Marketer</a>|"
                  "<a style='color:blue;' href='store_ui.cgi?sort=StoreName'>Store</a>|"
                  "<a style='color:blue;' href='store_ui.cgi?sort=Featured'>Featured</a>|"
                  "<a style='color:blue;' href='store_ui.cgi?sort=Enabled'>Enabled</a>"); 
            
    getFilteredData( "StoreID, MarketerName, StoreName,"
                     "case Featured when 1 then 'Yes' else 'No' end as Featured,"
                     "case Enabled when 1 then 'Yes' else 'No' end as Enabled", 
                     "Store s inner join Marketer m on s.MarketerID = m.MarketerID", 
                     "StoreName like '$filter$%'", "StoreName" );
    emitData();    
    emitNavigation("store_ui.cgi");    
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
  Store_Form  stor(script); 
  Store_List mylist(script); // so you can set cookies 
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << pgTemplate.getParagraph("top");

    stor.loadControlTemplates("Templates/form.htmp");  
    stor.form_action();  
    stor.form_display();
    
    
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


 
