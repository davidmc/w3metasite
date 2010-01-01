#ifndef FMT_TIME_H
#define FMT_TIME_H
tm * parseTime( const char * strTime, const char * strFormat, tm * t );

time_t fixTime(struct tm *tp);

size_t fmtTime(char * s, size_t max, const char *format, const struct tm *tp);

tm * currentTime( tm* t );

#endif // FMT_TIME_H
