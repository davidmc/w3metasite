/* tctest.c - Simple example of TCLink usage.
 * __LICENSE_HEADER__ */

#include "tclink.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Insert your custid and password below.
 */

#define CUSTID   "XXXX"
#define PASSWORD "PPPP"

int main()
{
	char buf[1024];

	TCLinkHandle handle = TCLinkCreate();

	printf("Using TCLink version %s\n", TCLinkGetVersion(buf));

	TCLinkPushParam(handle, "custid",   CUSTID);
	TCLinkPushParam(handle, "password", PASSWORD);
	TCLinkPushParam(handle, "action",   "sale");
	TCLinkPushParam(handle, "amount",   "100");                /* $1.00 */
	TCLinkPushParam(handle, "cc",       "4111111111111111");   /* test Visa card */
	TCLinkPushParam(handle, "exp",      "0404");

	TCLinkSend(handle);

	printf("%s", TCLinkGetEntireResponse(handle, buf, sizeof(buf)));

	return 0;
}

