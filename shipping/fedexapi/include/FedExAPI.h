#ifndef _FEDEXAPI_H_
#define _FEDEXAPI_H_

#ifdef __cplusplus
extern "C" 
{
#endif


#ifndef WIN32
#define WINAPI
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/* Indicates successful API function					*/
#define API_OK									0

/* not inited yet, need to do							*/
/* a APIInit()											*/
#define API_NOT_INIT_ERROR						-8

/* Initialization in APIInit()							*/
/* failed, probably system resource						*/
/* error.												*/
#define API_INIT_ERROR							-24
		
/* Invalid IP Address. Insure the						*/
/* IP Address for ATOM is correct.						*/
#define API_UNKNOWN_HOST_EXCEPTION				-2201

/* Invalid IP Address or port for						*/
/* the ATOM you are trying to							*/
/* connect with, or the ATOM you are					*/
/* trying to connect with is not						*/
/* running.												*/
#define API_UNABLE_TO_OPEN_SOCKET				-2202

/* Setting the read timeout you							*/
/* requested failed. Check your							*/
/* timeout value. (not used by the C api)				*/
#define API_SET_TIMEOUT_FAILED 					-2203

/* Unable to obtain resources							*/
/* necessary for communicating with						*/
/* the server.  Try closing some						*/
/* applications. (not used by the C api)				*/
#define API_UNABLE_TO_OPEN_OUTPUTSTREAM 		-2204

/* Unable to obtain resources							*/
/* necessary for communicating with						*/
/* the server.  Try closing some						*/
/* applications.(not used by the C api)					*/
#define API_UNABLE_TO_OPEN_INPUTSTREAM			-2205

/* The connection to FedEx timed out					*/
/* before receiving all of the reply					*/
/* or the api gets an error reading						*/
/* the data from the server.							*/
#define API_ERROR_READING_REPLY					-2206

/* The connection to FedEx closed						*/
/* before receiving any of the reply.					*/
#define API_ERROR_READING_HEADER				-2207

/* Contact FedEx. (not used by the C api)				*/
#define API_ERROR_READING_INPUT					-2208

/* Contact FedEx. (not used by the C api)				*/
#define API_ENCODING_EXCEPTION					-2209

/* Unable to determine the IP							*/
/* Address of this system.								*/
/* (not used by the C api)								*/
#define API_UNKNOWN_HOST_EXCEPTION_CLIENT		-2210

/* Invalid data received in reply.						*/
#define API_HEADER_INTERPRETATION_ERROR			-2211

/* The reply contained no data.							*/
#define API_ZERO_LENGTH_REPLY					-2212

/* Buffer passed to APIReceive() or						*/
/* APITransaction() is too small for					*/
/* message received.									*/
/* Max bytes allowed SHRT_MAX(128K)						*/
#define API_CLIENT_BUFFER_TOO_SMALL				-2213

/* API_ERROR_INVALID_STATUS								*/
/* (not used by the C api)								*/
#define API_ERROR_INVALID_STATUS				-2214

/* The communications channel may						*/
/* have been inadvertently closed.						*/
#define API_ERROR_SENDING_REQUEST				-2215

/* The transaction thread was							*/
/* interrupted before it finished.						*/
/* (not used by the C api)								*/
#define API_THREAD_INTERRUPTED_EXCEPTION		-2217

/* Unable to open HTTP connection						*/
#define API_HTTP_CONNECTION_FAILED				-2218

/* Unable to resolve proxy settings						*/
/* for an HTTP connection								*/
#define API_PROXY_ERROR							-2219

/* Transaction is Null or does not						*/
/* exist												*/
#define API_REQUEST_CONTAINED_NO_DATA			-2220

/* Unable to close socket handle						*/
#define API_UNABLE_TO_CLOSE_SOCKET				-2221

/* Unable to destroy socket handle						*/
#define API_UNABLE_TO_DESTROY_SOCKET			-2224

/* Invalid Port specified								*/
#define API_INVALID_PORT						-2225

/* General catch-all error								*/
#define API_UNKNOWN_ERROR						-2226

/* FedEx Ship Manager API is not or						*/
/* has not been installed on this						*/
/* machine.												*/
#define API_NOT_INSTALLED						-2256


typedef int session_t;


int WINAPI FedExAPIInit ();

void WINAPI FedExAPIRelease ();

int WINAPI FedExAPITransaction (char *system, int port, char *sBuf, 
                                          int sBufLen, 
                                          long universalTransCode, 
                                          char *rBuf, int rBufLen, 
                                          int *actualRecvBufLen);


#ifdef __cplusplus
}
#endif


#endif //FEDEXAPI_H
