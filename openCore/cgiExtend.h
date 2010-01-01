/*
  cgiExtend.h

  Common Gateway Interface Class Definitions.
     Extended classes for cgi support
     -- generic tag container
  copyright (c) 2001

  David McCombs davidmc@newcottage.com

  Nuclear Core Class Library

  freely re-usable, redistributable.

*/
#ifndef cgiEXTEND_H
#define cgiEXTEND_H
#include "cgiClass.h"

/*
  A generic html tag container
*/

class cgiCan : public cgiBase
{
protected:
  bool isClosed;
public:

  cgiCan( const char * name, const char * attr = NULL)
  {
    isClosed = false;
    id = generic;
    opening = "<";
    opening += name;
    if( attr && strlen(attr) )
    {
      opening += " ";
      opening += attr;
    }
    opening += ">";
    cgiBase::close = "</";
    cgiBase::close += name;
    cgiBase::close += ">";
    *this << opening;
  }

  virtual cgiCan & open()
  {
    if( isClosed )
    {
      *this << cgiBase::opening << endl;
      flush();
      isClosed = false;
    }
    return *this;
  }
  virtual cgiCan & close()
  {
    if( !isClosed )
    {
      *this << cgiBase::close << endl;
      flush();
      isClosed = true;
    }
    return *this;
  }
  virtual ~cgiCan()
  {
    if( !isClosed )
    {
      *this << cgiBase::close << endl;
      flush();
    }
  }
  cgiCan & noClose( void )
  {
    isClosed = true;
    return *this;
  }
};

class cgiTextbox: public cgiBase
{
public:
  cgiTextbox( const char * name,
              const char * value = NULL,
              const char * size = NULL,
              const char * max = NULL,
              const char * attr = NULL )
  {
    id = generic;

    *this << "<input type='text'";
    if ( name && strlen(name) )
    {
      *this << " name='" << name << "'";
    }
    if ( value && strlen(value) )
    {
      *this << " value=\"" << value << "\"";
    }
    if ( size && strlen(size) )
    {
      *this << " size='" << size << "'";
    }
    if ( max && strlen(max) )
    {
      *this << " maxlength='" << max << "'";
    }
    if ( attr && strlen(attr) )
    {
      *this << " " << attr;
    }
    *this << ">";
    flush();
  }

  // unary tag - not a container
  virtual ~cgiTextbox(){;}
};

class cgiPassword: public cgiBase
{
public:
  cgiPassword( const char * name,
              const char * value = NULL,
              const char * size = NULL,
              const char * max = NULL,
              const char * attr = NULL )
  {
    id = generic;

    *this << "<input type='password'";
    if ( name && strlen(name) )
    {
      *this << " name='" << name << "'";
    }
    if ( value && strlen(value) )
    {
      *this << " value=\"" << value << "\"";
    }
    if ( size && strlen(size) )
    {
      *this << " size='" << size << "'";
    }
    if ( max && strlen(max) )
    {
      *this << " maxlength='" << max << "'";
    }
    if ( attr && strlen(attr) )
    {
      *this << " " << attr;
    }
    *this << ">";
    flush();
  }
  // unary tag - not a container
  virtual ~cgiPassword(){;}
};

class cgiSelect: public cgiCan
{
protected:
  string m_selected;
public:
  cgiSelect(const char * attr = NULL):cgiCan( "select", attr )
  {
    ;
  }
  void setSelected( const char * selected )
  {
    m_selected = selected;
  }
  void addOption( const char * label, const char * value, bool selected=false, const char * attr = NULL )
  {
    *this << "<option";
    if( attr ) *this << " " << attr;
    if( (m_selected == value) || selected ) *this << " selected";
    *this << " value='" << value << "'>" << label << "</option>";
  }
  ~cgiSelect(){;}
};

class cgiTextarea: public cgiCan
{

public:
  cgiTextarea(const char * attr = NULL):cgiCan( "textarea", attr?attr:" rows='10' cols='80'" )
  {
    ;
  }
  ~cgiTextarea(){;}
};

class cgitr
{
private:
  cgiCan & m_parent;
  cgitr & t( char type, const char * text, const char * attr = NULL )
  {
    m_parent << "<t" << type;
    if( attr && strlen(attr) )
    {
      m_parent << " " << attr;
    }
    m_parent << ">";
    if( text && strlen(text) )
    {
      m_parent << text;
    }
    m_parent << "</t" << type << ">";
    return *this;
  }
public:
  cgitr( cgiCan & parent ): m_parent(parent){;}
  ~cgitr(){;}

  // char * overloads
  cgitr & td( const char * text, const char * attr = NULL )
  {
    return t( 'd', text, attr );
  }
  cgitr & th( const char * text, const char * attr = NULL )
  {
    return t( 'h', text, attr );
  }
  // string overloads
  cgitr & td( const string & text )
  {
    return t( 'd', text.c_str() );
  }
  cgitr & th( const string & text )
  {
    return t( 'h', text.c_str() );
  }
  // string with attribute overloads
  cgitr & td( const string & text, const string & attr )
  {
    return t( 'd', text.c_str(), attr.c_str() );
  }
  cgitr & th( const string & text, const string & attr )
  {
    return t( 'h', text.c_str(), attr.c_str() );
  }
  void begin(const char * attr = "")
  {
    m_parent << "<tr";
    if( attr && strlen(attr) )
    {
      m_parent << " " << attr;
    }
    m_parent << ">";
  }
  void end()
  {
    m_parent << "</tr>" << endl;
  }
};

class cgiTable: public cgiCan
{
private:
   cgitr m_tr;

public:
  cgiTable (const char * attr = ""):cgiCan( "table", attr ),m_tr(*this){;}
  cgitr & tr( const char * attr = "" )
  {
    m_tr.begin(attr);
    return m_tr;
  }
};

#endif
