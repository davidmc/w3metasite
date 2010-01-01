MLIBDIR=/usr/lib/mysql/
PLIBDIR=/usr/lib/pgsql/
INCLUDEDIR=/usr/include/mysql/
MYINCLUDE=/usr/include/mysql/
PINCLUDEDIR=/usr/include/postgres
LIBDIR=/usr/lib/
OBJDIR=$(HOME)/development/objects/
INCLDIR=$(HOME)/development/include/
XINCDIR=$(HOME)/development/intra/xtrasource/
FORMDIR=$(HOME)/development/openForms/
METASRCDIR=$(HOME)/development/metaSite/source/
ODD=$(HOME)/development/objects/mySQL/
LDDIR=./

# used by webconsole makefiles
METADIR=$(HOME)/development/webconsole/metasite_source/
SADMIN=$(HOME)/development/webconsole/sysadmin_source/
OTD=$(HOME)/development/openCore/
OMD=$(HOME)/development/openMailer/
OPD=./


CC=g++  -s
#CC=g++  -g

DATALIB=$(ODD)openMyDB.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)fmtTime.o
CGILIB=$(OBJDIR)Astring.o $(OBJDIR)cgiClass.o $(OBJDIR)cgiTemplates.o $(OBJDIR)cgiCookie.o
DATAINC=-I$(INCLUDEDIR) -I$(INCLDIR) -DDEFINE_OPENMYDB
PDATAINC=-I$(PINCLUDEDIR) -I$(INCLDIR) -DDEFINE_OPENDBPG
CGIINC=-I$(INCLDIR) -I$(FORMDIR) -I.
XGIINC=-I$(XINCDIR) -I$(FORMDIR) -I.
MTAINC=-I$(METASRCDIR)
MLIB= -L$(MLIBDIR) /usr/lib/mysql/libmysqlclient.so -lz
PLIB=-L$(PLIBDIR) -lpq
SCGILIB=$(CGILIB) 
SDATALIB=$(DATALIB)
 
ifeq ($(XTRA),TRUE)
CGIINC=$(XGIINC)
endif

# for the .so libs
shared_compile_opts=-fpic
shared_link_opts=-shared -lc
