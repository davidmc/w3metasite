/*
  cgiCalendar.h

  Common Gateway Interface Class Definitions.
     Extended classes for cgi support
     -- generic calendar container
  copyright (c) 2002

  Open Connections LLC, Dalton, Ga.

  David McCombs davidmc@newcottage.com

  Nuclear Core Class Library

  freely re-usable, redistributable.

*/
#ifndef cgiCALENDAR_H
#define cgiCALENDAR_H
#include <ctime>
#include "cgiExtend.h"
#include <map>

/*
  dayItems
  --------
  A multimap of day of month integer value and itmes to put on the particular day.
  This used used to fill the calendar with items on particular days.

*/
typedef multimap< int, string > dayItems;
typedef multimap< string, string > timeItems;

typedef enum { posTop, posBottom, posBoth } eHeaderPos;
/*
  A generic html calendar

*/

class cgiCalendar: public cgiCan
{
protected:

  tm     m_tm;
  string m_formatted;
  string header;  
  eHeaderPos headerPos;
  string weekDayHeader;
  bool isDayMode;

  bool fillDays ( dayItems & items );
  bool fillDay ( dayItems & items );
  
  bool fillHalfHours ( timeItems & items );
  bool fillHalfHour ( timeItems & items, string & key);
  
  bool dayAdd( void );

public:
  /// for changing the day number on the (month view) calendar - default:   <div class='day'>$day</div>
  aString dayTemplate;
  /// for changing the time on the (day view) calendar - default: <div class='time'>$time</div>
  /// also, additional replacement key $time24
  aString timeTemplate;
  // the day view starting hour - default: 6
  int iHourStart;
  // the day view ending hour - default: 22
  int iHourEnd;
  
  string addedCriteria;
  
  cgiCalendar( aString & year, aString & month, aString day = "" );
  virtual ~cgiCalendar();

  inline cgiCalendar & setHeaderPos( eHeaderPos inPos )
  {
    headerPos =  inPos;
    return *this;
  }
  bool fill( dayItems & items, const char * url = NULL );
  bool fill( timeItems & items, const char * url = NULL );  
  string & strYear( void );
  string & strMonth( void );
  int year ( void );
  int month( void );
  void WeekDayHeader( string newWeekDayHeader );
};


#endif
