# GENERAL PURPOSE DEFINITION
include ../../build.mak
# for WEBDIR target
include makefile.metasite

all: $(WEBDIR)modules/report_module.so

clean: 
	rm *.o


# // shared libs
$(WEBDIR)modules/report_module.so: report_module.o
	gcc $(shared_link_opts)  -o $(WEBDIR)modules/report_module.so report_module.o $(CGILIB) $(DATALIB) $(MLIB) -luuid
 

#
# link applications
#

$(WEBDIR)Report_ui.cgi:  Report_ui.o
	$(CC) -o $(WEBDIR)Report_ui.cgi  Report_ui.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid

$(WEBDIR)Reports.cgi:  Reports.o
	$(CC) -o $(WEBDIR)Reports.cgi  Reports.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid


#
# --------------- Compile objects
#

Report_ui.o: Report_ui.cpp connectstring makefile $(SADMIN)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(OPD) -I$(METADIR) Report_ui.cpp
  
Reports.o: Reports.cpp Report.hpp rep_base.hpp col_handling_rep.hpp connectstring makefile $(SADMIN)openLogin.h
	$(CC) -c $(DATAINC) $(CGIINC) -I$(SADMIN) -I$(METADIR) Reports.cpp

report_module.o: report_module.cpp
	gcc  -c $(shared_compile_opts) $(CGIINC) $(DATAINC) report_module.cpp

