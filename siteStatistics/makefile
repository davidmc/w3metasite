# this is the makefile
include ../build.mak

# all entries

all: slurpy  adminLogs.cgi

#clean
clean:
	rm *.o
	rm slurpy
	rm adminLogs.cgi

# link entries
slurpy: slurpy.o
	$(CC) -o slurpy  slurpy.o $(DATALIB) $(MLIB)

adminLogs.cgi: adminLogs.o 
	$(CC) -o adminLogs.cgi adminLogs.o  $(CGILIB) $(DATALIB) $(MLIB) 

# compile entries
slurpy.o: slurpy.cpp Site_History.hpp 
	$(CC) -c $(DATAINC) $(CGIINC) slurpy.cpp
	
 adminLogs.o: adminLogs.cpp
	$(CC) -c $(DATAINC) $(CGIINC)  adminLogs.cpp  $(IMPL_DEFS)