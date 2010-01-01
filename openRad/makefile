# GENERAL PURPOSE DEFINITION
include ../build.mak

WEBDIR=../web/rad/

all: $(WEBDIR)pickClassFile.cgi $(WEBDIR)pickClass.cgi \
$(WEBDIR)pickFields.cgi $(WEBDIR)pickTemplate.cgi   \
$(WEBDIR)parseDiaFile.cgi

clean: 
	rm *.o



#
# link applications
#

$(WEBDIR)pickClassFile.cgi:  pickClassFile.o  
	$(CC) -o $(WEBDIR)pickClassFile.cgi  pickClassFile.o  $(CGILIB) 
	cp pickClassFile.html $(WEBDIR) 
  
$(WEBDIR)pickClass.cgi:  pickClass.o  
	$(CC) -o $(WEBDIR)pickClass.cgi  pickClass.o  $(CGILIB) 
	cp pickClass.html $(WEBDIR) 
  
$(WEBDIR)pickFields.cgi:  pickFields.o  
	$(CC) -o $(WEBDIR)pickFields.cgi  pickFields.o  $(CGILIB)
	cp pickFields.html $(WEBDIR) 
  
$(WEBDIR)pickTemplate.cgi:  pickTemplate.o  
	$(CC) -o $(WEBDIR)pickTemplate.cgi  pickTemplate.o  $(CGILIB)
	cp pickTemplate.html $(WEBDIR) 
  
$(WEBDIR)parseDiaFile.cgi:  parseDiaFile.o
	$(CC) -o $(WEBDIR)parseDiaFile.cgi  parseDiaFile.o  $(CGILIB)
	cp pickDiaFile.html $(WEBDIR)
#
#compile objects
#
pickClassFile.o: pickClassFile.cpp 
	$(CC) -c  -I$(OPD) $(CGIINC) pickClassFile.cpp

pickClass.o: pickClass.cpp 
	$(CC) -c -I$(OPD) $(CGIINC) pickClass.cpp

pickFields.o: pickFields.cpp 
	$(CC) -c  -I$(OPD) $(CGIINC) pickFields.cpp

pickTemplate.o: pickTemplate.cpp codeMap.hpp
	$(CC) -c  -I$(OPD) $(CGIINC) pickTemplate.cpp


parseDiaFile.o: parseDiaFile.cpp codeMap.hpp
	$(CC) -c  -I$(OPD) $(CGIINC) parseDiaFile.cpp
	