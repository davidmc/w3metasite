CC=g++

OBJDIR=../objects/
INCLDIR=../include/
INCL=-I$(INCLDIR)

all: $(INCLDIR)ocTrustCommerce.h $(INCLDIR)ocPayflowPro.h $(INCLDIR)paymentEnumerations.h T2

$(INCLDIR)paymentEnumerations.h: paymentEnumerations.h
	cp paymentEnumerations.h $(INCLDIR)
  
$(INCLDIR)ocTrustCommerce.h: ocTrustCommerce.h tcInfo.h config.h avsmap.h  tcInfoClass.h
	cp ocTrustCommerce.h $(INCLDIR)
	cp config.h $(INCLDIR)     
	cp avsmap.h $(INCLDIR)
	cp tcInfoClass.h $(INCLDIR)
   
$(INCLDIR)ocPayflowPro.h: ocPayflowPro.h
	cp ocPayflowPro.h $(INCLDIR)

testHarness: libpfpro.so testHarness.o
	$(CC) -o testHarness testHarness.o libpfpro.so

T2: libtclink.so T2.o
	$(CC) -o T2 T2.o libtclink.so    

testHarness.o: testHarness.cpp ocPayflowPro.h ocTrustCommerce.h   tcInfoClass.h
	 $(CC) -c testHarness.cpp $(INCL)

T2.o: T2.cpp ocTrustCommerce.h   tcInfoClass.h
	 $(CC) -c T2.cpp $(INCL)

clean:
	rm *.o T2 testHarness
   
