#ifndef OC_TYPES_H
#define OC_TYPES_H

#include <cstdlib>
#include <memory.h>
#include <sys/types.h>
#include <locale.h>
#include <time.h>
#include <monetary.h>
#include <math.h>
#include "ocString.h"
using namespace std;
/*
  class time_date:
  inherits tm:
    int tm_sec;    Seconds. [0-60] (1 leap second)
    int tm_min;    Minutes. [0-59]
    int tm_hour;   Hours. [0-23]
    int tm_mday;   Day.  [1-31]
    int tm_mon;    Month. [0-11]
    int tm_year;   Year - 1900.
    int tm_wday;   Day of week. [0-6]
    int tm_yday;   Days in year.[0-365]
    int tm_isdst;  DST.  [-1/0/1]
*/

class time_date: public tm
{
  bool good_parse; 
public:
  time_date & now( void )
  {
    time_t lt;
    time(&lt);
    localtime_r(&lt,this);
    return *this;  
  }
  time_date():tm(),good_parse(false)
  {
    now();
  }
  time_date( const time_date & in )
    :tm(dynamic_cast< const tm & >(in)),good_parse(in.good_parse)
  {;}
  time_date( const tm & in )
    :tm(dynamic_cast< const tm & >(in)),good_parse(false)
  {;}
  time_date & operator = ( const time_date in )
  {
    good_parse = in.good_parse;
    dynamic_cast< tm & >( *this ) = dynamic_cast< const tm & >(in);
    return * this;
  }
  time_date & operator = ( const tm & in )
  {
    dynamic_cast< tm & >( *this ) = in;
    return *this; 
  }
  virtual ~time_date(){;}  
 
  int addSeconds( int seconds )
  {
    return (tm_sec+=seconds);
  }
  int addMinutes( int minutes )
  {
    return (tm_min+=minutes);
  }
  int addHours( int hours )
  {
    return (tm_hour+=hours);
  }  
  int addDays( int days )
  {
    return (tm_mday+=days);
  }
  int addMonths( int months )
  {
    return (tm_mon+=months);
  }
  int addYears( int years )
  {
    return (tm_year+=years)+1900;
  }
  time_t normalize( void )
  {
    return mktime( & dynamic_cast< tm & >( *this ));
  }
  /* 
    Typical format specifiers:
    %m := month[01,12], %M := minute[[00,59]]
    %a := abreviated weekday name, %A := full weekday anme
    %b := abbreviated month name, %B := full month name.
    %c := locale date, %d := day of month[01,31] 
    %p := [a.m.,p.m.] 
    %H := 24 hour hour[00,23], %I := 12 hour hour[01,12]
    %M := minutes
    %S := seconds[00,61]
    %y := 2 digit year, %Y := 4 digit year     
  */  
  string format( const char *format, size_t max = 256) const
  {
    char * temp = new char[max+1];
    strftime(temp, max, format, this);
    string strTemp = temp;
    delete [] temp;
    return strTemp;
  }    
  time_date & parse( const char * strTime, const char * strFormat )
  {
    *this=tm();
    good_parse = strptime(strTime, strFormat, this) != 0;
    return *this;
  }

  // encapsulated get access
  int seconds(void)      { return tm_sec; }   //  Zero based Seconds. [0-60] (1 leap second) 
  int minutes(void)      { return tm_min; }   //  Zero based Minutes. [0-59]
  int hour(void)         { return tm_hour; }  //  Zero based Hour of day. [0-23]
  int clock_hour(void)   { return (tm_hour%12==0?12:tm_hour%12); } // hours [1-12]
  char clock_period(void) { return tm_hour>11?'P':'A'; } // morning or evening [A,P]
  int day_of_month(void) { return tm_mday; }  //  Day of Month.  [1-31]
  int month(void)        { return tm_mon+1; } //  Month. [1-12]
  int year(void)         { return tm_year+1900; } //  Year.
  int day_of_week(void)  { return tm_wday; }  // Day of week. [0-6]
  int day_of_year( void ){ return tm_yday; }  //  Days in year.[0-365]
  int is_DST(void)       { return tm_isdst; } //  DST.  [-1 == indeterminant/0=no/1=true]   
  // encapsulated set access
  void seconds(int in)      { tm_sec=in; }   //  Zero based Seconds. [0-60] (1 leap second) 
  void minutes(int in)      { tm_min=in; }   //  Zero based Minutes. [0-59]
  void hour(int in)         { tm_hour=in; }  //  Zero based Hour of day. [0-23]
  void day_of_month(int in) { tm_mday=in; }  //  Day of Month.  [1-31]
  void month(int in)        { tm_mon=in-1; } //  Month. [1-12]
  void year(int in)         { tm_year=in-1900; }    //  Year.  
  void is_DST(int in)       { tm_isdst=in; } //  DST.  [-1 == indeterminant/0=no/1=true]

  bool is_good_parse(void){return good_parse;}
};

// specializations
class oc_date : public time_date
{
  string fmt; 
public:
  oc_date():time_date(),fmt("%m/%d/%Y"){;}
  oc_date( const time_date & in ):time_date(in),fmt("%m/%d/%Y"){;}
  oc_date( const tm & in ):time_date(in),fmt("%m/%d/%Y"){;}
  oc_date & operator = ( const tm & in )
  {
    dynamic_cast< tm & >( *this ) = in;
    return *this;
  }
  oc_date & operator = ( const oc_date & in )
  {
    dynamic_cast< tm & >( *this ) = dynamic_cast< const tm & >(in);
    return *this;
  }
  ~oc_date(){;}
  string format ( size_t max = 256) const
  {
    return time_date::format( fmt.c_str(), max );
  }
  oc_date & setFormat( string fmtIn )
  {
    fmt = fmtIn;
    return *this;
  }
  oc_date & parse( const char * strDate )
  {
    ocString newDate = strDate;
    time_date::parse( newDate.replaceAll("-","/").c_str(), fmt.c_str() ); 
    return *this;
  }
};

class oc_time : public time_date
{
  string fmt; 
public:
  oc_time():time_date(),fmt("%I:%M %p"){;}
  oc_time( const time_date & in ):time_date(in),fmt("%I:%M %p"){;}
  oc_time( const tm & in ):time_date(in),fmt("%I:%M %p"){;}
  oc_time & operator = ( const tm & in )
  {
    dynamic_cast< tm & >( *this ) = in;
    return *this;
  }
  ~oc_time(){;}
  
  oc_time & milTime(void) { fmt="%H:%M:%S"; return *this;}
  oc_time & milTimeNoSec(void) { fmt="%H:%M"; return *this;}
  oc_time & noSec(void) { fmt="%I:%M %p"; return *this; }
  oc_time & withSec(void) { fmt="%I:%M:%S %p"; return *this;}
  oc_time & setFormat( string fmtIn )
  {
    fmt = fmtIn;
    return *this;
  }
  string format ( size_t max = 256) const
  {
    return time_date::format( fmt.c_str(), max );
  }    
  oc_time & parse( const char * strDate )
  {
    ocString newDate = strDate;
    time_date::parse( newDate.c_str(), fmt.c_str() );
    // It wont normalize this way, it needs more info, but
    // we don't care what the date is, just give it something say; april fools...
    day_of_month(4); month(1); year(2006);
    return *this;
  }
};

inline std::ostream& operator << (std::ostream & strm, const oc_date & theDate )
{
  strm << theDate.format();
  return strm;
}
inline std::ostream& operator << (std::ostream & strm, const time_date & theTimeDate )
{
  strm << theTimeDate.format("%m/%d/%Y  %H:%M");
  return strm;
}
inline std::ostream& operator << (std::ostream & strm, const oc_time & theTime )
{
  strm << theTime.format();
  return strm;
}

typedef class money
{
  double m_d;
  void localeSet( void )
  {
    localeconv(); //initialize the locale struct
    setlocale( LC_MONETARY, "en_US" ); // make sure its set for english US  
  }
public:
  // double m_d;
  money():m_d()
  { localeSet(); }
  ~money()
  { localeSet(); }
  money(const money & in):m_d(in.m_d)
  { localeSet(); }
  money(double in):m_d(in)
  { localeSet(); }
  money & operator = (const money & in)
  {
    m_d = in.m_d;
  }
  money & operator = (const double & in)
  {
    m_d = in;
  }
  /*
    Format specifiers: expression preceded by %
    =f  := specify fill character 'f' 
    ^   := no monetary grouping
    w   := justified field width
    #n  := whole precision
    .p  := fractional precision
    n   := default format for locale   
    !   := suppress currency symbol
    -   := left align
  */
  string format ( const char * spec, size_t max = 256 ) const
  {    
    char * temp = new char[max+1];
    strfmon(temp, max, spec, m_d+0.000001); // consistantly round up when at 0.055
    string strTemp = temp;
    delete [] temp;
    return strTemp;  
  }
  long whole( void )
  {
    long l = long();
    double w;
    double f = modf(m_d, &w);
    if( !isnan(w)) l = static_cast<long>(w);
    return l;
  }  
  long fractional( void )
  {
    long l = long();
    double w;
    double f = modf(m_d, &w);
    if( !isnan(f)) 
    {
      // this has a math lib bug, commented out till fsf fixes it
      // l = static_cast<long>(f*100.00);
      
      // a workaround...
      char buf[8];
      f*=100;
      sprintf(buf,"%.2f",fabs(f));
      l = atol(buf);      
    }
    return l;    
  }
  double & amount(void) { return m_d; }
};

inline std::ostream& operator << (std::ostream & strm, const money & mony )
{
  strm << mony.format("%n");
  return strm;
}
#endif
#ifdef IN_T2_TESTHARNESS
/* test harness */

{

  time_date td;
  cout << "NOW" << endl;
  cout << td << endl;
  
  td.addDays(300);
  td.normalize();
  cout << "NORMALIZED NOW + 300 DAYS" << endl;
  cout << td << endl;
  td.addMonths(15);
  td.normalize();
  cout << "NORMALIZED NOW + 15 MONTHS, 300 DAYS" << endl;
  cout << td << endl;  
  tm t = tm();  // zeroed struct
  td = t;
  cout << "TIME ZERO: " << endl;
  cout << td << endl;    
  td.year(2007);
  td.normalize();
  cout << "NORMALIZED TIME Y2007: " << endl;
  cout << td << endl;
  td.addDays(-3);
  td.normalize();
  cout << td << endl;
  cout << "day of week: " << td.day_of_week() << endl;
  td.addMonths(2);
  td.normalize();
  cout << td << endl;
  cout << "sequence day of week: " << td.day_of_week() << endl;
  td.addDays(1);
  td.normalize();
  cout << td << endl;
  cout << "sequence day of week: " << td.day_of_week() << endl;  
  td.addDays(1);
  td.normalize();
  cout << td << endl;
  cout << "sequence day of week: " << td.day_of_week() << endl;  
  td.addDays(1);
  td.normalize();
  cout << td << endl;
  cout << "sequence day of week: " << td.day_of_week() << endl;    
  td.addDays(1);
  td.normalize();
  cout << td << endl;
  cout << "sequence day of week: " << td.day_of_week() << endl;    
  td.addDays(1);
  td.normalize();
  cout << td << endl;
  cout << "sequence day of week: " << td.day_of_week() << endl;
  cout << td << endl;
  td.addYears(1);
  td.normalize();
  cout << td << endl;
  cout << "addYears(1) day of week: " << td.day_of_week() << endl;
  cout << td << endl;
  td.addYears(1);
  td.normalize();
  cout << td << endl;
  cout << "addYears(1) day of week: " << td.day_of_week() << endl;
  cout << td << endl;
  td.now();
  td.normalize();
  cout << td << endl;
  cout << "now() day of week: " << td.day_of_week() << endl;
  cout << td << endl;
  td.addDays(1);
  td.normalize();
  
  cout << td << endl;  
  td.addDays(1);
  td.normalize();
  cout << td << endl;
  cout << "addDays(1) day of week: " << td.day_of_week() << endl;

  td.addDays(-1);
  td.normalize();
  cout << td << endl;
  cout << "addDays(-1) day of week: " << td.day_of_week() << endl;
  

  td.addDays(1);
  td.normalize();
  cout << td << endl;
  cout << "addDays(1) day of week: " << td.day_of_week() << endl;
 

  td.addDays(-7);
  td.normalize();
  cout << td << endl;
  cout << "addDays(-7) day of week: " << td.day_of_week() << endl;
 

  td.addDays(1);
  td.normalize();
  cout << td << endl;
  cout << "addDays(1) day of week: " << td.day_of_week() << endl;
 

  td.addDays(1);
  td.normalize();
  cout << td << endl;
  cout << "addDays(1) day of week: " << td.day_of_week() << endl;
 

  td.addDays(1);
  td.normalize();
  cout << td << endl;
  cout << "addDays(1) day of week: " << td.day_of_week() << endl;
 

  td.addDays(1);
  td.normalize();
  cout << td << endl;
  cout << "addDays(1) day of week: " << td.day_of_week() << endl;
 

  td.addDays(1);
  td.normalize();
  cout << td << endl;
  cout << "addDays(1) day of week: " << td.day_of_week() << endl;
 
 
  td.addDays(1);
  td.normalize();
  cout << td << endl;
  cout << "addDays(1) day of week: " << td.day_of_week() << endl;
 
 
  td.addDays(1);
  td.normalize();
  cout << td << endl;
  cout << "addDays(1) day of week: " << td.day_of_week() << endl;
 

  td.addDays(1);
  td.normalize();
  cout << td << endl;
  cout << "day of week: " << td.day_of_week() << endl;
                     
  td.addSeconds(-12);
  td.normalize();
  cout << "LAST TIME -12 seconds : " << endl;
  cout << "Time: " << td.hour() << ":" << td.minutes() << ":" << td.seconds() << endl;
  cout << "Date: " << td.year() << "-" << td.month() << "-" << td.day_of_month() << endl;
  cout << "Clock Time: " << td.clock_hour() << ":" << td.minutes() << " " 
       << td.clock_period() << "M." << endl;  
  td.addHours(7);
  td.normalize();
  cout << "LAST TIME +7 hours : " << endl;
  cout << "Time: " << td.hour() << ":" << td.minutes() << ":" << td.seconds() << endl;
  cout << "Date: " << td.year() << "-" << td.month() << "-" << td.day_of_month() << endl;
  cout << "Clock Time: " << td.clock_hour() << ":" << td.minutes() << " " 
       << td.clock_period() << "M." << endl;    
  td.addHours(7);
  td.normalize();
  cout << "LAST TIME +7 hours : " << endl;
  cout << "Time: " << td.hour() << ":" << td.minutes() << ":" << td.seconds() << endl;
  cout << "Date: " << td.year() << "-" << td.month() << "-" << td.day_of_month() << endl;
  cout << "Clock Time: " << td.clock_hour() << ":" << td.minutes() << " " 
       << td.clock_period() << "M." << endl;   
  td.addHours(7);
  td.normalize();
  cout << "LAST TIME +7 hours : " << endl;
  cout << "Time: " << td.hour() << ":" << td.minutes() << ":" << td.seconds() << endl;
  cout << "Date: " << td.year() << "-" << td.month() << "-" << td.day_of_month() << endl;
  cout << "Clock Time: " << td.clock_hour() << ":" << td.minutes() << " " 
       << td.clock_period() << "M." << endl;   
  td.addHours(7);
  td.normalize();
  cout << "LAST TIME +7 hours : " << endl;
  cout << "Time: " << td.hour() << ":" << td.minutes() << ":" << td.seconds() << endl;
  cout << "Date: " << td.year() << "-" << td.month() << "-" << td.day_of_month() << endl;
  cout << "Clock Time: " << td.clock_hour() << ":" << td.minutes() << " " 
       << td.clock_period() << "M." << endl;    
  td.now();
  cout << "NOW" << endl;
  cout << "Time: " << td.hour() << ":" << td.minutes() << ":" << td.seconds() << endl;
  cout << "Date: " << abs(td.year()) << "-" << td.month() << "-" << td.day_of_month() << endl;
  cout << "Clock Time: " << td.clock_hour() << ":" << td.minutes() << " " 
       << td.clock_period() << "M." << endl; 
  string strDT = td.format( "%m/%d/%Y  %I:%M:%S %p" );
  cout << strDT << endl;  
  td = t;
  
  // Fails! normalize 'mktime' fails before 1970!
  td.day_of_month(210);
  td.year(1960);
  td.month(1);
  td.normalize();
  cout << "TIME ZERO: " << td.format( "%m/%d/%Y  %I:%M:%S %p" ) << endl;
  td.parse(strDT.c_str(),"%m/%d/%Y  %I:%M:%S %p");
  cout << "TIME RESTORED: " << td.format( "%m/%d/%Y  %I:%M:%S %p" ) << endl;

  // test parse failures & successes
  string testDate = "2006-12-31";
  cout << "test parse failures & successes for : " << testDate << endl;
  td.parse(testDate.c_str(),"%m-%d-%Y");
  cout << "Parse Success for %m-%d-%Y? " << td.is_good_parse() << endl;
  
  if( !td.is_good_parse() )
  {
    cout << "Try alternate (working) parse FMT: " << endl;
    td.parse(testDate.c_str(),"%Y-%m-%d");
    cout << "Parse Success for %Y-%m-%d? " << td.is_good_parse() << endl;
    cout << "DATE: " << td.format( "%m/%d/%Y" ) << endl;
  }
  else
  {
    cout << "AHRGGG!*&*(&() Parse Success when it SHOULDN'T" << endl;
  }


  // money type
  money m = 723.44;
  cout << "MONEY: " << m.format("%n") << endl;
  cout << " WHOLE: " << m.whole()  << " FRACTIONAL: " << m.fractional() << endl;
  m.amount()+=5612.33;
  cout << "MONEY: " << m.format("%n") << endl;
  money more_money;
  more_money = 2345.43;
  cout << "MORE MONEY: " << more_money.format("%n") << endl;
  m.amount() *= more_money.amount();
  cout << "MONEY X MORE MONEY: " << m.format("%n") << endl;
  
  
  oc_date myDate;
  cout << "current date: " << myDate << endl;
  myDate.parse("10/27/1972");
  cout << "birth date: " << myDate << endl;
  myDate.addDays(5);
  myDate.normalize();
  cout << "five days after birthday: " << myDate << endl;
  
  // money type
  cout << "Testing money again for neg value " << -23.99 << endl;
  money mny = -23.99;
  cout << " WHOLE: " << mny.whole()  << " FRACTIONAL: " << mny.fractional() << endl;
  double dt = -23.05;
  cout << "Testing money again for neg value " << dt << endl;
  mny = dt;
  cout << " WHOLE: " << mny.whole()  << " FRACTIONAL: " << mny.fractional() << endl;
  
  dt = 23.05;
  cout << "Testing money again for pos value " << dt << endl;
  mny = dt;
  cout << " WHOLE: " << mny.whole()  << " FRACTIONAL: " << mny.fractional() << endl;
  
  // Stress testing for specific problems in production on oc_date
  cout << endl << "Testing for specific problems in production on oc_date" << endl;

  oc_date baddt;
  for(int xx=0;xx<10;xx++)
  {
    baddt.parse("01/27/2006");
    cout << "BAD DATE 1st:" <<  baddt << endl;
    baddt.normalize();
    cout << "BAD DATE normalized:" <<  baddt << endl;
    for(int x=0; x<35; x++ )
    {
      baddt.addDays(1);
      cout << "BAD DATE day added:" <<  baddt << endl;
      baddt.normalize(); 
      cout << "BAD DATE normalized:" <<  baddt << endl;
    }    
  }
}  
  // Testing new oc time 4/14/06  
  oc_time testTime;
  cout << "test time instance:" <<  testTime << endl;
  cout << "try to break it" << endl;
  testTime.parse("44:23:22");
  cout << "test time after parse 44:23:22:" <<  testTime << endl;
  testTime.normalize();
  cout << "test time after parse 44:23 22:" <<  testTime << endl;

  testTime.parse("12:23:22 am");
  cout << "test time after parse 12:23:22 am:" <<  testTime << endl;

  testTime.parse("12:23 am");
  cout << "test time after parse 12:23 am:" <<  testTime << endl;
  cout << "milTime: " << testTime.milTime() << endl;



  for(int h=0;h<24;h++)
  {
    cout << "hour " << h <<  " ";
    testTime.addHours(1);
    time_t t = testTime.normalize();
    
    cout << t << " time: " << testTime << " ";
  }
  for(int m=0;m<240;m++)
  {
    cout << "minute " << m << " ";
    testTime.addMinutes(m);
    time_t t = testTime.normalize();
    cout << t << " time: " << testTime << " ";
  }

  oc_time t1Time;
  t1Time.setFormat("%I:%M %p");
  t1Time.parse( "11:34 AM" );
  t1Time.normalize();
  
  cout <<  t1Time.time_date::format("%I:%M %p")  << " should be 11:34 AM" << endl;
  
  cout << "testing time_date" << endl;
  
  time_date Request_Time;
  cout << "before parse " << Request_Time << " next parse 10/27/1957 12:33 " << endl;
  string fmt = "%m/%d/%Y %H:%M";
  string val = "10/27/1957 12:33";
  Request_Time.parse(val.c_str(),fmt.c_str());
  Request_Time.normalize();
  cout << "x good parse? " << Request_Time.is_good_parse() << " format " << Request_Time.format( "%d/%b/%Y %T" ) << endl;
  
  oc_date addt; 
  cout << "before parse " << addt << endl;
  addt.parse( "10/27/1997" );
  cout << "x good parse? " << addt.is_good_parse() << " format " << addt << endl;


/* end test harness */
#endif


