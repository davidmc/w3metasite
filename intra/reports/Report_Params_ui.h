/*
  Report_Params_ui.h

  Report_Parameters UI and class logic for reports
  
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
// for caching parameters
#include "Session.hpp"

// #define DO_OPEN_LOGGING
#include "openLogger.h"

/* 
  For auto type params, to get one or more values back.
   If multiple, they are seperated by commoas. 
*/
string multivalLookup ( string sql )
{
  string ret;
  quickQuery qqry;
  openRS & rs = qqry.getData(sql);
  bool opened = qqry.opened;  
  while( opened )
  {
    if( ret.length() > 0 )
      ret += ", ";
    ret += rs.getField(0).format();
    opened = rs.next();
  }  
  rs.close();
  return ret;
}  

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
  string Display_Label;  // What shows on the input form

  string Data_Element;   // The form name (could be the data element name, but not necessarily so
                         // Also used for the map of parameters.
 
  string Tag_Name;       // the tag text in the SQL code to be replaced by the real value
  string Choices_Query;  // Only for combo boxes
  string Parameter_Type; // the type of parameter
  int    List_Order;
  string Pre_Content;
  string Post_Content;
  
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
  Parameter_Type(""),
  List_Order(0),
  Pre_Content(""),
  Post_Content("")
  {
    
  }
  
  /*/ copy constructor
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
    
}*/
  
   Report_Parameters( const Report_Parameters & in):
   Id(in.Id),
   Report_Id(in.Report_Id),
   Display_Label(in.Display_Label),
   Data_Element(in.Data_Element),
   Tag_Name(in.Tag_Name),
   Choices_Query(in.Choices_Query),
   Parameter_Type(in.Parameter_Type),
   List_Order(in.List_Order),
   Pre_Content(in.Pre_Content),
   Post_Content(in.Post_Content)
   {
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
    List_Order = in.List_Order;
    Pre_Content = in.Pre_Content;
    Post_Content = in.Post_Content;
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
    List_Order = in.List_Order;
    Pre_Content = in.Pre_Content;
    Post_Content = in.Post_Content;
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
  
  void preHasValueCheckInit( void )
  {
    /* {{login}}, {{group}}
       modify the Choices_Query here with 
       the values from the global login object
    */
    ocString nChoices_Query = Choices_Query;
    Choices_Query =  nChoices_Query
                      .replaceAll( "{{login}}", oLogin.Id() )
                      .replaceAll( "{{group}}", oLogin.GroupId())
                      .replaceAll( "{{staff}}", oLogin.myStaff());
    // IF Parameter_Type is 'AUTO' then get the data value now!
    if(Parameter_Type == "AUTO" )
    {
      value_str = multivalLookup( Choices_Query );
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
    // Parameter_Type = "STRING=STRING,DATE=DATE,SELECT=SELECT,AUTO=AUTO
    if( Parameter_Type == "AUTO" ) if(!hasValue())
      { ret = "Error! You may not have the permissions to run this report <!-- ";
        ret += Choices_Query + "-->";
      }
    if( Parameter_Type == "STRING") ret = fb.makeTextBox(Display_Label, Data_Element, value_str ,"255","25");
    else if( Parameter_Type == "DATE" ) ret = fb.makeDateBox( Display_Label, Data_Element, value_str, "10");
    else if( Parameter_Type == "SELECT" ) ret = fb.makeComboBox(Display_Label, Data_Element, value_str ,Choices_Query);    
    if( ret.length() )
    {
      ret +=
          "<script  type='text/javascript'>createUserVal( '" +
          Data_Element +
          "' , '" +
          Display_Label +
          "' );</script>";
      ret += "<br class='clearall'>";
    }
    return ret;
  }

  // formatted for a query of course
  string formattedValue( void )
  {
    string ret;
    // Parameter_Type = "STRING=STRING,DATE=DATE,SELECT=SELECT,AUTO=AUTO
    if( Parameter_Type == "STRING") ret = value_str;        
    else if( Parameter_Type == "DATE" ) ret = dynamic_cast<time_date&>(value_ocd).format("%Y-%m-%d");
    else if( Parameter_Type == "SELECT" ) ret = value_str;
    else if( Parameter_Type == "AUTO" ) ret = value_str;
    return ret;
  }
  
  string filter( ocString temp )
  {
    ocString value = formattedValue();
    /*
      Special case of a replacement value needing to take on more than one value.
      Do one at a time replacements as parsed from the input argument
    */
    if( (Parameter_Type == "SELECT"  ||  Parameter_Type == "AUTO") &&
         value.find("|") != string::npos )
    {
      string tok = value.parse("|");
      while( tok.length() )
      {
        temp = temp.replace(Tag_Name,tok.c_str());
        tok = value.parse("|");
      }
    }
    else  // General case: Just replace all
    {
      temp = temp.replaceAll(Tag_Name,value.c_str());
    }
    return temp;
  }

};

// Data_Element is used as the key, by the way
typedef map< string, Report_Parameters> rep_params;
typedef vector< string > param_order;

using namespace std;

/*

  Used UI wizard to generate this - but it is highly modified.

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
  param_order rpo;

  string reportName;
  static bool hasParamValues;
  
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
      bool isOpen = get_data(clause, "List_Order" ); 
      writelog2("opened rp data with ret", isOpen); 
      cgiInput & args = script.ClientArguments();          
      while( isOpen )
      {        
        Report_Parameters rp;
        rp=*this; 
        rp.getClientData(args); 
              
        writelog3("Data Elem. and Data: ", rp.Data_Element, rp.value_str);
        rpo.push_back(Data_Element); 
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
    bool bret = true;    
    rep_params::iterator pos = rps.begin();    
    /* 
      If no params to worry about, 
       this will simply return true
       without entering the while loop
    */    
    while( pos != rps.end() )
    {
      // this is for filter filter replacements and autovalues
      pos->second.preHasValueCheckInit();
      
      if( pos->second.hasValue()  == false ) 
      {
        bret = false;
        break;
      }  
      ++pos;
    }
    hasParamValues = bret;
    return bret;
  }
  
  // Implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    string Title = "<h2>";
    Title += reportName;
    Title += "</h2>\n";
    Title += "<h5>Please enter the following to run the report:</h5>";
    script << makeTop("",  Title);

    script << "<br class='clearall'>" << endl; 
    // itererate through the vector to the map, output suitable form elements
    for( int  x=0; x < rpo.size(); ++x)
    {
      rep_params::iterator pos = rps.find(rpo[x]);
      if( pos !=rps.end() )
      {
        Report_Parameters & rp = pos->second;
        if( rp.Pre_Content.length() )
          script << rp.Pre_Content;
        script << rp.uiGroup(*this);
        if( rp.Post_Content.length() )
          script << rp.Post_Content;
      }
    }
    script << formButtons( "", "&nbsp;Run Report&nbsp;", "");
    // we need a different kind of button here script << makeButtons( key() );        
    script << makeBottom( "--" ) << endl;
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
    // only save if not going after cached values
    if( script.ClientArguments().count("doParamFilter") == 0 )
      saveToSession();
    return temp;
  }

  void saveToSession( void )
  {
    rep_params::iterator pos = rps.begin();
    Session_Obj sess;
    string rep_params = "";
    while( pos != rps.end() )
    {
      rep_params += pos->second.Tag_Name;
      rep_params += "~";
      rep_params += pos->second.formattedValue();
      rep_params += "`";
      ++pos;
    }
    sess.SetData("rep_params",rep_params);
    sess.Synch();

  }
  /*
    Formatted List of filters in place when the report ran.
  */
  string filterList( string repTemplate )
  {
    string temp;
    if( script.ClientArguments().count("rep_params") )
    {
      loadControlTemplates(repTemplate);
      ocString paramHeading = formTemplate.getParagraph("param_heading");
      ocString paramItem = formTemplate.getParagraph("param_item");
      ocString paramEnding = formTemplate.getParagraph("param_ending");
      if( paramItem.length() )
      {
        temp = paramHeading;
        ocString params = script.ClientArguments()["rep_params"].c_str();
        ocString parseItem =  params.parse("`");  
        while( parseItem.length() )
        {
          string label = parseItem.parse("~");
          string value = parseItem.remainder();
          temp += paramItem.replace("$label$",label).replace("$value$",value);
          parseItem =  params.parse("`");
        }
        temp += paramEnding;
      }
    }
    return temp;
  }
};


#endif
