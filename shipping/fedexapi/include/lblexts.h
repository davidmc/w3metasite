/********************************************************************************/
/*              Module  :       lblexts.h                                                   */
/*              Purpose :       Define label Extension Function prototypes                  */
/*              Author  :       FedEx                                                       */
/*                                                                              */
/********************************************************************************/

#ifndef _LBLEXTS_H_
#define _LBLEXTS_H_

#ifdef __cplusplus
extern "C" 
{
#endif

/************************************************************/
/**     These functions can be called only by Windows clients. **/
/************************************************************/
#ifdef WIN32
int DisplayLabelInBrowser(char *RequestBuffer, char *ReplyBuffer, char *TemplatesPath, char *SavePath);
int DisplaySPODInBrowser(char *RequestBuffer, char *ReplyBuffer, char *TemplatesPath, char *SavePath);
int PrintThermalLabel(char *RequestBuffer, char *ReplyBuffer, short ComPort, long BaudRate, char *SavePath);
int PrintThermalWithNamedPrinter(char *PrinterName, char *RequestBuffer, char *ReplyBuffer, char *SavePath);
#endif
/**************************************************************************/
/**     These functions can be called by Windows, Linux, or Solaris clients. **/
/**************************************************************************/
int SaveLabelToFile(char *RequestBuffer, char *ReplyBuffer, char *TemplatesPath, char *SavePath, char *SavedFilename);
int SaveSPODToFile(char *RequestBuffer, char *ReplyBuffer, char *TemplatesPath, char *SavePath, char *SavedFilename);
int SaveThermalLabelToFile(char *RequestBuffer, char *ReplyBuffer, char *SavePath, char *SavedFilename);

//
// D E P R E C A T E D   F U N C T I O N S
//
/************************************************************/
/**     These functions can be called only by Windows clients. **/
/************************************************************/
#ifdef WIN32
int FedExLabelBrowser(char *RequestBuffer, char *ReplyBuffer);                                  /*      (deprecated) Print a Label via a Browser  */
int FedExSigProofOfDeliveryBrowser(char *RequestBuffer, char *ReplyBuffer);             /*  (deprecated) Print a Signature Proof of Delivery Letter via a browser  */
int FedExLabelThermal(char *RequestBuffer, char *ReplyBuffer, 
        char *CommPort, char *CommSpeed);                                                                                       /*      (deprecated) Print a Thermal Label  */
#endif
/**************************************************************************/
/**     These functions can be called by Windows, Linux, or Solaris clients. **/
/**************************************************************************/
int FedExPrepareLabel(char *RequestBuffer, char *ReplyBuffer, 
        char *OutBoundHTMLFilename, char *CODReturnHTMLFilename);                                       /*      (deprecated) Prepares a FedEx label -- GIF on PNG  */

#ifdef __cplusplus
}
#endif

/*
        Error codes defined for LBLEXTS.DLL
*/

#define LABEL_OK                                                                          0     /* Indicates successful Label function  */
#define LABEL_COMM_ERROR                                                         -1     /* Generic communications error                 */
#define LABEL_TIMEOUT_ERROR                                                      -2     /* Generic timeout error                            */
#define LABEL_NO_REQUEST                                                         -3     /* No Request Transaction Done              */
#define LABEL_NO_RESPONSE                                                        -4     /* No Response Transaction Done             */
#define LABEL_NO_XCTN                                                            -5     /* No XCTN found in the Request buffer  */
#define LABEL_NOT_SHIP_XCTN                                                      -6     /* Not a Ship Xctn been done                */
#define LABEL_WRITE_ERROR                                                        -7     /* Error writing file to disk               */
#define LABEL_MISSING_FILES                                                      -8     /* one of the support files is missing  */
#define LABEL_NO_TRACK_NUMBER                                            -9     /* No Tracking Number found in buffer   */
#define LABEL_INVALID_COMM_PORT                                         -10     /* COMM Port parameter is missing               */
#define LABEL_INVALID_COMM_SPEED                                        -11     /* COMM Speed parameter is missing              */
#define LABEL_COMM_OPEN_FAILURE                                         -12     /* Cannot open COMM Port                            */
#define LABEL_FILE_NOT_FOUND                                            -13     /* Cannot open label file                           */
#define LABEL_NO_SENDER_COUNTRY                                         -14 /* Sender country required              */
#define LABEL_NO_RECIP_COUNTRY                                          -15 /* Recipient country required           */
#define LABEL_NO_LABEL_PRINTER_TYPE_IN_REQUEST          -16 /* Label type not specified in request      */
#define LABEL_TYPE_NOT_SUPPORTED_FOR_BROWSER            -17     /* Invalid label type for browser               */
#define LABEL_ERROR_ALLOCATING_MEMORY_FOR_LABEL         -18 /* malloc failed on XML Label           */
#define LABEL_NO_LABEL_MEDIA_TYPE_IN_REQUEST        -19 /* The request did not contain a label  */
                                                        /* Media Type                           */
#define LABEL_ERROR_ALLOCATING_MEMORY_FOR_DECODING  -20 /* malloc failed on decoding of label   */
#define LABEL_UNABLE_TO_WRITE_LABEL_BUFFER_FILE     -21 /* error writing label buffer to file   */
#define LABEL_NO_RECIP_STATE                                            -22 /* Recipient state required                         */
#define LABEL_PRINTER_TYPE_NOT_SUPPORTED_FOR_THERMAL    -23     /* Invalid printer type for thermal */
#define LABEL_MEDIA_TYPE_NOT_SUPPORTED_FOR_THERMAL      -24     /* Invalid media type for thermal       */
#define LABEL_BUFFER_EMPTY                                                      -25 /* Label data length was zero           */
#define LABEL_PRINT_TO_NAMED_PRINTER_FAILED                     -26 /* Print to Windows printer failed      */
#define LABEL_NAMED_PRINTER_NOT_FOUND                           -27 /* Named Windows printer not available  */
#define LABEL_NO_LABEL_IN_RESPONSE                  -28 /* No Label in reply                    */

#define SPOD_OK                                       0 /* Indicates successful SPOD function   */
#define SPOD_NO_REQUEST_XCTN                       -201 /* No XCTN found in the Request buffer  */
#define SPOD_NO_TRACK_NUMBER                       -202 /* No tracking number in request        */
#define SPOD_NO_REQUEST_TYPE                       -203 /* No Request type in request           */
#define SPOD_REQUEST_TYPE_NOT_VALID_FOR_PRINTING   -204 /* Request Type was not for a printable */
                                                        /* Relpy, ie. you requested a Fax       */ 
#define SPOD_ERROR_ALLOCATING_BUFFER_MEMORY        -205 /* malloc failed in parsing of reply    */
#define SPOD_IMAGE_BUFFER_TOO_SMALL                -206 /* the image was too large for the      */
                                                                                                                /* buffer                               */
#define SPOD_NO_LETTER_FORMAT_IN_REPLY             -207 /* Letter format was missing from reply */
#define SPOD_MISSING_HTML_FILES                    -208 /* SPOD HTML templates are missing      */
#define SPOD_NOT_A_SPOD_XCTN                                       -209 /* Non-SPOD transaction passed in       */
#define SPOD_BAD_LETTER_FORMAT_IN_REPLY                    -210 /* Invalid format code in reply         */
#define SPOD_NO_RESPONSE_XCTN                      -211 /* No XCTN found in the Response buffer */

#endif
