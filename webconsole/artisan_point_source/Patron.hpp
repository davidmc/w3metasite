/* 
  Patron.hpp

  Object Definition and Implementation 
  for Patron.
  
  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef AP_Patron
#define AP_Patron
#include "Person.hpp"


/* The Patron is a Person */
class Patron: public Person
{
public:
  long long Person_Id; // derived reference to base Id
  long long Payment_Service_Id;
  bool Send_Newsletter;
  bool Give_Discount;
  bool Do_Cache_Data;
  // Constructor
  Patron()
  :Person(),Person_Id(0LL),Payment_Service_Id(0LL)
  ,Send_Newsletter(true),Give_Discount(false),Do_Cache_Data(true) 
  {;}
  void derivedNameAndMap( void )
  {
    clearMaps();
    Person_Id = Id; 
    data_name("Patron"); 
    addDXMap( new llongXfer("Person_Id", &Person_Id) );
    addDXMap( new llongXfer("Payment_Service_Id", &Payment_Service_Id) );
    addDXMap( new boolXfer("Send_Newsletter", &Send_Newsletter) );
    addDXMap( new boolXfer("Give_Discount", &Give_Discount) );
    addDXMap( new boolXfer("Do_Cache_Data", &Do_Cache_Data) );
    actDerived = true; 
  }  
  bool isupplemental( void )
  { 
    bool breturn = false;    
    derivedNameAndMap();
    breturn = db_insert();
    if( breturn ) breturn = get_data();
    return breturn; 
  }
  bool usupplemental( changeMap & changes )
  { 
    bool breturn = false;
    derivedNameAndMap();
    breturn = db_update(changes);
    if( breturn ) breturn = get_data();
    return breturn; 
  }
  bool dsupplemental( void )
  { 
    bool breturn = false;    
    derivedNameAndMap();
    breturn = db_delete();
    return breturn; 
  }
  bool ssupplemental( void )
  { 
    bool breturn = false;
    derivedNameAndMap();
    breturn = get_data();
    return breturn; 
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
