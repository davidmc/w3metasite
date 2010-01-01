include ../build.mak
WEBDIR=../web/

all: ../include/Session.hpp $(WEBDIR)xmlSession.cgi $(WEBDIR)XML_Session_svc.cgi
#../include/userLogin.h

clean:
	rm *.o
	
../include/Session.hpp : Session.hpp
	cp Session.hpp ../include
  
#../include/userLogin.h : userLogin.h
#	cp userLogin.h ../include

# link entries


$(WEBDIR)xmlSession.cgi: xmlSession.o
	$(CC) -o $(WEBDIR)xmlSession.cgi  xmlSession.o $(CGILIB) $(DATALIB) $(MLIB)  -luuid

$(WEBDIR)XML_Session_svc.cgi: XML_Session_svc.o
	$(CC) -o $(WEBDIR)XML_Session_svc.cgi  XML_Session_svc.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

  
# compile entries
     

xmlSession.o: xmlSession.cpp Session.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC) xmlSession.cpp $(IMPL_DEFS)

XML_Session_svc.o: XML_Session_svc.cpp Session.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC) XML_Session_svc.cpp $(IMPL_DEFS)

