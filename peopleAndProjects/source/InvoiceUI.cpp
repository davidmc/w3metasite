/*
  InvoiceUI.cpp

  User Interface Definition and Implementation 
  for Invoice.
  
  Copyright (c) 2006 by D.K. McCombs.
  
  davidmc@w3sys.com
  
  W3 Systems Design Inc.  
  
*/
#include <string>
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
using namespace std;


#include "w3intranet.h"
#include "Invoice.hpp"
#include "sublist_base.hpp"
#include "lookup.hpp"

class Hosting_List:  public sublist_base
{
public: 
  ocString Parent_Id;
  // Constructor
  Hosting_List(cgiScript&sc):sublist_base(sc){;}  
  ~Hosting_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    string sql =  "select h.Id, "
                  "c.Name, "
                  "h.From_Date, "
                  "h.To_Date, "
                  "h.Site_URL, "
                  "h.Billing_Interval, h.Charge_Per_Month, h.Charge "
                  " from Hosting h left join Company c on h.Company = c.Id "
                  " where h.Invoice = " ;
    sql +=  Parent_Id;
    hotCol=-2;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","HostingUI.meta");        
    string heading = "Id|"
                     "Company|"
                     "From_Date|"
                     "To_Date|"
                     "Site_URL|"
                     "Billing_Interval|"
                     "Charge_Per_Month|"
                     "Charge" ;
   
    emitHeadings(heading,"Hosting List");

    getData( sql );
    emitData();

    emitEnd();
    return breturn;
  }      
}; 
 class Charge_List:  public sublist_base
{

public: 
  ocString Parent_Id;
  double total;
  
  // Constructor
  Charge_List(cgiScript&sc):sublist_base(sc){;}  
  ~Charge_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    string sql =  "select g.Id, "
                     "c.Name, "
                     "p.Name, "
                     "g.Name, " 
                     "g.Description, "
                     "g.MyCost, "
                     "g.Charge "
                     " from Charge g left join Company c on c.Id = g.Company "
                     "left join Project p on p.Id = g.Project where g.Invoice = " ;
    sql +=  Parent_Id;

   
    hotCol=-2;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","ChargeUI.meta");
    string heading =
      "Id|"
      "Company|"
      "Project|"
      "Name|"
      "Description|"
      "MyCost|"
      "Charge";                    ;
    emitHeadings(heading,"Charge List");
    getData( sql );
    emitData();

    emitEnd();
    return breturn;
  }
  virtual void sendHotField( int iField, ocString & td )
  {
    if( iField == 6 )
    {
      total += atof( rs.getField(iField).format().c_str());
    }
    sublist_base::sendHotField( iField, td );
  }
  void derived_complete_event( void )
  {
    double rate;
    money amount= total;
    webIO << "<tr><th colspan='6'>Total:</th><td> &nbsp" << amount << "</td></tr>" << endl;
  }
}; 


class Time_List:  public sublist_base
{

  public:
    ocString Parent_Id;
    ocString Invoice_Id;
    double total;
    bool lockChanges;
    
  // Constructor
    Time_List(cgiScript&sc):sublist_base(sc),total(0.0),lockChanges(false){;}
    ~Time_List(){;}

    
    bool list_display( void )
    {
      bool breturn = true;
      string sql =  "select t.Id,tk.Name,t.Start,t.Description,t.Hours,t.Status,t.Invoice "
                    "from Time t inner join Task tk on tk.Id = t.Task left join Project p on p.Id = tk.Project ";
      if( lockChanges )
      {
        sql += "where t.Invoice = ";
        sql += Invoice_Id;
      }
      else
      {
        sql += "where (t.Status = 8 and p.Id = " ;
        sql +=  Parent_Id;
        sql += ") or t.Invoice = ";
        sql += Invoice_Id;
      }
      hotCol=-2;
      editLink = listTemplate.getParagraph("hotcolumn");
      editLink = editLink.replace("$prog$","Time_Popup.cgi");
      string heading = "Id|Task|Start|Description|Hours|Status|Invoice";
      emitHeadings(heading);
      getData( sql );
      emitData();

      emitEnd();
      return breturn;
    }
    
    virtual void sendHotField( int iField, ocString & td )
    {
      if( iField == 4 )
      {
        total += atof( rs.getField(iField).format().c_str());
      }
      if( iField == 6 )
      {
        string item;
        webIO << td.replace( "$data$", makeTimeCheckBox(rs.getField(0).format(), rs.getField(iField).format(), item).c_str() );
      }
      else  sublist_base::sendHotField( iField, td );
    }
    string & makeTimeCheckBox(string & id, string & invoiceId, string & item)
    {
      if( lockChanges )
      {
        item += "locked";
      }
      else
      {
      item += "<input type='checkbox' id='ck";
      item += id;
      item += "' value='Invoice=";
      item +=Invoice_Id;
      item += "&Id=";
      item += id;
      item += "' onClick='handleClick(this)' ";
      if( atoll(invoiceId.c_str()) > 0 ) item += "checked='yes' ";
      item += "/>&nbsp add";
      }
      return item;
    }
    void derived_complete_event( void )
    {
      double rate;
      money amount;
      if(  Parent_Id.length() )
      {
        string sql = "select   Rate from  Project where Id = " + Parent_Id;
        string srate = tableLookup(sql);
        if(srate.length())
        {
          rate = atof( srate.c_str() );
          amount = total * rate;
        }
      }
      webIO << "<tr><th colspan='4'>Total:</th><td>" << total << "</td><td>" << amount << "</td></tr>" << endl;;
    }  
}; 


class Invoice_List:  public list_base
{
public: 
  // Constructor
  Invoice_List(cgiScript&sc,Session_Obj & session):list_base(sc,session){;}   
  ~Invoice_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;

    hotCol=-2;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","InvoiceUI.meta");
    // TODO:  Make sure you set the filter by name
    emitFilter( "InvoiceUI.meta",
                "<h5>Invoices · Search by Project</h5>" );
    string heading =
        "<a class='sortcol' href='InvoiceUI.meta?sort=inv.Id'>Id</a>|"
        "<a class='sortcol' href='InvoiceUI.meta?sort=c.Name'>Company</a>|"
        "<a class='sortcol' href='InvoiceUI.meta?sort=p.Name'>Project</a>|"
        "<a class='sortcol' href='InvoiceUI.meta?sort=inv.Notes'>Notes</a>|"
        "<a class='sortcol' href='InvoiceUI.meta?sort=inv.Invoice_Date'>Invoice_Date</a>|"
        "<a class='sortcol' href='InvoiceUI.meta?sort=inv.Invoice_Time'>Invoice_Time</a>|"
        "<a class='sortcol' href='InvoiceUI.meta?sort=inv.Status'>Status</a>|"
        "<a class='sortcol' href='InvoiceUI.meta?sort=inv.Paid_Date'>Paid_Date</a>"
                    ;
    emitHeadings(heading);
    getFilteredData(
                    "inv.Id, "
                    "c.Name, "
                    "p.Name, "
                    "inv.Notes, "
                    "inv.Invoice_Date, "
                    "inv.Invoice_Time, "
                    "s.Name, "
                    "inv.Paid_Date "
                     ,"Invoice inv left join Project p on p.Id = inv.Project "
                      "left join Company c on c.Id = inv.Company "
                      "left join Status s on s.Id = inv.Status ",
                     // TODO: Make sure you set the filter by field                    
                     "p.Name like '$filter$%'", "inv.Id desc" );
    emitData();
    emitNavigation("InvoiceUI.meta");         
    emitEnd();
    return breturn;
  }

}; 

class Invoice_form:  public Invoice_Obj, public forms_base
{
public: 
  Invoice_form(cgiScript & script):Invoice_Obj(),forms_base(script){setKey(*this);} 
  virtual ~Invoice_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    llongFXfer( "Company", Company);
    llongFXfer( "Project", Project);
    llongFXfer( "Contact", Contact);
    stringFXfer( "Notes", Notes);
    dateFXfer( "Invoice_Date", Invoice_Date);
    timeFXfer( "Invoice_Time", Invoice_Time);
    llongFXfer( "Status", Status);
    dateFXfer( "Paid_Date", Paid_Date);
 
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;

    // If true don't do checkboxes, and only show items that are part of this invoice 
    bool lockChanges = false;
    llong invoiceStatus = l_Status_Enum(Status);
    if( invoiceStatus == Time_Invoiced ||  invoiceStatus == Time_Paid )
    lockChanges = true;
    
    ocString sql = "select distinct c.Id, c.Name from Company c inner join Project p on p.Company = c.Id "; // for combo boxes
    // TODO: Set the sql for combos, replace any fields with special controls, properly set lengths.
    script << makeTop("InvoiceUI.meta", "Invoice")
           << formTemplate.getParagraph("advanced_begin");
    script << makeStaticBox("Id", "Id", Id ,"8");
    script << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");
    if( Project > 0 )
    {
      sql += " where p.Id = ";
      sql.append(Project);
    }
    sql += " order by c.Name ";
    script << makeComboBox("Company", "Company", Company ,sql,"Please Choose");
    script << "<br class='clearall'>" << endl;
    sql = "select Id, Name from Project";
    if(  Company > 0)
    {
      sql += " where  Company = ";
      sql.append(Company);
    }
    sql += " order by Name";
    
    script << makeComboBox("Project", "Project", Project ,sql,"Please Choose");
    script << "<br class='clearall'>" << endl;
    sql = "select Id, First, Last from Contact";
    if(  Company > 0 )
    {
      sql += " where  Company = ";
      sql.append(Company);
    }
    
    script << makeComboBox("Contact", "Contact", Contact ,sql,"Please Choose");
    script << "<br class='clearall'>" << endl;
    
    script << makeTextArea("Notes", "Notes", Notes ,"4","55");
    script << "<br class='clearall'>" << endl; 
    script << makeDateBox("Invoice_Date", "Invoice_Date", Invoice_Date ,"10");
    script << "<br class='clearall'>" << endl; 
    script << makeTimeBox("Invoice_Time", "Invoice_Time", Invoice_Time ,"8");
    script << "<br class='clearall'>" << endl;
    sql = "select Id, Name from Status where class = 'Time' order by Sequence";
    script << makeComboBox("Status", "Status", Status ,sql,"Please Choose");
    script << "<br class='clearall'>" << endl; 
    script << makeDateBox("Paid_Date", "Paid_Date", Paid_Date ,"10");
    script << "<br class='clearall'>" << endl; 
  
    script << makeButtons( key() );
           
    if(  key() > 0 && Project > 0 )
    {
      script << "<br class='clearall'>" << endl;
      Time_List TimeList(script);
      TimeList.Parent_Id.append(Project);
      TimeList.Invoice_Id.append(key());
      TimeList.lockChanges =  lockChanges;
      TimeList.loadListTemplates("Templates/spawninglist.htmp");
      script << "<h3>Times:</h3>" << endl;
      TimeList.list_display();

      Charge_List ChargeList(script);
      ChargeList.Parent_Id.append(key());
      ChargeList.loadListTemplates("Templates/spawninglist.htmp");
      script << "<h3>Charges:</h3>" << endl;
      ChargeList.list_display();

      Hosting_List HostingList(script);
      HostingList.Parent_Id.append(key()); 
      HostingList.loadListTemplates("Templates/spawninglist.htmp");
      script << "<h3>Hosting:</h3>" << endl;
      HostingList.list_display();
     
    }
     
    if( key() )
    {
      script << "<a href='http://devlinux/projects/Reports.cgi?Id=5&Invoice=" << key() << "'>Print Invoice</a>";
    }
      

    
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};

bool intraMain(page & pg ,cgiScript & script)
{

  Invoice_form myFrm(script); 
  Invoice_List mylist(script,oLogin.Session());  

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
  baseUrl = "InvoiceUI.meta";  // this is my compiled name!
  // because we are checking
  cgiScript script("text/html",false);
  isSignedOn = check(script);
  script.closeHeader();
  
  /* TODO: Optionally replace intranet.meta with anything you please, or not,
     make sure an associated page entry exists in the metasite database. 
  script.ScriptName() =
    ocString( script.ScriptName().c_str()).replace("InvoiceUI.meta","intranet.meta").c_str();  
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

