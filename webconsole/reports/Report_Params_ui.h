/*
  Report_Params_ui.h

  User Interface Definition and Implementation 
  for Report_Parameters.
  
  Copyright (c) 2005 by D.K. McCombs.
  davidmc@w3sys.com
  W3 Systems Design Inc : http://www.w3sys.com  
  
*/
#ifndef Report_Params_ui_h
#define Report_Params_ui_h

#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "cgiTemplates.h"
#include "Report_Parameters.hpp"
#include "list_base.hpp"
#include "forms_base.hpp"

// #define DO_OPEN_LOGGING
#include "openLogger.h"

/* 
   The primitive that holds the parameter type,
   the label name, the tag name, etc.
   Most importantly, also holds the value.
*/   
   
class Report_Parameters
{
 
public:
  identifier Id;
  llong Report_Id;       // report id
  string Display_Label;  // what shows on the input form
  string Data_Element;   // the form name (could be the data elament name, but nbot necessarily so
  string Tag_Name;       // the tag text in the SQL code to be replaced by the real value
  string Choices_Query;  // Only for combo boxes
  string Parameter_Type; // the type of parameter

    
  /*
   list of value types tat are now implemented: 
     Parameter_Types = "STRING,DATE,SELECT(llong or string)" 
  */   

  string value_str; // always filled! (for unformated val, and value checking)
  money value_mny;
  oc_date value_ocd;
  time_date value_td;
   
  // Default constructor
  Report_Parameters():
  Id(0LL),
  Report_Id(0LL),
  Display_Label(""),
  Data_Element(""),
  Tag_Name(""),
  Choices_Query(""),
  Parameter_Type("")
  {
    ;
  }
  
  // copy constructor
  Report_Parameters( const Report_Parameters & in )
  {
    Id=in.Id;
    Report_Id=in.Report_Id;
    Display_Label=in.Display_Label;
    Data_Element=in.Data_Element;
    Tag_Name=in.Tag_Name;
    Choices_Query=in.Choices_Query;
    Parameter_Type=in.Parameter_Type;
    value_str=in.value_str;
    value_mny=in.value_mny;
    value_ocd=in.value_ocd;
    value_td=in.value_td; 
    
  }
  
  // virtual destructor
  virtual ~Report_Parameters()
  {
    ;
  }
  
  // assignment operator
  Report_Parameters & operator = ( const Report_Parameters & in )
  {
    Id=in.Id;
    Report_Id=in.Report_Id;
    Display_Label=in.Display_Label;
    Data_Element=in.Data_Element;
    Tag_Name=in.Tag_Name;
    Choices_Query=in.Choices_Query;
    Parameter_Type=in.Parameter_Type;
    value_str=in.value_str;
    value_mny=in.value_mny;
    value_ocd=in.value_ocd;
    value_td=in.value_td;     
    return *this;
  } 
    // assignment operator # 2
  Report_Parameters & operator = ( const Report_Parameters_Obj  & in )
  {
    Id=in.Id;
    Report_Id=in.Report_Id;
    Display_Label=in.Display_Label;
    Data_Element=in.Data_Element;
    Tag_Name=in.Tag_Name;
    Choices_Query=in.Choices_Query;
    Parameter_Type=in.Parameter_Type;
    return *this;
  } 
  void getClientData(cgiInput & clientArgs)
  {
    writelog2("Looking for", Data_Element);    
    if( clientArgs.count(Data_Element.c_str()) > 0 )
    {
      writelog2("Found", Data_Element);
      value_str = clientArgs[Data_Element.c_str()].c_str();
      writelog2("Set to",value_str);
      if( Parameter_Type == "DATE" ) value_ocd.parse(value_str.c_str());      
    }
  }
  bool hasValue ( void )
  {
    bool bret = false;
    if( value_str.length() ) bret = true;
    return bret; 
  }
  
  string uiGroup( forms_base & fb )
  {
    string ret;
    // Parameter_Type = "STRING=STRING,DATE=DATE,SELECT=SELECT
    if( Parameter_Type == "STRING") ret = fb.makeTextBox(Display_Label, Data_Element, value_str ,"255","25");
    else if( Parameter_Type == "DATE" ) ret = fb.makeDateBox( Display_Label, Data_Element, value_str, "10");
    else if( Parameter_Type == "SELECT" ) ret = fb.makeComboBox(Display_Label, Data_Element, value_str ,Choices_Query);    
    ret += "<br class='clearall'>";
    return ret;  
  }
  
  // formatted for a query of course
  string formattedValue( void )
  {
    string ret;
    // Parameter_Type = "STRING=STRING,DATE=DATE,SELECT=SELECT
    if( Parameter_Type == "STRING") ret = value_str;
    else if( Parameter_Type == "DATE" ) ret = dynamic_cast<time_date&>(value_ocd).format("%Y-%m-%d");
    else if( Parameter_Type == "SELECT" ) ret = value_str;
    return ret;
  }
  
  string filter( ocString temp )
  {
    return temp.replaceAll(Tag_Name,formattedValue().c_str());
  }
    
};

// Data_Element is used as the key, by the way
typedef map< string, Report_Parameters> rep_params;

using namespace std;

/* 
  Used UI wizard to generate this - highly modified.
  
  Use: 
    load params, figure out if correct params passed back
  LOGIC:
    Check for report parameters attached to this report.
    If we have the report parameters, fill the value map.
    else show a form so the user can give them to you.

*/
class Report_Parameters_form:  public Report_Parameters_Obj, public forms_base
{
public: 
  rep_params rps;
  
  Report_Parameters_form(cgiScript & script):Report_Parameters_Obj(),forms_base(script)
  {
    setKey(*this);    
  }
  
  virtual ~Report_Parameters_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  
  void form_data_transfer( void )
  {
    llongFXfer( "Report_Id", Report_Id);
    stringFXfer( "Display_Label", Display_Label);
    stringFXfer( "Data_Element", Data_Element);
    stringFXfer( "Tag_Name", Tag_Name);
    stringFXfer( "Choices_Query", Choices_Query);
    stringFXfer( "Parameter_Type", Parameter_Type); 
  } 
  
  /* 
    This over-rode implementation does nothing.
  */   
  bool dbf_action( string mode, changeMap & changes )
  {
    return true;
  }
  /* 
    Get the data if any.
  */
  bool get( void )
  {
    bool bret = false;
    // We should have a report id
    if( Report_Id )
    {
      // see if there is any data
      ocString clause = " Report_Id = ";
      clause.append(Report_Id);
      writelog("opening rp data...");      
      bool isOpen = get_data(clause); 
      writelog2("opened rp data with ret", isOpen); 
      cgiInput & args = script.ClientArguments();          
      while( isOpen )
      {        
        Report_Parameters rp;
        rp=*this; 
        rp.getClientData(args); 
              
        writelog3("Data Elem. and Data: ", rp.Data_Element, rp.value_str);
        rps.insert( make_pair(Data_Element,rp) );
        writelog("Getting Next");
        isOpen=next();
        writelog2("Next Returned", isOpen );
      }
      bret = true;
    }
    return bret;
  }
   
  bool haveFormValues( void )
  {
    bool bret = false;
    
    rep_params::iterator pos = rps.begin();
    if( pos == rps.end() ) bret = true;
    // if the first has it they all will
    else if( pos->second.hasValue() ) bret = true;
    return bret;    
  }
  
  // Implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script << makeTop("", "Please enter these parameters in order to run the report:");
    script << "<br class='clearall'>" << endl; 
    // itererate through the map, output suitable form elements
    rep_params::iterator pos;
    for( pos=rps.begin(); pos!=rps.end(); ++pos)
    {
      Report_Parameters & rp = pos->second;      
      script << rp.uiGroup(*this);
    }
    script << formButtons( "", "&nbsp;Run Report&nbsp;", "");
    // we need a different kind of button here script << makeButtons( key() );        
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
  
  string paramFilters( string temp )
  {
    rep_params::iterator pos;
    for( pos=rps.begin(); pos!=rps.end(); ++pos)
    {
      Report_Parameters & rp = pos->second;      
      temp = rp.filter(temp);
    }
    return temp;
  }
  
  
};


#endif
