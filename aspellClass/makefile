# makefile for spellRight
include ../build.mak


#all entries

all: spellRight $(INCLDIR)AspellWrapperClass.hpp

clean:
	rm *.o  spellRight 
  
# link entries
spellRight: spellRight.o 
	$(CC) -o spellRight spellRight.o  -l aspell

     
# compile entries
  
spellRight.o: spellRight.cpp AspellWrapperClass.hpp 
	$(CC) -c spellRight.cpp  -I$(INCLDIR)

#move includes
$(INCLDIR)AspellWrapperClass.hpp: AspellWrapperClass.hpp
	cp  AspellWrapperClass.hpp  $(INCLDIR)
	

