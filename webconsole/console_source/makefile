# GENERAL PURPOSE DEFINITION
MLIBDIR=/usr/lib/mysql/
INCLUDEDIR=/usr/include/mysql/
LIBDIR=/usr/lib/
OBJDIR=../../objects/
INCLDIR=../../include/
WEBDIR=../../web/console/
ODD=../../objects/mySQL/
OTD=../../openTools/
FORMDIR=../../openForms/
LDDIR=./

# LINK AND PATH DEFS FOR CGI AND DATA LIBS
DATALIB=$(ODD)openMyDB.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)fmtTime.o
CGILIB=$(OBJDIR)Astring.o $(OBJDIR)cgiClass.o $(OBJDIR)cgiTemplates.o $(OBJDIR)cgiCookie.o
DATAINC=-I$(INCLUDEDIR) -I$(OTD) -I$(INCLDIR) -DDEFINE_OPENMYDB
CGIINC=-I$(INCLDIR) -I$(FORMDIR) -I.
MLIB= -L$(MLIBDIR) -lmysqlclient -lz

#CC=g++ -ddbg
#CC=g++ -Os
CC=g++

# for the .so libs
shared_compile_opts=-fpic
shared_link_opts=-shared -lc

# for the exec. that links dynamically
dyna_link_lib=-ldl

all: $(WEBDIR)menu.cgi $(WEBDIR)modules/module_base.so 

clean:
	rm *.o

# // shared lib
$(WEBDIR)modules/module_base.so: module_base.o
	gcc $(shared_link_opts)  -o $(WEBDIR)modules/module_base.so module_base.o $(CGILIB) $(DATALIB) $(MLIB)  -luuid
  
# // compile

menu.o: menu.cpp ocDynLib.hpp
	$(CC) -c $(DATAINC) $(CGIINC) menu.cpp    

module_base.o: module_base.cpp
	gcc  -c $(shared_compile_opts) $(CGIINC) module_base.cpp
  
# // link

$(WEBDIR)menu.cgi: menu.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o 
	$(CC) -o $(WEBDIR)menu.cgi  menu.o $(DATALIB) $(CGILIB) $(MLIB) $(dyna_link_lib)    





