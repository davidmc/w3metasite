
                                           /*
  cgiExtend.cpp

  Common Gateway Interface Class Implementation.
     Extended classes for cgi support
     -- generic tag container
  copyright (c) 2001

  David McCombs davidmc@newcottage.com

  Nuclear Core Class Library

  freely re-usable, redistributable.

*/
#include "cgiExtend.h"

/*
  A generic html tag container
*/


cgiCan::cgiCan( const char * name, const char * attr = NULL)
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

void cgiCan::open()
{
  if( isClosed )
  {
    *this << cgiBase::opening << endl;
    flush();
    isClosed = false;
  }
}
void cgiCan::close()
{
  if( !isClosed )
  {
    *this << cgiBase::close << endl;
    flush();
    isClosed = true;
  }
}
cgiCan::~cgiCan()
{
  if( !isClosed )
  {
    *this << cgiBase::close << endl;
    flush();
  }
}



cgiTextbox::cgiTextbox( const char * name,
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
cgiTextbox::~cgiTextbox(){;}



cgiPassword::cgiPassword( const char * name,
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
cgiPassword::~cgiPassword(){;}



cgiSelect::cgiSelect(const char * attr = NULL):cgiCan( "select", attr )
{
  ;
}
void cgiSelect::setSelected( const char * selected )
{
  m_selected = selected;
}
void cgiSelect::addOption( const char * label, const char * value, bool selected=false, const char * attr = NULL )
{
  *this << "<option";
  if( attr ) *this << " " << attr;
  if( (m_selected == value) || selected ) *this << " selected";
  *this << " value='" << value << "'>" << label << "</option>";
}
cgiSelect::~cgiSelect(){;}


cgiTextarea::cgiTextarea(const char * attr = NULL):cgiCan( "textarea", attr?attr:" rows='10' cols='80'" )
{
  ;
}
cgiTextarea::~cgiTextarea(){;}
