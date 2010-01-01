# GENERAL PURPOSE DEFINITIONS
include ../../build.mak


all: metasiteImport metasiteExport

#
# Link applications
#

metasiteImport:  metasiteImport.o $(DATALIB) $(CGILIB)
	$(CC) -o metasiteImport  metasiteImport.o $(DATALIB) $(CGILIB) $(MLIB) -luuid

metasiteExport:  metasiteExport.o $(DATALIB) $(CGILIB)
	$(CC) -o metasiteExport  metasiteExport.o $(DATALIB) $(CGILIB) $(MLIB) -luuid
    
#
# Compile objects
#

metasiteImport.o: metasiteImport.cpp $(INCLDIR)ocImportParser.h  SysImportFixup.h
	$(CC) -c  $(DATAINC) $(CGIINC) $(IMPL_DEFS) metasiteImport.cpp

metasiteExport.o: metasiteExport.cpp $(INCLDIR)ocImportParser.h  SysImportFixup.h
	$(CC) -c  $(DATAINC) $(CGIINC) $(IMPL_DEFS) metasiteExport.cpp

