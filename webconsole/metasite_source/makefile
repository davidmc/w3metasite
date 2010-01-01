# GENERAL PURPOSE DEFINITION
MLIBDIR=/usr/lib/mysql/
INCLUDEDIR=/usr/include/mysql/
LIBDIR=/usr/lib/
OBJDIR=../../objects/
INCLDIR=../../include/
FORMDIR=../../openForms/
WEBDIR=../../web/sys/
#WEBDIR=../../web/demo-auction/sys/
#WEBDIR=../../web/customlogomats/sys/

ODD=../../objects/mySQL/
OTD=../../openTools/
OMD=../../openMailer/
OPD=./
LDDIR=./

# LINK AND PATH DEFS FOR CGI AND DATA LIBS
DATALIB=$(ODD)openMyDB.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)fmtTime.o
CGILIB=$(OBJDIR)Astring.o $(OBJDIR)cgiClass.o $(OBJDIR)cgiTemplates.o $(OBJDIR)cgiCookie.o
DATAINC=-I$(INCLUDEDIR) -I$(OTD) -I$(INCLDIR) -DDEFINE_OPENMYDB
CGIINC=-I$(INCLDIR) -I$(FORMDIR) -I.
MLIB= -L$(MLIBDIR) -lmysqlclient -lz

# for the .so libs
shared_compile_opts=-fpic
shared_link_opts=-shared -lc

#CC=g++ -ddbg
#CC=g++ -Os
CC=g++


all: $(WEBDIR)modules/metasite_module.so $(WEBDIR)modules/imeta_module.so \
$(WEBDIR)site_ui.cgi $(WEBDIR)global_content_ui.cgi \
$(WEBDIR)template_ui.cgi $(WEBDIR)imagePick.cgi $(WEBDIR)filePick.cgi $(WEBDIR)link_ui.cgi \
$(WEBDIR)menu_ui.cgi $(WEBDIR)menu_item_ui.cgi $(WEBDIR)metatagPick.cgi $(WEBDIR)page_ui.cgi \
$(WEBDIR)paragraph_ui.cgi $(WEBDIR)page_menu_ui.cgi $(WEBDIR)urlpick.cgi $(WEBDIR)move_mi_svc.cgi \
$(WEBDIR)resourcesPick.cgi

clean: 
	rm *.o


# // shared libs
$(WEBDIR)modules/metasite_module.so: metasite_module.o
	gcc $(shared_link_opts)  -o $(WEBDIR)modules/metasite_module.so metasite_module.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)modules/imeta_module.so: imeta_module.o
	gcc $(shared_link_opts)  -o $(WEBDIR)modules/imeta_module.so imeta_module.o $(CGILIB) $(DATALIB) $(MLIB) -luuid
  
#
# link applications
#

$(WEBDIR)site_ui.cgi:  site_ui.o  
	$(CC) -o $(WEBDIR)site_ui.cgi  site_ui.o $(DATALIB) $(CGILIB) $(MLIB) -luuid

$(WEBDIR)template_ui.cgi:  template_ui.o  
	$(CC) -o $(WEBDIR)template_ui.cgi  template_ui.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid

$(WEBDIR)link_ui.cgi:  link_ui.o  
	$(CC) -o $(WEBDIR)link_ui.cgi  link_ui.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid

$(WEBDIR)menu_ui.cgi:  menu_ui.o  
	$(CC) -o $(WEBDIR)menu_ui.cgi  menu_ui.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid

$(WEBDIR)page_menu_ui.cgi:  page_menu_ui.o  
	$(CC) -o $(WEBDIR)page_menu_ui.cgi  page_menu_ui.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid
 
$(WEBDIR)menu_item_ui.cgi:  menu_item_ui.o  
	$(CC) -o $(WEBDIR)menu_item_ui.cgi  menu_item_ui.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid
 
$(WEBDIR)pageList.cgi:  pageList.o
	$(CC) -o $(WEBDIR)pageList.cgi  pageList.o $(DATALIB) $(CGILIB) $(MLIB) -luuid

$(WEBDIR)page_ui.cgi:  page_ui.o  
	$(CC) -o $(WEBDIR)page_ui.cgi  page_ui.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid

$(WEBDIR)paragraph_ui.cgi:  paragraph_ui.o  
	$(CC) -o $(WEBDIR)paragraph_ui.cgi  paragraph_ui.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid

$(WEBDIR)metatagPick.cgi:  metatagPick.o  
	$(CC) -o $(WEBDIR)metatagPick.cgi  metatagPick.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid
  
$(WEBDIR)global_content_ui.cgi:  global_content_ui.o  
	$(CC) -o $(WEBDIR)global_content_ui.cgi  global_content_ui.o $(DATALIB) $(CGILIB) $(MLIB) -luuid

$(WEBDIR)urlpick.cgi:  urlpick.o  
	$(CC) -o $(WEBDIR)urlpick.cgi  urlpick.o $(DATALIB) $(CGILIB) $(MLIB) -luuid

$(WEBDIR)imagePick.cgi:  imagePick.o  
	$(CC) -o $(WEBDIR)imagePick.cgi  imagePick.o  $(DATALIB) $(CGILIB) $(MLIB) -lMagick++ -lMagick -ltiff -ljpeg -lpng -luuid
  
$(WEBDIR)filePick.cgi:  filePick.o  
	$(CC) -o $(WEBDIR)filePick.cgi  filePick.o  $(DATALIB) $(CGILIB) $(MLIB)  -luuid
  
$(WEBDIR)move_mi_svc.cgi:  move_mi_svc.o  
	$(CC) -o $(WEBDIR)move_mi_svc.cgi  move_mi_svc.o  $(DATALIB) $(CGILIB) $(MLIB)  -luuid

$(WEBDIR)resourcesPick.cgi:  resourcesPick.o  
	$(CC) -o $(WEBDIR)resourcesPick.cgi  resourcesPick.o  $(DATALIB) $(CGILIB) $(MLIB)  -luuid
 
#
#compile objects
#
link_ui.o: link_ui.cpp connectstring makefile $(OPD)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) link_ui.cpp

menu_ui.o: menu_ui.cpp connectstring makefile $(OPD)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) menu_ui.cpp

menu_item_ui.o: menu_item_ui.cpp menu_item.hpp connectstring makefile $(OPD)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) menu_item_ui.cpp

imagePick.o: imagePick.cpp connectstring makefile $(OPD)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) imagePick.cpp

filePick.o: filePick.cpp connectstring makefile $(OPD)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) filePick.cpp

site_ui.o: site_ui.cpp connectstring makefile $(OPD)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) site_ui.cpp

group_site_ui.o: group_site_ui.cpp connectstring makefile $(OPD)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) group_site_ui.cpp

template_ui.o: template_ui.cpp connectstring makefile $(OPD)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) template_ui.cpp

page_menu_ui.o: page_menu_ui.cpp connectstring makefile $(OPD)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) page_menu_ui.cpp
    
page_ui.o: page_ui.cpp page.hpp connectstring makefile $(OPD)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) page_ui.cpp
 
paragraph_ui.o: paragraph_ui.cpp paragraph.hpp connectstring makefile $(OPD)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) paragraph_ui.cpp

metatagPick.o: metatagPick.cpp connectstring makefile $(OPD)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) metatagPick.cpp
  
global_content_ui.o: global_content_ui.cpp connectstring makefile $(OPD)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) global_content_ui.cpp

urlpick.o: urlpick.cpp connectstring makefile $(OPD)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) urlpick.cpp

metasite_module.o: metasite_module.cpp
	$(CC) -c $(shared_compile_opts) $(DATAINC) $(CGIINC) -I$(OPD) metasite_module.cpp
imeta_module.o: imeta_module.cpp
	$(CC) -c $(shared_compile_opts) $(DATAINC) $(CGIINC) -I$(OPD) imeta_module.cpp
	
move_mi_svc.o: move_mi_svc.cpp
	$(CC) -c $(shared_compile_opts) $(DATAINC) $(CGIINC) -I$(OPD) move_mi_svc.cpp
  
resourcesPick.o: resourcesPick.cpp connectstring makefile $(OPD)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) resourcesPick.cpp
