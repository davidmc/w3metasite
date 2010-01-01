/*
  Copyright 2002 - 2004 by D.K. McCombs.
  ======================================
    
*/

#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "list_base.hpp"

#include "openLogin.h"
#include "siteLimit.h"

using namespace std;
openLogin oLogin;

/*

CREATE TABLE page_menus (
  id bigint not null primary key auto_increment,
  site_id bigint NOT NULL,
  menu_id bigint NOT NULL,
  page_id bigint NOT NULL,
  place_order int,
  collapsible int
);

*/


class page_menu: public read_write_base
{
public:
  identifier id;
  long long site_id;  
  long long menu_id;  
  long long page_id;      
  int place_order;
  int collapsible;
  
  page_menu():read_write_base(),id(0LL),menu_id(0LL),page_id(0LL),place_order(0),collapsible(0)
  {  
    // set name
    data_name("metasite.page_menus");
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new llongXfer("site_id", &site_id ));    
    addDXMap( new llongXfer("menu_id", & menu_id));    
    addDXMap( new llongXfer("page_id", &page_id ));    
    addDXMap( new intXfer("place_order", &place_order ));    
    addDXMap( new intXfer("collapsible", &collapsible )); 
  }
};

class page_menu_form:  public page_menu, public forms_base
{
public: 
  page_menu_form(cgiScript & script):page_menu(),forms_base(script){setKey(*this);} 
  virtual ~page_menu_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    // Make data in controls back to object
    llongFXfer( "site_id", site_id );
    llongFXfer( "menu_id",menu_id  );
    llongFXfer( "page_id",page_id  );  
    intFXfer("place_order",place_order );
    intFXfer("collapsible",collapsible ); 
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    string siteLimitation = siteFocus(oLogin);
    ocString sql = "select id, name from metasite.sites";
    if( siteLimitation.length() != 0 )
    {
      sql += " where id ";
      sql += siteLimitation;
    }
    script << makeTop("page_menu_ui.cgi", "Page - Menu Assignment");
    
    script << formTemplate.getParagraph("advanced_begin")
           << makeStaticBox("Id", "id", id, "12")
           << "<br class='clearall'>" << endl
           << makeComboBox( "Site", "site_id", site_id, 
                            sql ); 
    sql = "select id, name from metasite.pages ";
    sql += " where id = ";
    sql.append(page_id);
    script << makeComboBox( "Page", "page_id", page_id, 
                            sql )
           << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end"); 
                                                           
    sql = "select id, name from metasite.menus ";
    if( siteLimitation.length() != 0 )
    {
      sql += " where site_id ";
      sql += siteLimitation;
    }    
    script << makeComboBox( "Menu", "menu_id", menu_id, 
                            sql );
                              
    script << makeTextBox("Place Order","place_order",place_order,"8")
           << makeBoolBox( "Collapsible?", "collapsible", (bool) collapsible ) 
           << "<br class='clearall'>" << endl
           << makeButtons( key() );
   
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  page_menu_form myFrm(script); 

  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgoo;    
    pgoo.load("Templates/adminPane.htmp");
    
    script << ocString(pgoo.getParagraph("top"))
            .replaceAll("$heading$","SITES");

    myFrm.loadControlTemplates("Templates/childdivform.htmp");  
    myFrm.form_action();  
    myFrm.form_display();
    
    ocString end = pgoo.getParagraph("bottom");
    script << end;      
  }
  else
  {
    script.Redirect("signIn.html"); 
  } 
}

// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"
