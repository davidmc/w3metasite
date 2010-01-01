/*
  Organization Form
  ===================================
  CGI User Interface for Pay Drop Organization
*/
#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "connectstring"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "list_base.hpp"
#include "cgiTemplates.h"
#include "nav_menu.h"
#include "Member.hpp"

class Member_Form:  public Member, public forms_base
{
public: 
  // Constructor
  Member_Form(cgiScript & script):Member(),forms_base(script)
  {
    MemberIsOrganization = true;
    setKey(*this);
  }  
  ~Member_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "MemberID", MemberID );
  }
  void form_data_transfer( void )
  {
    stringFXfer( "UniversalID", UniversalID );
    stringFXfer( "MemberFirstName", MemberFirstName );
    stringFXfer( "MemberLastName", MemberLastName ); 
    stringFXfer( "MemberContact", MemberContact );
    stringFXfer( "MemberPhone", MemberPhone );     
    stringFXfer( "MemberMobile",MemberMobile  );                  
    stringFXfer( "MemberAddress1", MemberAddress1 ); 
    stringFXfer( "MemberAddress2", MemberAddress2 ); 
    stringFXfer( "MemberCity", MemberCity ); 
    stringFXfer( "MemberState", MemberState ); 
    stringFXfer( "MemberProvince", MemberProvince ); 
    stringFXfer( "MemberPostalCode", MemberPostalCode ); 
    stringFXfer( "MemberCountry", MemberCountry ); 
    stringFXfer( "MemberEmail", MemberEmail ); 
    stringFXfer( "MemberPassword", MemberPassword );
    boolFXfer( "MemberWantsNewsletter", MemberWantsNewsletter );
    stringFXfer( "MemberOrganizationSiteURL", MemberOrganizationSiteURL );        
  } 
  bool dbf_action( string mode, changeMap & changes )
  {  
    return db_action( mode, changes );
  } 
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    ocString link = "<a target='myaccount' href='http://(SRVR)'>";
    string stat = m_result;
    if( MemberOrganizationSiteURL.length() )
    {
      stat += "<br>";
      stat += link.replace("(SRVR)",MemberOrganizationSiteURL.c_str());
      stat += "Go To Site...</a>";
    }
    script << makeTop( "organization_ui.cgi", "Organizations" )
           << makeStaticBox("Member ID","Id",MemberID,"12")
           << makeStaticBox("Member External ID","UniversalID",UniversalID,"56")  
           << makeTextBox("Organization Name","MemberFirstName",MemberFirstName,"56")
           << makeTextBox("Fund Name","MemberLastName",MemberLastName,"56")
           << makeTextBox("Contact","MemberContact",MemberContact,"256","56")
           << makeTextBox("Phone","MemberPhone",MemberPhone,"25")
           << makeTextBox("Mobile","MemberMobile",MemberMobile,"25") 
           << makeTextBox("Address1","MemberAddress1",MemberAddress1,"56")
           << makeTextBox("Address2","MemberAddress2",MemberAddress2,"56")
           << makeTextBox("City","MemberCity",MemberCity,"36")
           << makeStateBox("State","MemberState",MemberState)
           << makeTextBox("Province","MemberProvince",MemberProvince,"26")
           << makeTextBox("PostalCode","MemberPostalCode",MemberPostalCode,"6")
           << makeTextBox("Country","MemberCountry",MemberCountry,"26")
           << makeTextBox("Email","MemberEmail",MemberEmail,"56")  
           << makePasswordBox("Password","MemberPassword",MemberPassword,"56")
           << makeStaticBox("Member Since","MemberSince",MemberSince,"56")
           << makeStaticBox("Last Login","MemberLastLogin",MemberLastLogin,"56") 
           << makeBoolBox("Member Wants Newsletter?","MemberWantsNewsletter", MemberWantsNewsletter) 
           << makeTextBox("Organization Site URL","MemberOrganizationSiteURL",MemberOrganizationSiteURL,"256","56")                                
           << makeButtons( key() )
           << makeBottom( stat ) << endl; 
    return breturn;
  }      
};

class Member_List:  public list_base
{
public: 
  // Constructor
  Member_List(cgiScript&sc):list_base(sc)
  {
    addedCriteria = "M.MemberIsOrganization = 1";                   
  }  
  ~Member_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","organization_ui.cgi");
    hotCol=0;    
    
    emitFilter( "organization_ui.cgi",  // program name
                "Organization Name" );       // Label
    
    emitHeadings("Id|"
                 "<a style='color:blue;' href='organization_ui.cgi?sort=M.MemberLastName'>Name</a>|"
                 "<a style='color:blue;' href='organization_ui.cgi?sort=M.MemberEmail'>Email</a>|"
                 "<a style='color:blue;' href='organization_ui.cgi?sort=M.MemberPassword'>Password</a>|"
                 "<a style='color:blue;' href='organization_ui.cgi?sort=M.MemberSince'>Member Since</a>|"
                 "<a style='color:blue;' href='organization_ui.cgi?sort=M.MemberLastLogin'>Last Logon</a>|"
                 "<a style='color:blue;' href='organization_ui.cgi?sort=CurrentDiscount'>CurrentDiscount</a>"); 
     
    getFilteredData( "M.MemberID,concat(M.MemberFirstName,  '&nbsp;',  M.MemberLastName) AS MemberName, M.MemberEmail,"
                     "M.MemberPassword, M.MemberSince, M.MemberLastLogin, "
                     "coalesce( SUM(MP.PurchaseDiscountAmount), 0.00) AS CurrentDiscount ", 
                     "Member M LEFT OUTER JOIN MemberPurchase MP ON M.MemberID = MP.MemberID ", 
                     "M.MemberFirstName like '$filter$%'", 
                     "M.MemberLastName",
                     "M.MemberID, M.MemberFirstName, M.MemberLastName, M.MemberEmail, M.MemberPassword, " 
                     "M.MemberSince, M.MemberLastLogin",
                     "Member M");     
    emitData();    
    emitNavigation("organization_ui.cgi");    
    emitEnd();
    return breturn;
  }
  void derived_data_event( void )
  {  
      
  }        
};

int main( int argc, char ** argv )
{  
  cgiScript scrpt("text/html",false); 
  Member_Form form(scrpt);
  Member_List mylist(scrpt); // so you can set cookies 
  if( oLogin.testLoginStatus() )
  {
    scrpt.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    scrpt << pgTemplate.getParagraph("top");
  
    form.loadControlTemplates("Templates/form.htmp");  
    form.form_action();  
    form.form_display();
    
    mylist.loadListTemplates("Templates/list.htmp");  
    mylist.list_display();
    
    scrpt << pgTemplate.getParagraph("bottom");
  }
  else
  {
    scrpt.Redirect("signIn.html"); 
  } 
};

// as an alternative to building more objects
#include "read_write_base.cpp"
#include "forms_base.cpp"



 
