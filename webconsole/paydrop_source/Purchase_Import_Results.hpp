/*
  Purchase_Import_Results.hpp
  base classes:
  read_write_base
  PayDrop Business Object 
*/

#ifndef PD_Purchase_Import_Results
#define PD_Purchase_Import_Results
#include "read_write_base.hpp"
#include "Config.hpp"


enum enm_status
{
  egood,efail,edup
};
char * statii[] = { "Good","Bad","Duplicate" };

class  Purchase_Import_Results :  public read_write_base
{
public:
  string UniversalID;
  string MarketerStoreID;
  long long MemberID;
  long long StoreID;
  oc_date PurchaseDate;
  string PurchaseOrderNumber;
  string PurchaseSkuNumber;
  money PurchaseAmount;
  money CalculatedDiscountAmount;
  bool IsReturn;
  string Status;
  
  // not persistent in db, just for determining email
  // and need to email about registration
  string trueEmailAddress;
  bool isIncompleteRegistration;
  
  // see if the member is an organization
  bool MemberIsOrganization;
  
  Purchase_Import_Results()
  :read_write_base(),MemberID(0LL),StoreID(0L),IsReturn(false),Status(statii[egood])
  ,isIncompleteRegistration(false),MemberIsOrganization(false)
  {
    actDerived = true; // I'm keyless
    data_name("Purchase_Import_Results");

    addDXMap( new stringXfer("UniversalID", &UniversalID  ) );
    addDXMap( new stringXfer("MarketerStoreID", & MarketerStoreID ) );
    addDXMap( new llongXfer("MemberID", &MemberID) );
    addDXMap( new llongXfer("StoreID", &StoreID) );
    addDXMap( new oc_dateXfer("PurchaseDate", &PurchaseDate) );  
    addDXMap( new stringXfer("PurchaseOrderNumber", &PurchaseOrderNumber  ) );   
    addDXMap( new stringXfer("PurchaseSkuNumber", &PurchaseSkuNumber  ) );
    addDXMap( new moneyXfer("PurchaseAmount", &PurchaseAmount ) ); 
    addDXMap( new moneyXfer("CalculatedDiscountAmount", &CalculatedDiscountAmount ) );  
    addDXMap( new boolXfer("IsReturn", &IsReturn) );
    addDXMap( new stringXfer("Status", &Status ) );              
  }
  long long getDefaultUserId()
  {
    long long id = 0;
    Config config;    
    string clause = "ConfigVariable = 'Default Login Id'";
    if( config.get_data(clause) ) id = atoll(config.ConfigValue.c_str());
    return id;
  }
  bool setCalculatedDiscountAmount( void )
  {    
    // I need a store to do this
    Store store;
    ocString filter = MarketerStoreID;
    ocString clause = "MarketerStoreID = '%id'";    
    clause = clause.replace("%id",filter.replace("'","''").c_str());
    if( store.get_data(clause) )
    {    
      // Set the Store Id
      StoreID = store.StoreID;
      // figure the discount
      if( store.CommissionType == "%" )
      {
        // Most accurate result if dividing last by the largest amounts
        CalculatedDiscountAmount = (PurchaseAmount.amount()*store.Discount)/100.0;
      }
      else
      {
        CalculatedDiscountAmount = store.Discount;
      }
      if( CalculatedDiscountAmount.amount() < 0.0 )  IsReturn = true;
    }
    else
    {
      Status = statii[efail];
      return false;
    }
    return true;  
  }
  string fixMoney( ocString moneyIn )
  {
    return moneyIn.replace("$","").replaceAll(",","")
                  .replace("(","").replace(")","").replace(" ","");
  }
  string fixDateString( string date )
  {
    ocString checker = date;    
    string result = date;
    // look for alph like: '10-NOV-03' as in befree imports!*&#
    if( checker.regExMatch("[A-Z]") )
    {
      map <string, string> m;
      m["JAN"]="01";m["FEB"]="02";m["MAR"]="03";m["APR"]="04";
      m["MAY"]="05";m["JUN"]="06";m["JUL"]="07";m["AUG"]="08";
      m["SEP"]="09";m["OCT"]="10";m["NOV"]="11";m["DEC"]="12";
      string day = checker.parse("-");
      string month = m[checker.parse("-")];
      string year = checker.parse("-");
      result=month;
      result+="/";
      result+=day;
      result+="/";
      if( year.length()==2)
      {
        // will break in 96 years!
        string temp = "20";        
        temp += year;
        year = temp;
      }
      result+=year;      
    }
    return result;
  }
  // map columns from the parser accourding to the 
  bool Map( Marketer & marketer, ocFileCols & cols, long colCount )
  { 
    // must have marketer and member
    if( marketer.MemberIDColumn > -1 && marketer.MemberIDColumn < colCount )
    {
      UniversalID = cols[marketer.MemberIDColumn].value;            
      // try to get the member
      Member member;
      string clause = "UniversalID = '";
      clause += UniversalID;
      clause += "'";
      if( member.get_data(clause) )
      {
        trueEmailAddress = member.MemberEmail;
        MemberIsOrganization = member.MemberIsOrganization;
        MemberID = member.MemberID;
        if( !(member.MemberPostalCode.length() && member.MemberAddress1.length()) )
        {
          // they will get mailed if we determine that 
          // they would have gotten a discount if they were registered
          isIncompleteRegistration = true;
        }          
      }
      else
      {
        Status = statii[efail];
      }
    }
    else
    {
        Status = statii[efail];        
    }
    // must have marketer store
    if( marketer.StoreIDColumn > -1 && 
        marketer.StoreIDColumn < colCount &&
        cols[marketer.StoreIDColumn].value.length())
    {
      MarketerStoreID = cols[marketer.StoreIDColumn].value;
    }
    else
    {
      Status = statii[efail];
    }
    // must have purchase date
    if( marketer.PurchaseDateColumn > -1 && 
        marketer.PurchaseDateColumn < colCount &&
        cols[marketer.PurchaseDateColumn].value.length())
    {
      PurchaseDate.parse( fixDateString(cols[marketer.PurchaseDateColumn].value).c_str() );
      if( PurchaseDate.year() < 2000 ) Status = statii[efail]; // failure due to bad format of date
    }
    else
    {
      Status = statii[efail];
    }
    
    // non fail optional value
    if( marketer.PurchaseOrderNumberColumn > -1 && 
        marketer.PurchaseOrderNumberColumn    < colCount &&
        cols[marketer.PurchaseOrderNumberColumn].value.length())
    {
      PurchaseOrderNumber = cols[marketer.PurchaseOrderNumberColumn].value;
    }
    // non fail optional value
    if( marketer.PurchaseSkuNumberColumn > -1 && 
        marketer.PurchaseSkuNumberColumn    < colCount &&
        cols[marketer.PurchaseSkuNumberColumn].value.length())
    {
      PurchaseSkuNumber = cols[marketer.PurchaseSkuNumberColumn].value;
    }
    
    // must have purchase amount
    if( marketer.PurchaseAmountColumn > -1 && 
        marketer.PurchaseAmountColumn < colCount &&
        cols[marketer.PurchaseAmountColumn].value.length())
    {
      string purchase = fixMoney(cols[marketer.PurchaseAmountColumn].value);
      PurchaseAmount = atof(purchase.c_str());
      if( isnan(PurchaseAmount.amount())  == 0 )
      {
        setCalculatedDiscountAmount();
      }      
    }
    else
    {
      Status = statii[efail];
    } 
    return true;  
  }
  
  virtual ~Purchase_Import_Results()
  {
    
  }  
};

#endif


