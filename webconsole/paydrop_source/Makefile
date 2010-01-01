MLIBDIR=/usr/lib/mysql/
INCLUDEDIR=/usr/include/mysql/
LIBDIR=/usr/lib/
WEBDIR=../../web/paydrop/sys/
ODD=../../include/
OTD=../../openTools/
OMD=../../openMailer/
OPD=../../openMerchant/
OBJDIR=../../objects/
INCLDIR=../../include/
FORMDIR=../../openForms/
METADIR=$(HOME)/development/paydrop/metaSite/source/
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
CGILIB=$(OBJDIR)Astring.o $(OBJDIR)cgiClass.o $(OBJDIR)cgiTemplates.o $(OBJDIR)cgiCookie.o -luuid
DATAINC=-I$(INCLUDEDIR) -I$(OTD) -I$(INCLDIR) -DDEFINE_OPENMYDB
CGIINC=-I$(INCLDIR) -I$(FORMDIR) -I.

# for the .so libs
shared_compile_opts=-fpic
shared_link_opts=-shared -lc

all: $(WEBDIR)main_ui.cgi $(WEBDIR)category_ui.cgi $(WEBDIR)imagePick.cgi  $(WEBDIR)subcategory_ui.cgi \
$(WEBDIR)marketer_ui.cgi $(WEBDIR)store_ui.cgi $(WEBDIR)member_ui.cgi $(WEBDIR)config_ui.cgi \
$(WEBDIR)coupon_ui.cgi $(WEBDIR)member_purchase.cgi $(WEBDIR)member_check.cgi $(WEBDIR)checkrun.cgi \
$(WEBDIR)newsletter_ui.cgi $(WEBDIR)modules/paydrop_module.so $(WEBDIR)organization_ui.cgi \
$(WEBDIR)fileImportPick.cgi

clean:
	rm *.o
  

# // shared libs
$(WEBDIR)modules/paydrop_module.so: paydrop_module.o
	gcc $(shared_link_opts)  -o $(WEBDIR)modules/paydrop_module.so paydrop_module.o $(CGILIB) $(DATALIB) $(MLIB)
            
# link step 

$(WEBDIR)organization_ui.cgi: organization_ui.o
	$(CC) -o $(WEBDIR)organization_ui.cgi  organization_ui.o $(CGILIB)  $(DATALIB) -luuid    
              
  
$(WEBDIR)imagePick.cgi: imagePick.o 
	$(CC) -o $(WEBDIR)imagePick.cgi  imagePick.o $(CGILIB) $(DATALIB) -lMagick++ -lMagick -ltiff -ljpeg -lpng 
  
$(WEBDIR)category_ui.cgi: category_ui.o
	$(CC) -o $(WEBDIR)category_ui.cgi  category_ui.o $(CGILIB) $(DATALIB)
  
$(WEBDIR)config_ui.cgi: config_ui.o
	$(CC) -o $(WEBDIR)config_ui.cgi  config_ui.o $(CGILIB) $(DATALIB)
  
$(WEBDIR)coupon_ui.cgi: coupon_ui.o
	$(CC) -o $(WEBDIR)coupon_ui.cgi  coupon_ui.o $(CGILIB) $(DATALIB)  
  
$(WEBDIR)subcategory_ui.cgi: subcategory_ui.o
	$(CC) -o $(WEBDIR)subcategory_ui.cgi  subcategory_ui.o $(CGILIB) $(DATALIB)

$(WEBDIR)marketer_ui.cgi: marketer_ui.o
	$(CC) -o $(WEBDIR)marketer_ui.cgi  marketer_ui.o $(CGILIB) $(DATALIB)

$(WEBDIR)store_ui.cgi: store_ui.o
	$(CC) -o $(WEBDIR)store_ui.cgi  store_ui.o $(CGILIB) $(DATALIB)

$(WEBDIR)member_ui.cgi: member_ui.o
	$(CC) -o $(WEBDIR)member_ui.cgi  member_ui.o $(CGILIB)  $(DATALIB) -luuid
              
$(WEBDIR)main_ui.cgi: main_ui.o
	$(CC) -o $(WEBDIR)main_ui.cgi  main_ui.o $(CGILIB) $(DATALIB)
   
$(WEBDIR)member_purchase.cgi: member_purchase.o
	$(CC) -o $(WEBDIR)member_purchase.cgi  member_purchase.o $(CGILIB)  $(DATALIB) $(OBJDIR)ocMail.o -luuid

$(WEBDIR)member_check.cgi: member_check.o
	$(CC) -o $(WEBDIR)member_check.cgi  member_check.o $(CGILIB)  $(DATALIB) $(OBJDIR)ocMail.o -luuid
        
$(WEBDIR)checkrun.cgi: checkrun.o
	$(CC) -o $(WEBDIR)checkrun.cgi  checkrun.o $(CGILIB)  $(DATALIB) 

$(WEBDIR)newsletter_ui.cgi: newsletter_ui.o
	$(CC) -o $(WEBDIR)newsletter_ui.cgi  newsletter_ui.o $(CGILIB)  $(DATALIB)  $(OBJDIR)ocMail.o    

$(WEBDIR)fileImportPick.cgi:  fileImportPick.o  
	$(CC) -o $(WEBDIR)fileImportPick.cgi  fileImportPick.o  $(DATALIB) $(CGILIB) $(MLIB) 
      
# compile step  
organization_ui.o: organization_ui.cpp Member.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp \
$(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp nav_menu.h 
	$(CC) -c $(DATAINC) $(CGIINC) organization_ui.cpp   
  
paydrop_module.o: paydrop_module.cpp
	gcc  -c $(shared_compile_opts) $(CGIINC) $(DATAINC) paydrop_module.cpp

imagePick.o: imagePick.cpp 
	$(CC) -c $(DATAINC) $(CGIINC) imagePick.cpp
  
category_ui.o: category_ui.cpp Category.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp nav_menu.h
	$(CC) -c $(DATAINC) $(CGIINC) category_ui.cpp  
  
config_ui.o: config_ui.cpp Config.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp nav_menu.h
	$(CC) -c $(DATAINC) $(CGIINC) config_ui.cpp    

coupon_ui.o: coupon_ui.cpp Coupon.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp nav_menu.h
	$(CC) -c $(DATAINC) $(CGIINC) coupon_ui.cpp  
    
subcategory_ui.o: subcategory_ui.cpp Subcategory.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp nav_menu.h
	$(CC) -c $(DATAINC) $(CGIINC) subcategory_ui.cpp 
 
marketer_ui.o: marketer_ui.cpp Marketer.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp nav_menu.h
	$(CC) -c $(DATAINC) $(CGIINC) marketer_ui.cpp 
 
store_ui.o: store_ui.cpp Store.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp nav_menu.h
	$(CC) -c $(DATAINC) $(CGIINC) store_ui.cpp 
 
member_ui.o: member_ui.cpp Member.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp \
$(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp nav_menu.h 
	$(CC) -c $(DATAINC) $(CGIINC) member_ui.cpp 
 
member_purchase.o: member_purchase.cpp Member.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp \
$(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp nav_menu.h Purchase_Import_Results.hpp mail_control.hpp
	$(CC) -c $(DATAINC) $(CGIINC)  -I$(METADIR) member_purchase.cpp 
   
member_check.o: member_check.cpp Member.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp \
$(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp nav_menu.h mail_control.hpp
	$(CC) -c $(DATAINC) $(CGIINC) -I$(METADIR) member_check.cpp 

checkrun.o: checkrun.cpp 
	$(CC) -c $(DATAINC) $(CGIINC) checkrun.cpp 
                     
main_ui.o: main_ui.cpp nav_menu.h
	$(CC) -c $(DATAINC) $(CGIINC) main_ui.cpp  

newsletter_ui.o: newsletter_ui.cpp Newsletter.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp nav_menu.h mail_control.hpp
	$(CC) -c $(DATAINC) $(CGIINC) -I$(METADIR) newsletter_ui.cpp 
  
fileImportPick.o: fileImportPick.cpp connectstring Makefile 
	$(CC) -c $(DATAINC) $(CGIINC) fileImportPick.cpp
  
