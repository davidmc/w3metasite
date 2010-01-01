/*
  Payment_Type_Parameter_ui.cpp

  User Interface Definition and Implementation 
  for Payment_Type_Parameter.
  
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
#include "cgiTemplates.h"

#include "openLogin.h"
#include "read_write_base.hpp"
#include "list_base.hpp"
#include "forms_base.hpp"
#include "Payment_Type_Parameter.hpp"

using namespace std;
openLogin oLogin;

class Payment_Type_Parameter_List:  public list_base
{
public: 
  // Constructor
  Payment_Type_Parameter_List(cgiScript&sc):list_base(sc){;}  
  ~Payment_Type_Parameter_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;

    hotCol=-2;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","Payment_Type_Parameter_ui.cgi");
    emitFilter( "Payment_Type_Parameter_ui.cgi",
                "    <B>FILTER (by ???)</B>" );  
    string heading =
                     "<a class='sortcol' href='Payment_Type_Parameter_ui.cgi?sort=Id'>Id</a>|"                     
                     "<a class='sortcol' href='Payment_Type_Parameter_ui.cgi?sort=Service_Type'>Service_Type</a>|"                     
                     "<a class='sortcol' href='Payment_Type_Parameter_ui.cgi?sort=Value_Location'>Value_Location</a>|"                     
                     "<a class='sortcol' href='Payment_Type_Parameter_ui.cgi?sort=Param_Type'>Param_Type</a>|"                     
                     "<a class='sortcol' href='Payment_Type_Parameter_ui.cgi?sort=Interface_Type'>Interface_Type</a>|"                     
                     "<a class='sortcol' href='Payment_Type_Parameter_ui.cgi?sort=Name'>Name</a>|"                     
                     "<a class='sortcol' href='Payment_Type_Parameter_ui.cgi?sort=Machine_Name'>Machine_Name</a>|"                     
                     "<a class='sortcol' href='Payment_Type_Parameter_ui.cgi?sort=Value'>Value</a>|"                     
                     "<a class='sortcol' href='Payment_Type_Parameter_ui.cgi?sort=Instructions'>Instructions</a>|"                     
                    ;
    emitHeadings(heading);
    getFilteredData(
                     "Id, " 
                     "Service_Type, " 
                     "Value_Location, " 
                     "Param_Type, " 
                     "Interface_Type, " 
                     "Name, " 
                     "Machine_Name, " 
                     "Value, " 
                     "Instructions " 
                     ,"Payment_Type_Parameter",                     
                     "??? like '$filter$%'" );   
    emitData();
    emitNavigation("Payment_Type_Parameter_ui.cgi");         
    emitEnd();
    return breturn;
  }
  
  // override with lookups
  virtual void sendHotField( int iField, ocString & td )
  {
    string data = rs.getField(iField).format().c_str();
    switch( iField )
    {
      case 2:
        data=pay_val_location[atol(data.c_str())];
        break;
      case 3:
        data=pay_types[atol(data.c_str())];
        break;
      case 4:
        data=pay_interface_type[atol(data.c_str())];
        break;           
    }
    webIO <<  td.replace( "$data$",
                    editLink.replaceAll( "$key$",rs.getField(0).format().c_str())
                            .replaceAll( "$col$",data ).c_str());
  }     
}; 

class Payment_Type_Parameter_form:  public Payment_Type_Parameter_Obj, public forms_base
{
public: 
  Payment_Type_Parameter_form(cgiScript & script):Payment_Type_Parameter_Obj(),forms_base(script){setKey(*this);} 
  virtual ~Payment_Type_Parameter_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "Service_Type", Service_Type);
    longFXfer( "Value_Location", Value_Location);
    longFXfer( "Param_Type", Param_Type);
    longFXfer( "Interface_Type", Interface_Type);
    stringFXfer( "Name", Name);
    stringFXfer( "Machine_Name", Machine_Name);
    stringFXfer( "Value", Value);
    stringFXfer( "Instructions", Instructions);
 
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    ocString sql; // for combo boxes
    
    script << makeTop("Payment_Type_Parameter_ui.cgi", "Payment_Type_Parameter")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("Id", "Id", Id ,"8");
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");
    sql="select Type, Name from Payment_Service";
    script << makeComboBox("Service_Type", "Service_Type", Service_Type ,sql);
    /*
    script << makeManualComboBox("Service_Type", "Service_Type", Service_Type ,pay_types,pt_reverse);
    */
    script << "<br class='clearall'>" << endl; 
    script << makeManualComboBox("Value_Location", "Value_Location", Value_Location ,pay_val_location,pay_loc_count);
    script << "<br class='clearall'>" << endl; 
    script << makeManualComboBox("Param_Type", "Param_Type", Param_Type ,pay_types,pay_type_count);
    script << "<br class='clearall'>" << endl; 
    script << makeManualComboBox("Interface_Type", "Interface_Type", Interface_Type ,pay_interface_type,pay_iface_count);
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Name", "Name", Name ,"25");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Machine_Name", "Machine_Name", Machine_Name ,"25");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Value", "Value", Value ,"25");
    script << "<br class='clearall'>" << endl; 
    script << makeTextArea("Instructions", "Instructions", Instructions ,"5","50");
    script << "<br class='clearall'>" << endl; 
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
  Payment_Type_Parameter_form myFrm(script); 
  Payment_Type_Parameter_List mylist(script);  
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
                     .replaceAll("$heading$","Payment_Type_Parameter");

    myFrm.loadControlTemplates("Templates/divform.htmp");  
    myFrm.form_action();  
    myFrm.form_display();
    
    mylist.loadListTemplates("Templates/list.htmp");  
    mylist.list_display();
        
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

