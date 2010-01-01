/*
  HostingUI.cpp

  User Interface Definition and Implementation 
  for Hosting.

  Copyright (c) 2008 by D.K. McCombs.
  davidmc@w3sys.com
  W3 Systems Design Inc.

*/
#include <string>
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
using namespace std;


#include "w3intranet.h"
#include "Hosting.hpp"
#include "Status_Enumerations.h"

class Hosting_List:  public list_base
{
public: 
  // Constructor
  Hosting_List(cgiScript&sc,Session_Obj & session):list_base(sc,session,"Hosting"){;}   
  ~Hosting_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;

    hotCol=-2;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","HostingUI.meta");
    // TODO:  Make sure you set the filter by name
    emitFilter( "HostingUI.meta",
                "<strong>Hostings · Search by Company</strong>" );
    string heading =
      "<a class='sortcol' href='HostingUI.meta?sort=n.Id'>Id</a>|"
      "<a class='sortcol' href='HostingUI.meta?sort=c.Name'>Company</a>|"
      "<a class='sortcol' href='HostingUI.meta?sort=h.Invoice'>Invoice</a>|"
      "<a class='sortcol' href='HostingUI.meta?sort=h.From_Date'>From Date</a>|"
      "<a class='sortcol' href='HostingUI.meta?sort=h.To_Date'>To Date</a>|"
      "<a class='sortcol' href='HostingUI.meta?sort=h.Site_URL'>Site URL</a>|"
      "<a class='sortcol' href='HostingUI.meta?sort=h.Billing_Interval'>Billing Interval</a>"
                    ;
    emitHeadings(heading);
    getFilteredData(
                     "h.Id, "
                     "c.Name, "
                     "h.Invoice, "
                     "h.From_Date, "
                     "h.To_Date, " 
                     "h.Site_URL, " 
                     "h.Billing_Interval " 
                     ,"Hosting h left join Company c on h.Company = c.Id ",
                     "c.Name like '$filter$%'" );
    emitData();
    emitNavigation("HostingUI.meta");
    emitEnd();
    return breturn;
  }      
}; 

class Hosting_form:  public Hosting_Obj, public forms_base
{
public: 
  Hosting_form(cgiScript & script):Hosting_Obj(),forms_base(script){setKey(*this);} 
  virtual ~Hosting_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "Company", Company);
    llongFXfer( "Invoice", Invoice);
    dateFXfer( "From_Date", From_Date);
    dateFXfer( "To_Date", To_Date);
    stringFXfer( "Site_URL", Site_URL);
    intFXfer( "Billing_Interval", Billing_Interval);
    moneyFXfer( "Charge_Per_Month", Charge_Per_Month);
    moneyFXfer( "Charge", Charge);
 
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    ocString sql = "select Id, Name from Company"; // for combo boxes
    
    script << makeTop("HostingUI.meta", "Hosting")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("Id", "Id", Id ,"8");
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");
    
    script << makeComboBox("Company", "Company", Company ,sql,"Please Choose");
    script << "<br class='clearall'>" << endl;
    sql = "select Id, Invoice_Date from Invoice where (";
    sql += " Status = " +   Status_Id(Time_Pre_Invoice) ;
    sql += " or  Id = ";
    sql.append( Invoice );
    sql += ") and  Company = ";
    sql.append( Company );
    script << makeComboBox("Invoice", "Invoice", Invoice ,sql,"Please Choose");
    script << "<br class='clearall'>" << endl; 
    script << makeDateBox("From Date", "From_Date", From_Date ,"10");
    script << "<br class='clearall'>" << endl; 
    script << makeDateBox("To Date", "To_Date", To_Date ,"10");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Site_URL", "Site_URL", Site_URL ,"125","35");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Billing Interval", "Billing_Interval", Billing_Interval ,"8","8");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Charge Per Month", "Charge_Per_Month", Charge_Per_Month ,"16","16");
    script << "<br class='clearall'>" << endl; 
    script << makeTextBox("Charge", "Charge", Charge ,"16","16");
    script << "<br class='clearall'>" << endl; 
    script << makeButtons( key() );
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};

bool intraMain(page & pg ,cgiScript & script)
{

  Hosting_form myFrm(script); 
  Hosting_List mylist(script,oLogin.Session());  

  myFrm.loadControlTemplates("Templates/divform.htmp");
  myFrm.form_action();  
  myFrm.form_display();

  mylist.loadListTemplates("Templates/navlist.htmp");  
  mylist.list_display();
 
  return true;
}

// Standard main function
int main(int argc, char ** argv)
{  
  baseUrl = "HostingUI.meta";  // this is my compiled name!
  // because we are checking
  cgiScript script("text/html",false);
  bool isSignedOn = check(script);
  script.closeHeader();
  
  /* TODO: Optionally replace intranet.meta with anything you please, or not,
     make sure an associated page entry exists in the metasite database. 
  script.ScriptName() =
    ocString( script.ScriptName().c_str()).replace("HostingUI.meta","intranet.meta").c_str();  
  */  
  page pg(script);  
  
  writelog( "load page" );  
  if( pg.load() )
  {    
    writelog( "instance of page control" );
    page_control ctrl(pg);     
    ctrl.addOp ( "servicelist",  new servicelist_functor(pg,script,isSignedOn) );    
    ctrl.addOp ( "userinterface",  new form_functor(pg,script,isSignedOn) );    
    writelog( "page control emit" );
    ctrl.emit();    
  }
  else
  {
    script << "<html><head><title>Page Unavailable</title></head>" << endl
           << "<body><h1>Sorry</h1>" << endl
           << "<p>The page you requested is not available</p></body></html>";
  } 
  return 0;
}

// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"

