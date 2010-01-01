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

WEB=../web/
OBJ_TGT=../objects/
INCL_TGT=../include/


all: $(OBJ_TGT)Astring.o $(OBJ_TGT)cgiClass.o $(OBJ_TGT)cgiCookie.o $(OBJ_TGT)cgiTemplates.o \
$(OBJ_TGT)cgiCalendar.o \
$(INCL_TGT)Astring.h $(INCL_TGT)cgiCookie.h $(INCL_TGT)ocCoder.h  $(INCL_TGT)ocFileSys.h  \
$(INCL_TGT)Vector.h $(INCL_TGT)cgiCalendar.h  $(INCL_TGT)cgiExtend.h   $(INCL_TGT)ocCommon.h  $(INCL_TGT)ocString.h \
$(INCL_TGT)cgiClass.h   $(INCL_TGT)cgiTemplates.h  $(INCL_TGT)ocControls.h  $(INCL_TGT)openLogger.h \
$(INCL_TGT)ocXML.h $(INCL_TGT)ocTypes.h $(INCL_TGT)ocImportParser.h $(INCL_TGT)ocEnc.h \
testharness.cgi reflect.cgi

clean: 
	rm $(OBJ_TGT)Astring.o $(OBJ_TGT)cgiClass.o $(OBJ_TGT)cgiCookie.o $(OBJ_TGT)cgiTemplates.o \
$(OBJ_TGT)cgiCalendar.o \
$(INCL_TGT)Astring.h $(INCL_TGT)cgiCookie.h $(INCL_TGT)ocCoder.h  $(INCL_TGT)ocFileSys.h  \
$(INCL_TGT)Vector.h $(INCL_TGT)cgiCalendar.h  $(INCL_TGT)cgiExtend.h   $(INCL_TGT)ocCommon.h  $(INCL_TGT)ocString.h \
$(INCL_TGT)cgiClass.h   $(INCL_TGT)cgiTemplates.h  $(INCL_TGT)ocControls.h  $(INCL_TGT)openLogger.h \
$(INCL_TGT)ocXML.h $(INCL_TGT)ocTypes.h $(INCL_TGT)ocImportParser.h $(INCL_TGT)ocEnc.h
	rm T2 testharness.cgi  reflect.cgi
  
# move include files
$(INCL_TGT)Astring.h: Astring.h
	cp Astring.h $(INCL_TGT)Astring.h
$(INCL_TGT)cgiCookie.h: cgiCookie.h
	cp  cgiCookie.h $(INCL_TGT)cgiCookie.h
$(INCL_TGT)ocCoder.h: ocCoder.h
	cp  ocCoder.h $(INCL_TGT)ocCoder.h
$(INCL_TGT)ocFileSys.h: ocFileSys.h
	cp ocFileSys.h $(INCL_TGT)ocFileSys.h
$(INCL_TGT)Vector.h:
	cp  Vector.h $(INCL_TGT)Vector.h
$(INCL_TGT)cgiCalendar.h: cgiCalendar.h
	cp cgiCalendar.h  $(INCL_TGT)cgiCalendar.h
$(INCL_TGT)cgiExtend.h: cgiExtend.h
	cp  cgiExtend.h  $(INCL_TGT)cgiExtend.h
$(INCL_TGT)ocCommon.h: ocCommon.h
	cp   ocCommon.h $(INCL_TGT)ocCommon.h
$(INCL_TGT)ocString.h: ocString.h
	cp ocString.h $(INCL_TGT)ocString.h
$(INCL_TGT)cgiClass.h:  cgiClass.h
	cp cgiClass.h $(INCL_TGT)cgiClass.h
$(INCL_TGT)cgiTemplates.h:  cgiTemplates.h
	cp cgiTemplates.h $(INCL_TGT)cgiTemplates.h
$(INCL_TGT)ocControls.h:  ocControls.h
	cp ocControls.h $(INCL_TGT)ocControls.h
$(INCL_TGT)openLogger.h: openLogger.h
	cp openLogger.h $(INCL_TGT)openLogger.h
$(INCL_TGT)ocXML.h: ocXML.h
	cp ocXML.h $(INCL_TGT)ocXML.h
$(INCL_TGT)ocTypes.h: ocTypes.h
	cp ocTypes.h $(INCL_TGT)ocTypes.h 
$(INCL_TGT)ocImportParser.h: ocImportParser.h
	cp ocImportParser.h $(INCL_TGT)ocImportParser.h    
$(INCL_TGT)ocEnc.h: ocEnc.h
	cp ocEnc.h $(INCL_TGT)ocEnc.h    

# build object files
testharness.cgi: testharness.o $(OBJ_TGT)Astring.o $(OBJ_TGT)cgiClass.o $(OBJ_TGT)cgiCookie.o $(OBJ_TGT)cgiTemplates.o
	$(CC) -o testharness.cgi testharness.o $(OBJ_TGT)Astring.o $(OBJ_TGT)cgiClass.o $(OBJ_TGT)cgiCookie.o $(OBJ_TGT)cgiTemplates.o

reflect.cgi: reflect.o $(OBJ_TGT)Astring.o $(OBJ_TGT)cgiClass.o 
	$(CC) -o reflect.cgi reflect.o $(OBJ_TGT)Astring.o $(OBJ_TGT)cgiClass.o 
	
$(OBJ_TGT)Astring.o: Astring.cpp Astring.h Vector.h
	$(CC) -c Astring.cpp -o $(OBJ_TGT)Astring.o

$(OBJ_TGT)cgiClass.o: cgiClass.cpp cgiClass.h Astring.h Vector.h
	$(CC) -c cgiClass.cpp -o $(OBJ_TGT)cgiClass.o

$(OBJ_TGT)cgiCookie.o: cgiCookie.cpp cgiCookie.h Astring.h Vector.h
	$(CC) -c cgiCookie.cpp -o $(OBJ_TGT)cgiCookie.o

$(OBJ_TGT)cgiTemplates.o: cgiTemplates.cpp cgiTemplates.h Astring.h Vector.h ocString.h
	$(CC) -c cgiTemplates.cpp -o $(OBJ_TGT)cgiTemplates.o

$(OBJ_TGT)cgiCalendar.o: cgiCalendar.cpp cgiCalendar.h Astring.h Vector.h ocString.h
	$(CC) -c cgiCalendar.cpp -o $(OBJ_TGT)cgiCalendar.o    

testharness.o: testharness.cpp cgiTemplates.h Astring.h Vector.h ocString.h
	$(CC) -c testharness.cpp -o testharness.o

reflect.o: reflect.cpp Astring.h Vector.h ocString.h
	$(CC) -c reflect.cpp -o reflect.o




