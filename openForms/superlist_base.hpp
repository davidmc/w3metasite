#ifndef LIST_BASE_HPP
#define LIST_BASE_HPP

#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_base.hpp"
#include "cgiClass.h"
#include "cgiTemplates.h"
#include "Session.hpp"

#define DEBUGOUT

class list_base: public read_base
{
protected:
  cgiTemplates listTemplate;
  cgiScript & webIO;
  ocString td;
  /// the means to retention of state is in the Session
  Session_Obj & session;
  /// set this to an application unique list name if you want to retain list state (pg, filter, and order.) for a local list
  string sessionName;
public:
  bool opened;
  /// an edit link in the form of "<a target="_self" href='$prog$?Id=$key$'>$col$</a>"
  ocString editLink;
  /// -1 means no hot columns, -2 means all hot columns other-wise, the hot column is the ordinal final pos, MUST set edit link
  int hotCol;
  /// allows you to skip 1 column (to skip more than 1, override sendField() or sendHotField()
  int skipCol;
  /// retain added Criteria for listing here (this search criteria is not stored in the Session)
  string addedCriteria;
  
  
  // for filtering, sorting, and page;
  string cookiePrefix;  
  string filter, sort, direction; 
  int  listPg, itemsPerPg, recCount, colCount, pgCount, dsetCount;
  
    
  void setPage( cgiInput & args )
  {
    listPg = atol(session.GetData(sessionName+"lPg").c_str()); 
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
    ocString limit;
    int rowStart = listPg * itemsPerPg; 
    limit = " limit ";
    if( db.getProvider() == MySQL )
    {
      limit.append(rowStart);
      limit += ", ";
      limit.append(itemsPerPg);
    }
    else if( db.getProvider() == PostGresQL )
    {
      limit.append(itemsPerPg);
      limit += " offset ";
      limit.append(rowStart);     
    }   
    return limit;
  }
  void setSort( cgiInput & args )
  {
    direction = session.GetData(sessionName+"lDir");
    sort = session.GetData(sessionName+"lSort");
    // has the order been specified?
    if (args.count("sort") )
    { 
      if(  sort == args["sort"].c_str() )
      {
        // reverse direction
        direction = direction=="desc"?"asc":"desc";
        session.SetData(sessionName+"lDir",direction.c_str());
      }  
      sort = args["sort"].c_str();
      session.SetData(sessionName+"lSort",sort.c_str());
    }
  }
  /*
    reset is called by the constructor.
    initial calls to a page with list_base deived lists should call
    the page with the query string having init=1 
    (init=anything at all really, but 1 by convention)
  */
  void reset( cgiInput & args )
  {
    if( args.count("init") ) 
      reset();
  }
  void reset( void )
  {
    // kill all session data
    session.SetData(sessionName+"lFilter","");
    session.SetData(sessionName+"lDir","");
    session.SetData(sessionName+"lSort","");
    session.SetData(sessionName+"lPg",""); 
  }

  /*
    Looks for a 'filter' argument to place filter commands in the 
    'where' clause by replacing "$filter$" in the template parameter 
    'filterTemplate' of the 'getFilteredData()' method.
    Additionally sets a session cookie "lFilter" for persistence.
  */      
  void setFilter( cgiInput & args )
  {
    if( webIO.RequestMethod().upper() == "GET" ) // would be post on submission of filter
    {
      filter = session.GetData(sessionName+"lFilter");
    }  
    if( args.count("filter") )
    { 
      filter = args["filter"].c_str();
      session.SetData(sessionName+"lFilter",filter.c_str());      
    }
    else if( sessionName.length() > 0 ) // to limit b.w.c. issues, only get if we have a distinct session name
    {
      filter = session.GetData(sessionName+"lFilter");
    }
  }
  /*
     This constructor must be called!
     Call it before closing the script header!
  */   
  list_base(cgiScript&sc,Session_Obj & session,string sessionName=""):read_base(),webIO(sc),opened(false),hotCol(0),skipCol(-1)
  ,listPg(0), itemsPerPg(20), recCount(0), colCount(0)
  ,pgCount(0), dsetCount(0),session(session),sessionName(sessionName)
  {
    cgiInput & args = sc.ClientArguments(); 
    reset( args ); 
    // set and cache the current sort order  
    setSort( args ); 
    // set and cache the current filter
    setFilter( args );  
    // set and cache the current page of data 
    setPage( args );
  }
  
  virtual ~list_base()
  {
    session.Synch(); 
  }

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
      ocString filterParts = filter;
      filterParts = filterParts.replaceAll("|", "");
      if(  filterParts.length() ) // check for ghosts
      {
        filterParts = filter;
        filterTemplate.parseInit();
        ocString filterParts = filter;
        string filterPart = filterParts.parse("|");
        ocString filterTemp = filterTemplate.parse("|");
        do 
        {
          bool valSet = filterPart.length() > 0;
          if( valSet )
          {
            sql += ( sql.length() ) ?  " and " : " where ";
            sql += filterTemp.replace("$filter$", filterPart.c_str() );
          }
          filterTemp = filterTemplate.parse("|");
          filterPart = filterParts.parse("|");
          // DBG webIO << "WHERE CLAUSE SQL[" << sql << "]<br>" << endl;
        } while( !(filterTemplate.endOfParse() && filterTemp.length()==0) ); 
      } 
    }
    if( addedCriteria.length() )
    {
      sql += ( sql.length() ) ?  " and " : " where ";
      sql += addedCriteria;
    }
    return sql;
  }
  
  void maxPageFixup( void )
  {
    pgCount = (recCount/itemsPerPg)+1;
    if( listPg >= pgCount ) listPg = pgCount-1;   
    ocString Pg;
    Pg.append(listPg);
    session.SetData(sessionName+"lPg",Pg.c_str());      
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
#ifdef DEBUGOUT
    webIO << "<!--" << sql << "-->";
#endif
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
    if( sort.length() ) 
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
#ifdef DEBUGOUT    
    webIO << "<!--" << sql << "-->";
#endif    
    opened = rs.open(sql);    
    if( opened )
    {
      dsetCount = rs.getRecordCount();      
    }
    return opened;
  }
  list_base & emitFilter( string program, string label )
  {
    string value;
    if( webIO.ClientArguments().count("filter")  )
    {
      value = webIO.ClientArguments()["filter"].c_str();
    }
    ocString filterform = listTemplate.getParagraph("list_filter");
    if( program.find("?") != string::npos ) 
    {
      filterform = filterform.replace("?","&");
    }
    webIO << filterform.replaceAll( "$location", program.c_str())
                       .replaceAll("$label",label.c_str())
                       .replace("$value", value.c_str() );  
  }
  string filtersValue( int idx )
  {
    string val;
    if( webIO.ClientArguments().count("filter")  )
    {
      ocString values = webIO.ClientArguments()["filter"].c_str();
      for( int i = 0;
           (i <= idx) && ! (values.endOfParse() && values.length()==0);
           i++ )
      {
        val = values.parse("|");
      }
    }
    return val;
  }
  list_base & emitFilters( string program, ocString labels )
  {
    ocString values = this->filter;
    ocString fstart = listTemplate.getParagraph("multi_filter_start");
    ocString fitem = listTemplate.getParagraph("multi_filter_item");
    ocString fend = listTemplate.getParagraph("multi_filter_end");
    
    if( program.find("?") != string::npos )
    {
      fstart = fstart.replace("?","&");
    }
    webIO << fstart.replaceAll( "$location", program.c_str());
    ocString label = labels.parse("|");
    ocString nlabel = label.parse("::");
    string typeSection = label.parse("::");
    string value = values.parse("|");
    do
    {
      if(typeSection.length())
      {
        ocString fAltItem = listTemplate.getParagraph(typeSection);
        if( fAltItem.length() )
          webIO << fAltItem.replaceAll("$label",nlabel.c_str()).replace("$value", value.c_str() );
        else
          webIO << "<!-- NO [" << typeSection << "] found -->" ;
      }
      else
      {
        webIO << fitem.replaceAll("$label",label.c_str()).replace("$value", value.c_str() );
      }
      value = values.parse("|");
      label = labels.parse("|");
      nlabel = label.parse("::");
      typeSection = label.parse("::");
    } while ( ! (labels.endOfParse() && label.length()==0) );
    webIO <<  fend.replaceAll( "$location", program.c_str());
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

    // new! sep thead from tbody
    webIO << listTemplate.getParagraph("tbody");
    return * this;
  }
  list_base & emitNavigation( string program  )
  {
    ocString numcols, curPage, numPages,datasetCount,recordCount;
    numcols.append(colCount);
    curPage.append(listPg+1);
    numPages.append(pgCount);
    datasetCount.append(dsetCount);
    recordCount.append(recCount);
    
    ocString nav = listTemplate.getParagraph("list_nav");
    nav = nav.replaceAll("$cols",numcols.c_str())
       .replaceAll("$location",program.c_str())
       .replace("$Page",curPage.c_str())
       .replace("$total$",numPages.c_str())
       .replace("$dsetCount", datasetCount.c_str())
       .replace("$recCount", recordCount.c_str());     
    webIO << nav;       
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
    string tro = listTemplate.getParagraph("tro");
    td = listTemplate.getParagraph("td");
    string end_tr = listTemplate.getParagraph("end_tr");
    int fieldCount=0,rowCount=0;

    if( opened ) fieldCount = rs.getFieldCount();
    derived_commence_event();
    while( more_data )
    {   rowCount++;
      if( derived_predata_event() )
      {
        if( rowCount%2 == 1 && tro.length() > 0)
          webIO << tro;
        else
          webIO << tr;
        for( int i=0; i < fieldCount; ++i )
        {
          if( i != skipCol )
          {
            if( i == hotCol || hotCol == -2 )
            {
              sendHotField(i, td );  
            }
            else 
            {
              sendField(i, td ); 
            }  
          }
        }       
        webIO << end_tr;
      }
      derived_data_event(); 
      more_data = rs.next();  
    }
    dsetCount = rs.getRecordCount(); 
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
  virtual bool derived_predata_event( void ){ return true; }
  virtual void derived_data_event( void ){;}
  virtual void derived_complete_event( void ){;}
};

#endif
