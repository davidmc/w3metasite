/*
  base classes:
  read_write_base
      
*/
#ifndef AP_Products
#define AP_Products
#include "read_write_base.hpp"

/* 
  Products Business Object
*/  
class Products:  public read_write_base
{
public:
  identifier Id;
  long long Category_Id;
  long long Sub_Category_id;
  long long Artisan_Id;
  long long Handler_Id;
  long long Group_Id;
  string Title;
  string Description;
  string Dimensions;
  string Weight;
  money Price;
  string List_Picture;
  string Detail_Picture;
  string Keywords;
  string Stock_Number;
  long long Stock_Count;
  
  // Constructor
  Products()
  :read_write_base()
  ,Id(0LL),Category_Id(0LL)
  ,Sub_Category_id(0LL)
  ,Artisan_Id(0LL)
  ,Handler_Id(0LL)
  ,Group_Id(0LL)
  ,Price(0.0)
  ,Stock_Count(0LL)
  {
    data_name("Products");
    // ALWAYS make the key be first, so we can skip it in insert logic
    addDXMap( new llongXfer("Id", &Id) );
    addDXMap( new llongXfer("Category_Id", &Category_Id) );
    addDXMap( new llongXfer("Sub_Category_id", &Sub_Category_id) );
    addDXMap( new llongXfer("Artisan_Id", &Artisan_Id) );
    addDXMap( new llongXfer("Handler_Id", &Handler_Id) );
    addDXMap( new llongXfer("Group_Id", &Group_Id) );
    addDXMap( new stringXfer("Title", &Title) );
    addDXMap( new stringXfer("Description", &Description) );
    addDXMap( new stringXfer("Dimensions", &Dimensions) );
    addDXMap( new stringXfer("Weight", &Weight) );
    addDXMap( new moneyXfer("Price", &Price) );
    addDXMap( new stringXfer("List_Picture", &List_Picture) );
    addDXMap( new stringXfer("Detail_Picture", &Detail_Picture) );
    addDXMap( new stringXfer("Keywords", &Keywords) );
    addDXMap( new stringXfer("Stock_Number", &Stock_Number) );
    addDXMap( new llongXfer("Stock_Count", &Stock_Count) );     
  }
  
  /*
  Event hooks for business logic
  =========================
  // Done before any action - a false return prevents action
  virtual bool ivalidate( void  ) { return true; }
  virtual bool uvalidate( void  ) { return true; }
  virtual bool dvalidate( void ) { return true; }
  
  // Done after any action - a false return sets a warning
  virtual bool isupplemental( void ){ return true; }
  virtual bool usupplemental( changeMap & changes ){ return true; }
  virtual bool dsupplemental( void ){ return true; }
  virtual bool ssupplemental( void ){ return true; } 
  */
  
};
#endif

