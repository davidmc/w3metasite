/*
  XML_Time_svc.cpp
  (c) 2006 dkmccombs
*/
#include <iostream>
#include <iomanip>
#include "cgiClass.h"
#include "connectstring"
#include "read_write_base.hpp"
#include "cgiCookie.h"
#include "openLogin.h"
#include "Time.hpp"
#include "xml_forms.hpp"

using namespace std;
openLogin oLogin;
class Time_form:  public Time_Obj, public xml_form
{
  public:
    Time_form(cgiScript & script)
    :Time_Obj()
    ,xml_form(script)
    {setKey(*this);}

    virtual ~Time_form()
    {;}
  
    void form_id_transfer( void )
    {
      llongFXfer( "Id", Id );
    }

    void form_data_transfer( void )
    {
      llongFXfer( "Task", Task);
      dateFXfer( "Start", Start);
      dateFXfer( "Stop", Stop);
      stringFXfer( "Description", Description);
      doubleFXfer( "Hours", Hours);
      llongFXfer( "Status", Status);
      llongFXfer( "Invoice", Invoice);
      llongFXfer( "Performed_By", Performed_By);
    }

    bool dbf_action( string mode, changeMap & changes )
    {
      return db_action( mode, changes );
    }
  
  // Implement trivial form display
    bool form_display( void )
    {
      bool breturn = true;
      script << "<Time>" << endl;
      script << "<Id>" << Id << "</Id>" << endl;
      script << "<Task>" << Task << "</Task>" << endl;
      script << "<Start>" << Start << "</Start>" << endl;
      script << "<Stop>" << Stop << "</Stop>" << endl;
      script << "<Description>" << Description << "</Description>" << endl;
      script << "<Hours>" << Hours << "</Hours>" << endl;
      script << "<Status>" << Status << "</Status>" << endl;
      script << "<Invoice>" << Invoice << "</Invoice>" << endl;
      script << "<Performed_By>" << Performed_By << "</Performed_By>" << endl;
      script << "</Time>" << endl;
      return breturn;
    }
};

int main( int argc, char ** argv )
{
  cgiScript script("text/xml",true);
  script << "<?xml version='1.0' encoding='UTF-8' standalone='yes'?>"  << endl;
  script << "<W3SysService>" << endl;
  if( oLogin.testLoginStatus() )
  {
    Time_form myFrm(script);
    bool is_good = myFrm.form_action();
    if(is_good)
    {
      myFrm.form_display();
    }
    script << "<status>";
    if( is_good)  script << "GOOD";
    else script << myFrm.last_result();
    script << "</status>" << endl;
  }
  else
  {
    script << "<status>";
    script << "Invalid Login!" << endl;
    script << "</status>";
  }
  script << "</W3SysService>" << endl;
  return 0;
}
// compile implementations here
#include "read_write_base.cpp"
#include "xml_forms.cpp"

