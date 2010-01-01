# makefile for artisan point
include $(HOME)/development/build.mak

WEBDIR=../../web/console/


all: $(WEBDIR)category_form.cgi $(WEBDIR)category_list.cgi \
$(WEBDIR)sub_category_form.cgi $(WEBDIR)artisan_ui.cgi \
$(WEBDIR)art_product_ui.cgi $(WEBDIR)product_list.cgi \
$(WEBDIR)modules/artisan_module.so \
$(WEBDIR)artisan_group_ui.cgi $(WEBDIR)art_orders_ui.cgi \
$(WEBDIR)Payment_Type_Parameter_ui.cgi

clean:
	rm *.o
  
# // shared libs
$(WEBDIR)modules/artisan_module.so: artisan_module.o
	gcc $(shared_link_opts)  -o $(WEBDIR)modules/artisan_module.so artisan_module.o $(CGILIB) $(DATALIB) $(MLIB)
            
          
# link step 
$(WEBDIR)Payment_Type_Parameter_ui.cgi: Payment_Type_Parameter_ui.o
	$(CC) -o $(WEBDIR)Payment_Type_Parameter_ui.cgi  Payment_Type_Parameter_ui.o $(CGILIB) $(DATALIB) $(MLIB) -ltclink
  
$(WEBDIR)art_orders_ui.cgi: art_orders_ui.o
	$(CC) -o $(WEBDIR)art_orders_ui.cgi  art_orders_ui.o $(CGILIB) $(DATALIB) $(MLIB) -ltclink
  
$(WEBDIR)art_product_ui.cgi: art_product_ui.o
	$(CC) -o $(WEBDIR)art_product_ui.cgi  art_product_ui.o $(CGILIB) $(DATALIB) $(MLIB)

$(WEBDIR)artisan_ui.cgi: artisan_ui.o
	$(CC) -o $(WEBDIR)artisan_ui.cgi  artisan_ui.o $(CGILIB) $(DATALIB) $(MLIB)
  
$(WEBDIR)category_form.cgi: category_form.o
	$(CC) -o $(WEBDIR)category_form.cgi  category_form.o $(CGILIB) $(DATALIB) $(MLIB)

$(WEBDIR)sub_category_form.cgi: sub_category_form.o
	$(CC) -o $(WEBDIR)sub_category_form.cgi  sub_category_form.o $(CGILIB) $(DATALIB) $(MLIB)

$(WEBDIR)artisan_group_ui.cgi:  artisan_group_ui.o
	$(CC) -o $(WEBDIR)artisan_group_ui.cgi  artisan_group_ui.o $(CGILIB) $(DATALIB) $(MLIB)
       
$(WEBDIR)category_list.cgi: category_list.o
	$(CC) -o $(WEBDIR)category_list.cgi  category_list.o $(CGILIB) $(DATALIB) $(MLIB)      

$(WEBDIR)product_list.cgi: product_list.o
	$(CC) -o $(WEBDIR)product_list.cgi  product_list.o $(CGILIB) $(DATALIB) $(MLIB) 

  
    
# compile step  

artisan_module.o: artisan_module.cpp
	gcc  -c $(shared_compile_opts) $(CGIINC) $(DATAINC) artisan_module.cpp

art_product_ui.o: art_product_ui.cpp Products.hpp $(FORMDIR)dataXfer.hpp $(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp 
	$(CC) -c $(DATAINC) $(CGIINC) art_product_ui.cpp

art_orders_ui.o: art_orders_ui.cpp Orders.hpp $(FORMDIR)dataXfer.hpp $(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp 
	$(CC) -c $(DATAINC) $(CGIINC) art_orders_ui.cpp
   
artisan_ui.o: artisan_ui.cpp Person.hpp $(FORMDIR)dataXfer.hpp $(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp 
	$(CC) -c $(DATAINC) $(CGIINC) artisan_ui.cpp 
  
category_form.o: category_form.cpp Category.hpp $(FORMDIR)dataXfer.hpp $(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp 
	$(CC) -c $(DATAINC) $(CGIINC) category_form.cpp  

sub_category_form.o: sub_category_form.cpp Sub_Category.hpp $(FORMDIR)dataXfer.hpp $(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp 
	$(CC) -c $(DATAINC) $(CGIINC) sub_category_form.cpp 

artisan_group_ui.o: artisan_group_ui.cpp Artisan_Group.hpp $(FORMDIR)dataXfer.hpp $(FORMDIR)read_write_base.hpp $(FORMDIR)forms_base.hpp $(FORMDIR)read_write_base.cpp $(FORMDIR)forms_base.cpp 
	$(CC) -c $(DATAINC) $(CGIINC) artisan_group_ui.cpp 
      
category_list.o: category_list.cpp read_base.hpp $(FORMDIR)list_base.hpp 
	$(CC) -c $(DATAINC) $(CGIINC) category_list.cpp      

product_list.o: product_list.cpp read_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC) product_list.cpp   
  
Payment_Type_Parameter_ui.o: Payment_Type_Parameter_ui.cpp read_base.hpp $(FORMDIR)list_base.hpp
	$(CC) -c $(DATAINC) $(CGIINC) Payment_Type_Parameter_ui.cpp 

