# GENERAL PURPOSE DEFINITIONS
MLIBDIR=/usr/lib/mysql/
INCLUDEDIR=/usr/include/mysql/
INCLDIR=$(HOME)/development/include/
OBJDIR=$(HOME)/development/objects/
LIBDIR=/usr/lib/
FORMDIR=../../openForms/
ODD=$(HOME)/development/objects/mySQL/
OTD=$(HOME)/development/openTools/
OMD=$(HOME)/development/openMailer/
OPD=$(HOME)/development/openMerchant/
OAD=$(HOME)/development/openAdmin/
LDDIR=./
MLIB=-L$(MLIBDIR) -lmysqlclient -lz

CC=g++
# debug with CC=g++ -g

# LINK AND PATH DEFS FOR CGI AND DATA LIBS
DATALIB=$(ODD)openMyDB.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)fmtTime.o
CGILIB=$(OBJDIR)Astring.o $(OBJDIR)cgiClass.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
DATAINC=-I$(INCLUDEDIR) -I$(OTD) -I$(INCLDIR)
CGIINC= -I$(FORMDIR)  -I.
IMPL_DEFS=-DDEFINE_OPENMYDB

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

metasiteImport.o: metasiteImport.cpp ocImportParser.h  SysImportFixup.h
	$(CC) -c  $(DATAINC) $(CGIINC) $(IMPL_DEFS) metasiteImport.cpp

metasiteExport.o: metasiteExport.cpp ocImportParser.h  SysImportFixup.h
	$(CC) -c  $(DATAINC) $(CGIINC) $(IMPL_DEFS) metasiteExport.cpp

