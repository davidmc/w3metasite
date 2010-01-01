#ifdef __cplusplus
extern "C" {
#endif

#if defined(_AIX)
#define _NO_PROTO
#endif

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "pfpro.h"

#ifndef WIN32

#if !defined(USE_THREADS) & !defined(__sgi)
#define _MIT_POSIX_THREADS
#include <pthread.h>
pthread_t pthread_self() { return (pthread_t)1; }
int pthread_mutex_lock(pthread_mutex_t *mutex) { return 1; }
int pthread_mutex_unlock(pthread_mutex_t *mutex) { return 1; }
int pthread_mutex_init(pthread_mutex_t *mutex,
                const pthread_mutexattr_t *attr) { return 1; }
int pthread_mutex_destroy(pthread_mutex_t *mutex) { return 1; }

#endif

#endif
#ifdef __cplusplus
}
#endif


MODULE = PFProAPI           PACKAGE = PFProAPI

int
pfproInit()

void
pfproCleanup()

int
pfproCreateContext(context, host_addr, host_port, time_out, proxy_addr, proxy_port, proxy_logon, proxy_pass)
	int	&	context
	char *	host_addr
	int		host_port
	int		time_out
	char *	proxy_addr
	int		proxy_port
	char *	proxy_logon
	char *	proxy_pass

	OUTPUT:
	context

int
pfproDestroyContext(context)
	int		context

int
pfproSubmitTransaction(context, parmlist, parmlistlen, transresponse) 
	int     context
	char *	parmlist
	long	parmlistlen	
	char *	transresponse

	CODE:
	char *  response;
	char *vit = "&VIT_WRAPTYPE=Perl&VIT_WRAPVERSION=3.0";
	char *vitList = NULL;
	char *parmList = parmlist;
	if( strstr(parmList,"<?xml") == NULL )
	{
		vitList = (char *)malloc(strlen(parmlist) + strlen(vit) + 1);
		strcpy(vitList, parmlist);
		strcat(vitList, vit);
		parmList = vitList;
	}
	pfproSubmitTransaction(context, parmList, parmlistlen + strlen(vit), &response);
	if ( vitList != NULL ) free(vitList);
	transresponse = (char *)SvGROW(ST(3),strlen(response)+1);
	strcpy(transresponse, response);
	pfproCompleteTransaction(response);

	OUTPUT:
	transresponse
