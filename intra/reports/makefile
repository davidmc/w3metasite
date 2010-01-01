# GENERAL PURPOSE DEFINITION
 # This is an extranet
# XTRA=TRUE

# comment out for group+role security model
LOGINMODEL=-DLOGIN_LITE_SECURITY=TRUE
 
include ../../build.mak

# for WEBDIR target
#include makefile.nsp
include makefile.glassfusion
#include makefile.wtci
#include makefile.mohawk
#include makefile.hydrospec
#include makefile.southern
#include makefile.people
#include makefile.kleentex
#include makefile.mccright
#include makefile.mohawktasks

all: $(WEBDIR)ReportUI.meta $(WEBDIR)Reports.cgi $(WEBDIR)ReportListing.meta\
$(WEBDIR)Report_Templates_Popup.cgi $(WEBDIR)filePick.cgi \
$(WEBDIR)Report_Type_Popup.cgi \
$(WEBDIR)Report_Parameters_ui.cgi $(WEBDIR)Composite_Query_Popup.cgi $(WEBDIR)Composite_Query_Test.cgi \
$(WEBDIR)Report_Conditioning_Popup.cgi $(WEBDIR)WorkUI.meta $(WEBDIR)WorkList.meta \
$(WEBDIR)Scheme_FieldUI.meta $(WEBDIR)Scheme_TableUI.meta $(WEBDIR)Scheme_Table_Popup.cgi \
$(INCLDIR)col_handling_rep.hpp  $(INCLDIR)rep_base.hpp  $(INCLDIR)Query.hpp  $(INCLDIR)Report.hpp\
$(INCLDIR)Reports.hpp  $(INCLDIR)Composite_Query.hpp $(INCLDIR)Report_Params_ui.h $(WEBDIR)Formation.cgi

clean: 
	rm *.o
  
# copy styles and templates
$(WEBDIR)styles/reports.css: Templates/reports.css
	cp Templates/reports.css $(WEBDIR)styles
  
$(WEBDIR)reportTemplates/*.htmp:  Templates/*.htmp
	cp Templates/*.htmp $(WEBDIR)reportTemplates
	
# copy common include files
$(INCLDIR)col_handling_rep.hpp:   col_handling_rep.hpp
	cp   col_handling_rep.hpp  $(INCLDIR)
$(INCLDIR)rep_base.hpp:    rep_base.hpp
	cp   rep_base.hpp  $(INCLDIR)
$(INCLDIR)Query.hpp:  Query.hpp
	cp  Query.hpp  $(INCLDIR)
$(INCLDIR)Report.hpp:  Report.hpp
	cp Report.hpp $(INCLDIR)
$(INCLDIR)Reports.hpp:   Reports.hpp
	cp Reports.hpp $(INCLDIR)
$(INCLDIR)Composite_Query.hpp:  Composite_Query.hpp
	cp  Composite_Query.hpp  $(INCLDIR)
$(INCLDIR)Report_Params_ui.h:  Report_Params_ui.h
	cp  Report_Params_ui.h  $(INCLDIR)
		
#
# link applications
#

$(WEBDIR)Report_TypeUI.meta:  Report_Type_ui.o  
	$(CC) -o $(WEBDIR)Report_TypeUI.meta  Report_Type_ui.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid

$(WEBDIR)ReportUI.meta:  ReportUI.o  
	$(CC) -o $(WEBDIR)ReportUI.meta  ReportUI.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid

$(WEBDIR)Reports.cgi:  Reports.o 
	$(CC) -o $(WEBDIR)Reports.cgi  Reports.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid
  
$(WEBDIR)ReportListing.meta:  ReportListing.o 
	$(CC) -o $(WEBDIR)ReportListing.meta  ReportListing.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid

$(WEBDIR)Report_Templates_Popup.cgi: Report_Templates_Popup.o
	$(CC) -o $(WEBDIR)Report_Templates_Popup.cgi  Report_Templates_Popup.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)Report_Type_Popup.cgi: Report_Type_Popup.o
	$(CC) -o $(WEBDIR)Report_Type_Popup.cgi  Report_Type_Popup.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)Report_ConditioningUI.meta: Report_Conditioning_ui.o
	$(CC) -o $(WEBDIR)Report_ConditioningUI.meta  Report_Conditioning_ui.o $(CGILIB) $(DATALIB) $(MLIB) -luuid 

$(WEBDIR)Report_Parameters_ui.cgi: Report_Parameters_ui.o
	$(CC) -o $(WEBDIR)Report_Parameters_ui.cgi  Report_Parameters_ui.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)Composite_Query_Popup.cgi: Composite_Query_Popup.o
	$(CC) -o $(WEBDIR)Composite_Query_Popup.cgi  Composite_Query_Popup.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)Composite_Query_Test.cgi: Composite_Query_Test.o
	$(CC) -o $(WEBDIR)Composite_Query_Test.cgi  Composite_Query_Test.o $(CGILIB) $(DATALIB) $(MLIB) -luuid
   
$(WEBDIR)Report_Conditioning_Popup.cgi: Report_Conditioning_Popup.o
	$(CC) -o $(WEBDIR)Report_Conditioning_Popup.cgi  Report_Conditioning_Popup.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)WorkUI.meta: WorkUI.o
	$(CC) -o $(WEBDIR)WorkUI.meta  WorkUI.o $(CGILIB) $(DATALIB) $(MLIB) -luuid
 
$(WEBDIR)WorkList.meta: WorkList.o
	$(CC) -o $(WEBDIR)WorkList.meta  WorkList.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)Scheme_FieldUI.meta: Scheme_FieldUI.o
	$(CC) -o $(WEBDIR)Scheme_FieldUI.meta  Scheme_FieldUI.o $(CGILIB) $(DATALIB) $(MLIB) -luuid
  
$(WEBDIR)Scheme_TableUI.meta: Scheme_TableUI.o
	$(CC) -o $(WEBDIR)Scheme_TableUI.meta  Scheme_TableUI.o $(CGILIB) $(DATALIB) $(MLIB) -luuid  

$(WEBDIR)Scheme_Table_Popup.cgi: Scheme_Table_Popup.o
	$(CC) -o $(WEBDIR)Scheme_Table_Popup.cgi  Scheme_Table_Popup.o $(CGILIB) $(DATALIB) $(MLIB) -luuid  

$(WEBDIR)Formation.cgi: Formation.o
	$(CC) -o $(WEBDIR)Formation.cgi  Formation.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

# compile objects
#
ReportUI.o: ReportUI.cpp connectstring $(SADMIN)openLogin.h ReportUICommon.h  
	$(CC) -c $(CGIINC) $(DATAINC) $(LOGINMODEL)  ReportUI.cpp  

Reports.o: Reports.cpp Reports.hpp rep_base.hpp col_handling_rep.hpp connectstring \
 makefile $(SADMIN)openLogin.h Report_Params_ui.h   
	$(CC) -c $(CGIINC) $(DATAINC) -I$(SADMIN) -I$(METADIR) $(LOGINMODEL)  Reports.cpp 

ReportListing.o: ReportListing.cpp Reports.hpp rep_base.hpp connectstring makefile $(SADMIN)openLogin.h   
	$(CC) -c $(CGIINC) $(DATAINC) -I$(SADMIN) -I$(METADIR) $(LOGINMODEL)   ReportListing.cpp 

Report_Templates_Popup.o: Report_Templates_Popup.cpp Report_Templates.hpp   
	$(CC) -c $(CGIINC) $(DATAINC) -I$(SADMIN) -I$(METADIR) $(LOGINMODEL)  Report_Templates_Popup.cpp 


Report_Parameters_ui.o: Report_Parameters_ui.cpp Report_Parameters.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   
	$(CC) -c $(CGIINC) $(DATAINC) $(LOGINMODEL)  Report_Parameters_ui.cpp 

Composite_Query_Popup.o: Composite_Query_Popup.cpp Composite_Query.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   makefile
	$(CC) -c $(CGIINC) $(DATAINC) $(LOGINMODEL) Composite_Query_Popup.cpp

Composite_Query_Test.o: Composite_Query_Test.cpp Composite_Query.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   makefile
	$(CC) -c $(CGIINC) $(DATAINC) Composite_Query_Test.cpp $(LOGINMODEL)

Report_Type_Popup.o: Report_Type_Popup.cpp Report_Type.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   
	$(CC) -c $(CGIINC) $(DATAINC) Report_Type_Popup.cpp $(LOGINMODEL)

Report_Conditioning_Popup.o: Report_Conditioning_Popup.cpp Report_Conditioning.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp    
	$(CC) -c $(CGIINC) $(DATAINC) Report_Conditioning_Popup.cpp $(LOGINMODEL)

WorkUI.o: WorkUI.cpp Work.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp    
	$(CC) -c $(CGIINC) $(DATAINC) WorkUI.cpp $(LOGINMODEL)

WorkList.o: WorkList.cpp WorkList.hpp Work.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   
	$(CC) -c $(CGIINC) $(DATAINC) WorkList.cpp $(LOGINMODEL)

Scheme_FieldUI.o: Scheme_FieldUI.cpp Scheme_Field.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   
	$(CC) -c $(CGIINC) $(DATAINC) Scheme_FieldUI.cpp $(OLOGIN)

Scheme_TableUI.o: Scheme_TableUI.cpp Scheme_Table.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   
	$(CC) -c $(CGIINC) $(DATAINC) Scheme_TableUI.cpp $(LOGINMODEL)

Scheme_Table_Popup.o: Scheme_Table_Popup.cpp Scheme_Table.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   
	$(CC) -c $(CGIINC) $(DATAINC) Scheme_Table_Popup.cpp $(LOGINMODEL)

Formation.o: Formation.cpp reform_base.hpp $(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp   
	$(CC) -c $(CGIINC) $(DATAINC) Formation.cpp $(LOGINMODEL)
		
