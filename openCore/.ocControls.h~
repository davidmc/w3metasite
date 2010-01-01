#ifndef OC_CONTROLS_H
#define OC_CONTROLS_H
#include <vector>
#include "ocString.h"

class ocBase
{
protected:
  string content;
  string tag;
  string attr;
  bool container;
  ocBase * next;

public:
  ocBase():container(true),next(NULL){;}
  ocBase(string inTag, string attrIn = ""):container(true),next(NULL)
  {
    tag = inTag;
    attr = attrIn;
  } 

  // 5-12=01 new for combined group of controls for things like date and time and radio buttons etc.
  ocBase * addNext(ocBase * newnext)
  {    
    ocBase ** next = &(this->next);
    while( *next )
    { 
      next = &((*next)->next);
    }
    
    *next = newnext;  
    return this;
  }
  ocBase * getNext( void )
  {
    return next;
  }
  virtual ~ocBase(){
    erase(next);
  }
  // recursively eliminate links
  void erase(ocBase *toErase)
  {
    if( toErase )
    {
      if( toErase->next ) toErase->erase(toErase->next);
      delete next;
      next=NULL;
    }
  }
  // end 5-12=01 new
  string getContent( void ) { return content; }
  ocBase & setContent( string inContent )
  {
    content = inContent;
    return *this;
  }
  virtual string getHtml( void )
  {
    string result;
    if( tag.length() > 0 )
    {
      result += "<";
      result += tag;
      if( attr.length() > 0 )
      {
        result += " ";
        result += attr;
      }
      if( container ) result += ">";
      else  result += " />";
    }
    result += content;
    if( tag.length() > 0 && container )
    {
      result += "</";
      result += tag;
      result += ">";
    }
    return result;
  }
};

class ocTextbox: public ocBase
{
public:
  ocTextbox ( string name,
              string size = "",
              string max = "",
              string attrIn = "" ):ocBase()
  {
    tag = "input type='text'";
    if ( name.length() )
    {
      attr += " name='";
      attr += name;
      attr += "'";
    }
    if ( size.length() )
    {
      attr += " size='" ;
      attr += size ;
      attr += "'";
    }
    if ( max.length() )
    {
      attr += " maxlength='";
      attr += max ;
      attr += "'";
    }
    if ( attrIn.length() )
    {
      attr += " " ;
      attr += attrIn;
    }

    // unary tag - not a container
    container = false;
  }
  virtual ~ocTextbox(){;}
  string getHtml( void )
  {
    string result;
    if( tag.length() > 0 )
    {
      result += "<";
      result += tag;
      if( attr.length() > 0 )
      {
        result += " ";
        result += attr;
      }
      if( content.length() )
      {
        result += " value=\"";
        result += content;
        result += "\"";
      }

      result += " />";
    }
    return result;
  }
};

class ocFilebox: public ocBase
{
public:
  ocFilebox ( string name,
              string size = "",
              string max = "",
              string attrIn = "" ):ocBase()
  {
    tag = "input type='file'";
    if ( name.length() )
    {
      attr += " name='";
      attr += name;
      attr += "'";
    }
    if ( size.length() )
    {
      attr += " size='" ;
      attr += size ;
      attr += "'";
    }
    if ( max.length() )
    {
      attr += " maxlength='";
      attr += max ;
      attr += "'";
    }
    if ( attrIn.length() )
    {
      attr += " " ;
      attr += attrIn;
    }

    // unary tag - not a container
    container = false;
  }
  virtual ~ocFilebox(){;}
  string getHtml( void )
  {
    string result;
    if( tag.length() > 0 )
    {
      result += "<";
      result += tag;
      if( attr.length() > 0 )
      {
        result += " ";
        result += attr;
      }
      if( content.length() )
      {
        result += " value=\"";
        result += content;
        result += "\"";
      }
      result += " />";
    }
    return result;
  }
};

class ocHidden: public ocBase
{
public:
  ocHidden ( string name  ):ocBase()
  {
    tag = "input type='hidden'";
    if ( name.length() )
    {
      attr += " name='";
      attr += name;
      attr += "' id='";
      attr += name;
      attr += "'";
    }
    // unary tag - not a container
    container = false;
  }
  virtual ~ocHidden(){;}
  string getHtml( void )
  {
    string result;
    if( tag.length() > 0 )
    {
      result += "<";
      result += tag;
      if ( attr.length() )
      {
        result += attr;
      }
      if( content.length() )
      {
        result += " value=\"";
        result += content;
        result += "\"";
      }
      result += " />";
    }
    return result;
  }
};

class ocPassword: public ocBase
{
public:
  ocPassword( string name,
              string size = "",
              string max = "",
              string attrIn = "" ):ocBase()
  {
    tag = "input type='password'";
    if ( name.length() )
    {
      attr += " name='";
      attr += name;
      attr += "'";
    }
    if ( size.length() )
    {
      attr += " size='" ;
      attr += size ;
      attr += "'";
    }
    if ( max.length() )
    {
      attr += " maxlength='";
      attr += max ;
      attr += "'";
    }
    if ( attrIn.length() )
    {
      attr += " " ;
      attr += attrIn;
    }

    // unary tag - not a container
    container = false;
  }
  virtual ~ocPassword(){;}
  string getHtml( void )
  {
    string result;
    if( tag.length() > 0 )
    {
      result += "<";
      result += tag;
      if( attr.length() > 0 )
      {
        result += " ";
        result += attr;
      }
      if( content.length() )
      {
        result += " value=\"";
        result += content;
        result += "\"";
      }
      result += " />";
    }
    return result;
  }
};

class ocSelect: public ocBase
{
protected:
  vector< string > option_labels;
  vector< string > option_values;
  map< string, string > option_attrs;

public:
  ocSelect(string attr = ""):ocBase( "select", attr )
  {
    ;
  }
  void addOption( string label, string value, string attr = "" )
  {
     option_labels.push_back( label );
     option_values.push_back( value );
     if( attr.length() )
     {
       option_attrs.insert( make_pair( label, attr ) );
     }
  }
  virtual ~ocSelect(){;}
  string getHtml( void )
  {
    string result;
    result += "<";
    result += tag;
    if( attr.length() > 0 )
    {
      result += " ";
      result += attr;
    }
    result += ">";
    for( int i=0; i<option_labels.size(); i++ )
    {
      result += "<option";
      string & label =  option_labels[i];
      if( option_attrs.find(label) !=  option_attrs.end() )
      {
        result += " " ;
        result += option_attrs[label];
      }
      if( isSelected(i) )
      {
        result += " selected ";
      }
      result += " value=\"" ;
      result += option_values[i];
      result += "\">" ;
      result += label ;
      result += "</option>";
    } // end for each option
    result += "</";
    result += tag;
    result += ">";
    return result;
  }
  bool isSelected( int iOPtion )
  {
    bool bRet = false;
    if( content.length() )
    {
      string & option = option_values[iOPtion];
      ocString ocContent = content;
      while( ocContent.endOfParse() == false )
      {
        string cVal = ocContent.parse("|");
        if( cVal == option )
        {          
          bRet = true;
          break;
        }
      }
    }
    return bRet;
  }
};

/* 
  as an alternative to a single select control
*/
class ocRadio: public ocBase
{
protected:
  vector< string > option_labels;
  vector< string > option_values;
  map< string, string > option_attrs;
  string name;  

public:
  ocRadio(string name, string attr = ""):ocBase( "div", attr ),name(name)
  {
    ;
  }
  void addOption( string label, string value, string attr = "" )
  {
     option_labels.push_back( label );
     option_values.push_back( value );
     if( attr.length() )
     {
       option_attrs.insert( make_pair( label, attr ) );
     }
  }
  virtual ~ocRadio(){;}  
  string getHtml( void )
  {
    string result;
    result += "<";
    result += tag;
    if( attr.length() > 0 )
    {
      result += " ";
      result += attr;
    }
    // all individual checks are grouped in here
    result += " id='";
    result += name;
    result += "'>";
    for( int i=0; i<option_labels.size(); i++ )
    {
      result += "<input type='radio' name='";
      result += name;
      result += "'";
      string & label =  option_labels[i];
      if( option_attrs.find(label) !=  option_attrs.end() )
      {
        result += " " ;
        result += option_attrs[label];
      }
      if( isSelected(i) )
      {
        result += " checked='true' ";
      }
      result += " value=\"" ;
      result += option_values[i];
      result += "\"> " ;
      result += label ;
      result += " &nbsp;";
    } // end for each option
    result += "</";
    result += tag;
    result += ">";
    return result;
  }

  bool isSelected( int iOPtion )
  {
    bool bRet = false;
    if( content.length() )
    {
      string & option = option_values[iOPtion];
      ocString ocContent = content;
      while( ocContent.endOfParse() == false )
      {
        string cVal = ocContent.parse("|");
        if( cVal == option )
        {          
          bRet = true;
          break;
        }
      }
    }
    return bRet;
  }
};

/* 
  as an alternative to a mult-select control
*/
class ocCheckBoxes: public ocBase
{
protected:
  vector< string > option_labels;
  vector< string > option_values;
  map< string, string > option_attrs;
  string name;
public:
  ocCheckBoxes(string name, string attr = ""):ocBase( "div", attr ),name(name)
  {
    ;
  }
  void addOption( string label, string value, string attr = "" )
  {
     option_labels.push_back( label );
     option_values.push_back( value );
     if( attr.length() )
     {
       option_attrs.insert( make_pair( label, attr ) );
     }
  }
  virtual ~ocCheckBoxes(){;}
  string getHtml( void )
  {
    string result;
    result += "<";
    result += tag;
    if( attr.length() > 0 )
    {
      result += " ";
      result += attr;
    }
    result += ">";
    for( int i=0; i<option_labels.size(); i++ )
    {
      result += "<input type='checkbox' name='";
      result += name;
      result += "'";
      string & label =  option_labels[i];
      if( option_attrs.find(label) !=  option_attrs.end() )
      {
        result += " " ;
        result += option_attrs[label];
      }
      if( isSelected(i) )
      {
        result += " checked='true' ";
      }
      result += " value=\"" ;
      result += option_values[i];
      result += "\"> " ;
      result += label ;
      result += " &nbsp; ";
    } // end for each option
    result += "</";
    result += tag;
    result += ">";
    return result;
  }
  bool isSelected( int iOPtion )
  {
    bool bRet = false;
    if( content.length() )
    {
      string & option = option_values[iOPtion];
      ocString ocContent = content;
      while( ocContent.endOfParse() == false )
      {
        string cVal = ocContent.parse("|");
        if( cVal == option )
        {          
          bRet = true;
          break;
        }
      }
    }
    return bRet;
  }
};


class ocTextarea: public ocBase
{

public:
  ocTextarea(string attr = ""):ocBase( "textarea", attr.length()>0?attr:" rows='10' cols='80'" )
  {
    ;
  }
  virtual ~ocTextarea(){;}
};

class ocBoolbox: public ocBase
{
  string svrTrue;
public:
  ocBoolbox( string name, string attrIn = "",
             string usrTrue="true", string svrTrueIn="Yes"):ocBase()
  {
    svrTrue = svrTrueIn;
    tag = "input type='checkbox'";
    if ( name.length() )
    {
      attr += " name='";
      attr += name;
      attr += "'";
    }
    // if it's checked, its true
    attr += " value=\"";
    attr += usrTrue;
    attr += "\" ";

    if ( attrIn.length() )
    {
      attr += " " ;
      attr += attrIn;
    }
    // unary tag - not a container
    container = false;
  }
  virtual ~ocBoolbox(){;}

  virtual string getHtml( void )
  {
    string result;
    if( tag.length() > 0 )
    {
      result += "<";
      result += tag;
      if( attr.length() > 0 )
      {
        result += " ";
        result += attr;
      }

      if( content == svrTrue )
      {
        result += " checked ";
      }
      result += " />";
    }
    // result += content;

    return result;
  }
};

#endif
