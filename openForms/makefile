include ../build.mak
#
#standard_tools directory
#
#make commands have two lines:
#1) The dependancy list:
#   target: dependencies
#2) The build line:
#   <tab> command line to build target from dependency list
#include ../build.mak

WEBDIR=../web/
OBJ_TGT=../objects/
INCL_TGT=../include/

all: $(WEBDIR)T2.cgi  $(WEBDIR)XMLTreeStates.cgi  $(WEBDIR)xmlAuditTable.cgi

clean:

# build object files
$(WEBDIR)T2.cgi: T2.o
	$(CC) -o $(WEBDIR)T2.cgi T2.o $(CGILIB) $(DATALIB) $(MLIB) -luuid


$(WEBDIR)XMLTreeStates.cgi: XMLTreeStates.o
	$(CC) -o $(WEBDIR)XMLTreeStates.cgi  XMLTreeStates.o $(CGILIB) $(DATALIB) $(MLIB) -luuid

$(WEBDIR)xmlAuditTable.cgi: xmlAuditTable.o
	$(CC) -o $(WEBDIR)xmlAuditTable.cgi  xmlAuditTable.o $(CGILIB) $(DATALIB) $(MLIB)  -luuid
	
T2.o: T2.cpp  openTrees.hpp
	$(CC) -c T2.cpp -o T2.o $(DATAINC) $(CGIINC) -I$(OTD)


XMLTreeStates.o: XMLTreeStates.cpp  $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC) XMLTreeStates.cpp $(IMPL_DEFS)

xmlAuditTable.o: xmlAuditTable.cpp AuditTable.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC) xmlAuditTable.cpp $(IMPL_DEFS)