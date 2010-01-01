/*
  base classes:
  read_write_base
  PayDrop StoreSubcategory Business Object 
*/

#ifndef PD_StoreSubcategory
#define PD_StoreSubcategory

class  StoreSubcategory :  public read_write_base
{
public:
 long long StoreID;
 long long SubcategoryID;   
 StoreSubcategory():read_write_base(),StoreID(0LL),SubcategoryID(0LL)
 {
   actDerived = true;
   data_name("StoreSubcategory"); 
   addDXMap( new llongXfer("StoreID", &StoreID  ) );
   addDXMap( new llongXfer("SubcategoryID", &SubcategoryID  ) );
   
 }
 virtual ~StoreSubcategory(){;}
};

#endif
