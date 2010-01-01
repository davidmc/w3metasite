/*

inner_forms_base.hpp
Forms Base Definition

Template CONVENTION

*/
#ifndef Forms_Base_Hpp
#define Forms_Base_Hpp

#include "cgiClass.h"
#include "cgiTemplates.h"
#include "cgiCookie.h"
#include "read_base.hpp"
string persistingKey = "key";

class inner_forms_base 
{
public:
  enum buttonStyle {   full, updateOnly, updateAndReset };
protected:    
  cgiTemplates formTemplate;
  cgiTemplates controlsTemplate;

  changeMap changes;
  string mode;
  string m_warn;
  string m_keyName;
  string m_formName;
  cgiScript & script;

  // form to class property exchange methods
  void stringFXfer( string in, string & out );
  void shortFXfer( string in, short & out );
  void intFXfer( string in, int & out );
  void unfmtLongFXfer( string in, long & out ); // remove comma formatting
  void longFXfer( string in, long & out );
  void llongFXfer( string in, long long & out );
  void doubleFXfer( string in, double & out );
  void moneyFXfer( string in, money & out );
  void boolFXfer( string in, bool & out );
  void dateFXfer( string in, oc_date & out );
  void dateFXfer( string in, time_date & out );
  void timeFXfer( string in, oc_time & out );
  
public:
  inner_forms_base(string formName, cgiScript & in);
  virtual ~inner_forms_base();
  
  // return embedded script
  cgiScript & Script(void){return script;}
    
  // virtual function
  virtual bool form_action( void );
  
  // pure virtual functions  
  virtual void form_id_transfer( void ) = 0;
  virtual void form_data_transfer( void ) = 0;
  virtual bool dbf_action( string mode, changeMap & changes ) = 0;
  virtual bool form_display( void ) = 0;

  // get directions about what to do from the hidden xml box
  bool getInstructions( void );  
  
  // load template for forms controls
  bool loadControlTemplates( string controlPath, string formPath  );
  cgiTemplates & getTemplates( void ) { return formTemplate; }
  
  virtual string makeTop(string uri, string name );
  virtual string makeBottom(string status  );
  
  virtual string makeButtons(long long int, inner_forms_base::buttonStyle);
  
  /* 
    Template method for making a combo box.
  */
  template<class valType> 
  string makeComboBox( string label, string name, valType value, 
                       string sql = "", ocString defaulted = "",
                       string attributes = "")
  {
    ocString ret = formTemplate.getParagraph("control_group");
    ocString selStart = controlsTemplate.getParagraph("dropdown_start");
    string strDropDown = selStart.replaceAll("$ID", name).replaceAll("$FORMID",m_formName);
    ocString selOption =  controlsTemplate.getParagraph("dropdown_option");
    ocString selectedValue;
    selectedValue.append(value);
    
    if(  defaulted.length() > 0 )
    {
      if(defaulted.find("=") == string::npos) // just a label 
      {
        strDropDown +=  selOption.replace("$VALUE","-1")
                                 .replace("$SELECTED","")
                                 .replace("$LABEL",defaulted);
      }
      else // a label with a specific value
      {
        string lbl =  defaulted.parse("=");
        string vlu =   defaulted.parse("=");
        strDropDown +=  selOption.replace("$VALUE",vlu)
                                 .replace("$SELECTED","")
                                 .replace("$LABEL",lbl);
      }
    }
    if( sql.length() )
    {
      quickQuery qq;
      openRS & rs = qq.getData(sql);
      for( bool result=qq.opened; result; result=rs.next() )
      {
        string label = "";
        string value = rs.getField(0).format();
        for( int lx=1; lx<rs.getFieldCount(); lx++ )
        {
          if( label.length() && rs.getField(lx).format().length() ) label += " - ";
          label += rs.getField(lx).format();
        }
        string selected = "";
        if( value == selectedValue)  selected = " selected='selected' ";
        strDropDown +=  selOption.replace("$VALUE",value)
                                .replace("$SELECTED",selected)
                                .replace("$LABEL",label);
      }
    }
    strDropDown += controlsTemplate.getParagraph("dropdown_end");
    ret = ret.replace("$label$",label.c_str()).replace("$name$",name.c_str())
               .replace("$control$",strDropDown );
    return ret;
  }

  template<class valType> 
  string makeManualComboBox( string label, string name, valType value, 
                       ocString labelValuePair = "", string defaulted = "", 
                       string attributes = "")
  {


    ocString selectedValue;
    selectedValue.append(value);
    return genericChoiceBox( "dropdown_start" , "dropdown_end",
                             "dropdown_option", label, name, selectedValue,
                             labelValuePair, "", " selected='selected' " );

  }
  
  template<class valType>
  string makeCheckBoxes( string label, string name, valType value,
                         ocString labelValuePair = "", string defaulted = "",
                         string attributes = "")
  {
    ocString selectedValue;
    selectedValue.append(value);
    return genericChoiceBox( "checkboxes_start" , "checkboxes_end",
                             "checkboxes_option", label, name, selectedValue,
                             labelValuePair, "", " checked='checked' " );
  }
  template<class valType>
  string makeRadios( string label, string name, valType value,
                     ocString labelValuePair = "", string defaulted = "",
                     string attributes = "")
  {
    ocString selectedValue;
    selectedValue.append(value);
    return genericChoiceBox( "radioboxes_start" , "radioboxes_end",
                             "radioboxes_option", label, name, selectedValue,
                             labelValuePair, "", " checked='checked' " );
  }

  /* 
    Template method for making a generic event text box.
  */                      
  string genericBox( string controlName, string label, string name,
                               string  value, string attr, string size,
                               string length );
  /* 
    Template method for making a text box.
  */                      
  template<class valType> 
  string makeTextBox( string label, string name, valType value, 
                      string size, string length = "")
  {
    ocString val;val.append(value);
    ocString ret = genericBox( "text",  label,  name,
                                val, "",  size,   length );
    return ret;
  }
  /* 
    Template method for making a hidden text box.
  */ 
  template<class valType> 
  string makeHiddenBox( string name, valType value )
  {
    ocString val;val.append(value);
    ocString strControlbox = controlsTemplate.getParagraph("hidden");
    return strControlbox.replaceAll("$ID",name).replace("$VALUE",val);
  }  
  /* 
    Template method for making a static text box.
  */ 
  template<class valType> 
  string makeStaticBox( string label, string name, valType value, 
                        string size, string length = "")
  {
    ocString val;val.append(value);
    ocString ret = genericBox( "static",  label,  name,
                                val, "",  size,   length );
    return ret;
  }
   

  string makeDateBox( string label, string name, string value, string size)
  {
    ocString val;val.append(value);
    ocString ret = genericBox( "date",  label,  name,
                                val, " class='dateChoice' ",  size,   size );
    return ret;
  }
  
  string makeDateBox( string label, string name, oc_date dateValue, string size)
  {
    ocString val;
    ocString ret;
    ret.append( dateValue.year() );
    if( dateValue.year() < 3000 )  val.append(dateValue);
    ret = genericBox( "date",  label,  name,
                                val, " class='dateChoice' ",  size,   size );
    return ret;
  }
  
  /* 
    Template method for making a time aware text box.
  */ 
  template<class valType> 
  string makeTimeBox( string label, string name, valType value, string size)
  {
    ocString val;val.append(value);
    ocString ret = genericBox( "time",  label,  name,
                                val, " class='timeChoice' ",  size,   size );
    return ret;
  }  
  /* 
    Template method for making a long number aware text box.
  */ 
  template<class valType> 
  string makeNumberBox( string label, string name, valType value, string size)
  {
    ocString val; val.append(value);
    
    // add comma's here (ported javascript from forms.js)
    int i;
    int len = val.length();
    string rev;
    for(i=1; i<=len; i++)
    {
      rev += val[len-i];
      if( i%3 == 0 ) rev +=",";     
    }    
    val="";
    len =rev.length();
    for(i=1; i<=len; i++)
    {
      val += rev[len-i];               
    }
    if(val[0]==',') val = val.replace(",",""); // gets the first one

    ocString ret = genericBox( "longNumber",  label,  name,
                                val, "",  size,   size );
    return ret;
  } 
  template<class valType>
  string makeShortBox( string label, string name, valType value, string size)
  {
    ocString val; val.append(value);
    // no comma processing
    ocString ret = genericBox( "shortNumber",  label,  name,
                                val, "",  size,   size );
    return ret;
  }
 
  /* 
    methods for making a email, Password and Zip Code aware text boxes.
  */    
  string makeEmailBox( string label, string name, string value, 
                       string size, string length = "");
 
  string makePasswordBox( string label, string name, 
                          string value, string size);
   
  string makeZipBox( string label, string name, string value, string size);
  string makePhoneBox( string label, string name, string value, string size);  
  string makeImageBox( string label, string name, string value, string size, string max);

  // For the file group with the popup file manager 
  string makeFilePickBox( string label, string name, string value, string size, string maxlen);

  // stand alone file control. make sure the form is set to multipart mime.
  string makeFileBox( string label, string name, string value, string size, string maxlen);

  string makeMetaBox( string label, string name, string value, string size, 
                      string metaFile = "", string context = "");                                                                   
  string makeTextArea( string label, string name, 
                       string value, string rows="12", string cols="80", string altTemplate = ""); 
  string makeStateBox( string label, string name, string value);                     
  string makeBoolBox( string label, string name, bool value );
  string genericChoiceBox(
                       string opening, string closing, string option,
                       string label, string name, ocString selectedValue,
                       ocString labelValuePair, ocString defaulted,
                       string selMarker );
  /* 
    Template method for making a date aware text box.
  */ 
  string makeDateGroup( string label, string name, time_date &  value, int yearStart = 2004, int yearEnd = 2020);
  /* 
    Returns the mode 
      (i,d,u,s,r,n) := 
      (insert,delete,update,show,reset,new)         
  */    
  string & getMode( void )
  {
    return mode;
  }
  string & keyName( void )
  {
    return m_keyName;
  }

  template<class baseType>
  void inner_forms_base::setKey ( baseType & base )
  {   
    if( getMode() != "r" || getMode() != "n" )
    {
      string keyName = this->keyName();
      if( keyName.length() &&  Script().ClientArguments().count( keyName.c_str() ) )
      {
        base.key(atoll(Script().ClientArguments()[keyName.c_str()].c_str()));
      }
      else if( Script().ClientArguments().count("Id") )
      {
        base.key(atoll(Script().ClientArguments()["Id"].c_str()));
      }
      else if( Script().ClientArguments().count("id") )
      {
        base.key(atoll(Script().ClientArguments()["id"].c_str()));
      }
    }
    if( !base.key())
    {  // If you still don't have a key, see if this is a post,
      //  if so try to set the key from the 'key' session cookie
      // (the key should be set by some browser Javascript)
      if( Script().RequestMethod().upper() == "POST" )
      {
        cgiCookie cookie;
        string keyForCookie = m_formName +  persistingKey;
        base.key( atoll(cookie.get( keyForCookie.c_str() ).c_str()) );
      }
    }
  }
}; // END OF forms_base CLASS


#endif

