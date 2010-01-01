#include <fstream>
#include <iomanip>
#ifndef openLogger_h
#define openLogger_h


#ifdef DO_OPEN_LOGGING
#define writelog( var ) { cerr << __FILE__ << " - " << __LINE__ << " : " << var << endl; cerr.flush(); }
#define writelog2( var1, var2 ) { cerr << __FILE__ << " - " << __LINE__ << " : " << var1 << " " << var2 << endl; cerr.flush(); }
#define writelog3( var1, var2, var3 ) { cerr << __FILE__ << " - " << __LINE__ << " : " << var1 << " " << var2 << " " << var3 << endl; cerr.flush(); }
#else
#define writelog( var )
#define writelog2( var1, var2 )
#define writelog3( var1, var2, var3 )
#endif

#endif
