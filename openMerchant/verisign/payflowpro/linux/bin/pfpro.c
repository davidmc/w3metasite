#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pfpro.h"

char *GetNextParm(char **buffer)
{
    char *ptr = *buffer;
    char token = ' ';
    char *retPtr = NULL;

    if (ptr) {

        /* skip white space */
        while (*ptr == ' ')
            ptr++;

        if (ptr && *ptr == '"') {
            token = '"';
            ptr++;
        }

        if (ptr && *ptr == '\'') {
            token = '\'';
            ptr++;
        }

        retPtr = ptr;

        while (ptr && *ptr != token)
            ptr++;

        if (ptr) {
            *ptr = 0;
            ptr++;
        }

        *buffer = ptr;
    }

    return(retPtr);
}

#define IBUFLEN	512

int main(int argc, char *argv[])
{
    char    *hostAddress	= "test-payflow.verisign.com";
    int     portNum			= 443;
    long    timeout			= 30;
    char    *proxyAddress	= NULL;
    int     proxyPort		= 0;
    char    *proxyLogon		= NULL;
    char    *proxyPassword	= NULL;
    char    *parmList;
    char    *tranResponse;
	char	ibuf[IBUFLEN], *iptr = ibuf; 
    char    *temp			= NULL;
    int		rc;
	int     i;
    char    rcBuf[16];
    char    *resp;
	int		context;
	int     parmLen;


    if ((argc == 2) && (!strcmp(argv[1],"-i"))) {
        /* read input from stdin */
        if (!fgets(ibuf,sizeof(ibuf),stdin)) {
            printf("\nError readingfrom stdin");
            return -99;
        }

        if (iptr)
            hostAddress = GetNextParm(&iptr);

        if (iptr)
            portNum = atoi(GetNextParm(&iptr));

        if (iptr)
            parmList = GetNextParm(&iptr);

        if (iptr)
            timeout = atol(GetNextParm(&iptr));

        if (iptr)
            proxyAddress = GetNextParm(&iptr);

        if (iptr)
            proxyPort = atoi(GetNextParm(&iptr));

        if (iptr)
            proxyLogon = GetNextParm(&iptr);

        if (iptr)
            proxyPassword = GetNextParm(&iptr);
    }
	else if ((argc < 4) || (argc > 10)) {
        printf("%s %s\n", argv[0], pfproVersion());
        printf("Usage Error: \n\n");
        printf("%s <hostAddress> <hostPort> <parmList> <timeOut> <proxyAddress> <proxyPort> <proxyLogon> <proxyPassword>\n\n", argv[0]);
        printf("<hostAddress>   host name                   'test-payflow.verisign.com'\n");
        printf("<hostPort>      host port number            '443'\n");
        printf("<parmList>      parameter list              'TRXTYPE=S&TENDER=C&USER=user&VENDOR=vendor&PARTNER=partner&PWD=password&ACCT=5105105105105105100&ExpDate=1210&AMT=1.00'\n");
        printf("<timeOut>       timeout(sec) - optional     '30'\n");
        printf("<proxyAddress>  proxy name - optional       'proxy'\n");
        printf("<proxyPort>     proxy port - optional       '8080'\n");
        printf("<proxyLogon>    proxy logon name - optional 'admin'\n");
        printf("<proxyPassword> proxy password - optional   'password'\n");
        return -99;

    }

    else {

        hostAddress = argv[1];
        portNum = atoi(argv[2]);
        parmList = argv[3];

        if (argc >= 5)
            timeout = atol(argv[4]);
        else
            timeout = 30;

        if (argc >= 6)
            proxyAddress = argv[5];

        if (argc >= 7)
            proxyPort = atoi(argv[6]);

        if (argc >= 8)
            proxyLogon = argv[7];

        if (argc >= 9)
            proxyPassword = argv[8];
    }

	if (pfproInit()) {
	    printf("Failed to initialize client.\n");
		return 1;
	}

	if (pfproCreateContext(&context, hostAddress, portNum, timeout, proxyAddress, proxyPort, proxyLogon, proxyPassword)) {
	    printf("Failed to create a context.\n");
		return 1;
	}

	
	parmLen = strlen(parmList);


	pfproSubmitTransaction(context, parmList, parmLen, &tranResponse);

    printf("%s",tranResponse);

	/* get the result code */
    resp = strstr(tranResponse,"RESULT=");
    if (resp) {
        i = 0;
        resp+=7;
        while ((*resp != '&') && (i<=2)) {
            rcBuf[i]=*resp;
            resp++;
            i++;
        }
        rcBuf[i] = 0;
        rc = atoi(rcBuf);
    }
    else
        rc = 1;

	pfproCompleteTransaction(tranResponse);
	pfproDestroyContext(context);
	pfproCleanup();

    return rc;
}
