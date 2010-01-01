# Hi Mom!
include ../../build.mak

WEBDIR=../../web/customlogomats/

# make this the name of the checkout program for the group
COPROG=-DCHECKOUT="\"checkOut.cgi\""

all: $(WEBDIR)openCart.cgi $(WEBDIR)checkOut.cgi 

clean:
	rm *.o
  
#
# Link applications
#  

$(WEBDIR)openCart.cgi: openCart.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o 
	$(CC) -o $(WEBDIR)openCart.cgi  openCart.o $(DATALIB) $(CGILIB) $(MLIB)
  
$(WEBDIR)checkOut.cgi: checkOut.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o 
	$(CC) -o $(WEBDIR)checkOut.cgi  checkOut.o $(OBJDIR)ocMail.o $(DATALIB) $(CGILIB) $(MLIB)  -ltclink 


          
#
# Compile objects
#
  
openCart.o: openCart.cpp $(INCLDIR)shoppingCart.hpp 
	$(CC) -c $(DATAINC) $(CGIINC) $(COPROG) openCart.cpp 
  
checkOut.o: checkOut.cpp checkout_control.hpp pay_form.hpp Customer_ui.h
	$(CC) -c $(DATAINC) $(CGIINC) -I../admin checkOut.cpp
  
