# base defs are here
include ../../build.mak

# comment out for group+role security model
#LOGINMODEL=-DLOGIN_LITE_SECURITY=TRUE

#WEBDIR=../../web/siltanium/
#WEBDIR=../../web/HospitalityCarpet/
#WEBDIR=../../web/TunnelHillFBC/
WEBDIR=../../web/Woodhollow/
#WEBDIR=../../web/junkmansson/
#WEBDIR=../../web/newsblogs/
#WEBDIR=../../web/projects/

all: $(WEBDIR)pages.meta $(WEBDIR)uploads.meta $(WEBDIR)show.meta $(WEBDIR)contactus.meta \
$(WEBDIR)intranet.meta $(WEBDIR)servicesUI.meta $(WEBDIR)usersUI.meta $(WEBDIR)ls_usersUI.meta\
$(WEBDIR)groupsUI.meta $(WEBDIR)rolesUI.meta $(WEBDIR)group_services_ui.cgi \
$(WEBDIR)services_liteUI.meta $(WEBDIR)role_services_Popup.cgi  \
$(WEBDIR)filePick.cgi  $(WEBDIR)imagePick.cgi    imageBatch  \
$(WEBDIR)XML_spell_svc.cgi  $(WEBDIR)imageUpload.cgi  $(WEBDIR)Document_Popup.cgi  $(WEBDIR)documents.meta $(WEBDIR)contentEditor.cgi $(WEBDIR)globalEditor.cgi $(WEBDIR)pageBuilder.cgi \
$(WEBDIR)SignIn.meta  $(WEBDIR)move_mi_svc.cgi $(WEBDIR)link_ui.cgi  $(WEBDIR)menuEditor.cgi \
$(WEBDIR)menu_item_ui.cgi $(WEBDIR)templates_Popup.cgi $(WEBDIR)edit_types_Popup.cgi \
$(WEBDIR)contentSearch.cgi $(WEBDIR)Templates/DefaultEditLinks.htmp  $(WEBDIR)metaParagraphSlides.cgi \
siteLinkFixup


#copy default edit links
$(WEBDIR)Templates/DefaultEditLinks.htmp:  DefaultEditLinks.htmp
	cp DefaultEditLinks.htmp $(WEBDIR)Templates

#
# link applications
#

siteLinkFixup: siteLinkFixup.o
	$(CC) -o siteLinkFixup  siteLinkFixup.o  $(CGILIB) $(DATALIB) $(MLIB) -luuid
		
$(WEBDIR)metaParagraphSlides.cgi: metaParagraphSlides.o
	$(CC) -o $(WEBDIR)metaParagraphSlides.cgi  metaParagraphSlides.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)edit_types_Popup.cgi: edit_types_Popup.o
	$(CC) -o $(WEBDIR)edit_types_Popup.cgi  edit_types_Popup.o $(CGILIB) $(DATALIB) $(MLIB) -luuid
	
$(WEBDIR)pages.meta: pages.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o 
	$(CC) -o $(WEBDIR)pages.meta  pages.o $(DATALIB) $(CGILIB) $(MLIB)   -luuid
	
$(WEBDIR)contactus.meta: contactus.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o $(OBJDIR)oc_Mail.o
	$(CC) -o $(WEBDIR)contactus.meta  contactus.o  $(DATALIB) $(CGILIB) $(MLIB)  $(OBJDIR)oc_Mail.o -luuid
	
$(WEBDIR)uploads.meta: uploads.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o 
	$(CC) -o $(WEBDIR)uploads.meta  uploads.o $(DATALIB) $(CGILIB) $(MLIB)   -luuid

$(WEBDIR)show.meta: show.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o
	$(CC) -o $(WEBDIR)show.meta  show.o $(DATALIB) $(CGILIB) $(MLIB)  -luuid  

$(WEBDIR)search.meta: search.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o 
	$(CC) -o $(WEBDIR)search.meta  search.o $(DATALIB) $(CGILIB) $(MLIB)  -luuid

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

$(WEBDIR)globalEditor.cgi: globalEditor.o
	$(CC) -o $(WEBDIR)globalEditor.cgi  globalEditor.o $(CGILIB) $(DATALIB) $(MLIB) -luuid
	
$(WEBDIR)pageBuilder.cgi: pageBuilder.o
	$(CC) -o $(WEBDIR)pageBuilder.cgi  pageBuilder.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)SignIn.meta: SignIn.o
	$(CC) -o $(WEBDIR)SignIn.meta  SignIn.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)link_ui.cgi:  link_ui.o  
	$(CC) -o $(WEBDIR)link_ui.cgi  link_ui.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid
 
$(WEBDIR)menu_item_ui.cgi:  menu_item_ui.o
	$(CC) -o $(WEBDIR)menu_item_ui.cgi  menu_item_ui.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid
 
$(WEBDIR)menuEditor.cgi:  menuEditor.o
	$(CC) -o $(WEBDIR)menuEditor.cgi  menuEditor.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid
	
$(WEBDIR)move_mi_svc.cgi:  move_mi_svc.o  
	$(CC) -o $(WEBDIR)move_mi_svc.cgi  move_mi_svc.o  $(DATALIB) $(CGILIB) $(MLIB)  -luuid


$(WEBDIR)templates_Popup.cgi: templates_Popup.o
	$(CC) -o $(WEBDIR)templates_Popup.cgi  templates_Popup.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)contentSearch.cgi: contentSearch.o
	$(CC) -o $(WEBDIR)contentSearch.cgi  contentSearch.o $(CGILIB) $(DATALIB) $(MLIB) -luuid
#
#compile objects
#

metaParagraphSlides.o: metaParagraphSlides.cpp  $(FORMDIR)read_write_base.hpp 
	$(CC) -c $(DATAINC) $(CGIINC)  $(OLOGIN) $(LOGINMODEL) metaParagraphSlides.cpp $(IMPL_DEFS)

pages.o: pages.cpp page.hpp page_template.hpp page_menu.hpp pg_paragraph.hpp page_control.hpp
	$(CC) -c $(DATAINC) $(CGIINC) $(LOGINMODEL) pages.cpp

contactus.o: contactus.cpp page.hpp page_template.hpp page_menu.hpp pg_paragraph.hpp contact_control.hpp Form_Data_ui.h
	$(CC) -c $(DATAINC) $(CGIINC) $(LOGINMODEL) contactus.cpp

uploads.o: uploads.cpp page.hpp page_template.hpp page_menu.hpp pg_paragraph.hpp page_control.hpp
	$(CC) -c $(DATAINC) $(CGIINC) $(LOGINMODEL) uploads.cpp
      
show.o: show.cpp page.hpp page_template.hpp page_menu.hpp pg_paragraph.hpp page_control.hpp show_control.hpp
	$(CC) -c $(DATAINC) $(CGIINC) $(LOGINMODEL) show.cpp  

search.o: search.cpp page.hpp page_template.hpp page_menu.hpp pg_paragraph.hpp page_control.hpp search_control.hpp
	$(CC) -c $(DATAINC) $(CGIINC) $(LOGINMODEL) search.cpp 
     

	
intranet.o: intranet.cpp w3intranet.h page.hpp page_template.hpp page_menu.hpp pg_paragraph.hpp page_control.hpp makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(LOGINMODEL) intranet.cpp
  
servicesUI.o: servicesUI.cpp services.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) $(LOGINMODEL) servicesUI.cpp $(IMPL_DEFS)

services_liteUI.o: services_liteUI.cpp services.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) $(LOGINMODEL) services_liteUI.cpp $(IMPL_DEFS)

    
usersUI.o: usersUI.cpp users.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) $(LOGINMODEL) usersUI.cpp $(IMPL_DEFS)

ls_usersUI.o: ls_usersUI.cpp users.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) $(LOGINMODEL) ls_usersUI.cpp $(IMPL_DEFS)

groupsUI.o: groupsUI.cpp groups.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp  makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) $(LOGINMODEL) groupsUI.cpp $(IMPL_DEFS)

rolesUI.o: rolesUI.cpp roles.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) $(LOGINMODEL) rolesUI.cpp $(IMPL_DEFS)

group_services_ui.o: group_services_ui.cpp group_services.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) $(LOGINMODEL) group_services_ui.cpp $(IMPL_DEFS)

role_services_Popup.o: role_services_Popup.cpp group_services.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) $(LOGINMODEL) role_services_Popup.cpp $(IMPL_DEFS)

imagePick.o: imagePick.cpp connectstring makefile  
	$(CC) -c $(DATAINC) $(CGIINC)  $(OLOGIN) $(LOGINMODEL) imagePick.cpp  $(IMPL_DEFS)

imageUpload.o: imageUpload.cpp connectstring makefile
	$(CC) -c $(DATAINC) $(CGIINC)  $(OLOGIN) $(LOGINMODEL) imageUpload.cpp  $(IMPL_DEFS)

filePick.o: filePick.cpp connectstring makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) $(LOGINMODEL) filePick.cpp $(IMPL_DEFS)


SpellChecker.o: SpellChecker.cpp SpellChecking.hpp $(FORMDIR)forms_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) $(LOGINMODEL) SpellChecker.cpp $(IMPL_DEFS)

XML_spell_svc.o: XML_spell_svc.cpp $(FORMDIR)forms_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) XML_spell_svc.cpp $(IMPL_DEFS)
	
imageBatch.o: imageBatch.cpp connectstring makefile
	$(CC) -c $(DATAINC) $(CGIINC)  $(OLOGIN) $(LOGINMODEL) imageBatch.cpp  $(IMPL_DEFS)

Document_Popup.o: Document_Popup.cpp Document.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) $(LOGINMODEL) Document_Popup.cpp $(IMPL_DEFS)
	
documents.o: documents.cpp Document.hpp w3intranet.h page.hpp page_template.hpp page_menu.hpp pg_paragraph.hpp page_control.hpp makefile
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) $(LOGINMODEL) documents.cpp

contentEditor.o: contentEditor.cpp paragraphs.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC)  $(OLOGIN) $(LOGINMODEL) contentEditor.cpp $(IMPL_DEFS)

globalEditor.o: globalEditor.cpp paragraphs.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC)  $(OLOGIN) $(LOGINMODEL) globalEditor.cpp $(IMPL_DEFS)

pageBuilder.o: pageBuilder.cpp paragraphs.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC)  $(OLOGIN) $(LOGINMODEL) pageBuilder.cpp $(IMPL_DEFS)
  
SignIn.o: SignIn.cpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp 
	$(CC) -c $(DATAINC) $(CGIINC) $(LOGINMODEL) SignIn.cpp $(OLOGIN)
		
move_mi_svc.o: move_mi_svc.cpp
	$(CC) -c $(shared_compile_opts) $(DATAINC) $(CGIINC) -I$(OPD) move_mi_svc.cpp
		  

link_ui.o: link_ui.cpp connectstring makefile $(OPD)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) $(LOGINMODEL) link_ui.cpp

menuEditor.o: menuEditor.cpp connectstring makefile $(OPD)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) $(LOGINMODEL) menuEditor.cpp

menu_item_ui.o: menu_item_ui.cpp menu_item.hpp connectstring makefile $(OPD)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) $(LOGINMODEL) menu_item_ui.cpp


edit_types_Popup.o: edit_types_Popup.cpp edit_types.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC)  $(LOGINMODEL) edit_types_Popup.cpp $(IMPL_DEFS)


templates_Popup.o: templates_Popup.cpp templates.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC) $(LOGINMODEL) templates_Popup.cpp $(IMPL_DEFS)

contentSearch.o: contentSearch.cpp paragraphs.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC)  $(OLOGIN) $(LOGINMODEL) contentSearch.cpp $(IMPL_DEFS)

siteLinkFixup.o:  siteLinkFixup.cpp
	$(CC) -c $(DATAINC) $(CGIINC) $(OLOGIN) $(LOGINMODEL) siteLinkFixup.cpp $(IMPL_DEFS)
	
clean:
	rm *.o
	
	