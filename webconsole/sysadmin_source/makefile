# GENERAL PURPOSE DEFINITION
WEBDIR=../../web/console/
#WEBDIR=../../web/customlogomats/sys/

MLIBDIR=/usr/lib/mysql/
INCLUDEDIR=/usr/include/mysql/
LIBDIR=/usr/lib/
ODD=../../include/
OTD=../../openTools/
OMD=../../openMailer/
OPD=../../openMerchant/
OBJDIR=../../objects/
INCLDIR=../../include/
FORMDIR=../../openForms/
ODD=../../objects/mySQL/
OTD=../../openTools/
OMD=../../openMailer/
OPD=../../openMerchant/
OCLIENT=../../openClientCode/
LDDIR=./
#CC=g++ -O
CC=g++ -s

MLIB= -L$(MLIBDIR) -lmysqlclient -lz
PLIB=-L$(PLIBDIR) -lpq
DATALIB=$(ODD)openMyDB.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)fmtTime.o $(MLIB)
CGILIB=$(OBJDIR)Astring.o $(OBJDIR)cgiClass.o $(OBJDIR)cgiTemplates.o $(OBJDIR)cgiCookie.o
DATAINC=-I$(INCLUDEDIR) -I$(OTD) -I$(INCLDIR) -DDEFINE_OPENMYDB
CGIINC=-I$(INCLDIR) -I$(FORMDIR) -I.

# for the .so libs
shared_compile_opts=-fpic
shared_link_opts=-shared -lc

#CC=g++ -O
CC=g++

all: $(WEBDIR)modules/admin_module.so $(WEBDIR)services_ui.cgi \
$(WEBDIR)users_ui.cgi $(WEBDIR)groups_ui.cgi $(WEBDIR)group_services_ui.cgi $(WEBDIR)roles_ui.cgi \
$(WEBDIR)group_sites_ui.cgi $(WEBDIR)sysadmin.cgi


# // shared libs
$(WEBDIR)modules/admin_module.so: admin_module.o
	gcc $(shared_link_opts)  -o $(WEBDIR)modules/admin_module.so admin_module.o $(CGILIB) $(DATALIB) $(MLIB)  -luuid


#
# link applications
#
$(WEBDIR)sysadmin.cgi:  sysadmin.o
	$(CC) -o $(WEBDIR)sysadmin.cgi  sysadmin.o  $(CGILIB) $(DATALIB)  -luuid

$(WEBDIR)services_ui.cgi: services_ui.o
	$(CC) -o $(WEBDIR)services_ui.cgi  services_ui.o $(CGILIB) $(DATALIB)  -luuid

$(WEBDIR)users_ui.cgi: users_ui.o
	$(CC) -o $(WEBDIR)users_ui.cgi  users_ui.o $(CGILIB) $(DATALIB)  -luuid

$(WEBDIR)groups_ui.cgi: groups_ui.o
	$(CC) -o $(WEBDIR)groups_ui.cgi  groups_ui.o $(CGILIB) $(DATALIB)  -luuid
  
$(WEBDIR)group_services_ui.cgi: group_services_ui.o
	$(CC) -o $(WEBDIR)group_services_ui.cgi  group_services_ui.o $(CGILIB) $(DATALIB)  -luuid

$(WEBDIR)roles_ui.cgi: roles_ui.o
	$(CC) -o $(WEBDIR)roles_ui.cgi  roles_ui.o $(CGILIB) $(DATALIB)  -luuid    

$(WEBDIR)group_sites_ui.cgi: group_sites_ui.o
	$(CC) -o $(WEBDIR)group_sites_ui.cgi  group_sites_ui.o $(CGILIB) $(DATALIB)  -luuid
         
#
#compile objects
#

admin_module.o: admin_module.cpp
	gcc  -c $(shared_compile_opts) $(CGIINC) $(DATAINC) admin_module.cpp
  
sysadmin.o: sysadmin.cpp connectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) sysadmin.cpp

services_ui.o: services_ui.cpp services.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp 
	$(CC) -c $(DATAINC) $(CGIINC) services_ui.cpp  

users_ui.o: users_ui.cpp users.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp 
	$(CC) -c $(DATAINC) $(CGIINC) users_ui.cpp 
   
groups_ui.o: groups_ui.cpp groups.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp 
	$(CC) -c $(DATAINC) $(CGIINC) groups_ui.cpp  
  
roles_ui.o: roles_ui.cpp roles.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp 
	$(CC) -c $(DATAINC) $(CGIINC) roles_ui.cpp  

group_services_ui.o: group_services_ui.cpp group_services.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp 
	$(CC) -c $(DATAINC) $(CGIINC) group_services_ui.cpp  
  
group_sites_ui.o: group_sites_ui.cpp group_sites.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp 
	$(CC) -c $(DATAINC) $(CGIINC) group_sites_ui.cpp  
     
# clean-up to recompile and link all
clean:
	rm *.o
  
# sign on information is in the private database
private:
	cp  openPrivateLogin.h  openLogin.h

# sign on information is in the public database 'metasite'
public:                
	cp  openMetaLogin.h  openLogin.h






