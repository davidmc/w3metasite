  /* 
  Form_Label.hpp

  Object Definition and Implementation 
  for Form_Label.
  
  Copyright (C) 2008 by D.K. McCombs.
  W3 Systems Design

*/  
#ifndef Form_Label_HPP
#define Form_Label_HPP

#include "read_write_base.hpp"

class Form_Label_Obj: public read_write_base
{
public:
  identifier Id;
  string form_name;
  string label_a;
  string label_b;
  string label_c;
  string label_d;
  string label_e;
  string label_f;
  string label_g;
  string label_h;
  string label_i;
  string label_j;
  string label_k;
  string label_l;
  string label_m;
  string label_n;
  string label_o;
  string label_p;
  string label_q;
  string label_r;
  string label_s;
  string label_t;
  string label_u;
  string label_v;
  string label_w;
  Form_Label_Obj():read_write_base()
  ,Id(0LL)
  ,form_name("")
  ,label_a("")
  ,label_b("")
  ,label_c("")
  ,label_d("")
  ,label_e("")
  ,label_f("")
  ,label_g("")
  ,label_h("")
  ,label_i("")
  ,label_j("")
  ,label_k("")
  ,label_l("")
  ,label_m("")
  ,label_n("")
  ,label_o("")
  ,label_p("")
  ,label_q("")
  ,label_r("")
  ,label_s("")
  ,label_t("")
  ,label_u("")
  ,label_v("")
  ,label_w("")
  {
    // set name
    data_name("Form_Label");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new stringXfer("form_name", &form_name ));
    addDXMap( new stringXfer("label_a", &label_a ));
    addDXMap( new stringXfer("label_b", &label_b ));
    addDXMap( new stringXfer("label_c", &label_c ));
    addDXMap( new stringXfer("label_d", &label_d ));
    addDXMap( new stringXfer("label_e", &label_e ));
    addDXMap( new stringXfer("label_f", &label_f ));
    addDXMap( new stringXfer("label_g", &label_g ));
    addDXMap( new stringXfer("label_h", &label_h ));
    addDXMap( new stringXfer("label_i", &label_i ));
    addDXMap( new stringXfer("label_j", &label_j ));
    addDXMap( new stringXfer("label_k", &label_k ));
    addDXMap( new stringXfer("label_l", &label_l ));
    addDXMap( new stringXfer("label_m", &label_m ));
    addDXMap( new stringXfer("label_n", &label_n ));
    addDXMap( new stringXfer("label_o", &label_o ));
    addDXMap( new stringXfer("label_p", &label_p ));
    addDXMap( new stringXfer("label_q", &label_q ));
    addDXMap( new stringXfer("label_r", &label_r ));
    addDXMap( new stringXfer("label_s", &label_s ));
    addDXMap( new stringXfer("label_t", &label_t ));
    addDXMap( new stringXfer("label_u", &label_u ));
    addDXMap( new stringXfer("label_v", &label_v ));
    addDXMap( new stringXfer("label_w", &label_w ));
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



