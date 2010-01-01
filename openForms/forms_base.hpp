/*

forms_base.hpp
Forms Base Definition

*/
#ifndef Forms_Base_Hpp
#define Forms_Base_Hpp

#include "cgiClass.h"
#include "cgiTemplates.h"
#include "cgiCookie.h"
#include "forms.h"

string persistingKey = "key";

/* 
  This allows you to alter the contents of a template variable,
  Change is 'undone' when destructor is called.
*/
class form_permute 
{
  string back;
  string & orig;
  private: form_permute():orig(back){;}
  public: form_permute( string & permuted, string newVal ):back(permuted),orig(permuted){ permuted=newVal; }
          ~form_permute(){ orig = back; }
};

class forms_base 
{
protected:    
  cgiTemplates formTemplate;
  changeMap changes;
  string mode;
  string m_warn;
  string m_keyName;
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
  forms_base(cgiScript & in);
  virtual ~forms_base();
  
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
  bool loadControlTemplates( string path );
  cgiTemplates & getTemplates( void ) { return formTemplate; }
  
  virtual string makeTop(string uri, string name );
  virtual string makeBottom(string status  );
  
  virtual string makeButtons( long long key,
                              string iLabel="&nbsp;Insert&nbsp;",
                              string uLabel="&nbsp;Update&nbsp;", 
                              string dLabel="&nbsp;Delete&nbsp;" );  
  
  /* 
    Template method for making a combo box.
  */
  template<class valType> 
  string makeComboBox( string label, string name, valType value, 
                       string sql = "", string defaulted = "", 
                       string attributes = "")
  {
    ocString ret = formTemplate.getParagraph("control_group");
    ocString val;val.append(value);
    ocSelect * pCombo = cmboEntry( name, sql, defaulted, attributes );    
    if( pCombo )
    {
      pCombo->setContent(val);      
      ret = ret.replace("$label$",label.c_str()).replace("$name$",name.c_str())
               .replace("$control$", pCombo->getHtml().c_str());
      delete pCombo;
    }
    return ret;
  }
  
  template<class valType> 
  string makeManualComboBox( string label, string name, valType value, 
                       ocString labelValuePair = "", string defaulted = "", 
                       string attributes = "")
  {
    ocString ret = formTemplate.getParagraph("control_group");
    ocString val;val.append(value);
    ocSelect * pCombo = cmboManualEntry( name, labelValuePair, defaulted, attributes );    
    if( pCombo )
    {
      pCombo->setContent(val);      
      ret = ret.replace("$label$",label.c_str()).replace("$name$",name.c_str())
               .replace("$control$", pCombo->getHtml().c_str());
      delete pCombo;
    }
    return ret;
  }
  template<class valType>
  string makeCheckBoxes( string label, string name, valType value,
                         ocString labelValuePair = "", string defaulted = "",
                         string attributes = "")
  {
    ocString ret = formTemplate.getParagraph("control_group");
    ocString val;val.append(value);

    ocCheckBoxes * pChecks  = checksEntry( name, labelValuePair, defaulted, attributes );
    if( pChecks )
    {
      pChecks->setContent(val);
      ret = ret.replace("$label$",label.c_str()).replace("$name$",name.c_str())
               .replace("$control$", pChecks->getHtml().c_str());
      delete pChecks;
    }
    return ret;
  }
  template<class valType>
  string makeRadios( string label, string name, valType value,
                     ocString labelValuePair = "", string defaulted = "",
                     string attributes = "")
  {
    ocString ret = formTemplate.getParagraph("control_group");
    ocString val;val.append(value);

    ocRadio * pRadios  = radioEntry( name, labelValuePair, defaulted, attributes );
    if( pRadios )
    {
      pRadios->setContent(val);
      ret = ret.replace("$label$",label.c_str()).replace("$name$",name.c_str())
          .replace("$control$", pRadios->getHtml().c_str());
      delete pRadios;
    }
    return ret;
  }
  template<class valType> 
  string makeManualComboBox( string label, string name, valType value, 
                       const char ** Values, size_t val_count, string defaulted = "", 
                       string attributes = "")
  {
    ocString ret = formTemplate.getParagraph("control_group");
    ocString val;val.append(value);
    ocSelect * pCombo = cmboManualEntry( name, "", defaulted, attributes );
      
    if( pCombo )
    {
      for( int i=0;i<val_count;++i)
      {
        ocString sid="";
        sid.append(i);
        pCombo->addOption( Values[i], sid);
      }
      pCombo->setContent(val);      
      ret = ret.replace("$label$",label.c_str()).replace("$name$",name.c_str())
               .replace("$control$", pCombo->getHtml().c_str());
      delete pCombo;
    }
    return ret;
  }
  /* 
    Template method for making a generic event text box.
  */                      
  template<class valType> 
  string makeGenericBox( string label, string name, valType value, 
                         string attr, string size, string length = "")
  {
    ocString ret = formTemplate.getParagraph("control_group");
    ocString val;val.append(value);
    ocTextbox * pText = textEntry( name, size, length, attr );
    if( pText )
    {
      pText->setContent(val);      
      ret = ret.replace("$label$",label.c_str()).replace("$name$",name.c_str())
               .replace("$control$", pText->getHtml().c_str());
      delete pText;
    }
    return ret;
  }  
  /* 
    Template method for making a text box.
  */                      
  template<class valType> 
  string makeTextBox( string label, string name, valType value, 
                      string size, string length = "")
  {
    ocString ret = formTemplate.getParagraph("control_group");
    ocString val;val.append(value);
    ocTextbox * pText = textEntry( name, size, length );
    if( pText )
    {
      pText->setContent(val);      
      ret = ret.replace("$label$",label.c_str()).replace("$name$",name.c_str())
               .replace("$control$", pText->getHtml().c_str());
      delete pText;
    }
    return ret;
  }
  /* 
    Template method for making a hidden text box.
  */ 
  template<class valType> 
  string makeHiddenBox( string name, valType value )
  {
    ocString ret;
    ocString val;val.append(value);
    ocHidden * pText = hiddenEntry(  name );    
    if( pText )
    {
      pText->setContent(val);      
      ret = pText->getHtml();
      delete pText;
    }
    return ret;
  }  
  /* 
    Template method for making a static text box.
  */ 
  template<class valType> 
  string makeStaticBox( string label, string name, valType value, 
                        string size, string length = "")
  {
    ocString ret = formTemplate.getParagraph("control_group");
    ocString val;val.append(value);
    ocTextbox * pText = staticEntry(  name,  size, length );    
    if( pText )
    {
      pText->setContent(val);      
      ret = ret.replace("$label$",label.c_str()).replace("$name$",name.c_str())
               .replace("$control$", pText->getHtml().c_str());
      delete pText;
    }
    return ret;
  }
   
  /* 
    Template method for making a date aware text box.
  */ 
  template<class valType> 
  string makeDateBox( string label, string name, valType value, string size)
  {
    ocString ret = formTemplate.getParagraph("control_group");
    ocString val; val.append(value);
    ocTextbox * pText = dateEntry(  name, size );    
    if( pText )
    {
      pText->setContent(val);      
      ret = ret.replace("$label$",label.c_str()).replace("$name$",name.c_str())
               .replace("$control$", pText->getHtml().c_str());
      delete pText;
    }
    return ret;
  }
  
  /* 
    Template method for making a time aware text box.
  */ 
  template<class valType> 
  string makeTimeBox( string label, string name, valType value, string size)
  {
    ocString ret = formTemplate.getParagraph("control_group");
    ocString val; val.append(value);
    ocTextbox * pText = timeEntry( name, size );    
    if( pText )
    {
      pText->setContent(val);      
      ret = ret.replace("$label$",label.c_str()).replace("$name$",name.c_str())
        .replace("$control$", pText->getHtml().c_str());
      delete pText;
    }
    return ret;
  }  
  /* 
    Template method for making a long number aware text box.
  */ 
  template<class valType> 
  string makeNumberBox( string label, string name, valType value, string size)
  {
    ocString ret = formTemplate.getParagraph("control_group");
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

    ocTextbox * pText = longNumberEntry(  name, size );    
    if( pText )
    {
      pText->setContent(val);      
      ret = ret.replace("$label$",label.c_str()).replace("$name$",name.c_str())
               .replace("$control$", pText->getHtml().c_str());
      delete pText;
    }
    return ret;
  } 
  template<class valType>
  string makeShortBox( string label, string name, valType value, string size)
  {
    ocString ret = formTemplate.getParagraph("control_group");
    ocString val; val.append(value);

   // no comma processing

    ocTextbox * pText = shortNumberEntry(  name, size );    
    if( pText )
    {
      pText->setContent(val);      
      ret = ret.replace("$label$",label.c_str()).replace("$name$",name.c_str())
               .replace("$control$", pText->getHtml().c_str());
      delete pText;
    }
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
  string makeImageBox( string label, string name, string value, string size);

  // For the file group with the popup file manager 
  string makeFileBox( string label, string name, string value, string size);

  // overloaded for a stand alone file control. make sure the form is set to multipart mime.
  string makeFileBox( string label, string name, string value, string size, string maxlen, string attr);

  string makeMetaBox( string label, string name, string value, string size, 
                      string metaFile = "", string context = "");                                                                   
  string makeTextArea( string label, string name, 
                       string value, string rows="12", string cols="80", string altTemplate = ""); 
  string makeStateBox( string label, string name, string value);                     
  string makeBoolBox( string label, string name, bool value );                                        
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
}; // END OF forms_base CLASS



// For setting db key from form var, or query string.
// Id or id is usually considered key.
template <class db_form_object> 
void setKey ( db_form_object & dbfo )
{   
  if( dbfo.getMode() != "r" ||   dbfo.getMode() != "n" )
  {
    string keyName = dbfo. keyName();
    if( keyName.length() &&  dbfo.Script().ClientArguments().count( keyName.c_str() ) )
    {
      dbfo.key(atoll(dbfo.Script().ClientArguments()[keyName.c_str()].c_str()));
    }
    else if( dbfo.Script().ClientArguments().count("Id") )
    {
      dbfo.key(atoll(dbfo.Script().ClientArguments()["Id"].c_str()));
    }
    else if( dbfo.Script().ClientArguments().count("id") )
    {
      dbfo.key(atoll(dbfo.Script().ClientArguments()["id"].c_str()));
    }
  }
  if(!dbfo.key())
  {  // If you still don't have a key, see if this is a post, 
     //  if so try to set the key from the 'key' session cookie 
     // (the key should be set by some browser Javascript)
    if( dbfo.Script().RequestMethod().upper() == "POST" )
    {
      cgiCookie cookie;
      dbfo.key( atoll(cookie.get( persistingKey.c_str() ).c_str()) );
    }  
  }   
}

// This template function is used in the unusual case for the key.
template <class db_form_object> 
void setKey ( db_form_object & dbfo, const char * keyName )
{   
  if( dbfo.getMode() != "r" ||   dbfo.getMode() != "n" )
  {
    if( dbfo.Script().ClientArguments().count(keyName) )
    {
      dbfo.key(atoll(dbfo.Script().ClientArguments()[keyName].c_str()));
    }    
  }
  if(!dbfo.key())
  {  // If you still don't have a key, see if this is a post, 
     //  if so try to set the key from the 'key' session cookie 
     // (the key should be set by some browser Javascript)
    if( dbfo.Script().RequestMethod().upper() == "POST" )
    {
      cgiCookie cookie;
      dbfo.key( atoll(cookie.get(persistingKey.c_str()).c_str()) );
    }  
  }   
}
#endif

