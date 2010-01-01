#!/usr/bin/perl
##########################

# Set environment
$ENV{LD_LIBRARY_PATH} .=":.:..:../lib";
$ENV{CLASSPATH} .= ":Verisign.jar:.";


# Compile the code
print `javac PFProJava.java`; 

# Run the test
print `java PFProJava test-payflow.verisign.com 443 "USER=user&VENDOR=vendor&PARTNER=partner&PWD=password&TRXTYPE=S&TENDER=C&ACCT=4222222222222&EXPDATE=1209&AMT=14.42&COMMENT1[3]=123&COMMENT2=Good Customer&INVNUM=1234567890&STREET=5199 JOHNSON&ZIP=94588" 30;`;
print "\n";
