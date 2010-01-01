/* tcexample.c - Complex example of TCLink API usage.
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

/* Insert some sample credit cards you wish to use into
 * this structure.
 */

enum { VISA = 0, MC, DISC, AMEX, ACH, BID, NUM_CARDS };

/* A few fake card numbers for testing.
 */
struct { char *cc, *exp; } cards[NUM_CARDS] =
{ { "4111111111111111", "0404" },      /* visa */
  { "5411111111111115", "0404" },      /* mastercard */
  { "6011111111111117", "0404" },      /* discover */
  { "341111111111111",  "0404" },      /* amex */
  { "123456789",  "55554444333222" },  /* ACH (electronic checks) */
	{ NULL, NULL }                       /* billing ID dummy value */
};

int main(int argc, char *argv[])
{
	TCLinkHandle handle;
	int arg = 1, card = -1, demo = 0, verify = 0, wallet = 0;
	char *action, *amount = 0, *transid = 0, *bid = 0;
	char buf[256], buf2[256];
	char *ptr;

	printf("TCTest (using TCLink version %s)\n", TCLinkGetVersion(buf));

	if (argc < 2) {
		printf("usage: %s <action> [<<card> | <transid>> <amount>] [bug] [demo] [verify] [wallet]\n",argv[0]);
		printf("  action  = sale | preauth | postauth | credit | store | unstore | walletsale\n");
		printf("  card    = visa | mc | disc | amex | bid | ach\n");
		printf("  transid = (14 character transid)\n");
		printf("  amount  = dollar amount in cents\n");
		printf("  demo    = (transaction is demo only)\n");
		return 4;
	}

	action = argv[arg++];
	if (!strcasecmp(action, "unstore"))
		bid = argv[arg++];
	else if (arg < argc)
	{
		if (isdigit(argv[arg][0])) transid = argv[arg++];
		else if (!strcasecmp(argv[arg], "visa")) card = VISA;
		else if (!strcasecmp(argv[arg], "mc"))   card = MC;
		else if (!strcasecmp(argv[arg], "disc")) card = DISC;
		else if (!strcasecmp(argv[arg], "amex")) card = AMEX;
		else if (!strcasecmp(argv[arg], "bid"))  card = BID;
		else if (!strcasecmp(argv[arg], "ach"))  card = ACH;
		if (card >= 0)
		  arg++;
	}
	
	if (card == BID && arg < argc)
		bid = argv[arg++];

	if (arg < argc && isdigit(argv[arg][0]))
		amount = argv[arg++];

	while (arg < argc)
	{
		if (!strcmp(argv[arg], "demo"))
			demo = 1;
		else if (!strcmp(argv[arg], "verify"))
			verify = 1;
		else if (!strcmp(argv[arg], "wallet"))
			wallet = 1;
		arg++;
	}

	handle = TCLinkCreate();

	TCLinkPushParam(handle, "custid", CUSTID);
	TCLinkPushParam(handle, "password", PASSWORD);
	TCLinkPushParam(handle, "action", action);
	if (amount)
		TCLinkPushParam(handle, "amount", amount);
	if (demo)
		TCLinkPushParam(handle, "demo", "y");
	if (verify)
		TCLinkPushParam(handle, "verify", "y");
	if (wallet)
		TCLinkPushParam(handle, "wallet", "y");

	if (!strcmp(action, "sale") || !strcmp(action, "walletsale") ||
	    !strcmp(action, "preauth") || !strcmp(action, "store"))
	{
		if (card == BID) {
			TCLinkPushParam(handle, "billingid", bid);
		}
		else {
			TCLinkPushParam(handle, "address1", "123 Test St.");
			TCLinkPushParam(handle, "zip", "90001");
			TCLinkPushParam(handle, "name", "Joe Test");

			if (card == ACH) {
				TCLinkPushParam(handle, "media", "ach");
				TCLinkPushParam(handle, "routing", cards[card].cc);
				TCLinkPushParam(handle, "account", cards[card].exp);
				TCLinkPushParam(handle, "checknum", "1001");     /* optional */
			}
			else {
				TCLinkPushParam(handle, "media", "cc");
				TCLinkPushParam(handle, "cc", cards[card].cc);
				TCLinkPushParam(handle, "exp", cards[card].exp);
			}
		}

		if (!strcmp(action, "sale") || !strcmp(action, "preauth"))
			TCLinkPushParam(handle, "avs", "n");  /* disable avs for testing */
	}
	else if (!strcmp(action, "credit"))
	{
		TCLinkPushParam(handle, "transid", transid);
	}
	else if (!strcmp(action, "postauth"))
	{
		TCLinkPushParam(handle, "transid", transid);
	}
	else if (!strcmp(action, "unstore"))
	{
		TCLinkPushParam(handle, "billingid", bid);
	}

	TCLinkSend(handle);

	ptr = TCLinkGetResponse(handle, "status", buf);
	if (ptr)
	{
		if (TCLinkGetResponse(handle, "transid", buf2))
			printf("transid is: %s\n", buf2);
		printf("status is: %s\n", ptr);
		if (!strcmp(ptr, "accepted") || !strcmp(ptr, "approved"))
			return 0;
		if (!strcmp(ptr, "decline") || !strcmp(ptr, "rejected")) {
			printf("declinetype is: %s\n", TCLinkGetResponse(handle, "declinetype", buf));
			return 1;
		}
		if (!strcmp(ptr, "error")) {
			printf("errortype is: %s\n", TCLinkGetResponse(handle, "errortype", buf));
			return 2;
		}
		if (!strcmp(ptr, "baddata")) {
			printf("offenders are: %s\n", TCLinkGetResponse(handle, "offenders", buf));
			return 3;
		}
	}

	return 3;
}

