/*
  base classes:
  read_write_base
        
*/
#ifndef AP_Artisan
#define AP_Artisan

#include "Person.hpp"

class Artisan: public Person
{
public:
  long long & Person_Id; // derived reference to base Id
  long long User_Id;
  long long Artisan_Group_Id;
  string Biography;
  long long Specialty;
  long long SubSpecialty;
  string image;
  
  // Constructor
  Artisan()
  :Person(),Person_Id(Id),User_Id(0LL),Artisan_Group_Id(0LL),Specialty(0LL),SubSpecialty(0LL)
  { ; }
  
  void derivedNameAndMap( void )
  {
    clearMaps(); 
    data_name("Artisan");    
    addDXMap( new llongXfer("Person_Id", &Person_Id) );
    addDXMap( new llongXfer("User_Id", &User_Id) );
    addDXMap( new llongXfer("Artisan_Group_Id", &Artisan_Group_Id) );
    addDXMap( new stringXfer("Biography", &Biography) );
    addDXMap( new llongXfer("Specialty", &Specialty) );
    addDXMap( new llongXfer("SubSpecialty", &SubSpecialty) );
    addDXMap( new stringXfer("image", &image) );
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
  
};

#endif
