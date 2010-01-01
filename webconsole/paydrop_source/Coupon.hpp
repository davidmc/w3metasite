/*
  base classes:
  read_write_base
  PayDrop Coupon Business Object 
*/

#ifndef PD_Coupon
#define PD_Coupon

class  Coupon  :  public read_write_base
{
public:
  identifier CouponID;
  long long StoreID;
  string CouponLink;
  string CouponInfo;
  oc_date ActiveDate;
  oc_date ExpirationDate;
  Coupon()
  :read_write_base(),CouponID(0LL),StoreID(0L)
  {    
    data_name("Coupon");       
    // ALWAYS make the key be first, so we can skip it in insert logic
    addDXMap( new llongXfer("CouponID", &CouponID) );
    addDXMap( new llongXfer("StoreID", &StoreID) );
    addDXMap( new stringXfer("CouponLink", &CouponLink) );
    addDXMap( new stringXfer("CouponInfo", &CouponInfo) );
    addDXMap( new oc_dateXfer("ActiveDate", &ActiveDate) );        
    addDXMap( new oc_dateXfer("ExpirationDate", &ExpirationDate) );    
  } 
  virtual ~Coupon(){;}     
};

#endif
