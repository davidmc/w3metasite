/*
  base classes:
  read_write_base
  
  PayDrop 
  Member Purchase Business Object
  
*/
#include <uuid/uuid.h>
// need to link libuuid

#ifndef PD_Member_Purchase
#define PD_Menber_Purchase

class  MemberPurchase :  public read_write_base
{
public:

 identifier MemberPurchaseID;
 long long MemberID;
 long long StoreID;
 time_date PurchaseDate;
 string PurchaseOrderNumber;
 string PurchaseSkuNumber;
 money PurchaseAmount;
 double PurchaseDiscountAmount; 
 long long MemberCheckID; 
public: 
  MemberPurchase():read_write_base(),MemberPurchaseID(0LL),MemberID(0LL),StoreID(0LL)
   ,PurchaseDiscountAmount(0.0),MemberCheckID(0LL)
  {
    data_name("MemberPurchase");
    addDXMap( new llongXfer("MemberPurchaseID", &MemberPurchaseID  ) );
    addDXMap( new llongXfer("MemberID", & MemberID ) );
    addDXMap( new llongXfer("StoreID", &StoreID  ) );
    addDXMap( new time_dateXfer("PurchaseDate", &PurchaseDate  ) );
    addDXMap( new stringXfer("PurchaseOrderNumber", &PurchaseOrderNumber) );
    addDXMap( new stringXfer("PurchaseSkuNumber", &PurchaseSkuNumber) );
    addDXMap( new moneyXfer("PurchaseAmount", &PurchaseAmount) );
    addDXMap( new doubleXfer("PurchaseDiscountAmount", &PurchaseDiscountAmount) );
    addDXMap( new llongXfer("MemberCheckID", &MemberCheckID  ) );                
  }  
  virtual ~MemberPurchase(){;} 
};



#endif

