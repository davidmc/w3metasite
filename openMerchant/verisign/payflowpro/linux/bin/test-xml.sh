#!/bin/sh
#!/bin/sh
echo  "--------------------------------------------------------------"
echo  " ***> Performing 'pfpro' binary test transaction using 'transaction.xml' file ....." 
libpath=.:../lib
LD_LIBRARY_PATH=$libpath:${LD_LIBRARY_PATH:-};export LD_LIBRARY_PATH

 #AIX machine lib path environmental variable
LIBPATH=../lib
export LIBPATH

 #HPUX machine lib path environmental variable
SHLIB_PATH=../lib
export SHLIB_PATH

PFPRO_CERT_PATH=../certs;export PFPRO_CERT_PATH

./pfpro-file test-payflow.verisign.com 443 transaction.xml 10 
echo  "Done with 'pfpro' binary test transaction..."
echo  "---------------------------------------------------"

