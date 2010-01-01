#include <time.h>
#include <cstdlib>
#include <memory.h>
tm * parseTime( const char * strTime, const char * strFormat, tm * t )
{
  memset( t, 0, sizeof(t) );
  strptime(strTime, strFormat, t);
  return t;
}

time_t fixTime(struct tm *tp)
{
  return mktime(tp);
}

size_t fmtTime(char * s, size_t max, const char *format, const struct tm *tp)
{
  return strftime(s, max, format, tp);
}

tm * currentTime( tm* t )
{
  time_t lt;
  time(&lt);
  localtime_r(&lt,t);
  return t;
}
