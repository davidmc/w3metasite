/*
  XML_Session_svc.cpp
  (c) 2006 dkmccombs
  
*/
#include <iostream>
#include <iomanip>
#include "cgiClass.h"
#include "connectstring"
#include "read_write_base.hpp"
#include "cgiCookie.h"
#include "Session.hpp"
#include "xml_forms.hpp"

using namespace std;
class Session_form:  public Session_Obj, public xml_form
{
public: 
  Session_form(cgiScript & script)
  :Session_Obj()
  ,xml_form(script)
  {setKey(*this);}

  virtual ~Session_form()
  {;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }

  void form_data_transfer( void )
  {
    // get client variables
    queryStringMap & args = script.ClientArguments().TheMap();
    // for each do SetData( string Variable_Name, string Data )
    queryStringMap::iterator it = args.begin();
    while( it != args.end() )
    {
      string varName = it->first;
      string varValue = it->second.c_str();
      SetData( varName, varValue );
      ++it;
    }

  }

  bool dbf_action( string mode, changeMap & changes )
  {
    return Synch();
  } 

  // Implement trivial form display
  bool form_display( void )
  {
    bool breturn = true;
    script << "<Session id=\"" << Id << "\">" << endl;
    script << "<Id>" << Id << "</Id>" << endl;
    script << "<User_Id>" << User_Id << "</User_Id>" << endl;
    script << "<UUID>" << UUID << "</UUID>" << endl;
    script << "<Instance_Stamp>" << Instance_Stamp << "</Instance_Stamp>" << endl;
    script << "<XML_Variables>" << XML_Variables << "</XML_Variables>" << endl;
    script << "</Session>" << endl; 
    return breturn;
  }
};

int main( int argc, char ** argv )
{
  cgiScript script("text/xml",true);
  script << "<?xml version='1.0' encoding='UTF-8' standalone='yes'?>"  << endl;
  script << "<W3SysService>" << endl;

  Session_form myFrm(script);
  bool is_good = myFrm.form_action();
  if(is_good)
  {
    myFrm.form_display();
  }
  script << "<status>";
  if(is_good)  script << "GOOD";
  else script << myFrm.last_result();
  script << "</status>" << endl;

  script << "</W3SysService>" << endl;
  return 0;
}
// compile implementations here
#include "read_write_base.cpp"
#include "xml_forms.cpp"

