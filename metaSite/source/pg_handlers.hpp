/* 
  Page Handler Class - pg_handlers.hpp
  ----------------------------------------  
  This class represents 
  an abstraction of a web pages menus.  
*/

// Sentry
#ifndef Pg_Handler_Hpp
#define Pg_Handler_Hpp

#include "ocString.h"

typedef class page;

class base_pg_handler 
{
protected:
  page & pg;  
public:
  ocString uiTemplate; 
  base_pg_handler(page & pgIn):pg(pgIn){;}
  virtual ~base_pg_handler(){;}
  
  virtual bool operator()( void )
  {
    return false;
  }  
};

typedef map< string, base_pg_handler * > pgHandlerMap;

void cleanHandlerMap(pgHandlerMap & in)
{ 
  pgHandlerMap::iterator pos;
  for( pos=in.begin(); pos!=in.end(); ++pos )
  {
    delete pos->second;
  }
  in.clear();
} 




#endif 
