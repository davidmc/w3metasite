/* site.hpp"

  Site Object
*/  
#ifndef SITE_HPP
#define SITE_HPP

#include "read_write_base.hpp"
class siteObject: public read_write_base
{
public:
  identifier id;
  string name;
  string domain;
  string url;
  string path;
  
  siteObject():read_write_base(),id(0LL)
  {  
    // set name
    data_name("metasite.sites");
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new stringXfer("name", &name ));
    addDXMap( new stringXfer("domain", &domain ));
    addDXMap( new stringXfer("url", &url ));
    addDXMap( new stringXfer("path", &path ));

  }
};
#endif
