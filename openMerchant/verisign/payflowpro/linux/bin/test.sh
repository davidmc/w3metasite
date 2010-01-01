#!/bin/sh
echo  "----------------------------------------------------"
echo  " ***> Performing 'pfpro' binary test transaction....."
libpath=.:../lib
LD_LIBRARY_PATH=$libpath:${LD_LIBRARY_PATH:-};export LD_LIBRARY_PATH

 #AIX machine lib path environmental variable
LIBPATH=../lib
export LIBPATH

 #HPUX machine lib path environmental variable
SHLIB_PATH=../lib
export SHLIB_PATH
              
PFPRO_CERT_PATH=../certs;export PFPRO_CERT_PATH
./pfpro test-payflow.verisign.com 443 "USER=user&VENDOR=vendor&PARTNER=partner&PWD=password&TRXTYPE=S&TENDER=C&ACCT=5105105105105100&EXPDATE=1299&AMT=27.23" 10 
echo  "Done with 'pfpro' binary test transaction..."
echo  "---------------------------------------------------"

