#
include ../../build.mak

WEBDIR=../../web/store/


# PUT IT IN THE TEMPLATE!
# make this the name of the checkout program for the group
COPROG=-DCHECKOUT="\"https://www.designmymat.com/checkOut.cgi\""


all: $(WEBDIR)openCart.cgi  $(INCLDIR)shoppingCart.hpp

#
# link applications
#
$(WEBDIR)openCart.cgi: openCart.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o 
	$(CC) -o $(WEBDIR)openCart.cgi  openCart.o $(DATALIB) $(CGILIB) $(MLIB)
	cp $(WEBDIR)openCart.cgi $(WEBDIR2)openCart.cgi

$(INCLDIR)shoppingCart.hpp: shoppingCart.hpp   
	cp shoppingCart.hpp $(INCLDIR)
    
#
#compile objects
#
openCart.o: openCart.cpp shoppingCart.hpp 
	$(CC) -c $(DATAINC) $(CGIINC) $(COPROG) openCart.cpp
  

clean: 
	rm openCart.o openCart.cgi $(WEBDIR)openCart.cgi
  
