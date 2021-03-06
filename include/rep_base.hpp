/*
  rep_base.hpp
  
  ==============  
  Reporting Base Class  
  davidmc@w3sys.com 
  copyright (C) 2005 
  
*/

#ifndef rep_base_HPP
#define rep_base_HPP

#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_base.hpp"
#include "cgiClass.h"
#include "cgiTemplates.h"

using namespace std;


/*
  generic report class

*/


class rep_base: public read_base
{
public:
  cgiTemplates reportParts;
  string head;
  string cachedSQL;
  ostream & webIO;
  bool opened;
  bool more_data;
  long row;
  
  string topKey, endKey, rowKey, endRowKey, headingKey, dataKey;

  /*
     This constructor must be called!
  */
  rep_base(ostream&sc):read_base(),webIO(sc),
  opened(false),more_data(false),row(0),
  topKey("top"), endKey("end"), rowKey("tr"), endRowKey("end_tr") ,headingKey("th"), dataKey("td")
  {
  }
   
  virtual ~rep_base(){;}
  
  // load template for pieces
  bool loadTemplates( string path )
  {
    bool ret = reportParts.load(path.c_str());
    if( ret == false )
    {
      webIO << "<!-- failed to load " << path << "-->";
    }
    return ret;
  }
  
  ocString part( string id )
  {
    string & ret = reportParts.getParagraph(id);
    if( ret.length() == 0 ) webIO << "<!-- failed to find template item " << id << "-->";
    return ret;
  }
  
  rep_base & copyTemplates( const cgiTemplates & in )
  {
    reportParts = in;
    return * this;
  }
   
  virtual bool getData( string sql )
  {
    cachedSQL = sql;
    iGetData(sql);     
    return opened;
  }
  
  virtual bool iGetData( string sql )
  {
    opened = rs.open(sql);
    return opened;
  }   
  
  rep_base & emitTop( string Name )
  {
    ocString list_begin = reportParts.getParagraph(topKey);
    list_begin = list_begin.replace("$name$", Name.c_str() );
    list_begin = list_begin.replace("$title$", Name.c_str() );
    webIO << list_begin;
  }
  rep_base &  emitEmptySet( void )
  {
    int cols = simulateHeader();
    // if we have a traditional TD, it should add a colspan
    ocString Cols = "<td colspan='";
    Cols.append(cols);
    Cols+="' style='text-align: center' ";
    webIO << part(rowKey);
    ocString td = part(dataKey);
    webIO << td.replace( "$data$", "No Data" ).replace("<td",Cols);
    webIO << part(endRowKey);
    webIO << "<!-- " << cachedSQL << "-->" << endl;
  }
  int simulateHeader( void )
  {
    int cols = 0;
    webIO << part(rowKey);
    ocString chop = cachedSQL;
    chop.parse("select");
    chop = chop.parse("from");
    chop.parseInit();
    chop.parse(" as ");
    ocString col = chop.parse(",");
    while ( col.length() && !chop.endOfParse() && cols < 100 )
    {
      cols++;
      ocString tst = col;
      string colName;
      colName = tst.tokenParse("'");
      ocString th = part(headingKey);
      webIO << th.replace( "$data$", colName.c_str() );
      chop.parse(" as ");
      col = chop.parse(",");
    }
    webIO << part(endRowKey);
    return cols;
  }
  rep_base & emitHeadings( string pipedelimited = "" )
  {
    if( head.length() == 0 )
    {
      ocString headings;
      if( pipedelimited.length() )
      {
        headings = pipedelimited;    
      }
      else if ( headings.length() )
      {
        headings.parseInit();
      }
      head =  part(rowKey);
      ocString th = part(headingKey);
      while( ! headings.endOfParse() )
      {
        string heading = headings.parse("|");
        head += th.replace( "$data$", heading.c_str() );
      } 

      head += part(endRowKey);      
    }
    webIO << head;
    return * this;
  }

  rep_base & emitEnd( string extra = "" )
  {    
    ocString list_end = part(endKey);
    webIO << list_end.replace("<!--extra-->",extra);
    return * this;  
  }
  
  void setColFormat( string fmt, int pos )
  {
    if( pos < rs.getFieldCount() )
    {
      rs.getField(pos).setFormatMask(fmt);
    }
  }
  
  rep_base & emitHeader( void )
  {
    if( head.length() == 0 )
    {
      int fieldCount;
      ocString th = part(headingKey);
      //if( opened && th.length() )
      if( th.length() )
      {
        fieldCount = rs.getFieldCount();
        head = part(rowKey);
        for( int i=0; i < fieldCount; ++i )
        {
          head += getHeadCell(i, th );         
        }       
        head +=part(endRowKey); 
      }
    }
    webIO << head;
    return * this;
  }
    
  // To be able to override each field
  virtual string getHeadCell( int iField, ocString & th )
  {
    return th.replace( "$data$", rs.getField(iField).getName().c_str() ); 
  }  

  
  rep_base & emitData( void )
  {
    more_data = opened;
    int fieldCount;
    if( opened ) fieldCount = rs.getFieldCount();
    derived_commence_event();
    while( more_data )
    {
      derived_prerow_event();
      webIO << part(rowKey);
      ocString td = part(dataKey);
      for( int i=0; i < fieldCount; ++i )
      {
         sendField(i, td );         
      }       
      webIO << part(endRowKey);
      derived_postrow_event(); 
      more_data = rs.next();
      if( more_data ) row++;
    }
    derived_complete_event();
    return * this;
  }
  
  // To be able to override each field
  virtual void sendField( int iField, ocString & td )
  {
    webIO << part(dataKey).replace( "$data$", rs.getField(iField).format().c_str() ); 
  }  
    
  // events
  virtual void derived_commence_event( void )
  {
    ;
  }
  virtual void derived_prerow_event( void )
  {
    ;
  }
  virtual void derived_postrow_event( void )
  {
    ;
  }
  virtual void derived_complete_event( void )
  {
    ;
  }

};




#endif
