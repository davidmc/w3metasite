# makefile for rainbow custom logo mats e-commerce administration
include ../../build.mak

# GENERAL PURPOSE DEFINITIONS
WEBDIR=../../web/customlogomats/sys/

#all entries
# Ordered_ui.cgi removed from all as it is typicaly specialized elsewhere.
all: $(WEBDIR)modules/ecomm_module.so  $(WEBDIR)Payment_Service_ui.cgi \
$(WEBDIR)Payment_Type_Parameter_ui.cgi $(WEBDIR)Order_Status_ui.cgi

clean:
	rm *.o
  
# shared libs

$(WEBDIR)modules/ecomm_module.so: ecomm_module.o
	gcc $(shared_link_opts)  -o $(WEBDIR)modules/ecomm_module.so ecomm_module.o $(CGILIB) $(DATALIB) $(MLIB)  -luuid
 

# link entries

$(WEBDIR)Payment_Service_ui.cgi: Payment_Service_ui.o
	$(CC) -o $(WEBDIR)Payment_Service_ui.cgi  Payment_Service_ui.o $(CGILIB) $(DATALIB) $(MLIB)  -luuid

$(WEBDIR)Payment_Type_Parameter_ui.cgi: Payment_Type_Parameter_ui.o
	$(CC) -o $(WEBDIR)Payment_Type_Parameter_ui.cgi  Payment_Type_Parameter_ui.o $(CGILIB) $(DATALIB) $(MLIB)   -luuid
      
$(WEBDIR)Ordered_ui.cgi: Ordered_ui.o
	$(CC) -o $(WEBDIR)Ordered_ui.cgi  Ordered_ui.o $(CGILIB) $(DATALIB) $(MLIB)  -luuid

$(WEBDIR)Order_Status_ui.cgi: Order_Status_ui.o
	$(CC) -o $(WEBDIR)Order_Status_ui.cgi  Order_Status_ui.o $(CGILIB) $(DATALIB) $(MLIB)   -luuid

$(WEBDIR)Customer_ui.cgi: Customer_ui.o
	$(CC) -o $(WEBDIR)Customer_ui.cgi  Customer_ui.o $(CGILIB) $(DATALIB) $(MLIB)   -luuid
 
$(WEBDIR)Product_Categories_ui.cgi: Product_Categories_ui.o
	$(CC) -o $(WEBDIR)Product_Categories_ui.cgi  Product_Categories_ui.o $(CGILIB) $(DATALIB) $(MLIB)   -luuid
               
# compile entries
  
ecomm_module.o: ecomm_module.cpp
	gcc  -c $(shared_compile_opts) $(CGIINC) $(DATAINC) -I$(METADIR) ecomm_module.cpp

Payment_Type_Parameter_ui.o: Payment_Type_Parameter_ui.cpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp 
	$(CC) -c $(DATAINC) $(CGIINC) -I$(METADIR) Payment_Type_Parameter_ui.cpp $(IMPL_DEFS)
  
Payment_Service_ui.o: Payment_Service_ui.cpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp 
	$(CC) -c $(DATAINC) $(CGIINC) -I$(METADIR) Payment_Service_ui.cpp $(IMPL_DEFS)

Ordered_ui.o: Ordered_ui.cpp Ordered.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp 
	$(CC) -c $(DATAINC) $(CGIINC) -I$(METADIR) Ordered_ui.cpp $(IMPL_DEFS)

Order_Status_ui.o: Order_Status_ui.cpp Order_Status.hpp $(FORMDIR)dataXfer.hpp \
$(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp \
$(FORMDIR)list_base.hpp 
	$(CC) -c $(DATAINC) $(CGIINC) -I$(METADIR) Order_Status_ui.cpp $(IMPL_DEFS)
  
