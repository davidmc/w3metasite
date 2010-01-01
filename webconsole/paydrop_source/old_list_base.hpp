#ifndef LIST_BASE_HPP
#define LIST_BASE_HPP

#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_base.hpp"
#include "cgiClass.h"
#include "cgiTemplates.h"
#include "cgiCookie.h"

class list_base: public read_base
{
protected:
  cgiTemplates listTemplate;
  cgiScript & webIO;
  cgiCookie cookie;
public:
  bool opened;
  ocString editLink;
  int hotCol;
  
  // for filtering, sorting, and page;
  string cookiePrefix;
  string filter, sort, direction; 
  int  listPg, itemsPerPg, recCount, colCount, pgCount, dsetCount;
  
  void setPage( cgiInput & args )
  {
    listPg = atol(cookie.get("lPg").c_str()); 
    if (args.count("Pg") )
    {
      listPg = (atol(args["Pg"].c_str())-1);      
    }
    else if(args.count("navigate") )
    {
      if( args["navigate"] == "prev" ) listPg--;
      if( args["navigate"] == "next" ) listPg++;
    }
    if( listPg < 0 ) listPg = 0;    
  }
  string limitClause( void )
  {
    int rowStart = listPg * itemsPerPg;
    ocString limit = " limit ";
    limit.append(rowStart);
    limit += ", ";
    limit.append(itemsPerPg);
    return limit;
  }
  void setSort( cgiInput & args )
  {
    direction = cookie.get("lDir");
    sort = cookie.get("lSort");
    // has the order been specified?
    if (args.count("sort") )
    { 
      if(  sort == args["sort"].c_str() )
      {
        // reverse direction
        direction = direction=="desc"?"asc":"desc";
        cookie.set("lDir",direction.c_str());
      }  
      sort = args["sort"].c_str();
      cookie.set("lSort",sort.c_str());
    }
  }  
  void setFilter( cgiInput & args )
  {
     if( args.count("filter") )
    { 
      filter = args["filter"].c_str();
      if( filter == " " ) filter = "";
      cookie.set("lFilter",filter.c_str());
      // kill all the other list cookies!
      cookie.set("lDir","");
      cookie.set("lSort","");
      cookie.set("lPg","");
    } 
  }
  /*
     This constructor must be called!
     Call it before closing the script header!
  */   
  list_base(cgiScript&sc):read_base(),webIO(sc),opened(false),hotCol(0)
  ,listPg(0), itemsPerPg(12), recCount(0), colCount(0)
  ,pgCount(0), dsetCount(0)
  {
    cgiInput & args = sc.ClientArguments();        
    filter = cookie.get("lFilter"); 
    // set and cache the current sort order  
    setSort( args ); 
    // set and cache the current filter
    setFilter( args );  
    // set and cache the current page of data 
    setPage( args );         
  }
  
  virtual ~list_base(){;}
  // load template for forms controls
  bool loadListTemplates( string path )
  {
    return listTemplate.load(path.c_str());
  } 
  list_base & copyListTemplates( const cgiTemplates & in )
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
  string whereClause( ocString & filterTemplate )
  { 
    string sql = "";
    if( filter.length() )
    {
      sql += " where ";
      sql += filterTemplate.replace("$filter$", filter.c_str() );
    }
    return sql;
  }
  void maxPageFixup( void )
  {
    pgCount = (recCount/itemsPerPg)+1;
    if( listPg >= pgCount ) listPg = pgCount-1;       
  }
  bool getFilteredData( string rows, 
                        string table,
                        ocString filterTemplate, 
                        string defaultSort = "",
                        string groupBy = "",
                        string countTable = "" ) // for cases where grouping limits the real count.
  {
  
    string sql = "select count(*) from ";
    sql += countTable.length()?countTable:table;    
    sql += whereClause( filterTemplate );
    webIO << "<!--" << sql << "-->";
    
    opened = rs.open(sql);
    if( opened )
    {
       recCount = atol(rs.getField(0).format().c_str());
       maxPageFixup();
       rs.close();
    } 
     
    sql = "select ";
    sql += rows;
    sql += " from ";
    sql +=  table;
    sql += whereClause( filterTemplate ); 
    if( groupBy.length() )
    {
      sql += " group by ";
      sql += groupBy;
    }   
    if( sort.length() && rows.find(sort) != string::npos)
    {
      sql += " order by ";
      sql += sort;
      sql += " ";
      sql += direction;
    }
    else if ( defaultSort.length() )
    {
      sql += " order by ";
      sql += defaultSort;
    }
    sql += limitClause();
    webIO << "<!--" << sql << "-->";
    opened = rs.open(sql);    
    if( opened )
    {
      dsetCount = rs.getRecordCount();      
    }
    return opened;
  }
  
  
  list_base & emitFilter( string program, string label )
  {
    ocString filterform = listTemplate.getParagraph("list_filter");
    webIO << filterform.replaceAll( "$location", program.c_str())
                       .replaceAll("$label",label.c_str());  
  }
  list_base & emitHeadings( string pipedelimited )
  {
    ocString headings(pipedelimited);
    string list_begin = listTemplate.getParagraph("list_begin");
    string tr = listTemplate.getParagraph("tr");
    ocString th = listTemplate.getParagraph("th");
    string end_tr = listTemplate.getParagraph("end_tr");
    webIO << list_begin << tr;
    while( ! headings.endOfParse() )
    {
      colCount++;
      string heading = headings.parse("|");
      webIO << th.replace( "$data$", heading.c_str() );
    }    
    webIO << end_tr;
    return * this;
  }
  list_base & emitNavigation( string program  )
  {
    ocString numcols, curPage, numPages,datasetCount,recordCount;;
    numcols.append(colCount);
    curPage.append(listPg+1);
    numPages.append(pgCount);
    datasetCount.append(dsetCount);
    recordCount.append(recCount);
    
    ocString nav = listTemplate.getParagraph("list_nav");
    webIO << 
    nav.replace("$cols",numcols.c_str())
       .replaceAll("$location",program.c_str())
       .replace("$Page",curPage.c_str())
       .replace("$total$",numPages.c_str())
       .replace("$dsetCount", datasetCount.c_str())
       .replace("$recCount", recordCount.c_str());    
    return * this;   
  }
  list_base & emitEnd( void )
  {    
    ocString list_end = listTemplate.getParagraph("list_end");
    ocString lPg;
    lPg.append(listPg);     
    webIO << list_end.replace("$lpg",lPg.c_str()); 
    return * this;  
  }
  
  void setColFormat( string fmt, int pos )
  {
    if( pos < rs.getFieldCount() )
    {
      rs.getField(pos).setFormatMask(fmt);
    }
  }
  
  list_base & emitData( void )
  {
    bool more_data = opened;
    string tr = listTemplate.getParagraph("tr");
    ocString td = listTemplate.getParagraph("td");
    string end_tr = listTemplate.getParagraph("end_tr");
    int fieldCount;
    if( opened ) fieldCount = rs.getFieldCount();
    derived_commence_event();
    while( more_data )
    {
      derived_predata_event();
      webIO << tr;
      for( int i=0; i < fieldCount; ++i )
      {
        if( i == hotCol )
        {
          webIO <<  td.replace( "$data$",
                    editLink.replaceAll( "$key$",rs.getField(0).format().c_str())
                            .replaceAll( "$col$",rs.getField(i).format().c_str()).c_str());  
        }
        else
        {
          webIO <<  td.replace( "$data$", rs.getField(i).format().c_str()); 
        }  
      }       
      webIO << end_tr;
      derived_data_event(); 
      more_data = rs.next();  
    }
    derived_complete_event();
    return * this;
  }
  
  virtual void derived_commence_event( void ){;}
  virtual void derived_predata_event( void ){;}
  virtual void derived_data_event( void ){;}
  virtual void derived_complete_event( void ){;}
};

#endif
