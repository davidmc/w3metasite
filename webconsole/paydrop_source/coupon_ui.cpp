/*
  Coupon Form
  ===================================
  CGI User Interface for Pay Drop Category            
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
#include "Coupon.hpp"
#include "nav_menu.h"

/*
  Coupon Form - Glues the business object to the form object through 
                     multiple inheritance.
*/
class Coupon_Form:  public Coupon, public forms_base
{
  
public: 

  // to check if we should do extra logic to clean up dead coupons
  string deadcoupons;
  
  // Constructor
  Coupon_Form(cgiScript & in):Coupon(),forms_base(in){setKey(*this);}  
  ~Coupon_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "CouponID", CouponID );
  }
  
  void form_data_transfer( void )
  {
    llongFXfer( "StoreID", StoreID );
    stringFXfer( "CouponLink", CouponLink );
    stringFXfer( "CouponInfo", CouponInfo );
    dateFXfer( "ActiveDate", ExpirationDate );  
    dateFXfer( "ExpirationDate", ExpirationDate );
    
    // extra for dead coupons
    stringFXfer( "deadcoupons", deadcoupons );          
  }
   
  bool dbf_action( string mode, changeMap & changes )
  {
    if( deadcoupons == "clean" )
    {
      string sql = "delete from Coupon where ExpirationDate < current_date()";
      if( cmd.execute(sql) )
        script << "<h4>Expired coupons deleted.</h4>";
    }
    return db_action( mode, changes );
  }
   
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script << makeTop( "coupon_ui.cgi", "Coupons" )
           << makeStaticBox(  "Coupon ID","Id",CouponID,"12" ) 
           << makeComboBox( "Store", "StoreID", StoreID, 
                            "select StoreID, StoreName from Store order by StoreName" )                       
           << makeTextBox( "Coupon Link","CouponLink",CouponLink,"512", "75")    
           << makeTextArea( "Coupon Description","CouponInfo", CouponInfo )
           << makeDateBox( "Active Date", "ActiveDate", ActiveDate, "12")
           << makeDateBox( "Expiration Date", "ExpirationDate", ExpirationDate, "12")           
           << makeButtons( key() )           
           << makeBottom( m_result ) << endl;     
    script << "<div><a class='' href='?deadcoupons=clean'>Clean Dead Coupons</a></div>" << endl;
    return breturn;
  }
  
};

class Coupon_List:  public list_base
{
public: 
  // Constructor
  Coupon_List(cgiScript&sc):list_base(sc)
  {
    ;                   
  }  
  ~Coupon_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","coupon_ui.cgi");
    hotCol=2;    
    
    emitFilter( "coupon_ui.cgi",  // program name
                "Coupon (filter by store name)" ); // Label              
                     
    emitHeadings("Id|"                 
      "<a class='sortcol' href='coupon_ui.cgi?sort=StoreName'>Store</a>|"
      "<a class='sortcol' href='coupon_ui.cgi?sort=ExpirationDate'>ExpirationDate</a>" ); 
            
    getFilteredData( "CouponID, StoreName, ExpirationDate", 
                     "Coupon c inner join Store s on s.StoreID = c.StoreID", 
                     "StoreName like '$filter$%'", "StoreName, ExpirationDate" );     
    emitData();
    
    emitNavigation("coupon_ui.cgi");

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
  Coupon_Form  scat(script); 
  Coupon_List mylist(script); // so you can set cookies 
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << pgTemplate.getParagraph("top");

    scat.loadControlTemplates("Templates/form.htmp");  
    scat.form_action();  
    scat.form_display();
    
    
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

