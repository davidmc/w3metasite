/*
  sublist_base.hpp
  
  Non-caching list
  
*/
#ifndef SUBsublist_base_HPP
#define SUBsublist_base_HPP

#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_base.hpp"
#include "cgiClass.h"
#include "cgiTemplates.h"

#define DEBUGOUT


class sublist_base: public read_base
{
protected:
  cgiTemplates listTemplate;
  ostream & webIO;
  ocString td; 

public:
  bool opened;
  /// an edit link in the form of "<a target="_self" href='$prog$?Id=$key$'>$col$</a>"
  ocString editLink;
  /// -1 means no hot columns, -2 means all hot columns other-wise, the hot column is the ordinal final pos, MUST set edit link
  int hotCol;
  int skipCol;
  string addedCriteria;
  
  
  // for filtering, sorting, and page;
  string cookiePrefix;  
  int  recCount, colCount;
 

  /*
     This constructor must be called!
     Call it before closing the script header!
  */   
  sublist_base(ostream&sc):read_base(),webIO(sc),opened(false),hotCol(0),skipCol(-1)
  ,recCount(0), colCount(0)  
  {

  }
  
  virtual ~sublist_base()
  {
  }
  
  // load template for forms controls
  bool loadListTemplates( string path )
  {
    return listTemplate.load(path.c_str());
  }
   
  sublist_base & copyListTemplates( const cgiTemplates & in )
  {
    listTemplate = in;
    return * this;
  }
   
  bool getData( string sql )
  {
    opened = rs.open(sql);
    if( opened )  recCount = rs.getRecordCount();  
    return opened;
  }
  
  
  sublist_base & emitHeadings( string pipedelimited, string title="" )
  {
    ocString headings(pipedelimited);
    ocString list_begin = listTemplate.getParagraph("list_begin");
    string tr = listTemplate.getParagraph("tr");
    ocString th = listTemplate.getParagraph("th");
    string end_tr = listTemplate.getParagraph("end_tr");
    webIO << list_begin.replace("$heading$",title) << tr;
    while( ! headings.endOfParse() )
    {
      colCount++;
      string heading = headings.parse("|");
      webIO << th.replace( "$data$", heading.c_str() );
    }    
    webIO << end_tr;
    return * this;
  }
  
  sublist_base & emitEnd( void )
  {    
    ocString list_end = listTemplate.getParagraph("list_end");   
    webIO << list_end; 
    return * this;  
  }
  
  void setColFormat( string fmt, int pos )
  {
    if( pos < rs.getFieldCount() )
    {
      rs.getField(pos).setFormatMask(fmt);
    }
  }
  
  sublist_base & emitData( void )
  {
    bool more_data = opened;
    string tr = listTemplate.getParagraph("tr");
    td = listTemplate.getParagraph("td");
    string end_tr = listTemplate.getParagraph("end_tr");
    int fieldCount;
    if( opened ) fieldCount = rs.getFieldCount();
    derived_commence_event();
    while( more_data )
    {
      if(derived_predata_event())
      {
        webIO << tr;
        for( int i=0; i < fieldCount; ++i )
        {
          if( i != skipCol && (i == hotCol || hotCol == -2) )
          {
            sendHotField(i, td );  
          }
          else if( i != skipCol )
          {
            sendField(i, td ); 
          }  
        }       
        webIO << end_tr;
      }
      derived_data_event(); 
      more_data = rs.next();  
    }
    derived_complete_event();
    return * this;
  }
  
  // To be able to override each field
  virtual void sendField( int iField, ocString & td )
  {
    webIO << td.replace( "$data$", rs.getField(iField).format().c_str() ); 
  }
  
  // to be able to override the hot column (the one with the link.)
  virtual void sendHotField( int iField, ocString & td )
  {
    webIO <<  td.replace( "$data$",
                    editLink.replaceAll( "$key$",rs.getField(0).format().c_str())
                            .replaceAll( "$col$",rs.getField(iField).format().c_str()).c_str());
  }
    
  // events
  virtual void derived_commence_event( void ){;}
  virtual bool derived_predata_event( void ){return true; }
  virtual void derived_data_event( void ){;}
  virtual void derived_complete_event( void ){;}
};

#endif
