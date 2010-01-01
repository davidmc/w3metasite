Notes for Sample program webetest.exe

1)	This is a sample C/C++ program that is command line driven.
	It accepts as the first argument an input file from which to read
	The second argument is the output file to write for transaction results.
	If no arguments are provided, then the program will:
		A)	Read from trans.in
		B)	Write to trans.out

2)	The source to this program is in webetest.c.  You may
	examine this and use as an example for developing your own
	solutions.

3)	A sample trans.in file is provided that has sample transactions for:
		
		Type 402 -	Track by ship date
		Type 021 -	Perform US Domestic Shipment

4)	You will be required to include the Universal Transaction I.D. to the front
	of the transaction inside the trans.in file.  Here is an example of the 
	beginning of a ship transaction:

		3000:0,"021".....
	

	There are comments in this file on where to add your own
	variables such as FedEx Meter# and FedEx Acct# for fields
	10 and 498.


	