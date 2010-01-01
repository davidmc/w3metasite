#ifndef MENU_HPP
#define MENU_HPP

#include "read_write_base.hpp"
using namespace std;

class menu: public read_write_base
{
public:
  identifier id;
  long long site_id;  
  string name;
  string reference_template;
  string template_open_tag;
  string template_close_tag;
  
  menu():read_write_base()
      ,id(0LL),site_id(0LL),name("")
      ,reference_template("")
      ,template_open_tag("")
      ,template_close_tag("")
  {  
    // set name
    data_name("metasite.menus");
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new llongXfer("site_id", &site_id ));    
    addDXMap( new stringXfer("name", &name ));
    addDXMap( new stringXfer("reference_template", &reference_template ));
    addDXMap( new stringXfer("template_open_tag", &template_open_tag ));
    addDXMap( new stringXfer("template_close_tag", &template_close_tag ));    
  }
  virtual bool dsupplemental( void )
  { 
    // delete all associated content
    ocString sql = "delete from menu_items where menu_id = ";
    sql.append(id);
    cmd.execute(sql);
    return true; 
  }
};
#endif
