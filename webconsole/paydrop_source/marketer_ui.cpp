/*
  Marketer Form
  ===================================
  CGI User Interface for Pay Drop Marketer
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
#include "Marketer.hpp"
#include "nav_menu.h"

/*
  Marketer Form - Glues the business object to the form object through 
                     multiple inheritance.
*/
class Marketer_Form:  public Marketer, public forms_base
{
public: 
  // Constructor
  Marketer_Form(cgiScript & in):Marketer(),forms_base(in){setKey(*this);}  
  ~Marketer_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "MarketerID", MarketerID );
  }
  void form_data_transfer( void )
  {    
    stringFXfer( "MarketerName", MarketerName );
    stringFXfer( "TrackingCode", TrackingCode );
    longFXfer( "FileType",FileType);
    longFXfer( "HeaderLines",HeaderLines);
    longFXfer( "MemberIDColumn", MemberIDColumn );
    longFXfer( "StoreIDColumn", StoreIDColumn );
    longFXfer( "PurchaseDateColumn", PurchaseDateColumn );
    longFXfer( "PurchaseOrderNumberColumn", PurchaseOrderNumberColumn );
    longFXfer( "PurchaseSkuNumberColumn", PurchaseSkuNumberColumn );
    longFXfer( "PurchaseAmountColumn", PurchaseAmountColumn );
  } 
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script << makeTop( "marketer_ui.cgi", "Marketers" )
           << makeTextBox( "Marketer ID","Id",MarketerID,"12" )                  
           << makeTextBox("Marketer Name*:","MarketerName",MarketerName,"35")
           << makeTextBox("Tracking Code*:","TrackingCode",TrackingCode,"8") 
           << makeComboBox( "Import File Type", "FileType", FileType, 
                            "select FileTypeID, Name from FileType" )             
           << makeTextBox("Header Lines*:","HeaderLines",HeaderLines, "8") 
           << makeTextBox("Member ID Column*:","MemberIDColumn",MemberIDColumn,"8") 
           << makeTextBox("Store ID Column*:","StoreIDColumn",StoreIDColumn,"8") 
           << makeTextBox("Purchase Date Column*:  	","PurchaseDateColumn",PurchaseDateColumn,"8") 
           << makeTextBox("Purchase Order Number Column*:","PurchaseOrderNumberColumn",PurchaseOrderNumberColumn,"8") 
           << makeTextBox("Purchase SKU Number Column*:","PurchaseSkuNumberColumn",PurchaseSkuNumberColumn,"8") 
           << makeTextBox("Purchase Amount Column*:","PurchaseAmountColumn",PurchaseAmountColumn,"8") 
           << makeButtons( key() )
           << makeBottom( m_result ) << endl; 
    return breturn;
  }      
};

class Marketer_List:  public list_base
{
public: 
  // Constructor
  Marketer_List(cgiScript&sc):list_base(sc)
  {
    ;                   
  }  
  ~Marketer_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","marketer_ui.cgi");
    hotCol=1;    
    
    emitFilter( "marketer_ui.cgi",  // program name
                "Marketer" );       // Label
    
    emitHeadings("Id|Marketer|Tracking Code"); 
            
    getFilteredData( "MarketerID, MarketerName, TrackingCode", 
                     "Marketer", 
                     "MarketerName like '$filter$%'", "MarketerName" );     
    emitData();
    
    emitNavigation("marketer_ui.cgi");
    
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
  Marketer_Form  marktr(script); 
  Marketer_List mylist(script); // so you can set cookies 
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << pgTemplate.getParagraph("top");

    marktr.loadControlTemplates("Templates/form.htmp");  
    marktr.form_action();  
    marktr.form_display();
    
    
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


 
