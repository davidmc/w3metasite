PAYFLOW PRO TRANSACTION CLIENT V3 - CGITEST EXAMPLE
---------------------------------------------------

This directory contains the Verisign cgitest example.

NOTE: This example is intended to allow verification that CGI scripts
      are successfully executing, and that the Payflow Pro client is
      able to send transactions to the Verisign server.

      It is a very simplified test which is not designed for actual web
      server use.

CONTENTS
--------
readme.txt: 	This file
cgitest.cgi:	Example cgi script
cgitest.htm:	Sample html for cgi script


RUNNING THE CGI EXAMPLE
-----------------------

1)You must have Perl installed to run this example.  This may be obtained from ActiveState at http://activestate.com

2)You must set the environment variable PFPRO_CERT_PATH to point to the directory that contains the file f73e89fd.0 in the certs subdirectory.  See ..\..\..\bin\readme.txt for instructions on setting this variable.

3)Create a new directory in the root web directory, ie. c:\inetpub\wwwroot.

4)Enable execute & script permissions in this directory from the web server.  

5)Create an application mapping in this directory to perl.exe for .cgi file types.

6)Copy cgitest.htm, cgitest.cgi, PFPro.exe into this directory.  The file PFPro.exe is found in the bin directory (..\..\..\bin).  Copy pfpro.dll to winnt\system32 directory.

7)Point your web browser to the cgitest.htm file, ie: http://<server_name>/<new_directory_name>/cgitest.htm, edit the values for User, Vendor and Partner.  Process.

8)The following should appear in your browser ( number values may  differ) as a Successful Result:

     response = RESULT=0&PNREF=V18A01039078&RESPMSG=Approved&AUTHCODE=419PNI&AVSADDR=X&AVSZIP=X 



CONTACT
-------
Verisign, Inc. 
http://www.verisign.com
See contact.txt for additional contact information