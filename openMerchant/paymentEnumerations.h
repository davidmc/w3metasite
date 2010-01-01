/*
  paymentEnumerations.h
  
  2005 by davidmc@w3sys.com
  
+----+--------------+----------------+------------+----------------+--------------------+--------------+----------+--------------------------------------------------------------------------------------+
| Id | Service_Type | Value_Location | Param_Type | Interface_Type | Name               | Machine_Name | Value    | Instructions                                                                         |
+----+--------------+----------------+------------+----------------+--------------------+--------------+----------+--------------------------------------------------------------------------------------+
|  1 |            1 |              1 |          2 |              0 | action             | action       | preauth  | Pre-authorization action                                                             |
|  2 |            1 |              1 |          3 |              0 | action             | action       | postauth | Post-authorization action                                                            |
|  3 |            1 |              1 |          4 |              0 | action             | action       | credit   | Credit after sale (or preauth/postauth pair)                                         |
|  4 |            1 |              1 |          0 |              0 | Payment Media      | media        | cc       | For all credit card and debit card transactions                                      |
|  5 |            1 |              1 |          0 |              0 | Test               | demo         | y        | This is used to test the system - remove when live                                   |
|  6 |            1 |              2 |          2 |              1 | Name               | name         |          | Please Enter your name as it appears on the card                                     |
|  7 |            1 |              2 |          2 |              2 | Credit Card Number | cc           |          | Please enter your card number                                                        |
|  8 |            1 |              2 |          2 |              3 | Expiration Date    | exp          |          | Please enter the expiration date in the form 'MMYY' example: '1006' for October 2006 |
|  9 |            1 |              3 |          2 |              0 | Order ID           | ticket       |          | Refers back to the artisan group and our unique internal id of the order             |
| 10 |            1 |              5 |          2 |              0 | Amount             | amount       |          | Amount of purchase                                                                   |
| 11 |            1 |              4 |          3 |              0 | Transaction ID     | transid      |          | Transaction ID from pre-authorization                                                |
| 12 |            1 |              4 |          4 |              0 | Transaction ID     | transid      |          | Transaction ID from sale to be reversed                                              |
+----+--------------+----------------+------------+----------------+--------------------+--------------+----------+--------------------------------------------------------------------------------------+
  
  
*/
enum param_val_location
{
  pml_parm = 1,             // parms of the system itself, requires no user setup
  pml_user_parm = 2,        // requires user setup
  pml_dynamic_order_id = 3, // system generated, order id
  pml_dynamic_tran_id = 4,  // system generated transaction id
  pml_dynamic_amount = 5    // system calculated amount
};

enum param_types
{
  pt_all = 0,           // all transactions
  pt_pay = 1,           // pay specific parameter
  pt_preauthorize = 2,  // pre-authorize specific parameter
  pt_postauthorize = 3, // post-authorize specific parameter
  pt_reverse = 4        // reversal specific (credit after sale)    
};

enum interface_type
{
  ui_none = 0,
  ui_text = 1,
  ui_cc_number = 2,
  ui_cc_exp = 3,
  ui_cvs = 4, // NEW for card verification system, 3 digit pin
  ui_addr_verification = 5 // NEW, the application logic should by default, populate these  with the user address. MAP: param_names  user table fields.
};

// for combos...
const char * pay_val_location[] = 
{
  "none - please choose", // 
  "sys parm",             // parms of the system itself, requires no user setup
  "user parm",        // requires user setup
  "dynamic order id", // system generated, order id
  "dynamic transaction id",  // system generated transaction id
  "dynamic amount"    // system calculated amount
};

const char * pay_types [] =
{
  "all",           // all transactions
  "pay",           // pay specific parameter
  "preauthorize",  // pre-authorize specific parameter
  "postauthorize", // post-authorize specific parameter
  "reverse"        // reversal specific (credit after sale)    
};

const char * pay_interface_type [] =
{
  "none", 
  "text",
  "cc number",
  "cc expires",
  "cvs", // NEW for card verification system, 3 digit pin
  "address verification" // NEW, the application logic should by default, populate these  with the user address. MAP: param_names  user table fields.
};

const size_t pay_loc_count=6;
const size_t pay_type_count=5;
const size_t pay_iface_count=6;
