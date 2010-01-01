PAYFLOW PRO JAVA NATIVE INTERFACE (JNI) CLIENT 
----------------------------------------------
This directory contains the Payflow Pro Java Native Interface (JNI) client
and corresponding examples.  The Payflow Pro JNI Client allows the processing
of realtime credit card transactions using Verisign Payment Services.


SUPPORTED PLATFORMS
-------------------
The Payflow Pro Java Native Interface (JNI) client is currently supported on
the following platforms, built using the corresponding JDK version:

Platform:               	Directory Name:    JDK Version:
    Windows NT/2000              win32             1.2.2
    SunOS 5.6/Intel             suni86pc           1.1.3
    SunOS 5.6 & 5.7/Sparc       solaris            1.1.6
    Linux 2.0.36/Intel           linux             1.1.7a
    IRIX 6.2/MIPS                irix              1.1.6
    IBM AIX 4.3/RS6000            aix              1.1.6

The JNI client is for use with any JDK versions 1.1 and 1.2.


CONTENTS
--------
Verisign.jar:              Java archive

JNI Library:
  pfprojni.dll   (NT):   Supporting Dynamic Link Library
      or
  libpfprojni.so (Unix): Supporting Shared Object

PFProJava.java:          Example that shows how to use Java client
readme.txt:              This file

javatest.bat/javatest.pl: Example script to run a test transaction


INSTALLATION
------------

Please reference http://www.verisign.com/payment for full instructions 
on setting up an account with Verisign.
 
Please see the enclosed, general client documentation for more information.

  Make sure that Verisign.jar and the current directory is in your CLASSPATH,
  and that the JNI Library may be found in the path:

        NT/95: 
           set CLASSPATH=%CLASSPATH%;Verisign.jar;.

           Copy pfprojni.dll to a directory in the path, ie. c:\winnt\system32.

        UNIX:

           The instructions below include both the C Shell and Bourne Shell
           environment commands for setting both the CLASSPATH and the Runtime
           Library Path which enables libPFProJNI.so to
           be found when executing the Java Native Interface for Payflow Pro Client.

           Note: If you copy the libpfprojni.so file
                 to a directory in the path, i.e. /usr/lib, you do not need
                 to perform the setting of the Runtime Library Path below,
                 however you must still set the CLASSPATH in any case.

           Note: 
               For AIX  platforms, use LIBPATH in place of LD_LIBRARY_PATH.
               For HPUX platforms, use SHLIB_PATH in place of LD_LIBRARY_PATH.
               For all other platforms, LD_LIBRARY_PATH is appropriate.

           For the environment settings below, you may change the libpath value
           to be the correct directories into which the Payflow Pro client pfpro Library
           shared object libPFProJNI.so have been installed.

           C Shell:

           #!/bin/csh
           if ($?CLASSPATH) then
               setenv CLASSPATH ${CLASSPATH}:Verisign.jar:.
           else
               setenv CLASSPATH Verisign.jar:.
           endif

           set libpath=.:../jni

           if ($?LD_LIBRARY_PATH) then
               setenv LD_LIBRARY_PATH $libpath:${LD_LIBRARY_PATH}
           else
               setenv LD_LIBRARY_PATH $libpath
           endif

           Bourne Shell:

           #!/bin/sh
           CLASSPATH=${CLASSPATH:-}:Verisign.jar:.;export CLASSPATH

           libpath=.:../jni

           LD_LIBRARY_PATH=$libpath:${LD_LIBRARY_PATH:-};export LD_LIBRARY_PATH


EXAMPLE
-------
For code examples, please see PFProJava.java
See the javadoc documentation in javadoc.

To compile the PFProJava example:
        javac PFProJava.java

To run a test transaction using the PFProJava example 
(you may need to change the user, vendor, partner, and password to what you signed up with):
        java PFProJava test.verisign.com 443 "USER=user&VENDOR=vendor&PARTNER=partner&PWD=password&TRXTYPE=S&TENDER=C&ACCT=5105105105105100&EXPDATE=1209&AMT=1.23&ZIP=12345&comment1=Test JNI Transactions" 30

You should receive a response similar to this:

        RESULT=0&PNREF=VXYZ05238423&RESPMSG=Approved&AUTHCODE=596PNI&AVSADDR=Y&AVSZIP=Y 

For convenience, you can run javatest.bat (on NT), or perl javatest.pl (on Unix), to perform
the above steps to compile and run the test transaction using the PFProJava example.


CHANGES
-------
        PFProAPI is now in the package com.Verisign.payment.  You must use:

                import com.Verisign.payment.PFProAPI;

        to use the calls. Again, please be sure that your classpath
        has been set apropriately and that java can find the pfprojni
        shared library (DLL or .so).


NOTE
----
* You must set the environment variable PFPRO_CERT_PATH to point to the directory that 
    contains the file f73e89fd.0 in the certs subdirectory


CONTACT
-------
Verisign, Inc. 
http://www.verisign.com
See contact.txt for additional contact information