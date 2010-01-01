/**************************************************************

	Sample Program:		webetest.c
	Purpose:			Demonstrate a 'C'-based FedExAPI sample

			Program Description

	This program will read a file called 'trans.in' and process
	transactions from it, assuming there is one transaction per
	line, using standard FedExAPI transaction format. Each
	transaction must be prefixed by the UTI (universal transaction
	identifier) for that transaction type and a colon character
	(":").
	
	The results will be written to a file called 'trans.out'.
	It is possible to override the input and out file file
	with command line options.

***************************************************************/
#ifdef WIN32
#include <windows.h>
#else
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#endif
#include <stdio.h>
#include <time.h>
#include "FedExAPI.h"
#include "lblexts.h"
#include "FDXParser.h"

int TranType;
int LabelType;


int get_time(char *tbuff)
{
	char stdate[20], sttime[20];
	int j = 0;
	struct tm *time_now;
	time_t secs_now;

		tzset ();
		time (&secs_now);
		time_now = localtime (&secs_now);
		strftime (stdate,
			10, "%m/%d/%y", time_now);
		strftime (sttime,
			10, "%H:%M:%S", time_now);

	 	printf("%-10.10s  %-10.10s  %s\n",
	 			stdate,
	 			sttime,
	 			tbuff);

		return(0);
}

/***********************************************************************************
	getUniversalTransactionID() - returns the UTI from the request buffer.
************************************************************************************/

long getUniversalTransactionID(char* fedexRequestBuffer)
{
    char *p;
	char temp[7];
	int i;
	int bufLen;
	long uti;

	bufLen = sizeof(fedexRequestBuffer);
	memset(temp, 0, 7);

	for(i = 0;i < 7;i++)
	{
		p = &fedexRequestBuffer[i];

		// if you reach a ":" in the request buffer, break.
		if( ( fedexRequestBuffer[i] ) == 0x3a )
			break;

		if( ( fedexRequestBuffer[i] ) == 0 )
			break;

	}

    if( i > 6 )
    {
        printf("Malformed UTI or UTI missing or the colon is missing");
        return -1;
    }
    strncpy(temp, fedexRequestBuffer, i);
	uti = atol(temp);

    if(temp != NULL)
        return uti;
    else
        return -1;
}

/*****************************************************************
	decode_fedexapi_request() - breaks the request buffer into
							   its name/value pairs
							   returns number of variables found
*****************************************************************/
int decode_fedexapi_request(char *fedexapi_req_buffer, long *offset)
{
    void *FdxRequestParser;
    char *Tag;
    char *Value;
    long tagBufSize = 10;
    long tagLength = 0;
    long valueLength = 0;
    int  nRetval = 0;

    Tag = (char *)malloc(10);
	memset(Tag, 0, 10);
    Value = (char *)malloc(200);
    memset(Value, 0, 200);

    FdxRequestParser = createParser();                                   // Create a Parser for the request
    if( !parse(FdxRequestParser, fedexapi_req_buffer, offset))    // Parse the request
    {
        nRetval = 1;
    }
    else
    {
        getTransactionType(FdxRequestParser, Value, 200, &valueLength);  // Get the transaction type
        printf("FedExAPI Input variable 0 = %s\n", Value );
		TranType = atoi(Value);
        getField(FdxRequestParser, "1369", Value, 200, &valueLength);
		LabelType = atoi(Value);
        memset(Value, 0, 200);

        enumerateTags(FdxRequestParser);                                 // Enumerate the tags this must be
																		 // done before calling getNextTag()
        getNextTag(FdxRequestParser, Tag, tagBufSize, &tagLength);       // Get the first tag

        while( Tag[0] != 0 )
        {
            getField(FdxRequestParser, Tag, Value, 200, &valueLength);    // Get the field that goes with the tag
            printf("FedExAPI Input variable %s = %s\n", Tag, Value );
            memset(Tag, 0, 10);
            memset(Value, 0, 200);
            getNextTag(FdxRequestParser, Tag, tagBufSize, &tagLength);    // Get the next tag
	    }

	}

    destroyParser(FdxRequestParser);                                      // Destroy the request parser
    free(Tag);
    free(Value);

	return(nRetval);
}


int decode_fedexapi_reply(char * fedexapi_req_buffer, char *fedexapi_reply_buffer, long *offset)
{


    void *FdxReplyParser;
    char *LabelFilename;
    char *CodLabelFilename;
    char *SPODFilename;
    char *Tag;
    char *Value;
    long tagBufSize = 10;
    long tagLength = 0;
    long valueLength = 0;
    int  nRetval = 0;

    Tag = (char *)malloc(10);
	memset(Tag, 0, 10);
    Value = (char *)malloc(200);
    memset(Value, 0, 200);
    LabelFilename = (char *)malloc(128);
    memset(LabelFilename, 0, 128);
    CodLabelFilename = (char *)malloc(128);
    memset(CodLabelFilename, 0, 128);
    SPODFilename = (char *)malloc(128);
    memset(SPODFilename, 0, 128);

	
	FdxReplyParser = createParser();                                  // Create a Parser for the reply

	if( !parse(FdxReplyParser, fedexapi_reply_buffer, offset)) // Parse the reply
        {
            nRetval = 2;
        }
        else
        {
           if( TranType == 21)
            {
               if( LabelType == 1)                     // Plain Paper Label image
			   {
							/* NOTE: The label extensions need to know where to find the
									 label template files.  You can pass their path in the
									 third argument of the following function OR you can
									 copy all the HTML files from the labels directory
									 into the current working directory and pass NULL as
									 the third argument, which tells the label extensions to
									 look for the templates in the current working directory.

							         The fourth argument is the path for the output files
							         generated by the label extensions.  If you pass NULL as
									 the fourth argument, they will be placed in the current
									 working directory.
							*/
                            nRetval = SaveLabelToFile(fedexapi_req_buffer, fedexapi_reply_buffer,
                                                  NULL, NULL, LabelFilename);
                }
                else                                           // Thermal label
                {
							/* NOTE: The third argument is the path for the output files
							         generated by the label extensions.  If you pass NULL as
									 the third argument, they will be placed in the current
									 working directory.
							*/
                            nRetval = SaveThermalLabelToFile(fedexapi_req_buffer, fedexapi_reply_buffer,
                                                    NULL, LabelFilename);
                }
             }
	         else if( TranType == 405)   // SPOD Transaction
             {
                nRetval = SaveSPODToFile(fedexapi_req_buffer, fedexapi_reply_buffer,
	                                     NULL, NULL, SPODFilename);
             }
        }
    
	destroyParser(FdxReplyParser);                                    // Destroy the reply parser

	free(Tag);
    free(Value);
    free(LabelFilename);
    free(CodLabelFilename);
    free(SPODFilename);
	return(nRetval);

}


main(int argc, char *argv[] )
{

	int i, q, rsize, fctn = 0, ll;
	char *p, system[200], Port[200];
    long UTI = 0;
	char in_name[200], out_name[200];
	int attempts = 1, port;
    char *sbuf = 0, *rbuf = 0;
	int sbuflength;
	int nRetval;
	long offset;
    FILE *fptr, *fptr1;

	printf("\nFedEx Ship Manager API TRANS.IN Processing System - Version 3.0\n\n");
	printf("This program is designed to take a file in your directory called\n");
	printf("'Trans.in' as input and read transactions.  Each transaction is then\n");
	printf("executed using FedEx Ship Manager API.  The results are then written to the \n");
	printf("file named 'Trans.out'");
	printf("\nYou may use the command line to specify the input and output file names.\nas arguments 1 and 2.\n\n");
	printf("NOTE: This program is hardcoded to use '127.0.0.1' as the service name\n");
	printf("      and 8190 as the port number.  You may specify a different host name\n");
	printf("      and port number on the command line as arguments 3 and 4.\n");
	printf("      For example: webetest trans.in trans.out 127.0.0.1 8190 \n\n");


	strncpy(in_name,"trans.in", 199);
	strncpy(out_name,"trans.out",199);
	strncpy(system,"127.0.0.1",199);
	strncpy(Port,"8190",199);

	if ( argc > 1)
	{
		printf("Using %s as input file\n", argv[1]);
		strncpy(in_name,argv[1],199);
	}

	if ( argc > 2)
	{
		printf("Using %s as output file\n", argv[2]);
		strncpy(out_name,argv[2],199);
	}

	if ( argc > 3 )
	{
		printf("Will use %s as the host name for NT Service\n", argv[3]);
		strncpy(system,argv[3],199);
	}

	if ( argc > 4 )
	{
		printf("Will use %s as the port number for NT Service\n", argv[4]);
		strncpy(Port,argv[4],199);
	}

	fptr = fopen(in_name, "r+");
	if ( fptr == NULL )
	{
		printf("ERROR - cannot open file %s for input\n", in_name);
		exit(-1);
	}

    port = atoi(Port);

	remove(out_name);

	for (;;)
	{

		fctn++;
        sbuf = (char *) malloc(32000);
		memset(sbuf, 0, 32000);
		fgets(sbuf, 4096, fptr);

		UTI = getUniversalTransactionID(sbuf);
		if (UTI < 0)
			break;

		if ( sbuf == NULL )
		{
			break;
		}
		else
		{
			i = strlen(sbuf);
			sbuf[i] = 0;
			if ( strlen(sbuf) < 5 )
				break;

			for (i=strlen(sbuf)-1; i > 0; i--)
			{
				if ( sbuf[i] > 32 )
					break;
				else
				{
					if ( sbuf[i] == 32 )
					{
						sbuf[i] = 0;
					}
				}
			}

		    // strip the Universal Transaction Identifier from the request buffer
		    p = (char *) strpbrk(sbuf, ":");
		    p++;
		    strcpy(sbuf, p);

            rbuf = (char *) malloc(128000);
			memset(rbuf, 0, 128000);
			sbuflength = strlen(sbuf);

            nRetval = decode_fedexapi_request(sbuf, &offset);

            printf("***********************************************\n");
            if( nRetval == 1 )
            {
               printf(" Error parsing request Transaction %d at offset %d.\n", fctn, offset);
			   break;
            }
            printf("***********************************************\n");

			printf("---------------------------------------------------------\n");
			printf("Number %d : Performing FedExAPITransaction()\n",  attempts);
			attempts++;
			printf("Sending: %s\n", sbuf);

			get_time("performing FedExAPITransaction()\n");


			i = FedExAPITransaction(system, port, sbuf, sbuflength, UTI, rbuf, 128000, &rsize);

			if ( i != API_OK )
			{
				printf("FedExAPITransaction Error %d\n", i);
				break;
			}
			else
			{
                int nRetval = decode_fedexapi_reply(sbuf, rbuf, &offset);
                printf("***********************************************\n");
                if( nRetval == 2 )
                {
                   printf(" Error parsing the reply Transaction at offset %d.\n", offset);
                }
                if( nRetval < 0)
                {
                   printf(" Error %d processing the label. Please refer to\n", nRetval);
                   printf(" FedExAPIErrors.pdf for description of error.\n");
                }
               printf("***********************************************\n");
			}

				get_time("FedExAPITransaction Results");
				printf("Received %d bytes, message below:\n----------------------------------------\n", rsize);
				q = rsize;
				if ( rsize > 500 )
					q = 500;
				for (ll=0; ll < q; ll++)
				{
					if ( ( isalnum(rbuf[ll])  ) || (rbuf[ll]=='"') || ( rbuf[ll] == ',' ) || ( rbuf[ll] == ' ' ) || ( rbuf[ll] == '.' ) || ( rbuf[ll] == '-'))
						printf("%c", rbuf[ll]);
					else
						printf("?");
				}
				printf("\n----------------------------------------\n");



				fptr1 = fopen(out_name, "a+b");
				if ( fptr1 != NULL )
				{
					fprintf(fptr1,"%s\n", rbuf);
					fclose(fptr1);
				}
		}
	}

	fclose(fptr);
    free(sbuf);
    free(rbuf);

    return(0);
}