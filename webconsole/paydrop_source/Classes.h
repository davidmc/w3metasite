class  Category {
 long long CategoryID;
 string CategoryName;   
};

class  Config {
 long long ConfigID;
 string ConfigVariable;
 string ConfigValue; 
};

-class  Coupon {
 long long CouponID;
 long long StoreID;
 string CouponLink;
 string CouponInfo;
 time_date ExpirationDate;  
};

class  Marketer {
 long long MarketerID;
 string MarketerName;
 string TrackingCode;
 long FileType;
 long HeaderLines;
 long  MemberIDColumn;
 long  StoreIDColumn;
 long  PurchaseDateColumn;
 long  PurchaseOrderNumberColumn;
 long  PurchaseSkuNumberColumn;
 long  PurchaseAmountColumn;  
};

class  Member {
 long long MemberID;
 string UniversalID;
 string MemberFirstName;
 string MemberLastName;
 string MemberAddress1;
 string MemberAddress2;
 string MemberCity;
 string MemberState;
 string MemberProvince;
 string MemberPostalCode;
 string MemberCountry;
 string MemberEmail;
 string MemberPassword;
 time_date MemberLastLogin; 
 bool MemberWantsNewsletter; 
};

class  MemberCheck {
 long long MemberCheckID;
 long long MemberID;
 time_date CheckDate;
 money CheckAmount;  
};

class  MemberPurchase {
 long long MemberPurchaseID;
 long long MemberID;
 long long StoreID;
 datetime PurchaseDate;
 string PurchaseOrderNumber;
 string PurchaseSkuNumber;
 money PurchaseAmount;
 double PurchaseDiscountAmount; 
 long long MemberCheckID; 
};

class  Store {
 long long StoreID;
 long long MarketerID;
 string MarketerStoreID;
 string StoreName;
 string StoreLink;
 string StoreInfo;
 string StoretextType;
 string StoretextData;
 string CommissionType;
 double Commission;
 double Discount;
 string Footnote;
 bool Featured;   
};

class  StoreSubcategory {
 long long StoreID;
 long long SubcategoryID;   
};

class  Subcategory {
 long long SubcategoryID;
 long long CategoryID;
 string SubcategoryName;   
};

