/*
  AP Orders UI
  =============
  List and Form for AP Orders
  
*/
#include "connectstring"
#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "cgiTemplates.h"
#include "nav_menu.h"
#include "list_base.hpp"
#include "Orders.hpp"

class Order_Items_List:  public list_base
{
public:
  long long Order_Id; 
  // Constructor
  Order_Items_List(cgiScript&sc):list_base(sc){;}  
  ~Order_Items_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    hotCol=-1;
    webIO << "<tr><td colspan='2'><b>All Items in this Order:</b><br>" << endl;
    string heading = " Id | Title | Count | Total Price ";    
    emitHeadings(heading);
    ocString sql = "select oi.Id, p.Title, oi.Product_Count, oi.Total_Price from " 
             "Ordered_Items oi "
             "inner join Products p on p.Id = oi.Product_Id where oi.Order_Id = ";
    sql.append(Order_Id);
    getData( sql);
    emitData();
    webIO << "</td></tr>" << endl;
    emitEnd();
    return breturn;
  }      
};

class Patron_View: public read_base
{
  ocString sql;
  ocString row;

public:
  Patron_View()
  :sql("select concat(First_Name, ' ', Last_Name), Address, City, State, Zip, Phone, EMail from Person where Id = ")
  ,row("<tr><td style='text-align:right; font-weight:600;'>%s</td><td>%s</td>")
  {;}
  ~Patron_View(){;}
  bool display( llong id, cgiScript & webIO )
  {
    sql.append(id);
    webIO << "<tr><td colspan='2'><div style='float:left; font-weight:strong; margin: 10px;'>Patron shipping information:</div>" << endl
          << "<table style='float: left'>" << endl;
    
    bool isOK = rs.open(sql);
    if(isOK)
    {
      
      webIO << row.replace("%s","Name ").replace("%s",rs.getField(0).format().c_str()) << endl;
      webIO << row.replace("%s","Address").replace("%s",rs.getField(1).format().c_str()) << endl;
      webIO << row.replace("%s","City").replace("%s",rs.getField(2).format().c_str()) << endl;
      webIO << row.replace("%s","State").replace("%s",rs.getField(3).format().c_str()) << endl;
      webIO << row.replace("%s","Zip code").replace("%s",rs.getField(4).format().c_str()) << endl;
      webIO << row.replace("%s","Phone").replace("%s",rs.getField(5).format().c_str()) << endl;
      webIO << row.replace("%s","E Mail").replace("%s",rs.getField(6).format().c_str()) << endl;
      rs.close();
    }
    webIO << "</table></td></tr>" << endl;    
  }
}; 
/* 
  Order select List
  -----------------
    
  Person( Id bigint, First_Name text, Last_Name text )
  Products( Id bigint, Category_Id bigint, Sub_Category_id, Artisan_Id, Handler_Id bigint, Title text)
  Orders( Id bigint, Product_Id bigint, Patron_Id bigint, Order_Status_Id bigint, Order_Total double, Order_Date datetime)
  
*/
class Orders_List:  public list_base
{
public: 
  // Constructor
  Orders_List(cgiScript&sc):list_base(sc){;}  
  ~Orders_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    hotCol=1;    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","art_orders_ui.cgi");
    string heading = " <a style='color:darkgreen;' href='art_orders_ui.cgi?sort=Id'>Id</a> |"
                     " <a style='color:darkgreen;' href='art_orders_ui.cgi?sort=p.Title'>Product Title</a> |" 
                     " <a style='color:darkgreen;' href='art_orders_ui.cgi?sort=pa.Last_Name'>Patron</a> |"
                     " <a style='color:darkgreen;' href='art_orders_ui.cgi?sort=o.Order_Status_Id'>Status</a>";
    emitFilter( "art_orders_ui.cgi",  // program name
                "&nbsp;&nbsp;&nbsp;&nbsp;<B>FILTER (by Artist Last Name)</B>" );  // Label
    emitHeadings(heading);
    getFilteredData( "o.Id, p.Title, concat( pa.First_Name, ' ', pa.Last_Name ), os.Name", 
                     " Orders o "
                     "inner join Products p on p.Id = o.Product_Id "
                     "inner join Person pa on o.Patron_Id = pa.Id " 
                     "inner join Order_Status os on os.Id = o.Order_Status_Id ",
                     "pa.Last_Name like '$filter$%'", 
                     "o.Order_Status_Id" );
    emitData();
    emitNavigation("art_orders_ui.cgi");
    emitEnd();
    return breturn;
  }      
}; 


/*
  Order Form - Glues the business object to the form object through 
                 multiple inheritance.                  
*/ 
class Order_Form:  public Orders, public forms_base
{
public:
 
  // Constructor
  Order_Form(cgiScript & in):Orders(),forms_base(in){setKey(*this);}  
  ~Order_Form(){;}
  
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  
  void form_data_transfer( void )
  {
    // Order 
    llongFXfer( "Product_Id", Product_Id );
    llongFXfer( "Patron_Id", Patron_Id );
    llongFXfer( "Order_Status_Id", Order_Status_Id );
    llongFXfer( "Patron_Payment_Service_Id", Patron_Payment_Service_Id );
    moneyFXfer( "Product_Total", Product_Total );
    moneyFXfer( "Shipping_Total", Shipping_Total );
    moneyFXfer( "Tax_Total", Tax_Total );        
    moneyFXfer( "Order_Total", Order_Total );
    dateFXfer( "Order_Date", Order_Date );
    dateFXfer( "Ship_Date", Ship_Date );
    dateFXfer( "Reconcile_Date", Reconcile_Date );
    // llongFXfer( "Shipping_Co",  Shipping_Co);
    stringFXfer("Transactions_Id", Transactions_Id ); 
    stringFXfer( "Tracking_Number", Tracking_Number );
  }
     
  bool dbf_action( string mode, changeMap & changes )
  {  
    if( mode=="u") 
    {
      changes["Ship_Date"]="Ship_Date";
      changes["Reconcile_Date"]="Reconcile_Date"; 
    }    
    return db_action( mode, changes );
  }
   
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    ocString sqlCbo = "select Id, Title from Products where Id = ";
    sqlCbo.append(Product_Id); 

    script  << makeTop( "art_orders_ui.cgi", "Order Fulfillment" )
            << makeStaticBox("Id","Id",Id,"12")                        
            << makeComboBox("First Product","Product_Id"
                            ,Product_Id
                            ,sqlCbo.c_str() );
                            
    sqlCbo = "select Id, concat(First_Name,' ', Last_Name) from Person where Id = ";
    sqlCbo.append(Patron_Id);
                                 
    script  << makeComboBox("Patron","Patron_Id",Patron_Id,
                            sqlCbo.c_str() );
                            
    script  << makeComboBox("Order_Status_Id","Order_Status_Id", Order_Status_Id,
                            "select Id, Name from Order_Status order by Id" ); 
    sqlCbo = "select  pps.Id, ps.Name from Patron_Payment_Service pps "
             "inner join Payment_Service ps on pps.Payment_Service_Id = ps.Id Where pps.Id = ";
    sqlCbo.append(Patron_Payment_Service_Id); 

    script  << makeComboBox("Payment_Service","Patron_Payment_Service_Id"
                            ,Patron_Payment_Service_Id
                            ,sqlCbo.c_str() )                                                      
            << makeTextBox("Product Total","Product_Total",Product_Total,"9")
            << makeTextBox("Shipping Total","Shipping_Total",Shipping_Total,"9")
            << makeTextBox("Tax Total","Tax_Total",Tax_Total,"9")
            << makeTextBox("Order Total","Order_Total",Order_Total,"9")
            << makeDateBox( "Order_Date", "Order_Date", Order_Date, "8")
            << makeDateBox( "Ship_Date", "Ship_Date", Ship_Date, "8")
            << makeDateBox( "Reconcile_Date", "Reconcile_Date", Reconcile_Date, "8") 
            << makeTextBox("Transactions_Id","Transactions_Id",Transactions_Id,"30")
            << makeTextBox("Tracking_Number","Tracking_Number",Tracking_Number,"16")            
            << makeButtons( key() );
    if(key())
    {        
      Order_Items_List oilist(script);
      oilist.loadListTemplates("Templates/list.htmp"); 
      oilist.Order_Id = Id; 
      oilist.list_display();
      Patron_View patron;
      patron.display(Patron_Id,script);
      
    }
    script  << makeBottom( m_result ) << endl; 
    return breturn;
  }
  string makeButtons(long long key )
  {
    ocString ret = formTemplate.getParagraph("button_group");  
    return ret.replace("$buttons$",formButtons( key ).c_str());
  }
  string formButtons( long long key )
  {
    string ret;
    if( key )       
    { // Appropriate for update/delete
      ret += "<input type='button' onclick='doUpdate()' name='action' value='&nbsp;Update&nbsp;'>";
    }
    return ret;
  }
      
};


int main( int argc, char ** argv )
{
  cgiScript script("text/html",false);
  Order_Form  prod(script);     
  Orders_List mylist(script); 
   
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();  
    cgiTemplates pgTemplate;
    pgTemplate.load("Templates/adminPane.htmp");
    script << pgTemplate.getParagraph("top");  

    prod.loadControlTemplates("Templates/form.htmp"); 

    prod.form_action();
    prod.form_display();
 
    mylist.loadListTemplates("Templates/list.htmp");  
    mylist.list_display();
    
    ocString myBottom = pgTemplate.getParagraph("bottom");
    // don't let them update any of these
    script << myBottom.replace("/*_extra_js_*/",
    "myDom.getElementById('Product_Total').readOnly=true;\n"
    "myDom.getElementById('Shipping_Total').readOnly=true;\n"
    "myDom.getElementById('Tax_Total').readOnly=true;\n"
    "myDom.getElementById('Order_Total').readOnly=true;\n"
    "myDom.getElementById('Order_Date').readOnly=true;\n"
    "myDom.getElementById('Ship_Date').readOnly=true;\n"
    "myDom.getElementById('Reconcile_Date').readOnly=true;\n"
    "myDom.getElementById('Transactions_Id').readOnly=true;\n"        
    );
    
  }
  else
  {
    showSignOnForm(script); 
  } 
};

// as an alternative to building more objects
#include "read_write_base.cpp"
#include "forms_base.cpp"

