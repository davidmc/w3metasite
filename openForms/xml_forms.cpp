/*
xml_form.cpp
================
Forms Base Implementation

*/
#include "xml_forms.hpp"
#include "ocXML.h"

xml_form::xml_form(cgiScript & in):script(in)
{
  m_warn = "<br>";
  getInstructions();  
}

xml_form::~xml_form()
{
  ;
} 

bool xml_form::getInstructions( void )
{
  bool breturn = false;
  if( script.ClientArguments().count("w3sysMode") )
  { 
    mode = script.ClientArguments()["w3sysMode"].c_str();
    breturn=true;
  }
  // This is only needed in case of tricky boolean values
  //  which by the nature of html forms checkboxes,
  //  do not yield any value when not checked!
  // logic boolean off = w3sysChanges && no corresponding client arg
  //       boolean on = client arg present (doesnt care about  w3sysChanges)
  if( script.ClientArguments().count("w3sysChanges") )
  {
    ocString chgList = script.ClientArguments()["w3sysChanges"].c_str();
    string change = chgList.parse(",");
    while( change.length() )
    {
      changes[change]=change;
      change = chgList.parse(",");
    }
    breturn=true;
  }
  return breturn;
}

// implement pure virtual of form action
bool xml_form::form_action( void )
{  
  // if not reset or new mode, move id   
  if( mode != "r" && mode != "n" )
  {
    form_id_transfer();
  }
  // if not reset mode, move 
  if( mode != "r" )
  {
    form_data_transfer();
  }      
  return dbf_action( mode, changes );
}


// form to class property exchange methods
void xml_form::stringFXfer( string in, string & out )
{
  if( script.ClientArguments().count(in.c_str()) )
  {
    changes[in]=in;
    out=script.ClientArguments()[in.c_str()].c_str();
  }
}
void xml_form::llongFXfer( string in, long long & out )
{ 
  if( script.ClientArguments().count(in.c_str()) )
  {
    changes[in]=in;
    out=atoll(script.ClientArguments()[in.c_str()].c_str());
  }  
}
void xml_form::longFXfer( string in, long & out )
{ 
  if( script.ClientArguments().count(in.c_str()) )
  {
    changes[in]=in;
    out=atol(script.ClientArguments()[in.c_str()].c_str());
  }  
}
void xml_form::unfmtLongFXfer( string in, long & out )
{ 
  if( script.ClientArguments().count(in.c_str()) )
  {
    changes[in]=in;
    ocString fixUp = script.ClientArguments()[in.c_str()].c_str();
    out=atol(fixUp.replaceAll(",","").c_str());
  }  
}
void xml_form::intFXfer( string in, int & out )
{ 
  if( script.ClientArguments().count(in.c_str()) )
  {
    changes[in]=in;
    out=atoi(script.ClientArguments()[in.c_str()].c_str());
  }  
}
void xml_form::shortFXfer( string in, short & out )
{ 
  if( script.ClientArguments().count(in.c_str()) )
  {
    changes[in]=in;
    out=atoi(script.ClientArguments()[in.c_str()].c_str());
  }  
}
void xml_form::doubleFXfer( string in, double & out )
{
  if( script.ClientArguments().count(in.c_str()) )
  {
    changes[in]=in;   
    out=atof( script.ClientArguments()[in.c_str()].c_str());
  }
}
void xml_form::moneyFXfer( string in, money & out )
{
  if( script.ClientArguments().count(in.c_str()) )
  {
    changes[in]=in;
    ocString tmpVal = script.ClientArguments()[in.c_str()].c_str();
    double val = atof( tmpVal.replaceAll("$","").replaceAll(",","").c_str() );
    out=val;
  }
}
void xml_form::boolFXfer( string in, bool & out )
{
  out = false;
  if( script.ClientArguments().count(in.c_str()) )
  {
    changes[in]=in;
    out=true;
  }
}

void xml_form::dateFXfer( string in, oc_date & out )
{

  if( script.ClientArguments().count(in.c_str()) )
  {
    changes[in]=in;
    out.parse( script.ClientArguments()[in.c_str()].c_str() ); // date using locale
  }
}
void xml_form::dateFXfer( string in, time_date & out )
{

  if( script.ClientArguments().count(in.c_str()) )
  {
    changes[in]=in;
    out.parse( script.ClientArguments()[in.c_str()].c_str() ,"%m/%d/%Y  %I:%M:%S %p" ); // time_date using locale
  }
}
void xml_form::timeFXfer( string in, oc_time & out )
{
  
  if( script.ClientArguments().count(in.c_str()) &&  script.ClientArguments()[in.c_str()].length() )
  {
    changes[in]=in;
    out.setFormat("%I:%M %p");
    out.parse( script.ClientArguments()[in.c_str()].c_str() );
  }
}
