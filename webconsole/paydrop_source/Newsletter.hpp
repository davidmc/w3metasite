/*
  base classes:
  read_write_base
  PayDrop Newsletter Business Object 
*/

#ifndef PD_Newsletter
#define PD_Newsletter
#include "read_write_base.hpp"


class  Newsletter :  public read_write_base
{
public:
  // more in the mode of metasite schema
  identifier id;
  long long site_id;
  long long page_id;
  string address_query;
  // tors
  Newsletter():read_write_base()
  ,id(0LL),site_id(0LL),page_id(0LL)
  {
    data_name("newsletter");
    addDXMap( new llongXfer("id", &id ) );
    addDXMap( new llongXfer("site_id", &site_id ) );
    addDXMap( new llongXfer("page_id", &page_id ) );
    addDXMap( new stringXfer("address_query", &address_query ) );
  }
  virtual ~Newsletter(){;}
};

#endif

