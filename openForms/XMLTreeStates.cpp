/*
  XMLTreeStates.cpp

  // For setting expansion and contraction
     of  wTree nodes
  
  (c) 2006 dkmccombs
*/
#include <iostream>
#include <iomanip>
#include <set>
#include "cgiClass.h"
#include "../sessions/connectstring"
#include "read_write_base.hpp"
#include "cgiCookie.h"
#include "Session.hpp"
#include "xml_forms.hpp"

using namespace std;

typedef class set< string > expandoSet;
class expanded
{
  expandoSet expandos;
public:
  expanded( ocString toParse )
  {
    string val=toParse.parse(",");
    while( val.length() )
    {
      expandos.insert(val); 
      val = toParse.parse(",");
    }
  }
  ~expanded(){;}
  expanded & add( string toAdd )
  {
    expandos.insert(toAdd);
    return *this;
  }
  expanded & remove( string toRemove )
  {
    expandos.erase(toRemove);
    return *this;
  }
  string toString(void)
  {
    string ret;
    expandoSet::iterator it = expandos.begin();
    while(it!=expandos.end())
    {
      ret += *it;
      ++it;
      if( it!=expandos.end())
        ret += ",";
    }
    return ret;
  }
  expandoSet Expanded ( void )
  {
    return expandos;
  }
  
};
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
    queryStringMap::iterator it;

    // need TreeUid var
    it = args.find("TreeUid");
    if( it != args.end() )
    {
      string treeId = it->second.c_str();
      // get current list of expanded nodes
      expanded exState(GetData(treeId));

      // trying to expan???
      it = args.find("expand");
      if(  it != args.end() )
      {
        exState.add(it->second.c_str());
        SetData( treeId, exState.toString() );
      }

      // trying to contract???
      it = args.find("contract");
      if(  it != args.end() )
      {
        exState.remove(it->second.c_str());
        SetData( treeId, exState.toString() );
      }
      
      // node expand/contract flip indicator, to be used if they complain about the current implementation
      it = args.find("flip");
      if( it != args.end() )
      {
        treeId += "_flip";
        // is it there already?
        string flip_id = GetData(treeId);
        if( it->second == flip_id.c_str() )
        {
          flip_id = "";
        }
        SetData( treeId, flip_id );
      }
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

