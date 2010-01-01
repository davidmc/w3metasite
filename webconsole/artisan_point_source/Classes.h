class Category
{
  long integer Id;
  string Name;
  long integer Cat_Order;
  long integer Personalization_Owner;
  long integer Personalization_Group;
};
class Sub_Category
{
  long integer Id;
  long integer Category_Id;
  string Name;
  long integer Sub_Order;
  long integer Personalization_Owner;
  long integer Personalization_Group;
};
class Artisan
{
  long integer Person_Id;
  long integer User_Id;
  long integer Artisan_Group_Id;
  string Biography;
  long integer Specialty;
  long integer SubSpecialty;
  string image;
};
class Patron
{
  long integer Person_Id;
  long integer Payment_Type;
  boolean Send_Newsletter;
  boolean Give_Discount;
  boolean Do_Cache_Data;
};
class Person
{
  long integer Id;
  string First_Name;
  string Last_Name;
  string Login;
  string Password;
  string Address;
  string City;
  string State;
  string Zip;
  string Phone;
  string EMail;
};
class Artisan_Group
{
  long integer Id;
  string Name;
  long integer Site_Id;
  long integer Primary_Contact_Id;
  boolean Primary_Is_Default_Handler;
};
class Products
{
  long integer Id;
  long integer Category_Id;
  long integer Sub_Category_id;
  long integer Artisan_Id;
  long integer Handler_Id;
  long integer Group_Id;
  string Title;
  string Description;
  string Dimensions;
  string Weight;
  money Price;
  string List_Picture;
  string Detail_Picture;
  string Keywords;
  string Stock_Number;
  long integer Stock_Count;
};
class Payment_Service
{
  long integer Id;
  long integer Type;
  string Name;
  string Label;
  string Instructions;
};

enum param_types
{
  automatic = 0,       // get the data directly from the Payment_Service_Parameter
  pay = 1,             // pay specific parameter
  preauthorize = 2,    // pre-authorize specific parameter
  postauthorize = 3,   // post-authorize specific parameter
  reverse = 4,         // reversal specific 
  upay = 11,           // pay specific parameter from Patron_Payment_Service_Parameters
  upreauthorize = 12,  // pre-authorize specific parameter from Patron_Payment_Service_Parameters
  upostauthorize = 13, // post-authorize specific parameter from Patron_Payment_Service_Parameters
  ureverse = 14,       // reversal specific  from Patron_Payment_Service_Parameters  
  user = 20,           // get data from the Patron_Payment_Service_Parameters
  tran_id = 30,        // refers to a specific transaction (for postauthorize or reversal)
  order_id = 40,       // refers to our unique internal id of the order
  amount = 50          // amount parameter
};

class Payment_Service_Parameter
{
  long integer Id;
  long integer Service_Type;
  long integer Param_Type;
  string Name;
  string Machine_Name;
  string Value;
  string Instructions;
};

class Patron_Payment_Service
{
  long integer Id;
  long integer Patron_Id;
  long integer Payment_Service_Id;
  boolean Is_Default_Service;
  boolean Do_Cache_Data;
};
class Patron_Payment_Service_Parameters
{
  long integer Id;
  long integer Patron_Pmt_Service_Id;
  long integer Patron_Pmt_Svc_Param_Id;
  string Machine_Name;
  string Value;
};
class Orders
{
  long integer Id;
  long integer Product_Id;
  long integer Patron_Id;
  long integer Order_Status_Id;
  money Product_Total;
  money Shipping_Total;
  money Tax_Total;
  money Order_Total;
  date-time Order_Date;
  date-time Ship_Date;
  date-time Reconcile_Date;
  long integer Shipping_Co;
  string Tracking_Number;
  long integer Patron_Payment_Service_Id;
};
class Order_Status
{
  long integer Id;
  string Name;
};
class Ordered_Items
{
  long integer Id;
  long integer Product_Id;
  long integer Order_Id;
  integer Product_Count;
  money Total_Price;
};
