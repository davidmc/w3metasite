/*
  cgiCalendar.cpp

  Common Gateway Interface Class Implementations.
     Extended classes for cgi support
     -- generic calendar container
  copyright (c) 2002

  w3sys.com - Tunnel Hill, Ga.

  David McCombs davidmc@w3sys.com

  Nuclear Core Class Library

  freely re-usable, redistributable.

*/

#include "cgiCalendar.h"
#include "../openData/fmtTime.h"
#include <cstdlib>
#include <cstdio>
#include "ocString.h"
#include "ocTypes.h"

/*
  A generic html calendar
*/

cgiCalendar::cgiCalendar( aString & year, aString & month, aString day /* = "" */ )
:cgiCan( "table", " class='calendar'" ),headerPos(posTop),isDayMode(false)
,dayTemplate("<div class='day'>$day</div>"),timeTemplate("<div class='time'>$time</div>"),iHourStart(6),iHourEnd(22)
{
  char buffer[256];
  const char * DateFmt = "%Y-%m-%d";
  aString strDate;
  if(year.length()>0 && month.length()>0)
  {
    // construct the time from passed arguments
    memset( &m_tm, 0, sizeof(tm) );
    m_tm.tm_year = atol( year.str() ) - 1900;
    m_tm.tm_mon = atol( month.str() ) -1;
    m_tm.tm_mday = 1;
    if( day.length() )
    {
      m_tm.tm_mday = atol( day.c_str() );
      isDayMode = true;
    }    
    fixTime(&m_tm);
  }
  else
  {
    // assume current time
    currentTime(&m_tm);
    m_tm.tm_mday = 1;
    fixTime(&m_tm);
  }
  // default header for weekdays
  weekDayHeader = "<tr class='weekdays'><th class='weekday'>Sunday</th>"
                  "<th class='weekday'>Monday</th><th class='weekday'>Tuesday</th>"
                  "<th class='weekday'>Wednesday</th><th class='weekday'>"
                  "Thursday</th><th class='weekday'>Friday</th><th class='weekday'>Saturday</th></tr>";

}

void cgiCalendar::WeekDayHeader( string newWeekDayHeader )
{
  weekDayHeader = newWeekDayHeader;
}

bool cgiCalendar::fill( timeItems & items, const char * url /* = NULL*/ )
{ char buffer[512];
  bool bRet = false;
  cgiCan & rCan = *this;
  /*
  <tr class='month'><th colspan='2'>
  <a href='?day=31&month=01&year=2003'>&nbsp;&lt;&lt;&nbsp;</a> &nbsp;&nbsp;&nbsp;Sunday February 1, 2004 &nbsp;&nbsp;&nbsp; <a href='?day=2&month=03&year=2004'>&nbsp;&gt;&gt;&nbsp;</a>
  </th></tr>

  <tr class='odd'><th class='odd'>1:00 AM<td class='odd'>
  </td>
  <tr class='even'><th class='even'>1:30 AM<td class='even'>
  </td>
  */
  header = "<tr class='month'><th colspan='2'>";
  if( url )
  {
    header += "<a href='";
    header += url;
    header += "?";
    tm prevTm = m_tm;
    fixTime(&prevTm);
    fmtTime(buffer, sizeof(buffer), "month=%m&year=%Y", &prevTm );
    header += buffer;
    header += "'> &nbsp;&nbsp;&nbsp;";
    fmtTime(buffer, sizeof(buffer), "%A %B %d, %Y", &prevTm );;
    header += buffer;
    header += " &nbsp;&nbsp;&nbsp;</a>";
  }
  if(  headerPos == posTop || headerPos == posBoth )
  {
    rCan << header << endl;
  }
  
  bRet = fillHalfHours( items );
  if(  headerPos == posBottom || headerPos == posBoth )
  {
    rCan << header << endl;
  }
  return bRet;
} 
bool cgiCalendar::fillHalfHours ( timeItems & items )
{ bool bRet = false;
  char buffer[256];
  for( int iHour=iHourStart; iHour<=iHourEnd; iHour++)
  {    
    snprintf(buffer, sizeof(buffer), "%.2d:00", iHour);
    string key = buffer;
    fillHalfHour( items, key );
    snprintf(buffer, sizeof(buffer), "%.2d:30", iHour);
    key = buffer;
    fillHalfHour( items, key );
  }
  return bRet;
}
bool cgiCalendar::fillHalfHour ( timeItems & items, string & key ) 
{ bool bRet = false;
  cgiCan & rCan = *this;
  string strClass;
  ocString test = key;
  if( test.regExMatch(":00") ) strClass = "even"; else strClass = "odd";
  rCan << "<tr class='" << strClass << "'>";
  time_date td;
  td.parse(key.c_str(),"%H:%M");
  ocString timeTemp = timeTemplate.c_str();
  rCan << "<tr class='" << strClass 
       << "'><th class='" << strClass 
       << "'>" << timeTemp.replaceAll("$time24",key.c_str())
                          .replaceAll("$time",td.format( "%I:%M %p" ).c_str()) 
       << "<td class='" << strClass 
       << "'>" << endl;
  timeItems::iterator pos;
  for( pos = items.lower_bound(key);
      pos != items.upper_bound(key);
      ++pos )
  {
    rCan << "  " << pos->second << "<br>" << endl;
  }
  rCan << "</td></tr>" << endl;     
  return bRet;
}
bool cgiCalendar::fill( dayItems & items, const char * url /* = NULL*/)
{
    char buffer[256];
    bool ret = false;
    cgiCan & rCan = *this;
    header = "<tr class='month'><th colspan='7'>";
    if( url )
    {
      header += "<a href='";
      header += url;
      header += "?";
      tm prevTm = m_tm;
      prevTm.tm_mon -= 1;
      fixTime(&prevTm);
      fmtTime(buffer, sizeof(buffer), "month=%m&year=%Y", &prevTm );
      header += buffer;
      if( addedCriteria.length() )
      {
        header += "&" + addedCriteria;
      }
      header += "'>&nbsp;&lt;&lt;&nbsp;</a>";

    }

    fmtTime(buffer, sizeof(buffer), "%B %Y", &m_tm );
    header += " &nbsp;&nbsp;&nbsp; ";
    header += buffer;
    header += " &nbsp;&nbsp;&nbsp; ";
    if( url )
    {
      header += "<a href='";
      header += url;
      header += "?";
      tm nextTm = m_tm;
      nextTm.tm_mon += 1;
      fixTime(&nextTm);
      fmtTime(buffer, sizeof(buffer), "month=%m&year=%Y", &nextTm );
      header += buffer;
      if( addedCriteria.length() )
      {
        header += "&" + addedCriteria;
      }
      header += "'>&nbsp;&gt;&gt;&nbsp;</a></th></tr>";
    }

    if(  headerPos == posTop || headerPos == posBoth )
    {
      rCan << header << endl;
    }
    rCan << weekDayHeader << endl;
    
    ret = fillDays( items );

    if(  headerPos == posBottom || headerPos == posBoth )
    {
      rCan << header << endl;
    }
    return ret;
  }

bool cgiCalendar::fillDays ( dayItems & items )
{
  char classBuf[256];
  cgiCan & rCan = *this;
  bool bMoreWeeks = true;
  int iDay, iWeek=1, iMonth = m_tm.tm_mon;
  while( bMoreWeeks && m_tm.tm_mday <= 31)
  {
    if( iWeek%2==0 ) strcpy(classBuf,"even");
    else strcpy(classBuf,"odd");
    rCan << "<tr class='" << classBuf << "'>";
    for( iDay = 0; iDay < m_tm.tm_wday; iDay++ )
    {
      rCan << "<td class='empty'>&nbsp;</td>";
    }
    for( ; iDay<7 && bMoreWeeks; iDay++ )
    {
      aString dayTemp = dayTemplate;
      ocString dayVal;
      dayVal.append(m_tm.tm_mday);
      dayTemp.replaceFoundWith("$day", dayVal.c_str());
      rCan << "<td class='mday'>" << dayTemp;
      fillDay( items );
      rCan << "</td>";
      bMoreWeeks = dayAdd();
    }
    for( ; iDay<7; iDay++ )
    {
      rCan << "<td class='empty'>&nbsp;</td>";
    }
    rCan << "</tr>" << endl;
    iWeek ++;
  }
  return true;
}
bool cgiCalendar::fillDay ( dayItems & items )
{
  cgiCan & rCan = *this;
  // find all day items matching th current day
  dayItems::iterator pos;
  for( pos = items.lower_bound(m_tm.tm_mday);
      pos != items.upper_bound(m_tm.tm_mday);
      ++pos )
  {
    rCan << pos->second << "<br>";
  }
  return true;
}

bool cgiCalendar::dayAdd( void )
{
  bool bMoreWeeks = false;
  int iMonth = m_tm.tm_mon;
  m_tm.tm_mday+=1;
  long lRet = fixTime(&m_tm);
  if ( lRet != -1 )
  {
    bMoreWeeks = (iMonth == m_tm.tm_mon);
  }
  return bMoreWeeks;
}

string & cgiCalendar::strYear( void )
{
  char buffer[256];
  fmtTime(buffer, sizeof(buffer), "%Y", &m_tm );
  m_formatted = buffer;
  return m_formatted;
}

string & cgiCalendar::strMonth( void )
{
  char buffer[256];
  fmtTime(buffer, sizeof(buffer), "%m", &m_tm );
  m_formatted = buffer;
  return m_formatted;
}

int cgiCalendar::year ( void )
{
  return 1900 + m_tm.tm_year;
}

int cgiCalendar::month( void )
{
  return 1 + m_tm.tm_mon;
}

cgiCalendar::~cgiCalendar()
{
  ;
}

