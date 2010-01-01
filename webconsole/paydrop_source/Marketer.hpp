/*
  base classes:
  read_write_base
  PayDrop Marketer Business Object 
*/

#ifndef PD_Marketer
#define PD_Marketer
#include "read_write_base.hpp"

class  Marketer :  public read_write_base
{
public:
  identifier MarketerID;
  string MarketerName;
  string TrackingCode;
  long FileType;
  long HeaderLines;
  long MemberIDColumn;
  long StoreIDColumn;
  long PurchaseDateColumn;
  long PurchaseOrderNumberColumn;
  long PurchaseSkuNumberColumn;
  long PurchaseAmountColumn; 
  Marketer()
  :read_write_base(),MarketerID(0LL),FileType(0L),HeaderLines(0L)
  ,MemberIDColumn(0L),StoreIDColumn(0L),PurchaseDateColumn(0L)
  ,PurchaseOrderNumberColumn(0L),PurchaseSkuNumberColumn(0L),PurchaseAmountColumn(0L)
  {    
    data_name("Marketer");       
    // ALWAYS make the key be first, so we can skip it in insert logic
    addDXMap( new llongXfer("MarketerID", &MarketerID) );
    addDXMap( new stringXfer("MarketerName", &MarketerName) );
    addDXMap( new stringXfer("TrackingCode", &TrackingCode) );
    addDXMap( new longXfer("FileType", &FileType) );
    addDXMap( new longXfer("HeaderLines", &HeaderLines) );
    addDXMap( new longXfer("MemberIDColumn", &MemberIDColumn) );
    addDXMap( new longXfer("StoreIDColumn", &StoreIDColumn) );
    addDXMap( new longXfer("PurchaseDateColumn", &PurchaseDateColumn) );
    addDXMap( new longXfer("PurchaseOrderNumberColumn", &PurchaseOrderNumberColumn) );
    addDXMap( new longXfer("PurchaseSkuNumberColumn", &PurchaseSkuNumberColumn) );
    addDXMap( new longXfer("PurchaseAmountColumn", &PurchaseAmountColumn) );
  } 
  virtual ~Marketer(){;}  
};

#endif
