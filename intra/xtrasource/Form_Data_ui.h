/*
  Form_Data_ui.h

  User Interface Definition and Implementation 
  for Form_Data.

  Copyright (c) 2008 by D.K. McCombs.
  davidmc@w3sys.com
  W3 Systems Design Inc : http://www.w3sys.com 
*/

#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "cgiTemplates.h"

#include "Form_Data.hpp"
#include "Form_Label.hpp"

#include "list_base.hpp"
#include "forms_base.hpp"

using namespace std;

class Form_Data_form:  public Form_Data_Obj, public forms_base
{

public:

  ocString label_list;
  Form_Label_Obj label;
  bool haveForm;
  Form_Data_form(cgiScript & script):Form_Data_Obj(),forms_base(script)
  {
    setKey(*this);
  } 
  virtual ~Form_Data_form()
  {
  }
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }


  void formToVar( string & var, string & label, ocString & name )
  {
    stringFXfer( name, var);
    if( !haveForm )
    {
      name.parse("_");
      label = name.remainder();
    }
    name = label_list.parse(",");
  }
  
  void form_data_transfer( void )
  {
    stringFXfer( "form_name", form_name);
    
    string clause = "form_name = '" +  form_name + "'";
    if( label.get_data(clause) )
    {
       haveForm = true;
    }
    else
    {
      stringFXfer( "form_name", label.form_name);
    }
    label_list.parseInit();
    ocString name = label_list.parse(",");
    if( name.length() ) formToVar( data_a, label.label_a, name );
    if( name.length() ) formToVar( data_b, label.label_b, name );
    if( name.length() ) formToVar( data_c, label.label_c, name );
    if( name.length() ) formToVar( data_d, label.label_d, name );
    if( name.length() ) formToVar( data_e, label.label_e, name );
    if( name.length() ) formToVar( data_f, label.label_f, name );
    if( name.length() ) formToVar( data_g, label.label_g, name );
    if( name.length() ) formToVar( data_h, label.label_h, name );
    if( name.length() ) formToVar( data_i, label.label_i, name );
    if( name.length() ) formToVar( data_j, label.label_j, name );
    if( name.length() ) formToVar( data_k, label.label_k, name );
    if( name.length() ) formToVar( data_l, label.label_l, name );
    if( name.length() ) formToVar( data_m, label.label_m, name );
    if( name.length() ) formToVar( data_n, label.label_n, name );
    if( name.length() ) formToVar( data_o, label.label_o, name );
    if( name.length() ) formToVar( data_p, label.label_p, name );
    if( name.length() ) formToVar( data_q, label.label_q, name );
    if( name.length() ) formToVar( data_r, label.label_r, name );
    if( name.length() ) formToVar( data_s, label.label_s, name );
    if( name.length() ) formToVar( data_t, label.label_t, name );
    if( name.length() ) formToVar( data_u, label.label_u, name );
    if( name.length() ) formToVar( data_v, label.label_v, name );
    if( name.length() ) formToVar( data_w, label.label_w, name );

  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    bool ret = false;
    if( !haveForm ) ret = label.db_action( "i", changes );
    Form_Label_Id = label.key();
    ret = db_action( "i", changes );
    return ret;
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;

    return breturn;
  }
};


