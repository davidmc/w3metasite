/*

xml_forms.hpp
Forms Base Definition

*/
#ifndef Forms_Base_Hpp
#define Forms_Base_Hpp

#include "cgiClass.h"
#include "forms.h"
#include <set>



string persistingKey = "key";

class xml_form 
{
protected:    
  changeMap changes;
  string mode;
  string m_warn;
  cgiScript & script;
  
  // form to class property exchange methods
  void stringFXfer( string in, string & out );
  void shortFXfer( string in, short & out );
  void intFXfer( string in, int & out );
  void unfmtLongFXfer( string in, long & out ); // remove comma formatting
  void longFXfer( string in, long & out );
  void llongFXfer( string in, long long & out );
  void doubleFXfer( string in, double & out );
  void moneyFXfer( string in, money & out );
  void boolFXfer( string in, bool & out );
  void dateFXfer( string in, oc_date & out );
  void dateFXfer( string in, time_date & out );
  void timeFXfer( string in, oc_time & out );
public:
  xml_form(cgiScript & in);
  virtual ~xml_form();
  
  // return embedded script
  cgiScript & Script(void){return script;}
    
  // virtual function
  virtual bool form_action( void );
  
  // pure virtual functions  
  virtual void form_id_transfer( void ) = 0;
  virtual void form_data_transfer( void ) = 0;
  virtual bool dbf_action( string mode, changeMap & changes ) = 0;
  virtual bool form_display( void ) = 0;

  // get directions about what xmlmode and xmlchanges
  bool getInstructions( void );  
  /* 
    Returns the mode 
      (i,d,u,s,r,n) := 
      (insert,delete,update,show,reset,new)         
  */    
  string & getMode( void )
  {
    return mode;
  }
};

// For setting db key from form var, or query string.
// Id or id is usually considered key.
template <class db_form_object> 
void setKey ( db_form_object & dbfo )
{   
  if( dbfo.getMode() != "r" ||   dbfo.getMode() != "n" )
  {
    if( dbfo.Script().ClientArguments().count("Id") )
    {
      dbfo.key(atoll(dbfo.Script().ClientArguments()["Id"].c_str()));
    }
    if( dbfo.Script().ClientArguments().count("id") )
    {
      dbfo.key(atoll(dbfo.Script().ClientArguments()["id"].c_str()));
    }
  }
}

// This template function is used in the unusual case for the key.
template <class db_form_object> 
void setKey ( db_form_object & dbfo, const char * keyName )
{   
  if( dbfo.getMode() != "r" ||   dbfo.getMode() != "n" )
  {
    if( dbfo.Script().ClientArguments().count(keyName) )
    {
      dbfo.key(atoll(dbfo.Script().ClientArguments()[keyName].c_str()));
    }    
  }
}
#endif

