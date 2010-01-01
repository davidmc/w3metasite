# makefile for rainbow custom logo mats e-commerce administration
include ../../build.mak

# for WEBDIR target
#include makefile.hydro
#include makefile.auction
#include makefile.mohawk
#include makefile.media
#include makefile.mohawkcontractors
#include makefile.southern
include makefile.glassfusion
#include makefile.people
#include makefile.kleentex
#include makefile.mohawktasks

#all entries

all: $(INCLDIR)page_enhancements.hpp $(WEBDIR)intranet.meta $(WEBDIR)servicesUI.meta $(WEBDIR)usersUI.meta $(WEBDIR)ls_usersUI.meta\
$(WEBDIR)groupsUI.meta $(WEBDIR)rolesUI.meta $(WEBDIR)group_services_ui.cgi \
$(WEBDIR)services_liteUI.meta $(WEBDIR)role_services_Popup.cgi  \
$(WEBDIR)filePick.cgi  $(WEBDIR)imagePick.cgi  $(WEBDIR)paragraph_ui.cgi   imageBatch  \
$(WEBDIR)XML_spell_svc.cgi  $(WEBDIR)imageUpload.cgi  $(WEBDIR)Document_Popup.cgi  $(WEBDIR)documents.meta \
$(WEBDIR)contentEditor.cgi

clean:
	rm *.o  

# copy entries
$(INCLDIR)page_enhancements.hpp: page_enhancements.hpp
	cp page_enhancements.hpp $(INCLDIR)

# link entries
$(WEBDIR)intranet.meta: intranet.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o 
	$(CC) -o $(WEBDIR)intranet.meta  intranet.o $(DATALIB) $(CGILIB) $(MLIB) -luuid
  
$(WEBDIR)servicesUI.meta: servicesUI.o
	$(CC) -o $(WEBDIR)servicesUI.meta  servicesUI.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)services_liteUI.meta: services_liteUI.o
	$(CC) -o $(WEBDIR)services_liteUI.meta  services_liteUI.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)usersUI.meta: usersUI.o
	$(CC) -o $(WEBDIR)usersUI.meta  usersUI.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)ls_usersUI.meta: ls_usersUI.o
	$(CC) -o $(WEBDIR)ls_usersUI.meta  ls_usersUI.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)groupsUI.meta: groupsUI.o
	$(CC) -o $(WEBDIR)groupsUI.meta  groupsUI.o $(CGILIB) $(DATALIB) $(MLIB) -luuid
  
$(WEBDIR)rolesUI.meta: rolesUI.o
	$(CC) -o $(WEBDIR)rolesUI.meta  rolesUI.o $(CGILIB) $(DATALIB) $(MLIB) -luuid  

$(WEBDIR)group_services_ui.cgi: group_services_ui.o
	$(CC) -o $(WEBDIR)group_services_ui.cgi  group_services_ui.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)role_services_Popup.cgi: role_services_Popup.o
	$(CC) -o $(WEBDIR)role_services_Popup.cgi  role_services_Popup.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)imagePick.cgi:  imagePick.o  
	$(CC) -o $(WEBDIR)imagePick.cgi  imagePick.o  $(DATALIB) $(CGILIB) $(MLIB) -lMagick++ -lMagick -ltiff -ljpeg -lpng -luuid

$(WEBDIR)imageUpload.cgi:  imageUpload.o
	$(CC) -o $(WEBDIR)imageUpload.cgi  imageUpload.o  $(DATALIB) $(CGILIB) $(MLIB) -lMagick++ -lMagick -ltiff -ljpeg -lpng -luuid

$(WEBDIR)filePick.cgi:  filePick.o  
	$(CC) -o $(WEBDIR)filePick.cgi  filePick.o  $(DATALIB) $(CGILIB) $(MLIB)  -luuid

$(WEBDIR)paragraph_ui.cgi:  paragraph_ui.o  
	$(CC) -o $(WEBDIR)paragraph_ui.cgi  paragraph_ui.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid

$(WEBDIR)SpellChecker.cgi: SpellChecker.o
	$(CC) -o $(WEBDIR)SpellChecker.cgi  SpellChecker.o $(CGILIB) $(DATALIB) $(MLIB) -luuid -l aspell

$(WEBDIR)XML_spell_svc.cgi: XML_spell_svc.o
	$(CC) -o $(WEBDIR)XML_spell_svc.cgi  XML_spell_svc.o $(CGILIB) $(DATALIB) $(MLIB) -luuid -l aspell
	
imageBatch:  imageBatch.o
	$(CC) -o imageBatch  imageBatch.o  $(DATALIB) $(CGILIB) $(MLIB) -lMagick++ -lMagick -ltiff -ljpeg -lpng -luuid

$(WEBDIR)Document_Popup.cgi: Document_Popup.o
	$(CC) -o $(WEBDIR)Document_Popup.cgi  Document_Popup.o $(CGILIB) $(DATALIB) $(MLIB) -luuid
	
$(WEBDIR)documents.meta: documents.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o
	$(CC) -o $(WEBDIR)documents.meta  documents.o $(DATALIB) $(CGILIB) $(MLIB) -luuid

$(WEBDIR)contentEditor.cgi: contentEditor.o
	$(CC) -o $(WEBDIR)contentEditor.cgi  contentEditor.o $(CGILIB) $(DATALIB) $(MLIB) -luuid





		
# compile entries
  
intranet.o: intranet.cpp w3intranet.h $(INCLDIR)page.hpp $(INCLDIR)page_template.hpp $(INCLDIR)page_menu.hpp $(INCLDIR)pg_paragraph.hpp $(INCLDIR)page_control.hpp makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) intranet.cpp
  
servicesUI.o: servicesUI.cpp services.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) servicesUI.cpp $(IMPL_DEFS)

services_liteUI.o: services_liteUI.cpp services.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) services_liteUI.cpp $(IMPL_DEFS)

    
usersUI.o: usersUI.cpp users.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) usersUI.cpp $(IMPL_DEFS)

ls_usersUI.o: ls_usersUI.cpp users.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) ls_usersUI.cpp $(IMPL_DEFS)

groupsUI.o: groupsUI.cpp groups.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp  makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) groupsUI.cpp $(IMPL_DEFS)

rolesUI.o: rolesUI.cpp roles.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) rolesUI.cpp $(IMPL_DEFS)

group_services_ui.o: group_services_ui.cpp group_services.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) group_services_ui.cpp $(IMPL_DEFS)

role_services_Popup.o: role_services_Popup.cpp group_services.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) role_services_Popup.cpp $(IMPL_DEFS)

imagePick.o: imagePick.cpp connectstring makefile  
	$(CC) -c $(DATAINC) $(CGIINC)  $(OLOGIN) imagePick.cpp  $(IMPL_DEFS)

imageUpload.o: imageUpload.cpp connectstring makefile
	$(CC) -c $(DATAINC) $(CGIINC)  $(OLOGIN) imageUpload.cpp  $(IMPL_DEFS)

filePick.o: filePick.cpp connectstring makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) filePick.cpp $(IMPL_DEFS)

paragraph_ui.o: paragraph_ui.cpp paragraph.hpp connectstring makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN)  paragraph_ui.cpp $(IMPL_DEFS)

SpellChecker.o: SpellChecker.cpp SpellChecking.hpp $(FORMDIR)forms_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) SpellChecker.cpp $(IMPL_DEFS)

XML_spell_svc.o: XML_spell_svc.cpp $(FORMDIR)forms_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) XML_spell_svc.cpp $(IMPL_DEFS)
	
imageBatch.o: imageBatch.cpp connectstring makefile
	$(CC) -c $(DATAINC) $(CGIINC)  $(OLOGIN) imageBatch.cpp  $(IMPL_DEFS)


Document_Popup.o: Document_Popup.cpp Document.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) Document_Popup.cpp $(IMPL_DEFS)
	
documents.o: documents.cpp Document.hpp w3intranet.h $(INCLDIR)page.hpp $(INCLDIR)page_template.hpp $(INCLDIR)page_menu.hpp $(INCLDIR)pg_paragraph.hpp $(INCLDIR)page_control.hpp makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) documents.cpp

contentEditor.o: contentEditor.cpp paragraphs.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC)  $(OLOGIN) contentEditor.cpp $(IMPL_DEFS)



	
