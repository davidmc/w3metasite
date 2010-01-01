/* 
  Order.hpp

  Object Definition and Implementation 
  for Order.
  
  Copyright (C) 2005 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Order_HPP
#define Order_HPP

#include "read_write_base.hpp"

// The trust commerce payment gateway
#include "ocTrustCommerce.h"
#include "ocXML.h"
#include "Product.hpp"
#include "Ordered_Item_Option.hpp" // for generic way to do custom attributes
#include "../admin/Product.hpp"
#include "../admin/Size_Price.hpp"
#include "../admin/ItemDetail.hpp"

enum enm_order_status 
{
  os_awaiting_shipment=1, 
  os_shipped,
  os_recieved, // this will be easier to set with FEDEX shipping integration
  os_cancelled,
  os_returned,
  os_completed // this one isn't in the lookup table, set to this value when the check is written 
};


/*
  Definition of Ordered_Item_Obj
  generated by w3sys.radware    
*/


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
  
  // vector of items in this order
  items_ordered items;
  
  
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
  
  void shipInc (double & ship )
  {
    Shipping_Total.amount() += ship;
  }
  
  // Stub placeholder for shipping until ship API integration 
  string netShipping( void )
  { // Enum through items, get the shipping cost of each 
    return Shipping_Total.format("%n");
  }
  
  // Stub placeholder for taxes 
  string setTax( void )
  { // Not a sales tax watchdog just yet.
    Tax_Total = 0.0;
    return Tax_Total.format("%n");
  }
  string setOrderTotal( void )
  { // Add all up
    Order_Total.amount() = Product_Total.amount() 
                         + Shipping_Total.amount() 
                         + Tax_Total.amount();
    ocString formattedAmount =  Order_Total.format("%n");                    
    return formattedAmount.remove("$").replaceAll(",","").remove(".");
  }
    
  // For final check out
  double setOrderedItems( string XML )
  { xmlParser parser( XML );
    parser.parse();
    node_vector & xnodes = parser.nodeList();
    int i;
    Product_Total = 0.0;
    // for each  node in the XML, lines of text is in data
    for(i=0;i<xnodes.size();i++)
    { 
    
      ItemDetail dtl;
      dtl = xnodes[i]; 

      ocString productId;  productId.append(dtl.Product());
      ocString count; count.append(dtl.Count());
                  
      if( dtl.Product() ) 
      { // Set the order's Product_Id to the first product
        if(i==0)Product_Id = dtl.Product();         
        Ordered_Item oi;
          
        double mon = dtl.Price().amount();          
        long cnt = dtl.Count();
        double ship =  dtl.Shipping().amount();
        shipInc(ship);
        // long stock_cnt = atol(rs.getField(3).format().c_str() );
        oi.Product_Id = dtl.Product();
        oi.Order_Id = Id;
        oi.Product_Count = cnt;
        oi.Total_Price = ((double)cnt)*mon; 
        oi.Shipping=ship*cnt;
        oi.Internal_Id = dtl.SKU();
        oi.External_Id = dtl.ProductName();

        // Add order item Customization!
        // oi.addOrderItemOptions(xnodes[i]);

        items.push_back(oi);
        Product_Total.amount() += oi.Total_Price.amount();              
            
      }   
    } 
  }
  bool saveOrderedItems( void )
  {
    bool bret = false;
    for( size_t i=0; i<items.size();++i)
    {
      items[i].Order_Id = Id;
      Ordered_Item_Obj toSave;
      toSave = items[i];
      bret = toSave.db_insert();
      // if(bret) toSave.saveOptions();
      if( !bret ) break; // get out on first failure
    }    
    return bret;
  }
  bool decrementStock( void )
  {
    bool bret = false;
    
    for( size_t i=0; i<items.size();++i)
    {
      Ordered_Item item = items[i];
      Product_Obj prod;
      prod.decrementStock(item.Product_Id,item.Product_Count);      
    }        
    return bret; 
  }
  bool preauth( long long Payment_Service_Id )
  {
    bool bret = false;

    netShipping();
    setTax();    

    Payment_Service ps;
    ps.key(Payment_Service_Id);

    if( ps.get_data() && ps.getPaymentServiceParameters()  )
    {
      Customer_Payment_Service pps;      
      ocString where = "Customer_Id = ";
      where.append(Customer_Id);
      where += " and Payment_Service_Id = ";
      where.append(Payment_Service_Id);

      if( pps.get_data(where) && pps.getParameters() )
      {
      
        Customer_Pmt_Svc_Id = pps.Id;
        ocTCLink tcLink;
        
        // Get Param_Types pt_all and pt_preauthorize
        pushParams( tcLink, ps, pps, pt_preauthorize );        
        
        // get the user payment setup
        size_t pos2;
        for( pos2=0; pos2<pps.params.size();++pos2)
        {
          tcLink.PushParam(pps.params[pos2].Machine_Name,pps.params[pos2].Value);
        } 
        // now finalize
        if( tcLink.Send().IsGood() )     
        {
          // get the transaction id and save the order
          Transaction_Id = tcLink.TransactionId();
          bret = db_insert();
          Id = key();
          if( bret )  
          {
            bret = saveOrderedItems();            
            // decrement product stock   
            decrementStock();
          }  
        }
        else
        {
          m_result += "Payment Transaction Failed: ";
          m_result +=tcLink.TransactionInfo();
        }
      }
      else
      {
        m_result += "Customer payment setup failed: ";
        m_result += pps.last_result();
      }
    }
    else
    {
      m_result += "payment setup load failed: ";
      m_result +=  ps.last_result();
    }
    return bret; 
  }
  /*
    bool removeParams() 
    This is used when we do not want to retain any customer payment data.
    This makes it perfectly safe for clients to order stuff from sites 
    that implement this.  We should prefer NOT to save payment information.    
  */
  bool removeParams()
  {
    ocString delSql = "delete from Customer_Payment_Parameter "
                      "where Customer_Pmt_Service_Id = ";
    delSql.append(Customer_Pmt_Svc_Id);
    return cmd.execute(delSql);
  }
  
  bool pay( long long Payment_Service_Id )
  {
    bool bret = false;

    netShipping();
    setTax();    

    Payment_Service ps;
    ps.key(Payment_Service_Id);

    if( ps.get_data() && ps.getPaymentServiceParameters()  )
    {
      Customer_Payment_Service pps;      
      ocString where = "Customer_Id = ";
      where.append(Customer_Id);
      where += " and Payment_Service_Id = ";
      where.append(Payment_Service_Id);

      if( pps.get_data(where) && pps.getParameters() )
      {
      
        Customer_Pmt_Svc_Id = pps.Id;
        ocTCLink tcLink;
        
        // Get Param_Types pt_all and pt_preauthorize
        pushParams( tcLink, ps, pps, pt_pay );        
        
        // get the user payment setup
        size_t pos2;
        for( pos2=0; pos2<pps.params.size();++pos2)
        {
          tcLink.PushParam(pps.params[pos2].Machine_Name,pps.params[pos2].Value);
        } 
     
        if( tcLink.Send().IsGood() )     
        {
          // get the transaction id and save the order
          Transaction_Id = tcLink.TransactionId();
          bret = db_insert();
          Id = key();
          if( bret )  
          {
            bret = saveOrderedItems();            
            // decrement product stock   
            decrementStock();
          }  
        }
        else
        {
          m_result += "Payment Transaction Failed: ";
          m_result +=tcLink.TransactionInfo();
        }
        // Optional, but a good idea, delete the payment info
        removeParams();
      }
      else
      {
        m_result += "Customer payment setup failed: ";
        m_result += pps.last_result();
      }
    }
    else
    {
      m_result += "payment setup load failed: ";
      m_result +=  ps.last_result();
    }
    return bret; 
  }

  bool postauth( void )
  {
    bool bret = false;
    m_result = "POST-AUTHORIZATION - ";
    // only if Id
    if( Id && Customer_Pmt_Svc_Id )
    { 
      Customer_Payment_Service pps;
      pps.key(Customer_Pmt_Svc_Id);
      if( pps.get_data() ) 
      {
        Payment_Service ps;
        ps.key(pps.Payment_Service_Id);
        if( ps.get_data() && ps.getPaymentServiceParameters()  )
        {
          ocTCLink tcLink;
          
          // Get Param_Types pt_all and pt_postauthorize
          pushParams( tcLink, ps, pps, pt_postauthorize );  
          
          // send it
          if( tcLink.Send().IsGood() )     
          {
            m_result += "Successful";
            bret = true;
          }
          else
          {
            m_result += "Payment Transaction Failed: ";
            m_result +=tcLink.TransactionInfo();
          }
        }      
      }      
    }
    return bret;
  }
  
  bool credit( void )
  {
    bool bret = false;
    m_result = "CREDIT REVERSAL - ";
    // only if Id
    if( Id && Customer_Pmt_Svc_Id ) 
    {
      if( Order_Status_Id == os_shipped || Order_Status_Id ==os_recieved ) 
      {
        Customer_Payment_Service pps;
        pps.key(Customer_Pmt_Svc_Id);
        if( pps.get_data() ) 
        {
          Payment_Service ps;
          ps.key(pps.Payment_Service_Id);
          if( ps.get_data() && ps.getPaymentServiceParameters()  )
          {
            ocTCLink tcLink;

            // Get Param_Types pt_all and pt_reverse
            pushParams( tcLink, ps, pps, pt_reverse );  

            // send it
            if( tcLink.Send().IsGood() )     
            {
              m_result += "Successful";
              Order_Status_Id = os_cancelled;
              bret = true;
            }
            else
            {
              m_result += "Payment Transaction Failed: ";
              m_result +=tcLink.TransactionInfo();
            }
          }      
        }  
      }
      else
      {
        m_result += "No Payment Transaction reversal until payment is made!";
      }
      // re-increment product stock   
      // incrementStock();
    }
    return bret;
  }
  
  void pushParams( ocTCLink & tcLink, Payment_Service & ps, Customer_Payment_Service & pps, param_types ptype )
  {
    // Get Param_Types pt_all 
    Payment_Type_Parameters::iterator pos1;
    for( pos1=ps.sys_params.lower_bound(pt_all); pos1!=ps.sys_params.upper_bound(pt_all); ++pos1 )
    {
      tcLink.PushParam(pos1->second.Machine_Name,pos1->second.Value);
    }
    // Get all appropriate for type
    for( pos1=ps.sys_params.lower_bound(ptype); pos1!=ps.sys_params.upper_bound(ptype); ++pos1 )
    {
      tcLink.PushParam(pos1->second.Machine_Name,pos1->second.Value);
    }
    // get params with dynamically generated values
    for( pos1=ps.dyn_params.lower_bound(ptype);  pos1!=ps.dyn_params.upper_bound(ptype); ++pos1 )
    {
      tcLink.PushParam(pos1->second.Machine_Name,dynValue(pos1->second.Value_Location));
    }
  }
  
  
  // Return the dynamic value 
  //  based on the parameter location type
  string dynValue( long loco )
  {
    ocString value;    
    if( loco==pml_dynamic_order_id)
    {                  // orderid to gateway is the Customer id and the 1st product id
      value.append(Customer_Id);
      value+="-";
      value.append(Product_Id);
    }        
    else if( loco==pml_dynamic_amount)
    {
      value=setOrderTotal(); // Set total, return in e format
    }  
    else if( loco==pml_dynamic_tran_id )
    {
      value= Transaction_Id; // return the transaction id from the gateway 
    }  
    return value;
  }
  
  // override update validation
  bool uvalidate( void  ) 
  { 
    // get the original status 
    ocString sqlStatus = "select Order_Status_Id from Orders where Id = ";
    sqlStatus.append(key());
    if( rs.open(sqlStatus) )
    {
      long long statusVal = atoll(rs.getField(0).format().c_str());
      rs.close();
      // check all affirmative cases, not related to the 'Shipped' transition
      if( statusVal == Order_Status_Id || // nothing changed
          ( statusVal > os_awaiting_shipment && // not orig awaiting shipment
            Order_Status_Id > os_awaiting_shipment // not set back to await ship 
          ) ) 
      {
        return true;
      }
      else if(  statusVal == os_awaiting_shipment &&
                ( Order_Status_Id == os_shipped ||
                  Order_Status_Id == os_recieved ) )
      {
        // do cc order finalization
         if( postauth() ) 
         {
           // set the ship date to NOW
           Ship_Date.now();
          
           // set the reconciliation date to NOW + 1 month 
           // (for charge-back protection.)
           Reconcile_Date.now();
           Reconcile_Date.addMonths(1);

                      
           // return cc success result
           return true;
         }
      }
      else if(  statusVal == os_awaiting_shipment &&
                ( Order_Status_Id == os_cancelled ||
                  Order_Status_Id == os_returned ) )
      {
        // do cc order reversal
        if( credit() )
        // return cc success result
        return true;
      }     
      else
      {
        m_result = "<b>NO CHANGE MADE!</b> It's illegal to change the status that way!";
      }            
    }
    return false; 
  }   

};

#endif

