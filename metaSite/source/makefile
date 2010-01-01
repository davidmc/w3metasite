# base defs are here
include  ../../build.mak


WEBDIR=../../web/artisanpoint/



all: $(WEBDIR)pages.meta $(WEBDIR)uploads.meta $(WEBDIR)show.meta $(WEBDIR)contactus.meta 

includes: $(INCLDIR)page.hpp  $(INCLDIR)page_menu.hpp  $(INCLDIR)page_template.hpp  \
$(INCLDIR)pg_paragraph.hpp $(INCLDIR)page_control.hpp \
$(INCLDIR)pg_handlers.hpp

# copy page defs for subclassing
$(INCLDIR)page.hpp: page.hpp
	cp page.hpp $(INCLDIR)

$(INCLDIR)page_menu.hpp: page_menu.hpp
	cp page_menu.hpp $(INCLDIR)

$(INCLDIR)page_template.hpp: page_template.hpp
	cp page_template.hpp $(INCLDIR)

$(INCLDIR)pg_paragraph.hpp: pg_paragraph.hpp
	cp pg_paragraph.hpp $(INCLDIR)

$(INCLDIR)page_control.hpp: page_control.hpp
	cp page_control.hpp $(INCLDIR)

$(INCLDIR)pg_handlers.hpp: pg_handlers.hpp
	cp pg_handlers.hpp $(INCLDIR)


#
# link applications
#
$(WEBDIR)pages.meta: pages.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o 
	$(CC) -o $(WEBDIR)pages.meta  pages.o $(DATALIB) $(CGILIB) $(MLIB)
 
$(WEBDIR)uploads.meta: uploads.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o 
	$(CC) -o $(WEBDIR)uploads.meta  uploads.o $(DATALIB) $(CGILIB) $(MLIB)
     
$(WEBDIR)show.meta: show.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o 
	$(CC) -o $(WEBDIR)show.meta  show.o $(DATALIB) $(CGILIB) $(MLIB)   

$(WEBDIR)search.meta: search.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o 
	$(CC) -o $(WEBDIR)search.meta  search.o $(DATALIB) $(CGILIB) $(MLIB) 
    
$(WEBDIR)contactus.meta: contactus.o $(OBJDIR)Astring.o $(OBJDIR)cgiClass.o $(OBJDIR)oc_Mail.o
	$(CC) -o $(WEBDIR)contactus.meta  contactus.o  $(OBJDIR)oc_Mail.o  $(DATALIB) $(CGILIB) $(MLIB) -luuid

#
#compile objects
#
pages.o: pages.cpp page.hpp page_template.hpp page_menu.hpp pg_paragraph.hpp page_control.hpp
	$(CC) -c $(DATAINC) $(CGIINC) pages.cpp

uploads.o: uploads.cpp page.hpp page_template.hpp page_menu.hpp pg_paragraph.hpp page_control.hpp
	$(CC) -c $(DATAINC) $(CGIINC) uploads.cpp
      
show.o: show.cpp page.hpp page_template.hpp page_menu.hpp pg_paragraph.hpp page_control.hpp show_control.hpp
	$(CC) -c $(DATAINC) $(CGIINC) show.cpp  

search.o: search.cpp page.hpp page_template.hpp page_menu.hpp pg_paragraph.hpp page_control.hpp search_control.hpp
	$(CC) -c $(DATAINC) $(CGIINC) search.cpp 
     
contactus.o: contactus.cpp page.hpp page_template.hpp page_menu.hpp pg_paragraph.hpp
	$(CC) -c $(DATAINC) $(CGIINC) contactus.cpp  
  
clean:
	rm *.o

