# makefile for rainbow custom logo mats e-commerce administration
include ../../build.mak

# comment out for group+role security model
LOGINMODEL=-DLOGIN_LITE_SECURITY=TRUE

# This is an extranet
XTRA=TRUE

# GENERAL PURPOSE DEFINITIONS
WEBDIR=../../web/projects/
TIMERENTRY=

#all entries

all: $(WEBDIR)CompanyUI.meta  $(WEBDIR)ContactUI.meta $(WEBDIR)ProjectUI.meta   \
$(WEBDIR)TaskUI.meta  $(WEBDIR)Time_Popup.cgi  $(WEBDIR)Status_Popup.cgi $(WEBDIR)StatusUI.meta  \
$(WEBDIR)InvoiceUI.meta  $(WEBDIR)XML_Time_svc.cgi  $(WEBDIR)Task_Popup.cgi $(WEBDIR)Messages_Popup.cgi $(WEBDIR)Attachments_Popup.cgi  \
$(WEBDIR)HostingUI.meta $(WEBDIR)ChargeUI.meta

clean:
	rm *.o

# link entries

$(WEBDIR)ChargeUI.meta: ChargeUI.o
	$(CC) -o $(WEBDIR)ChargeUI.meta  ChargeUI.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)HostingUI.meta: HostingUI.o
	$(CC) -o $(WEBDIR)HostingUI.meta  HostingUI.o $(CGILIB) $(DATALIB) $(MLIB) -luuid
	
$(WEBDIR)CompanyUI.meta: CompanyUI.o
	$(CC) -o $(WEBDIR)CompanyUI.meta  CompanyUI.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)ContactUI.meta: ContactUI.o
	$(CC) -o $(WEBDIR)ContactUI.meta  ContactUI.o $(CGILIB) $(DATALIB) $(MLIB)  -luuid

$(WEBDIR)ProjectUI.meta: ProjectUI.o
	$(CC) -o $(WEBDIR)ProjectUI.meta  ProjectUI.o $(CGILIB) $(DATALIB) $(MLIB)  -luuid

$(WEBDIR)TaskUI.meta: TaskUI.o
	$(CC) -o $(WEBDIR)TaskUI.meta  TaskUI.o $(CGILIB) $(DATALIB) $(MLIB)  -luuid

$(WEBDIR)Time_Popup.cgi: Time_Popup.o
	$(CC) -o $(WEBDIR)Time_Popup.cgi  Time_Popup.o $(CGILIB) $(DATALIB) $(MLIB)  -luuid

$(WEBDIR)Status_Popup.cgi: Status_Popup.o
	$(CC) -o $(WEBDIR)Status_Popup.cgi  Status_Popup.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)StatusUI.meta: StatusUI.o
	$(CC) -o $(WEBDIR)StatusUI.meta  StatusUI.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)InvoiceUI.meta: InvoiceUI.o
	$(CC) -o $(WEBDIR)InvoiceUI.meta  InvoiceUI.o $(CGILIB) $(DATALIB) $(MLIB)  -luuid

$(WEBDIR)XML_Time_svc.cgi: XML_Time_svc.o
	$(CC) -o $(WEBDIR)XML_Time_svc.cgi  XML_Time_svc.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)Task_Popup.cgi: Task_Popup.o
	$(CC) -o $(WEBDIR)Task_Popup.cgi  Task_Popup.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)Messages_Popup.cgi: Messages_Popup.o
	$(CC) -o $(WEBDIR)Messages_Popup.cgi  Messages_Popup.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)Attachments_Popup.cgi: Attachments_Popup.o
	$(CC) -o $(WEBDIR)Attachments_Popup.cgi  Attachments_Popup.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

# compile entries

ChargeUI.o: ChargeUI.cpp Charge.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp 
	$(CC) -c $(CGIINC) $(DATAINC) $(LOGINMODEL) ChargeUI.cpp $(IMPL_DEFS)

HostingUI.o: HostingUI.cpp Hosting.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp 
	$(CC) -c $(CGIINC) $(DATAINC) $(LOGINMODEL) HostingUI.cpp $(IMPL_DEFS)

CompanyUI.o: CompanyUI.cpp Company.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c  $(XGIINC) $(DATAINC) $(LOGINMODEL) CompanyUI.cpp $(IMPL_DEFS)

ContactUI.o: ContactUI.cpp Contact.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(XGIINC) $(DATAINC) $(LOGINMODEL) ContactUI.cpp $(IMPL_DEFS)

ProjectUI.o: ProjectUI.cpp Project.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(XGIINC) $(DATAINC) $(LOGINMODEL) ProjectUI.cpp $(IMPL_DEFS)
                                
TaskUI.o: TaskUI.cpp Task.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(XGIINC) $(DATAINC) $(LOGINMODEL) TaskUI.cpp $(IMPL_DEFS)

Time_Popup.o: Time_Popup.cpp Time.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(XGIINC) $(DATAINC) $(LOGINMODEL) Time_Popup.cpp $(IMPL_DEFS)

Status_Popup.o: Status_Popup.cpp  $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(XGIINC) $(DATAINC) $(LOGINMODEL) Status_Popup.cpp $(IMPL_DEFS)

StatusUI.o: StatusUI.cpp  $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(XGIINC) $(DATAINC) $(LOGINMODEL) StatusUI.cpp $(IMPL_DEFS)

InvoiceUI.o: InvoiceUI.cpp Invoice.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c  $(XGIINC) $(DATAINC) $(LOGINMODEL) InvoiceUI.cpp $(IMPL_DEFS)

XML_Time_svc.o: XML_Time_svc.cpp Time.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(XGIINC) $(DATAINC) $(LOGINMODEL) XML_Time_svc.cpp $(IMPL_DEFS)

Task_Popup.o: Task_Popup.cpp Task.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(XGIINC) $(DATAINC) $(LOGINMODEL) Task_Popup.cpp $(IMPL_DEFS)
	
 Messages_Popup.o: Messages_Popup.cpp Messages.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c  $(XGIINC) $(DATAINC) $(LOGINMODEL) Messages_Popup.cpp $(IMPL_DEFS)

Attachments_Popup.o: Attachments_Popup.cpp Attachments.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(XGIINC) $(DATAINC) $(LOGINMODEL) Attachments_Popup.cpp $(IMPL_DEFS)

