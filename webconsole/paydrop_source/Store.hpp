/*
  base classes:
  read_write_base
  PayDrop Store Business Object 
*/

#ifndef PD_Store
#define PD_Store

class  Store :  public read_write_base
{
public:
  identifier StoreID;
  identifier MarketerID;
  string MarketerStoreID;
  string StoreName;
  string StoreLink;
  string StoreInfo;
  long StoreTemplateID;
  string StoreImage;
  string CommissionType;
  double Commission;
  double Discount;
  string Footnote;
  bool Featured;
  bool Enabled; 
  // a pipe delimited list of selected subCategories
  ocString SubcategoryIDs;

  Store(): read_write_base(),StoreID(0LL),MarketerID(0LL)
  ,Commission(0.0),Discount(0.0),StoreTemplateID(1L),Featured(false),Enabled(true)
  {
    data_name("Store");
    // ALWAYS make the key be first, so we can skip it in insert logic
    addDXMap( new llongXfer("StoreID", &StoreID) );
    addDXMap( new llongXfer("MarketerID", &MarketerID) );
    addDXMap( new stringXfer("MarketerStoreID", &MarketerStoreID) );
    addDXMap( new stringXfer("StoreName", &StoreName) );
    addDXMap( new stringXfer("StoreLink", &StoreLink) );
    addDXMap( new stringXfer("StoreInfo", &StoreInfo) );
    addDXMap( new longXfer("StoreTemplateID", &StoreTemplateID) );
    addDXMap( new stringXfer("StoreImage", &StoreImage) );
    addDXMap( new stringXfer("CommissionType", &CommissionType) );
    addDXMap( new doubleXfer("Commission", &Commission) );
    addDXMap( new doubleXfer("Discount", &Discount) );
    addDXMap( new stringXfer("Footnote", &Footnote) );
    addDXMap( new boolXfer("Featured", &Featured) );
    addDXMap( new boolXfer("Enabled", &Enabled) );            
  }
  virtual ~Store(){;}

  // on Event of form submission, update
  virtual bool setSubCategories( void )
  {
    bool bRet = false;
    if( SubcategoryIDs.length() )
    {
      ocString sql;
      ocString sqlStart = "insert into StoreSubcategory (StoreID,SubcategoryID) values (";
      sqlStart.append(StoreID);
      sqlStart += ",";
      SubcategoryIDs.parseInit();
      while( !SubcategoryIDs.endOfParse() )
      {
        sql = sqlStart;
        sql += SubcategoryIDs.parse("|");
        sql += ")";        
        bRet = cmd.execute(sql);
        if(!bRet) break;
      }      
    }
    return bRet;
  }
  
  // on Event of form submission, delete
  virtual bool deleteSubCategories( void )
  {
    bool bRet = false;
    ocString sql = "delete from StoreSubcategory where StoreID = ";
    sql.append(StoreID);
    bRet = cmd.execute(sql);
    return bRet;
  }
  
  // on event of getting data
  virtual bool retrieveSubCategories( void )
  {
    bool bRet = false; 
    SubcategoryIDs = "";   
    ocString sql = "select SubcategoryID from StoreSubcategory where StoreID = ";
    sql.append(StoreID);
    if( rs.open(sql) )
    {
      bRet = true;
      bool moreData = true;
      do
      {
        SubcategoryIDs += rs.getField(0).format();
        moreData = rs.next();
        if( moreData ) SubcategoryIDs += "|";
      }
      while( moreData );
    }  
    return bRet;
  }  
};

#endif
