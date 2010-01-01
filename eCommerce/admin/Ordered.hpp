/* 
  Ordered.hpp

  Object Definition and Implementation 
  for Ordered.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Ordered_HPP
#define Ordered_HPP

#include "read_write_base.hpp"

class Ordered_Obj: public read_write_base
{
public:
  identifier Id;
  llong Product_Id;
  llong Customer_Id;
  llong Order_Status_Id;
  money Product_Total;
  money Shipping_Total;
  money Tax_Total;
  money Order_Total;
  time_date Order_Date;
  time_date Ship_Date;
  time_date Reconcile_Date;
  string Transaction_Id;
  llong Shipping_Co;
  string Tracking_Number;
  string Notes;
  llong Customer_Pmt_Svc_Id;
  
  Ordered_Obj():read_write_base()
  ,Id(0LL)
  ,Product_Id(0LL)
  ,Customer_Id(0LL)
  ,Order_Status_Id(1LL)
  ,Product_Total(0.0)
  ,Shipping_Total(0.0)
  ,Tax_Total(0.0)
  ,Order_Total(0.0)
  ,Order_Date( )
  ,Ship_Date( )
  ,Reconcile_Date( )
  ,Transaction_Id("")
  ,Shipping_Co(0LL)
  ,Tracking_Number("")
  ,Notes("")
  ,Customer_Pmt_Svc_Id(0LL)
  {  
    // set name
    data_name("Ordered");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new llongXfer("Product_Id", &Product_Id ));
    addDXMap( new llongXfer("Customer_Id", &Customer_Id ));
    addDXMap( new llongXfer("Order_Status_Id", &Order_Status_Id ));
    addDXMap( new moneyXfer("Product_Total", &Product_Total ));
    addDXMap( new moneyXfer("Shipping_Total", &Shipping_Total ));
    addDXMap( new moneyXfer("Tax_Total", &Tax_Total ));
    addDXMap( new moneyXfer("Order_Total", &Order_Total ));
    addDXMap( new time_dateXfer("Order_Date", &Order_Date ));
    addDXMap( new time_dateXfer("Ship_Date", &Ship_Date ));
    addDXMap( new time_dateXfer("Reconcile_Date", &Reconcile_Date ));
    addDXMap( new stringXfer("Transaction_Id", &Transaction_Id ));
    addDXMap( new llongXfer("Shipping_Co", &Shipping_Co ));
    addDXMap( new stringXfer("Tracking_Number", &Tracking_Number ));
    addDXMap( new stringXfer("Notes", &Notes ));
    addDXMap( new llongXfer("Customer_Pmt_Svc_Id", &Customer_Pmt_Svc_Id ));
  }
  

};
#endif

