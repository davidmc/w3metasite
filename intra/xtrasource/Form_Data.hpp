/* 
  Form_Data.hpp

  Object Definition and Implementation 
  for Form_Data.
  
  Copyright (C) 2008 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Form_Data_HPP
#define Form_Data_HPP

#include "read_write_base.hpp"

class Form_Data_Obj: public read_write_base
{
public:
  identifier Id;
  llong Form_Label_Id;
  string form_name;
  time_date sent;
  string recipient;
  bool is_read;
  time_date read_on;
  string data_a;
  string data_b;
  string data_c;
  string data_d;
  string data_e;
  string data_f;
  string data_g;
  string data_h;
  string data_i;
  string data_j;
  string data_k;
  string data_l;
  string data_m;
  string data_n;
  string data_o;
  string data_p;
  string data_q;
  string data_r;
  string data_s;
  string data_t;
  string data_u;
  string data_v;
  string data_w;
  Form_Data_Obj():read_write_base()
  ,Id(0LL)
  ,Form_Label_Id(0LL)
  ,form_name("")
  ,sent( )
  ,recipient("")
  ,is_read(false)
  ,read_on( )
  ,data_a("")
  ,data_b("")
  ,data_c("")
  ,data_d("")
  ,data_e("")
  ,data_f("")
  ,data_g("")
  ,data_h("")
  ,data_i("")
  ,data_j("")
  ,data_k("")
  ,data_l("")
  ,data_m("")
  ,data_n("")
  ,data_o("")
  ,data_p("")
  ,data_q("")
  ,data_r("")
  ,data_s("")
  ,data_t("")
  ,data_u("")
  ,data_v("")
  ,data_w("")
  {
    // set name
    data_name("Form_Data");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new llongXfer("Form_Label_Id", &Form_Label_Id ));
    addDXMap( new stringXfer("form_name", &form_name ));
    addDXMap( new time_dateXfer("sent", &sent ));
    addDXMap( new stringXfer("recipient", &recipient ));
    addDXMap( new boolXfer("is_read", &is_read ));
    addDXMap( new time_dateXfer("read_on", &read_on ));
    addDXMap( new stringXfer("data_a", &data_a ));
    addDXMap( new stringXfer("data_b", &data_b ));
    addDXMap( new stringXfer("data_c", &data_c ));
    addDXMap( new stringXfer("data_d", &data_d ));
    addDXMap( new stringXfer("data_e", &data_e ));
    addDXMap( new stringXfer("data_f", &data_f ));
    addDXMap( new stringXfer("data_g", &data_g ));
    addDXMap( new stringXfer("data_h", &data_h ));
    addDXMap( new stringXfer("data_i", &data_i ));
    addDXMap( new stringXfer("data_j", &data_j ));
    addDXMap( new stringXfer("data_k", &data_k ));
    addDXMap( new stringXfer("data_l", &data_l ));
    addDXMap( new stringXfer("data_m", &data_m ));
    addDXMap( new stringXfer("data_n", &data_n ));
    addDXMap( new stringXfer("data_o", &data_o ));
    addDXMap( new stringXfer("data_p", &data_p ));
    addDXMap( new stringXfer("data_q", &data_q ));
    addDXMap( new stringXfer("data_r", &data_r ));
    addDXMap( new stringXfer("data_s", &data_s ));
    addDXMap( new stringXfer("data_t", &data_t ));
    addDXMap( new stringXfer("data_u", &data_u ));
    addDXMap( new stringXfer("data_v", &data_v ));
    addDXMap( new stringXfer("data_w", &data_w ));
  }

  /*
  TODO: Add Event hooks for business logic
  =========================
  // Done before any action - a false return prevents action
  virtual bool ivalidate( void  ) { return true; }
  virtual bool uvalidate( changeMap & changes  ) { return true; }
  virtual bool dvalidate( void ) { return true; }

  // Done after any action - a false return sets a warning
  virtual bool isupplemental( void ){ return true; }
  virtual bool usupplemental( changeMap & changes ){ return true; }
  virtual bool dsupplemental( void ){ return true; }
  virtual bool ssupplemental( void ){ return true; } 
  */
};
#endif



