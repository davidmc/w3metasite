PLIBDIR=/usr/lib/
INCLUDEDIR=/usr/local/pgsql/include/
LIBDIR=/usr/lib/
WEBDIR=../../web/postOffice/admin/
INCLDIR=../../include/
OBJDIR=../../objects/
ODD=../../objects/pgSQL/
OMD=../../../openMailer/
OPD=../../../openMerchant/
OAD=../../openAdmin/
OSD=../../openSocket/

LDDIR=./
#CC=g++ -O
CC=g++
 
# LINK AND PATH DEFS FOR CGI AND DATA LIBS

DATALIB=$(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)fmtTime.o $(ODD)openDBPG.o -lpq 
CGILIB=$(OBJDIR)Astring.o $(OBJDIR)cgiClass.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
DATAINC=-I$(INCLUDEDIR) -I$(INCLDIR)
CGIINC=-I$(INCLDIR) -I$(OAD)

IMPL_DEFS=-DDEFINE_OPENDBPG

all: objectBase.o $(WEBDIR)admin.cgi $(WEBDIR)groupList.cgi $(WEBDIR)groupForm.cgi  $(WEBDIR)serviceList.cgi  \
$(WEBDIR)userList.cgi $(WEBDIR)userForm.cgi  $(WEBDIR)locationList.cgi \
$(WEBDIR)serviceForm.cgi $(WEBDIR)roleList.cgi $(WEBDIR)serviceParamForm.cgi $(WEBDIR)roleForm.cgi \
$(WEBDIR)roleServiceForm.cgi $(WEBDIR)locationForm.cgi $(WEBDIR)userRoleForm.cgi $(WEBDIR)mailGroupList.cgi \
$(WEBDIR)mailGroupForm.cgi $(WEBDIR)globalDirList.cgi  $(WEBDIR)globalDirForm.cgi \
$(WEBDIR)groupRoleForm.cgi $(WEBDIR)groupRoleList.cgi

# copy to site directory
$(WEBDIR)admin.cgi: admin.cgi
	cp admin.cgi $(WEBDIR)admin.cgi
	cp go_right.png $(WEBDIR)
	cp admin.html $(WEBDIR)
	cp signIn.html $(WEBDIR)

$(WEBDIR)userForm.cgi: userForm.cgi openDom.js  forms.js formEvents.js
	cp userForm.cgi $(WEBDIR)userForm.cgi

$(WEBDIR)globalDirForm.cgi: globalDirForm.cgi openDom.js  forms.js formEvents.js
	cp globalDirForm.cgi $(WEBDIR)globalDirForm.cgi

$(WEBDIR)userRoleForm.cgi: userRoleForm.cgi openDom.js  forms.js formEvents.js
	cp userRoleForm.cgi $(WEBDIR)userRoleForm.cgi

$(WEBDIR)groupRoleForm.cgi: groupRoleForm.cgi openDom.js  forms.js formEvents.js
	cp groupRoleForm.cgi $(WEBDIR)groupRoleForm.cgi

$(WEBDIR)groupRoleList.cgi: groupRoleList.cgi openDom.js  forms.js formEvents.js
	cp groupRoleList.cgi $(WEBDIR)groupRoleList.cgi

$(WEBDIR)locationForm.cgi: locationForm.cgi openDom.js  forms.js formEvents.js
	cp locationForm.cgi $(WEBDIR)locationForm.cgi

$(WEBDIR)mailGroupForm.cgi: mailGroupForm.cgi openDom.js  forms.js formEvents.js
	cp mailGroupForm.cgi $(WEBDIR)mailGroupForm.cgi

$(WEBDIR)serviceParamForm.cgi: serviceParamForm.cgi openDom.js  forms.js formEvents.js
	cp serviceParamForm.cgi $(WEBDIR)serviceParamForm.cgi
	cp new.png $(WEBDIR)
	cp exit.png $(WEBDIR)
	cp childForm.html $(WEBDIR)

$(WEBDIR)serviceForm.cgi: serviceForm.cgi openDom.js  forms.js formEvents.js
	cp serviceForm.cgi $(WEBDIR)serviceForm.cgi

$(WEBDIR)roleServiceForm.cgi: roleServiceForm.cgi openDom.js  forms.js formEvents.js
	cp roleServiceForm.cgi $(WEBDIR)roleServiceForm.cgi

$(WEBDIR)roleForm.cgi: roleForm.cgi openDom.js  forms.js formEvents.js
	cp roleForm.cgi $(WEBDIR)roleForm.cgi

$(WEBDIR)groupForm.cgi: groupForm.cgi openDom.js  forms.js formEvents.js
	cp groupForm.cgi $(WEBDIR)groupForm.cgi
	cp adminForm.html $(WEBDIR)
	cp openDom.js $(WEBDIR)
	cp forms.js $(WEBDIR)
	cp formEvents.js $(WEBDIR)

$(WEBDIR)groupList.cgi: groupList.cgi
	cp groupList.cgi $(WEBDIR)groupList.cgi
	cp adminList.html $(WEBDIR)

$(WEBDIR)mailGroupList.cgi: mailGroupList.cgi
	cp mailGroupList.cgi $(WEBDIR)mailGroupList.cgi

$(WEBDIR)serviceList.cgi: serviceList.cgi
	cp serviceList.cgi $(WEBDIR)serviceList.cgi

$(WEBDIR)roleList.cgi: roleList.cgi
	cp roleList.cgi $(WEBDIR)roleList.cgi

$(WEBDIR)locationList.cgi: locationList.cgi
	cp locationList.cgi $(WEBDIR)locationList.cgi

$(WEBDIR)globalDirList.cgi: globalDirList.cgi
	cp globalDirList.cgi $(WEBDIR)globalDirList.cgi

$(WEBDIR)userList.cgi: userList.cgi
	cp userList.cgi $(WEBDIR)userList.cgi
	cp filteredList.html $(WEBDIR)


#
# link applications
#
admin.cgi:  admin.o  $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o admin.cgi  admin.o  $(DATALIB) $(CGILIB)

groupList.cgi:  groupList.o  $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o groupList.cgi  groupList.o  $(DATALIB) $(CGILIB)
  
groupForm.cgi:  groupForm.o  objectBase.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o groupForm.cgi  groupForm.o  objectBase.o $(DATALIB) $(CGILIB)
  
userForm.cgi:  userForm.o  objectBase.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o userForm.cgi  userForm.o  objectBase.o $(DATALIB) $(CGILIB)

globalDirForm.cgi:  globalDirForm.o  objectBase.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o globalDirForm.cgi  globalDirForm.o  objectBase.o $(DATALIB) $(CGILIB)
  
userRoleForm.cgi:  userRoleForm.o  objectBase.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o userRoleForm.cgi  userRoleForm.o  objectBase.o $(DATALIB) $(CGILIB)
  
groupRoleForm.cgi:  groupRoleForm.o  objectBase.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o groupRoleForm.cgi  groupRoleForm.o  objectBase.o $(DATALIB) $(CGILIB)
  
groupRoleList.cgi:  groupRoleList.o  objectBase.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o groupRoleList.cgi  groupRoleList.o  $(DATALIB) $(CGILIB)
  
mailGroupForm.cgi:  mailGroupForm.o  objectBase.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o mailGroupForm.cgi  mailGroupForm.o  objectBase.o $(DATALIB) $(CGILIB)
  
locationForm.cgi:  locationForm.o  objectBase.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o locationForm.cgi  locationForm.o objectBase.o $(DATALIB) $(CGILIB)
  
serviceForm.cgi:  serviceForm.o  objectBase.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o serviceForm.cgi  serviceForm.o objectBase.o $(DATALIB) $(CGILIB)
  
roleServiceForm.cgi:  roleServiceForm.o  objectBase.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o roleServiceForm.cgi  roleServiceForm.o  objectBase.o $(DATALIB) $(CGILIB)
  
roleForm.cgi:  roleForm.o  objectBase.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o roleForm.cgi  roleForm.o  objectBase.o $(DATALIB) $(CGILIB)
   
serviceParamForm.cgi:  serviceParamForm.o  objectBase.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o serviceParamForm.cgi  serviceParamForm.o objectBase.o $(DATALIB) $(CGILIB)
  
mailGroupList.cgi:  mailGroupList.o  $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o mailGroupList.cgi  mailGroupList.o  $(DATALIB) $(CGILIB)
  
serviceList.cgi:  serviceList.o  $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o serviceList.cgi  serviceList.o  $(DATALIB) $(CGILIB)
  
roleList.cgi:  roleList.o  $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o roleList.cgi  roleList.o  $(DATALIB) $(CGILIB)
  
locationList.cgi:  locationList.o  $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o locationList.cgi  locationList.o  $(DATALIB) $(CGILIB)
  
globalDirList.cgi:  globalDirList.o  $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o globalDirList.cgi  globalDirList.o  $(DATALIB) $(CGILIB)
  
userList.cgi:  userList.o  $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o  $(ODD)openDBPG.o $(OBJDIR)openFields.o $(ODD)openDB.o $(OBJDIR)cgiCookie.o $(OBJDIR)cgiTemplates.o
	$(CC) -o userList.cgi  userList.o  $(DATALIB) $(CGILIB)

#
#compile objects
#
admin.o: admin.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) admin.cpp

groupList.o: groupList.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) groupList.cpp

groupForm.o: groupForm.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) groupForm.cpp

userForm.o: userForm.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) userForm.cpp

globalDirForm.o: globalDirForm.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) globalDirForm.cpp

userRoleForm.o: userRoleForm.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) userRoleForm.cpp

groupRoleForm.o: groupRoleForm.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) groupRoleForm.cpp

groupRoleList.o: groupRoleList.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) groupRoleList.cpp

mailGroupForm.o: mailGroupForm.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) mailGroupForm.cpp

locationForm.o: locationForm.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) locationForm.cpp

serviceForm.o: serviceForm.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) serviceForm.cpp

roleServiceForm.o: roleServiceForm.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) roleServiceForm.cpp

roleForm.o: roleForm.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) roleForm.cpp

serviceParamForm.o: serviceParamForm.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) serviceParamForm.cpp

mailGroupList.o: mailGroupList.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) mailGroupList.cpp

serviceList.o: serviceList.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) serviceList.cpp

roleList.o: roleList.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) roleList.cpp

locationList.o: locationList.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) locationList.cpp

globalDirList.o: globalDirList.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) globalDirList.cpp

userList.o: userList.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) userList.cpp

objectBase.o: objectBase.cpp pconnectstring makefile openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) $(IMPL_DEFS) objectBase.cpp











