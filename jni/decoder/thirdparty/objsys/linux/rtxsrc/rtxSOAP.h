/*
 * Copyright (c) 2003-2013 Objective Systems, Inc.
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license and with the
 * inclusion of the above copyright notice. This software or any other
 * copies thereof may not be provided or otherwise made available to any
 * other person. No title to and ownership of the software is hereby
 * transferred.
 *
 * The information in this software is subject to change without notice
 * and should not be construed as a commitment by Objective Systems, Inc.
 *
 * PROPRIETARY NOTICE
 *
 * This software is an unpublished work subject to a confidentiality agreement
 * and is protected by copyright and trade secret law.  Unauthorized copying,
 * redistribution or other use of this work is prohibited.
 *
 * The above notice of copyright on this source code product does not indicate
 * any actual or intended publication of such source code.
 *
 *****************************************************************************/
/**
 * @file rtxSOAP.h : common SOAP socket communications functions
 */
#ifndef _RTXSOAP_H_
#define _RTXSOAP_H_

#include "rtxsrc/rtxCommon.h"
#include "rtxsrc/rtxSocket.h"

#define HTTP_VERSION "1.1"

typedef enum { OSSOAPV1, OSSOAPV2 } OSSoapVersion;

typedef struct OSSOAPCONN {
   OSCTXT*      pctxt;
   OSRTSOCKET   socket;
   OSSoapVersion soapVersion;
   const char*  endpoint;
   char*        host;
   int          port;
   const char*  path;
   const char*  soapAction;
   OSBOOL       keepAlive;
   OSBOOL       chunked;
   OSOCTET      spare[2];  /* maintain word boundary */
   size_t       contentLength;
} OSSOAPCONN;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function initializes a connection to a SOAP endpoint.
 *
 * @param pSoapConn - Pointer to SOAP connection structure.
 * @param pctxt - Pointer to an XBinder run-time context structure.
 * @param soapv - SOAP version that is to be used.
 * @param url - URL to which to connect.
 * @return - Operation status: 0 if success, negative code if error.
 */
EXTERNRT int rtxSoapInitConn
(OSSOAPCONN* pSoapConn, OSCTXT* pctxt, OSSoapVersion soapv, const char* url);

/**
 * This function accepts an incoming connection request and sets up
 * a stream on which to receive messages.
 *
 * @param listenSocket - Listener socket
 * @param pSoapConn - Pointer to SOAP connection structure.
 * @return - Operation status: 0 if success, negative code if error.
 */
EXTERNRT int rtxSoapAcceptConn
(OSRTSOCKET listenSocket, OSSOAPCONN* pSoapConn);

/**
 * This function creates a connection to a SOAP endpoint.  The endpoint is
 * described by a SOAP connection structure which must have been initialized
 * using the rtxSoapInitConn function.
 *
 * @param pSoapConn - Pointer to SOAP connection structure.
 * @return - Operation status: 0 if success, negative code if error.
 */
EXTERNRT int rtxSoapConnect (OSSOAPCONN* pSoapConn);

/**
 * This function receives the initial header returned from an HTTP
 * request.  The header response information including content length
 * and whether the response is 'chunked' is stored in the connection
 * structure.
 *
 * @param pSoapConn - Pointer to SOAP connection structure.
 * @return - Operation status: 0 if success, negative code if error.
 */
EXTERNRT int rtxSoapRecvHttp (OSSOAPCONN* pSoapConn);

/**
 * This function receives a complete HTTP response from a SOAP
 * connection.  The response if stored in a dynamic memory buffer
 * which is returned via the buffer pointer argument.  Memory is
 * allocated for the response using XBinder memory management, so it
 * will be freed when the context is freed or the rtxMemFree function
 * is called.  This buffer can now be used in a decode function call
 * to parse the received XML message into a program structure.
 *
 * @param pSoapConn - Pointer to SOAP connection structure.
 * @param ppbuf - Pointer to pointer to receive content buffer.
 * @return - Operation status: 0 if success, negative code if error.
 */
EXTERNRT int rtxSoapRecvHttpContent
(OSSOAPCONN* pSoapConn, OSOCTET** ppbuf);

EXTERNRT int rtxSoapRecvHttpLine
(OSSOAPCONN* pSoapConn, char* lbuf, size_t lbufsiz);

/**
 * This function sends a complete HTTP request to a SOAP
 * connection.  The request is stored in the XBinder context buffer.
 * If an XML encode operation was just completed, the calling this
 * function will send the encoded XML message to the SOAP endpoint.
 *
 * @param pSoapConn - Pointer to SOAP connection structure.
 * @param soapMsg - SOAP XML message to be sent.
 * @return - Operation status: 0 if success, negative code if error.
 */
EXTERNRT int rtxSoapSendHttp
(OSSOAPCONN* pSoapConn, const OSUTF8CHAR* soapMsg);

EXTERNRT int rtxHttpParseHdr
(OSSOAPCONN* pSoapConn, const char* key, const char* value);

EXTERNRT int rtxHttpPost (OSSOAPCONN* pSoapConn, size_t count);

EXTERNRT int rtxHttpPostHdr
(OSSOAPCONN* pSoapConn, const char* key, const char* value);

EXTERNRT int rtxHttpTagCmp (const char *s, const char *t);

#ifdef __cplusplus
}
#endif

#endif /* _RTXSOAP_H_ */
