#ifndef _PFPRO_H_
#define _PFPRO_H_

#ifdef __cplusplus
extern "C" {
#endif

int pfproInit();
void pfproCleanup();
int pfproCreateContext(int * context, char *host_addr, int host_port, int time_out, char *proxy_addr, int proxy_port, char *proxy_logon, char *proxy_pass);
int pfproDestroyContext(int context);
int pfproSubmitTransaction(int context, char *strParm, long lParmLen, char **strResp);
void pfproCompleteTransaction(char *strResp);
char *pfproVersion();

#ifdef __cplusplus
}
#endif

#endif // _PFPRO_H_
