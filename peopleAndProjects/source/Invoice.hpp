/*
  Invoice.hpp

  Object Definition and Implementation 
  for Invoice.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Invoice_HPP
#define Invoice_HPP

#include "read_write_base.hpp"
#include "Status_Enumerations.h"

class Invoice_Obj: public read_write_base
{
public:
  identifier Id;
  llong Company;
  llong Project;
  llong Contact;
  string Notes;
  oc_date Invoice_Date;
  oc_time Invoice_Time;
  llong Status;
  oc_date Paid_Date;
  
  Invoice_Obj():read_write_base()
  ,Id(0LL)
  ,Company(0LL)
  ,Project(0LL)
  ,Contact(0LL)
  ,Notes("")
  ,Invoice_Date( )
  ,Invoice_Time( )
  ,Status(0LL)
  ,Paid_Date( )
  {  
    // set name
    data_name("Invoice");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new llongXfer("Company", &Company ));
    addDXMap( new llongXfer("Project", &Project ));
    addDXMap( new llongXfer("Contact", &Contact ));
    addDXMap( new stringXfer("Notes", &Notes ));
    addDXMap( new oc_dateXfer("Invoice_Date", &Invoice_Date ));
    addDXMap( new timeXfer("Invoice_Time", &Invoice_Time ));
    addDXMap( new llongXfer("Status", &Status ));
    addDXMap( new oc_dateXfer("Paid_Date", &Paid_Date ));
    Status = ll_Status_Id(Time_Pre_Invoice);
  }

  bool ivalidate( void  )
  {
    changeMap changes;
    fixup(changes);
    return true;
  }
  bool uvalidate( changeMap & changes  )
  {
    if( changes.count("Status") > 0 )
    {
      fixup(changes);
    }
    return true;
  }
  void fixup(changeMap & changes )
  {
    switch( l_Status_Enum( Status ) )
    {
      case Time_Pre_Invoice: // pre-invoice
        Invoice_Date.addDays( 1000 );
        Paid_Date.addDays( 1000 );
        changes["Invoice_Date"]="Invoice_Date";
        changes["Paid_Date"]="Paid_Date";
        break;
      case Time_Invoiced: // invoiced
        Invoice_Date.now();
        Invoice_Time.now();
        changes["Invoice_Date"]="Invoice_Date";
        break;
      case Time_Paid: // paid
        Paid_Date.now();
        changes["Paid_Date"]="Paid_Date";
        break;
    }
  }
      
  
  bool isupplemental( void )
  {
    setTimeStatus( );
    return true;
  }
  bool usupplemental( changeMap & changes )
  {
    if( changes.count("Status") > 0 )
      setTimeStatus(  );
    return true;
  }
  void setTimeStatus( void )
  {
    ocString sql = "update Time set Status = ";
    sql.append(Status);
    sql += " where  Invoice = ";
    sql.append(Id);
    cmd.execute(sql);
  }
  /*
  TODO: Add Event hooks for business logic
  =========================
  // Done before any action - a false return prevents action
  virtual bool ivalidate( void  ) { return true; }
  virtual bool uvalidate( changeMap & changes  ) { return true; }
  virtual bool dvalidate( void ) { return true; }
  
  // Done after any action - a false return sets a warning
  virtual bool isupplemental( void ){ return true; }
  virtual bool usupplemental( changeMap & changes ){ return true; }
  virtual bool dsupplemental( void ){ return true; }
  virtual bool ssupplemental( void ){ return true; } 
  */
};
#endif



