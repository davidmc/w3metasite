/*

  col_handling_rep.hpp
  derivation of rep_base for handling columns

*/

#ifndef col_handling_rep_HPP
#define col_handling_rep_HPP

#include "rep_base.hpp"
#include "Report_Params_ui.h"

/**
  Handlers for formatting the data

*/
class formatHandler
{
public:
  string ftype;

  formatHandler(string ftype):ftype(ftype){;}
  virtual string operator()( string in )
  {
    return in;
  }
};

class formatString: public formatHandler
{
public:
  formatString():formatHandler("STRING"){;}
  virtual string operator()( string in )
  {
    return in;
  }
};

class formatCurrency: public formatHandler
{
public:
  formatCurrency():formatHandler("CURRENCY"){;}
  virtual string operator()( string in )
  {
    money mon = atof( in.c_str() ); 
    return mon.format("%n");
  }
};
class formatDate: public formatHandler
{
public:
  formatDate():formatHandler("DATE"){;}
  virtual string operator()( string in )
  {
    time_date dt;
    dt.parse( in.c_str(), "%m-%d-%Y" );
    // handle locale sensitivity
    if(!dt.is_good_parse()) dt.parse( in.c_str(), "%Y-%m-%d" );
    if(!dt.is_good_parse()) return in;
    return dt.format("%m/%d/%Y");
  }
};
class formatInteger: public formatHandler
{
public:
  formatInteger():formatHandler("INTEGER"){;}
  virtual string operator()( string in )
  { 
    size_t sz =  in.length()*2; 
    char * dynStr = new char[sz];
    snprintf(dynStr,sz,"%i", atol(in.c_str()));
    in = dynStr;    
    return in;   

  }
};

class formatDecimal: public formatHandler
{
public:
  formatDecimal():formatHandler("DECIMAL"){;}
  virtual string operator()( string in )
  {   
    size_t sz =  3+(in.length()*2); 
    char * dynStr = new char[sz];
    snprintf(dynStr,sz,"%0.2f", atof(in.c_str()));
    in = dynStr;    
    return in;
  }
};

/// The format handler map
typedef map< int, class formatHandler* > formatMap;

/** 
  class colHandler
  
  base class for col event functors 
    
*/
class colHandler
{
protected:
  // data members
  int m_column;
  string m_name;
  rep_base & m_repBase;
  string m_Tuple_Section;

public:
  // the only constructor to use
  colHandler( int column, string name, string tuple, rep_base & in):
  m_column(column),
  m_name(name),
  m_repBase(in),
  m_Tuple_Section(tuple) 
  { 
    ;
  }
  // destructor
  virtual ~colHandler()
  {
    ;
  }
  // copy constructor 
  colHandler( colHandler & in):
  m_column(in.m_column),
  m_name(in.m_name),
  m_repBase(in.m_repBase),
  m_Tuple_Section(in.m_Tuple_Section)
  { 
    ;
  }
  
  virtual bool operator()( int ev )
  {
    return false;
  }
  
  // member access operators  
  // get
  int  column( void ) { return m_column; }
  // set
  void column( const int  in ) { m_column = in; }
  
  // get
  string & name( void ) { return m_name; }
  // set
  void name( const string & in ) { m_name = in; }
  
  // get
  rep_base & repBase( void ) { return m_repBase; } 
     
 // get
  string & Tuple_Section( void ) { return m_Tuple_Section; }
  // set
  void Tuple_Section( const string & in ) { m_Tuple_Section = in; }  
};

/// has to be a pointer collection for the derived classes sake
typedef map< int, class colHandler* > colHandlerMap;


/**
  col_handling_rep

  handles reports that perform special operations on columns
  like grouping. summing and specialized formatting.

*/
class col_handling_rep: public rep_base 
{
  // to get column handlers
  ocString handlerSQL;

public:
  /** events described as follows:
  <ul>
    <li>commence_event - event just before any data is displayed
    <li>prerow_event -  Before any data on the current row is displayed
        want to check for groups and sums on the previous rows
    <li>field_event - The field on a row of data is being rendered, give the handler an opportunity to do this
    <li>postfield_event -  After current field is displayed
    <li>postrow_event -  After all data on the row is displayed
    <li>complete_event - After all data is displayed
  </ul>
  */

  enum events {  commence_event, prerow_event, field_event, postfield_event,  postrow_event, complete_event };

  colHandlerMap colHandlers;
  formatMap     colFormats;
  int fieldCount;  
  int Formatting_Modulus;
  int Items_Per_Page;
  bool doParamFilter;

  col_handling_rep(ostream&sc):rep_base(sc),fieldCount(0),
  Formatting_Modulus(0),Items_Per_Page(0),doParamFilter(false)
  {
  }

  string paramFilter( ocString sql, string tag, ocString rep )
  {
    /*
    Special case of a replacement value needing to take on more than one value.
    Do one at a time replacements as parsed from the input argument
    */
    if( rep.find("|") != string::npos )
    {
      string tok = rep.parse("|");
      int count = 0;
      while( tok.length() )
      {
        ocString tgUniq = tag + ":"; 
        tgUniq.append(++count);
        // webIO << tgUniq << "<br>" << endl;
        sql = sql.replaceAll(tgUniq,tok.c_str());
        tok = rep.parse("|");
      }
    }
    else  // General case: replace all
    {
      sql = sql.replaceAll(tag,rep);
    }
    return sql;
  }

  string filter( string & sql )
  {
    string ret = sql;
    Session_Obj sess(false);
    ocString params =  sess.GetData("rep_params");
    if( params.length() )
    {
      ocString tokPair = params.parse("`");
      while( tokPair.length() )
      {
        ocString tag = tokPair.parse("~");
        string rep = tokPair.parse("~");
        ret = paramFilter( ret, tag, rep );
        tokPair = params.parse("`"); 
      }
    }
    return ret;
  }
  bool getData( string sql )
  {
    if( doParamFilter )
    {
      sql = filter(sql);
    }
    webIO << "<!-- " << sql << " -->" << endl;
    return rep_base::getData(sql);
  }

  void addColumnHandlers( long long Report_Id )
  {
    handlerSQL = "select Id, Report_Id, Col, Sort_Pos, Col_Handler, Direction, Format, "
                 " Tuple_Section, spawnedReport,  spawnedTuplePretext, spawnedTuplePosttext, spawningEvent "
                 " from Report_Conditioning where Report_Id = ";
    handlerSQL.append( Report_Id );
    for( bool bopen = rs.open(handlerSQL); bopen; bopen=rs.next() )
    {
      addHandler();      
      addFormat();
    }    
    rs.close();    
  }

  void addFormat( void )
  {
     int col = dynamic_cast<longField&>(rs.getField(2)).get();
     string Col_Format = rs.getField(6).format();  
     if(Col_Format=="CURRENCY")
     { 
       colFormats.insert(make_pair(col, new formatCurrency));
     }
     else if(Col_Format=="DATE")
     { 
       colFormats.insert(make_pair(col, new formatDate));
     }   
     else if(Col_Format=="DECIMAL")
     {
       colFormats.insert(make_pair(col, new formatDecimal));
     }
     else if(Col_Format=="INTEGER")
     {
       colFormats.insert(make_pair(col, new formatInteger));
     }
  }
  
  string formatField( int iField )
  {
    string val = rs.getField(iField).format();    
    formatMap::iterator pos = colFormats.find(iField); 
    if( pos != colFormats.end() )
    {  
      val = (*(pos->second))(val);
    }    
    return val;    
  }
  

   
  
  string formatValue( int iField, string val )
  {     
    formatMap::iterator pos = colFormats.find(iField); 
    if( pos != colFormats.end() )
    {  
      val = (*(pos->second))(val);
    }    
    return val;    
  }
  
  // Get the container with the right kind of alignment etc. for the data.
  ocString fpart( string key, int iField, bool doReplace = true )
  {
    string id = key;
    
    formatMap::iterator pos = colFormats.find(iField); 
    if( pos != colFormats.end() )
    {
      formatHandler * pFmtHndlr = pos->second;
      if( ( pFmtHndlr->ftype == "CURRENCY" ||
            pFmtHndlr->ftype == "DECIMAL" ||       
            pFmtHndlr->ftype == "INTEGER" ) &&
            doReplace )
      id += "_right";    
    }
    return reportParts.getParagraph(id);
  }
  void addHandler( void );

  void addHandler( colHandler * hndlr )
  {
    colHandlers.insert( make_pair( hndlr->column(), hndlr ) );
  }
  
  virtual ~col_handling_rep()
  {
    colHandlerMap::iterator pos;
    for(pos=colHandlers.begin(); pos!=colHandlers.end(); ++pos)
    {
      delete pos->second;
    }
    
    formatMap::iterator pos2;
    for(pos2=colFormats.begin(); pos2!=colFormats.end(); ++pos2)
    {
      delete pos2->second;
    }    
  }
  
  // To be able to override each field
  virtual void sendField( int iField, ocString & td )
  {
    colHandlerMap::iterator pos = colHandlers.find(iField);
    // style container that matches the data
    ocString ftd = fpart( "td", iField );  // easy enough to specify a template part
    
    if( pos != colHandlers.end() )
    {
      if(pos->second->Tuple_Section().length() > 0)
      {
        ftd = fpart( pos->second->Tuple_Section().c_str(), iField, false );
      }
      // if the field is emitting this, then simply return      
      if( (*(pos->second))(field_event) ) return;
    }
    
    ftd = ftd.replace( "$data$", formatField(iField).c_str() );
    ftd =  ftd.replace( "$label$", rs.getField(iField).getName().c_str() );
    webIO << ftd;
    /// NEW post field event
    if( pos != colHandlers.end() ) (*(pos->second))(postfield_event);
    webIO.flush();  
  }
  virtual string getHeadCell( int iField, ocString & th )
  {
    colHandlerMap::iterator pos = colHandlers.find(iField);
    // style container that matches the data
    ocString lth = th;  // easy enough to specify a template part
    
    if( pos != colHandlers.end() )
    {
      string sctName = pos->second->Tuple_Section();
      if(sctName.length() > 0)
      {
        string lkup = "lbl_" + sctName;
        string que = reportParts.getParagraph(lkup);
        if( que.length() )
        {
          lth = que;
        }
      }
    }
    return lth.replace( "$data$", rs.getField(iField).getName().c_str() );
  }
  
  // events

  // before any data is displayed
  virtual void derived_commence_event( void )
  {
    fieldCount = rs.getFieldCount();
    for( int i=0; i < fieldCount; ++i )
    {
      colHandlerMap::iterator pos = colHandlers.find(i); 
      if( pos != colHandlers.end() )
      {
        // if the field is handling this, then simply return      
        (*(pos->second))(commence_event);
      }
    }
  }

  /* Before any data on the current row is displayed
     want to check for groups and sums on the previous rows
  */
  virtual void derived_prerow_event( void )
  {
    if(Items_Per_Page)
    {
      if( more_data && row && row%Items_Per_Page == 0 )
      {
        webIO <<  part( "pagebreak" );
      }
    }  
    webIO.flush();
    //for( int i=0; i < fieldCount; ++i )
    for( int i=fieldCount-1; i>=0;--i )
    {
      colHandlerMap::iterator pos = colHandlers.find(i); 
      if( pos != colHandlers.end() )
      {
        // if the field is handling this, then simply return      
        (*(pos->second))(prerow_event);
      }
    } 
  }
  
  // After all data on the row is displayed
  virtual void derived_postrow_event( void )
  {
    webIO.flush();
    for( int i=0; i < fieldCount; ++i )
    {
      colHandlerMap::iterator pos = colHandlers.find(i); 
      if( pos != colHandlers.end() )
      {
        // if the field is handling this, then simply return      
        (*(pos->second))(postrow_event);
      }
    }
  }
  
  // After all data is displayed
  virtual void derived_complete_event( void )
  {
    webIO.flush();
    int skipped = 0;
    bool isRow = false;
    int i;

    // two passes, one for final group emmissions   
    //for( i=0; i < fieldCount; ++i )
    for( i=fieldCount-1; i>=0;--i )
    {
      colHandlerMap::iterator pos = colHandlers.find(i); 
      if( pos != colHandlers.end() && pos->second->name() == "GROUP" )
      {
        (*(pos->second))(complete_event);
      }
    }
    // This one for totals
    for( i=0; i < fieldCount; ++i )
    {
      colHandlerMap::iterator pos = colHandlers.find(i); 
      if( pos != colHandlers.end() && (pos->second->name() == "SUM" || pos->second->name() == "AVG") )
      {
        if( !isRow ) // Now we want to show a row if it wasn't shown
        {
          webIO << part("tr") << part("ts").replace("$data$","Totals: ");     
          isRow=true;
          skipped--; // eat the first skipped, 'cause we just used it for totals
        }        
        if( skipped ) // Play catch up
        {
          for(int j=0;j<skipped;j++)
          {
            webIO << part("ts").replace("$data$"," ");
          }
          skipped=0;
        }
        // Increment skipped if no emission      
        if(!(*(pos->second))(complete_event))        
          skipped++;        
      }
      else
      {
        skipped++;
      }    
    }
    for( i=0; i < skipped; i++ )
    {
      webIO << part("ts").replace("$data$"," ");
    }
    if( isRow ) webIO << part("end_tr");    
  }
  
  openRS & getRS() 
  {
    return rs;
  }
  
  friend class colHandler;  
}; // end col_handling_rep

/**
  Def's of typical col handlers 
  - grouping (complete)
  - sum      (complete)
  - avg      (complete)
  - max
  - min
  - mean
  - std dev.
 New Def's
  - spawn handler (complete)
  - lookup handler (beta, working)
*/

class  spawnHandler  : public colHandler
{
private:
  string spawnedReport;
  string spawnedTuplePretext;
  string spawnedTuplePosttext;
  int spawningEvent;
public:
  spawnHandler(int column, string name, string tuple, rep_base & in,
               string spawnedReport, string spawnedTuplePretext,
               string spawnedTuplePosttext, int spawningEvent)
    :colHandler(column,name,tuple,in)
    ,spawnedReport(spawnedReport)
    ,spawnedTuplePretext(spawnedTuplePretext)
    ,spawnedTuplePosttext(spawnedTuplePosttext)
    ,spawningEvent(spawningEvent)
  {
  }
  spawnHandler(spawnHandler & in):colHandler(in) {;}
  ~spawnHandler()
  {
  }
   // generic event
  virtual bool operator()( int ev )
  {
    bool bEmission = false; // caller wants to know you if you emit something
    col_handling_rep & dynRep = dynamic_cast<col_handling_rep&>(m_repBase);
    if( ev == spawningEvent )
    {
      Report_Obj report;
      report.key( atoll(spawnedReport.c_str()) );
      report.Id = report.key();
      if( report.get_data() )
      {
        string path = "reportTemplates/" + report.getTemplatePath();
        ocString sql = report.getCompositeQuery();
        columnTagFixup(sql,dynRep.getRS());

        // dynRep.webIO << sql;

        col_handling_rep rpt(dynRep.webIO);
        rpt.doParamFilter = Report_Parameters_form::hasParamValues;
        rpt.Formatting_Modulus = report.Formatting_Modulus;
        rpt. Items_Per_Page = report.Items_Per_Page;
        rpt.loadTemplates(path);
        // now lets fix the top header, with elements from the current data
        ocString top = rpt.part("top");
        columnTagFixup( top, dynRep.getRS() );
        rpt.reportParts.getParagraph("top") = top;
        // also fixup page break headers
        ocString pgbrk = rpt.part("pagebreak");
        if( pgbrk.length() )
        {
          columnTagFixup( pgbrk, dynRep.getRS() );
          rpt.reportParts.getParagraph("pagebreak") = pgbrk;
        }
        rpt.addColumnHandlers( report.Id );

        if( rpt.getData(sql) )
        {
          ocString spawnedTuplePretext=this->spawnedTuplePretext;
          ocString spawnedTuplePosttext=this->spawnedTuplePosttext;
          columnTagFixup(spawnedTuplePretext,dynRep.getRS());
          m_repBase.webIO << spawnedTuplePretext;
          rpt.emitTop(report.Name);
          rpt.emitHeader();
          rpt.emitData();
          rpt.emitEnd();
          columnTagFixup(spawnedTuplePosttext,dynRep.getRS());
          dynRep.webIO << spawnedTuplePosttext;
          bEmission = true;
        }
        else
        {
          dynRep.webIO << "<!-- No data with " <<  sql << " -->" << endl;
        }
      }
    }
    return bEmission;
  }
  void columnTagFixup( ocString & tgt, openRS & rs )
  {
    for( int i =0; i < rs.getFieldCount(); i++ )
    {
      ocString repTag = "{{";
      repTag.append(i);
      repTag += "}}";
      tgt = tgt.replaceAll(repTag, rs.getField(i).format());
    }
  }
};
typedef map< string, string > lookups;
typedef map< string, lookups > qryLookups;
 
 class lookupHandler: public colHandler
{
  public:
    // the list of lookups
    static qryLookups GotIt;
    // the lookup
    lookups lookup;
    string lookupSQL;
    lookupHandler( int column, string name, string tuple, rep_base & in, string & sql )
    :colHandler(column,name,tuple,in),lookupSQL(sql)
    {
    }
    ~lookupHandler(){;}
    lookupHandler(lookupHandler & in):colHandler(in) {;}

    // generic event
    virtual bool operator()( int ev )
    {
      bool bEmission = false; // caller wants to know you if you emit something
      col_handling_rep & dynRep = dynamic_cast<col_handling_rep&>(m_repBase);
      switch( ev )
      {
        case col_handling_rep::commence_event:
        {
          // see if we already have such a lookup
          qryLookups::iterator findPos = GotIt.find(lookupSQL);
          if( findPos == GotIt.end() )
          {
            quickQuery qry;
            openRS & qrs = qry.getData ( lookupSQL );
            if( qry.opened )
            {
              do
              {
                lookup[qrs.getField(0).format()]=qrs.getField(1).format();
              }while(qrs.next());
            }
          }
          else
          {
            lookup = findPos->second;
          }
          break;
        }
        case  col_handling_rep::field_event:
        {
          string id = dynRep.getRS().getField(m_column).format(); // lookup typically IS an id
          string val = lookup[id]; // find the value
          if( val.length() == 0 ) val = "REPORT LOOKUP FAIL:" + id; // couldn't find it
          ocString tmplt = Tuple_Section(); // see if we have alternative containing content for this data
          if(tmplt.length()) tmplt = m_repBase.part( tmplt ); // yep
          else tmplt = m_repBase.part("td"); // nope, use the default
          m_repBase.webIO << tmplt.replace("$data$",val.c_str() ); // send the data to the browser
          bEmission = true; // we did emit this, caller doesn't have to
          break;
        }
      }
      return bEmission;
    }
};
typedef map< string, double > group_sums;
class sumHandler: public colHandler
{
private:
  double sum;   
 
  // This is for formatting
  int fieldType;
public:
  string lastGroupName;
  // a group map of sums
  group_sums sums;
  
  sumHandler( int column, string name, string tuple, rep_base & in )
  :colHandler(column,name,tuple,in),sum(0.0),fieldType(-1)
  {
  }
  ~sumHandler(){;}
  sumHandler(sumHandler & in):colHandler(in) {;}

  // special event called from group handler on pre event
  virtual bool operator()( int ev, string groupName )
  {
    bool bEmission = false;    
    if( ev == col_handling_rep::prerow_event )
    {
      col_handling_rep & dynRep = dynamic_cast<col_handling_rep&>(m_repBase);
      double & val = sums[groupName];
      ocString Sum;
      Sum.append(val);
      Sum = dynRep.formatValue(m_column,Sum);
      m_repBase.webIO << m_repBase.part("ts").replace("$data$",Sum.c_str());
      val=0.0;
      bEmission=true;
    }    
    return bEmission;
  }
  // generic event
  virtual bool operator()( int ev )
  {
    bool bEmission = false; // caller wants to know you if you emit something
    col_handling_rep & dynRep = dynamic_cast<col_handling_rep&>(m_repBase);
    switch( ev )
    {
    case col_handling_rep::commence_event:
      sum=0.0;
      break; 
    case  col_handling_rep::field_event:
      {      
      double val = atof( dynRep.getRS().getField(m_column).format().c_str() );
      if( finite(val) )
      {
        sum += val;
        group_sums::iterator pos;
        for( pos=sums.begin(); pos!=sums.end();++pos)
        {
          pos->second += val;
        }
      }  
      break;
      }
    case col_handling_rep::complete_event:
      ocString Sum;
      Sum.append(sum);
      Sum = dynRep.formatValue(m_column,Sum);
      m_repBase.webIO << m_repBase.part("ts").replace("$data$",Sum.c_str());
      bEmission=true;
      break;
    }
    return bEmission;
  }
};

typedef map< string, int > group_counts;
class avgHandler: public colHandler
{
private:
  double sum;   
  int count;
  
  // This is for formatting
  int fieldType;
public:
  string lastGroupName;
  // a group map of sums
  group_sums sums;
  group_counts counts;
  
  avgHandler( int column, string name, string tuple, rep_base & in )
  :colHandler(column,name,tuple,in),sum(0.0),fieldType(-1)
  {
  }
  ~avgHandler(){;}
  avgHandler(sumHandler & in):colHandler(in) {;}
  
  // special event called from group handler on pre event
  virtual bool operator()( int ev, string groupName )
  {
    bool bEmission = false; 
    if( ev == col_handling_rep::prerow_event )
    {
      // you still have the old group name at this point
      col_handling_rep & dynRep = dynamic_cast<col_handling_rep&>(m_repBase);
      double & val = sums[groupName];
      int cnt = counts[groupName];
      ocString Sum;
      if( cnt )
      {
        Sum.append(val/cnt);
      }
      else
      {
        Sum.append(0);
      }
      Sum = dynRep.formatValue(m_column,Sum);
      m_repBase.webIO << m_repBase.part("ts").replace("$data$",Sum.c_str());
      val=0.0;
      counts[groupName] = 0;
      bEmission=true;
    }    
    return bEmission;
  }
  // generic event
  virtual bool operator()( int ev )
  {
    bool bEmission = false; // caller wants to know you if you emit something
    col_handling_rep & dynRep = dynamic_cast<col_handling_rep&>(m_repBase);
    switch( ev )
    {
    case col_handling_rep::commence_event:
      sum=0.0;
      count=0;
      break; 
    case  col_handling_rep::field_event:
      {      
      double val = atof( dynRep.getRS().getField(m_column).format().c_str() );
      if( finite(val) )
      {
        sum += val;
        count++;
        group_sums::iterator pos;
        for( pos=sums.begin(); pos!=sums.end();++pos)
        {
          pos->second += val;
          counts[pos->first]++;
        }
      }
      break;
      }
    case col_handling_rep::complete_event:
      ocString Sum;
      Sum.append(sum/count);
      Sum = dynRep.formatValue(m_column,Sum);
      m_repBase.webIO << m_repBase.part("ts").replace("$data$",Sum.c_str());
      bEmission=true;
      break;
    }
    return bEmission;
  }
};

class groupHandler: public colHandler
{
private:
  string last_val;
  string colName;
  int counter;
  bool hasSums;
public:
  groupHandler( int column, string name, string tuple, rep_base & in )
  :colHandler(column,name,tuple,in),counter(0),hasSums(false)
  {
  }
  
  ~groupHandler(){;}
  
  groupHandler(groupHandler & in):colHandler(in) {;}
  
  bool commence( col_handling_rep & dynRep )
  {
    last_val="";
    // Get the whole collection
    colHandlerMap & colHandlers = dynRep.colHandlers;
    openRS & rs = dynRep.getRS();
    int fieldCount = dynRep.fieldCount;
    colName = rs.getField(m_column).getName();
    for( int i=0; i < fieldCount; ++i )
    {
      colHandlerMap::iterator pos = colHandlers.find(i);
      if( pos !=  colHandlers.end() )
      {
        // find any SUM handlers and add this group initialized to 0 to each collection
        if( pos->second->name() == "SUM" )
        {          
          sumHandler & dyno = dynamic_cast<sumHandler &>(*(pos->second));
          dyno.sums.insert(make_pair(colName,0.0));
          dyno.lastGroupName = colName; 
          hasSums = true;
        }        
        // .. likewise for AVG etc.
        if( pos->second->name() == "AVG" )
        {
          avgHandler & dyno = dynamic_cast<avgHandler &>(*(pos->second));
          dyno.sums.insert(make_pair(colName,0.0));
          dyno.counts.insert(make_pair(colName,0));
          dyno.lastGroupName = colName;
          hasSums = true;
        }
      }
    }
    return false;    
  }
  
  bool pre( col_handling_rep & dynRep, bool atEnd=false )
  {
    preGroups(dynRep,atEnd);
    // before moving on, get the current sums and reset.
    if( counter && hasSums )
    {
      m_repBase.webIO << m_repBase.part("tr");    
      for( int i=0; i < m_column; ++i )
      {
        m_repBase.webIO << m_repBase.part("ts").replace("$data$"," ");
      }
      string prompt = last_val;
      prompt += " Sub-totals:";
      m_repBase.webIO << m_repBase.part("end_tg").replace("$data$",prompt.c_str());
      preSums( dynRep );     
      m_repBase.webIO << m_repBase.part("end_tr");
      counter=0; // do not allow sum line until more data is read
      last_val=""; // force data field to show on next read
    }
    if(atEnd==false && m_column ==0)
    {
      m_repBase.emitHeader();
    }  
    return true; 
  }

  // force subordinate groups to give up sums
  void preGroups(col_handling_rep & dynRep, bool atEnd=false )
  {
    int fieldCount = dynRep.fieldCount;
    int skipped = 0;  int i;
    for( i=m_column+1; i < fieldCount; ++i )
    {
      colHandlerMap::iterator pos = dynRep.colHandlers.find(i);
      if( pos != dynRep.colHandlers.end() )
      {
        // increment skipped if no emission
        if( pos->second->name() == "GROUP" )
        {
          groupHandler & dyno = dynamic_cast<groupHandler &>(*(pos->second));
          dyno.pre(dynRep,atEnd);
          return;
        }
      }
    }
  }
  void preSums(col_handling_rep & dynRep )
  {

    int fieldCount = dynRep.fieldCount;
    int skipped = 0;
    int i;
    for( i=m_column+1; i < fieldCount; ++i )
    {
      colHandlerMap::iterator pos = dynRep.colHandlers.find(i); 
      if( pos != dynRep.colHandlers.end() )
      {
        // increment skipped if no emission
        if( pos->second->name() == "SUM" )
        {
          sumHandler & dyno = dynamic_cast<sumHandler &>(*(pos->second));
          if(!dyno(col_handling_rep::prerow_event,colName))        
          {
            m_repBase.webIO << m_repBase.part("ts").replace("$data$"," ");
          }        
        }
        else if( pos->second->name() == "AVG" )
        {
          avgHandler & dyno = dynamic_cast<avgHandler &>(*(pos->second));
          if(!dyno(col_handling_rep::prerow_event,colName))
          {
            m_repBase.webIO << m_repBase.part("ts").replace("$data$"," ");
          }
        }
        else
        {
          m_repBase.webIO << m_repBase.part("ts").replace("$data$"," ");
        }          
      }
      else
      {
        m_repBase.webIO << m_repBase.part("ts").replace("$data$"," ");
      }    
    }

  }  
  virtual bool operator()( int ev )
  {
    bool bEmission = false; // Caller wants to know if you emit something
    string val;
    col_handling_rep & dynRep = dynamic_cast<col_handling_rep&>(m_repBase);
    openRS & rs = dynRep.getRS();
    if( rs.isOpen() )
    {
      val =  rs.getField(m_column).format(); 
    }
    switch( ev )
    {
    case col_handling_rep::commence_event:
      commence(dynRep);      
      break; 
    case col_handling_rep::prerow_event:       
      if( last_val.length() && val != last_val )  
      {
        bEmission = pre(dynRep); 
      }
      break;

    case  col_handling_rep::field_event:
      {
      ocString grpTuple;
      colHandlerMap::iterator pos =  dynRep.colHandlers.find(m_column);
      if( pos != dynRep.colHandlers.end() &&  pos->second->Tuple_Section().length() > 0)
      {
        grpTuple = dynRep.part(pos->second->Tuple_Section());
      }
      counter++;             
      if( val == last_val )
      {
        // we are assuming they went to a lot of trouble to name a special section
        // and therefore they aren't in need of and don't want a dummy section.
        if( !grpTuple.length() )
        {
          m_repBase.webIO << m_repBase.part("tg").replace("$data$"," ");
        }
      }
      else
      {
        if( grpTuple.length() )
        {
          m_repBase.webIO << grpTuple.replace("$data$",val.c_str() );
        }
        else
        {
          m_repBase.webIO << m_repBase.part("tg").replace("$data$",val.c_str() );
        }
      }
      bEmission = true;
      break;
      }
    case col_handling_rep::postrow_event:                   
      if( val.length() && val != last_val )
      {
        last_val = val;
      }
      break;
    case col_handling_rep::complete_event:
      bEmission = pre(dynRep,true);
      break;
    } // end switch
    return bEmission;
  }
};   // end of groupHandler


// method define outside class, after defining all handlers
void col_handling_rep::addHandler( void )
{
  int col = dynamic_cast<longField&>(rs.getField(2)).get();
  string Col_Handler = rs.getField(4).format();
  string Tuple_Section = rs.getField(7).format();
  if( Col_Handler == "SUM" )
  {
    addHandler( new sumHandler ( col, Col_Handler, Tuple_Section, *this ) ); 
  }
  else if( Col_Handler == "GROUP" )
  {
    addHandler( new groupHandler ( col, Col_Handler, Tuple_Section, *this ) ); 
  }
  else if( Col_Handler == "AVG" )
  {
    addHandler( new avgHandler ( col, Col_Handler, Tuple_Section, *this ) );
  }
  else if( Col_Handler == "SPAWN" )
  {
    string spawnedReport, spawnedTuplePretext, spawnedTuplePosttext;
    int spawningEvent = 0;
    spawnedReport = rs.getField("spawnedReport").format();
    spawnedTuplePretext = rs.getField("spawnedTuplePretext").format();
    spawnedTuplePosttext = rs.getField("spawnedTuplePosttext").format();
    string strSpawningEvent = rs.getField("spawningEvent").format();
    if( strSpawningEvent.length() ) spawningEvent = atoi(strSpawningEvent.c_str());
    addHandler( new spawnHandler( col, Col_Handler, Tuple_Section, *this,
                spawnedReport, spawnedTuplePretext, spawnedTuplePosttext, spawningEvent ) );
  }
  else if( Col_Handler == "LOOKUP" )
  {
    string spawnedTuplePretext;
    spawnedTuplePretext = rs.getField("spawnedTuplePretext").format();
    addHandler( new lookupHandler( col, Col_Handler, Tuple_Section, *this,
                spawnedTuplePretext ) );
  }
  else // the default
  {
    addHandler( new colHandler ( col, Col_Handler, Tuple_Section, *this ) );
  }
}

// This is for the implementor to define statics,
// Here we believe we are called just once in all translation units so we define it here.
// When we break implementation from definition, and create an '.so' then we will place this in the
// real implementation file.  In the meantime you can suppress in additional translation units.

#ifndef COL_HANDLING_REP_IMPL_SUPPRESS
qryLookups  lookupHandler::GotIt;
#endif

#endif
