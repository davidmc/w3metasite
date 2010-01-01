#include<string>
#include<iomanip>
#include "cgiClass.h"
#include <sys/types.h>
#include <unistd.h>
#include "connectstring"
#include "ocString.h"
//#include "ocImportParser.h"
#include "ocTypes.h"
#include "cgiTemplates.h"
#include "ocXML.h"
#include "read_base.hpp"
#include <set>

typedef vector<string> tagVec;
// have two sets in case of 3 dimensional
typedef map<string,long> cocounts;
typedef map<string,string> lookups;
typedef set<string> visitors;
typedef map<string,visitors> aggregates;
typedef vector<string> mapinserts;

class reporter: public read_base
{
public:
  long       maxCount;
  cocounts   coco;
  aggregates aggies;
  lookups httpstats;
  ocString pageClause;
  ocString refererClause;
  ocString dailyPageClause;
  ocString monthlyPageClause;
  ocString dailyVisitsClause;
  ocString monthlyVisitsClause;
  ocString byBrowserClause;
  ocString byRequestStatusClause;
  ocString byDetailClause;
  string log_file;

  string show;

  // for multi colored bars
  tagVec tags;
  int tagMax;

  // defaults based on typical combined httpd log format
  reporter():read_base(),maxCount(0),tagMax(3)
  {
    httpstats["200"]="&nbsp; OK";
    httpstats["301"]="&nbsp; MOVED_PERMANENTLY";
    httpstats["302"]="&nbsp; FOUND";
    httpstats["304"]="&nbsp; NOT_MODIFIED";
    httpstats["400"]="&nbsp; BAD_REQUEST";
    httpstats["401"]="&nbsp; UNAUTHORIZED";
    httpstats["403"]="&nbsp; FORBIDDEN";
    httpstats["404"]="&nbsp; NOT_FOUND";
    httpstats["500"]="&nbsp; INTERNAL_SERVER_ERROR";
    tags.push_back("reddata");
    tags.push_back("greendata");
    tags.push_back("bluedata");
    
    refererClause = "select Referer, count(*)"
        "from Site_History where log_file = '$log_file$' and Referer not like '%//$SVR$%' "
                    "group by Referer order by Referer";
    
    pageClause = "select  Resource_Requested, count(*)"
                  "from Site_History where log_file = '$log_file$' "
                  "group by Resource_Requested order by Resource_Requested";

    dailyPageClause = "select  date_format(Request_Time,'%m/%d/%y'), count(*)"
                      "from Site_History where log_file = '$log_file$' "
                      "group by date_format(Request_Time,'%m/%d/%y')  order by Request_Time desc";
    
    monthlyPageClause = "select  date_format(Request_Time,'%m/%y'), count(*)"
        "from Site_History where log_file = '$log_file$' "
        "group by date_format(Request_Time,'%m/%y')  order by Request_Time desc";
    
    dailyVisitsClause = "select distinct date_format(Request_Time,'%m/%d/%y'), Remote_Host "
                        "from Site_History where log_file = '$log_file$' "
                        "order by Request_Time desc";

    monthlyVisitsClause = "select distinct date_format(Request_Time,'%m/%y'), Remote_Host "
        "from Site_History where log_file = '$log_file$' "
        "order by Request_Time desc";

    byBrowserClause = "select  User_Agent, count(*)"
        "from Site_History where log_file = '$log_file$' "
        "group by User_Agent order by User_Agent";
    
    byRequestStatusClause = "select  Original_Status, count(*)"
        "from Site_History where log_file = '$log_file$' "
        "group by Original_Status order by Original_Status";
    byDetailClause = "select Remote_Host, Request_Time, Method, Resource_Requested, Referer, User_Agent "
        "from  Site_History where  where log_file = '$log_file$' and Remote_Host = '$calling$' "
        "order by Request_Time desc";
    ifstream setUp( "siteStatSetup.xml" );
    if( setUp )
    {
      xmlParser parser( &setUp );
      parser.parse();
      node_vector::iterator pos = parser.findFirstNodeByName( "logfile" );
      if( pos != parser.nodeList().end() )
      {
        log_file = pos->attr["name"];
      }
    }
  }
  ~reporter(){;}
  void referer( cgiScript & script, cgiTemplates & tmplt )
  {
    // referrer clause prework
    string thisSvr = script.ServerName().c_str();

    generic( script, tmplt, refererClause.replace("$log_file$",log_file).replace("$SVR$", thisSvr ) );
  }
  void pages( cgiScript & script, cgiTemplates & tmplt )
  {
    generic( script, tmplt, pageClause.replace("$log_file$",log_file) );
  }
  void monthly_pages( cgiScript & script, cgiTemplates & tmplt )
  {
    generic( script, tmplt, monthlyPageClause.replace("$log_file$",log_file) );
  }
  void daily_pages( cgiScript & script, cgiTemplates & tmplt )
  {
    generic( script, tmplt, dailyPageClause.replace("$log_file$",log_file) );
  }
  void browsers( cgiScript & script, cgiTemplates & tmplt )
  {
    generic( script, tmplt, byBrowserClause.replace("$log_file$",log_file) );
  }
  void request_status( cgiScript & script, cgiTemplates & tmplt )
  {
    generic( script, tmplt, byRequestStatusClause.replace("$log_file$",log_file), &httpstats );
  }
  void generic( cgiScript & script, cgiTemplates & tmplt , string sql, lookups * plk = 0 )
  {
        // To retreive in order of insertion
    mapinserts ordr;
    string sep = tmplt.getParagraph("chartdata");
    ocString label = tmplt.getParagraph("label");
    bool open = rs.open( sql );
    if( !open ) script << "<h3>No Data</h3><!-- " << sql << " -->" << endl;
    while( open )
    {
      string label = rs.getField(0).format();
      if(  plk )
      {
        lookups::iterator pos = plk->find(label);
        if( pos != plk->end() )
        {
          label = pos->second;
        }
      }
      long count = atol(rs.getField(1).format().c_str());
      ordr.push_back(label);
      coco[label] = count;
      maxCount = maxCount>count?maxCount:count;
      open = rs.next();
    }
    // now emit the stuff
    ocString dataEl;
    long inc = indexIncrement( maxCount );
    for( int idx=0; idx<ordr.size(); idx++ )
    {
      string lval = ordr[idx];
      script << label.replace("$label", lval );
      script << sep;
      dataEl = tmplt.getParagraph(tags[idx%tagMax]);
      script << dataEl.replace("$num$", scale( coco[lval], maxCount, 500 ).c_str())
          .replace("$num$", toString(coco[lval]));
    }
    tickMarks(script, tmplt);
  }
  
  void daily_visitors( cgiScript & script, cgiTemplates & tmplt )
  {
    rollup(script, tmplt,dailyVisitsClause.replace("$log_file$",log_file) );
  }
  void monthly_visitors( cgiScript & script, cgiTemplates & tmplt )
  {
    rollup(script, tmplt,monthlyVisitsClause.replace("$log_file$",log_file) );
  }
  void detail( cgiScript & script, cgiTemplates & tmplt)
  {
    cgiInput & args = script.ClientArguments();
    if( args.count("caller") )
    {
      string caller = args["caller"];
      string sql = byDetailClause.replace("$log_file$",log_file).replace("$calling$", caller);
      bool open = rs.open( sql );
      if( !open ) script << "<h3>No Data</h3><!-- " << sql << " -->" << endl;
      while( open )
      {
        // emit here

        open = rs.next();
      } 
    }
  }
  void rollup( cgiScript & script, cgiTemplates & tmplt , string sql)
  {
    // To retreive in order of insertion
    mapinserts ordr;
    string sep = tmplt.getParagraph("chartdata");
    ocString label = tmplt.getParagraph("label");
    bool open = rs.open( sql );
    if( !open ) script << "<h3>No Data</h3><!-- " << sql << " -->" << endl;
    while( open )
    {
      string label = rs.getField(0).format();
      string aggie =  rs.getField(1).format();
      if( aggies.find(label) == aggies.end() )
      {
        ordr.push_back(label);
      }
      
      visitors & lk = aggies[label]; // this will create and empty one if none exists
      addAgregate( lk, aggie );
      maxCount = maxCount>lk.size()?maxCount:lk.size();
      
      open = rs.next();
    }
    // now emit the stuff
    ocString dataEl;
    long inc = indexIncrement( maxCount );
    for( int idx=0; idx<ordr.size(); idx++ )
    {
      string lval = ordr[idx];
      long count = aggies[ lval ].size();
      script << label.replace("$label", lval );
      script << sep;
      dataEl = tmplt.getParagraph(tags[idx%tagMax]);
      script << dataEl.replace("$num$", scale( count, maxCount, 500 ).c_str())
                      .replace("$num$", toString(count));
    }
    tickMarks(script, tmplt);
  }

  void addAgregate( visitors & lk, string val )
  {
    if( lk.find(val) == lk.end() )
    {
      lk.insert( val );
    }
  }
  void tickMarks( cgiScript & script, cgiTemplates & tmplt  )
  {
    long inc = indexIncrement( maxCount );
    script << tmplt.getParagraph("scale");
    ocString tick = tmplt.getParagraph("tickmark");
    for( long incs=inc; incs<=maxCount; incs+=inc )
    {
      script << tick.replace("$num$", scale( inc, maxCount, 500 ).c_str() )
          .replace("$num$", toString(incs).c_str() ) ;
    }
    tick = tmplt.getParagraph("ticklabel");
    script << tick.replace("$label",show);
  }
  // Figure out the power of 10 for the index
  long indexIncrement( double in )
  {
    double lola = log10(fabs(in));
    double lolita = floor(lola);
    double inc = pow( 10.0, lolita );
    if( inc < 1 ) inc = 1;
    return (long) inc;
  }
  string scale( int data, int datamax, int pixels )
  {
    ocString ret;
    ret.append( ((data*pixels)/datamax) );
    return ret;
  }
  string toString( long data )
  {
    ocString ret;
    ret.append(data);
    return ret;
  }
};




 
int main( int argc, char ** argv )
{

  cgiScript script;
  string show,filter;
  cgiTemplates tmplt;
  tmplt.load("adminchart.htmp");  
  script << tmplt.getParagraph("top");

  // Overwrite defaults on show and filter
  cgiInput & args = script.ClientArguments();
  if( args.count("show") ) show = args["show"].c_str();
  if( args.count("filter") ) filter = args["filter"].c_str();  

  // Begin Emmission
  ocString heading = tmplt.getParagraph("charttitle");
  script << heading.replace("$title",show.c_str() );


  // the report aggregator
  reporter rptr;
  rptr.show = show;
  
  // progam the aggregator based on "show"
  if( show == "Daily Pages" )
  {
    rptr.daily_pages( script, tmplt );
  }
  else if( show == "Monthly Pages" )
  {
    rptr.monthly_pages( script, tmplt );
  }
  else if( show == "Daily Visitors" )
  {
    rptr.daily_visitors( script, tmplt );
  }
  else if( show == "Monthly Visitors" )
  {
    rptr.monthly_visitors( script, tmplt );
  }
  else if( show == "Page" )
  {
    rptr.pages( script, tmplt );
  }
  else if( show == "Referrer" )
  { 
    rptr.referer( script, tmplt );
  }
  else if( show == "Request Status" )
  {
    rptr.request_status( script, tmplt );
  }
  else if( show == "Browser" )
  {
    rptr.browsers( script, tmplt );
  }
  else if( show == "detail" )
  {
    rptr.detail( script, tmplt );
  }

  script << tmplt.getParagraph("end");
  return 0;
}

