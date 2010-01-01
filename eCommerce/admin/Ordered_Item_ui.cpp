/*
  Ordered_Item_ui.cpp

  User Interface Definition and Implementation 
  for Ordered_Item.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  
  
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
#include "cgiTemplates.h"

#include "Ordered_Item.hpp"
#include "list_base.hpp"

#include "openLogin.h"


using namespace std;
openLogin oLogin;

class Ordered_Item_form:  public Ordered_Item_Obj, public forms_base
{
public: 
  Ordered_Item_form(cgiScript & script):Ordered_Item_Obj(),forms_base(script)
  {
    setKey(*this);
  } 
  virtual ~Ordered_Item_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "Product_Id", Product_Id);
    llongFXfer( "Order_Id", Order_Id);
    intFXfer( "Product_Count", Product_Count);
    moneyFXfer( "Total_Price", Total_Price);
    moneyFXfer( "Shipping", Shipping);
    stringFXfer( "Internal_Id", Internal_Id);
    stringFXfer( "External_Id", External_Id);
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  { 
    bool breturn = true;
    script << makeTop("Ordered_Item_ui.cgi", "Ordered_Item");
    script << makeStaticBox("Id", "Id", Id ,"8");
    script << makeComboBox("Product_Id", "Product_Id", Product_Id ,sql);
    script << makeComboBox("Order_Id", "Order_Id", Order_Id ,sql);
    script << makeTextBox("Product_Count", "Product_Count", Product_Count ,"8","8");
    script << makeTextBox("Total_Price", "Total_Price", Total_Price ,"16","16");
    script << makeTextBox("Shipping", "Shipping", Shipping ,"16","16");
    script << makeTextBox("Internal_Id", "Internal_Id", Internal_Id ,"25");
    script << makeTextBox("External_Id", "External_Id", External_Id ,"25");
    /*    
      Special Purpose alternative field controls:
      string makeManualComboBox( string label, string name, valType value, 
                        ocString labelValuePair = "", string defaulted = "", 
                        string attributes = "");
      string makeHiddenBox( string name, valType value );
      string makeStaticBox( string label, string name, valType value, 
                          string size, string length = "");
      template<class valType> 
      string makeDateBox( string label, string name, valType value, string size);
      string makeEmailBox( string label, string name, string value, 
                        string size, string length = "");
      string makePasswordBox( string label, string name, 
                            string value, string size); 
      string makeZipBox( string label, string name, string value, string size);
      string makeImageBox( string label, string name, string value, string size); 
      string makeFileBox( string label, string name, string value, string size);                                         
      string makeTextArea( string label, string name, 
                        string value, string rows="12", string cols="80"); 
      string makeStateBox( string label, string name, string value);
    */    
    script << makeButtons( key() );        
    script << makeBottom( m_result ) << endl;     
    return breturn;
  }  
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  Ordered_Item_form myFrm(script); 
   
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
                     .replaceAll("$heading$","Ordered_Item");

    myFrm.loadControlTemplates("Templates/child_form.htmp");  
    myFrm.form_action();  
    myFrm.form_display();
        
    ocString end = pgTemplate.getParagraph("bottom");
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

