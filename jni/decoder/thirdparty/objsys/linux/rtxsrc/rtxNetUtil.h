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
 * @file rtxNetUtil.h
 */
#ifndef _RTXNETUTIL_H_
#define _RTXNETUTIL_H_

#ifndef _OS_NOSOCKET

#include "rtxsrc/rtxContext.h"
#include "rtxsrc/rtxSocket.h"

typedef enum {
   OSRTNetNoProto, OSRTNetHttp, OSRTNetFtp, OSRTNetOtherP
} OSRTNetProtocol ;

typedef enum {
   OSRTNetNoTrans, OSRTNetTCP, OSRTNetUDP, OSRTNetOtherT
} OSRTNetTransport ;

typedef struct OSRTNETCONN {
   OSCTXT*      pctxt;
   OSRTSOCKET   socket;
   char*        urlbuf;         /* mutable URL buffer */
   const char*  domain;
   const char*  path;
   int          port;
   OSRTNetProtocol protocol;
   OSRTNetTransport transport;
} OSRTNETCONN;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function creates a new network connection to the given URL.
 *
 * @param pctxt - Pointer to run-time context structure.
 * @param url - URL to which to connect.
 * @return - Pointer to allocated network connection object or null
 *   if error.  If error, error information is stored in context variable
 *   and can be accessed using rtxErr* functions.  Allocated memory
 *   will be freed when rtxNetCloseConn is called.
 */
EXTERNRT OSRTNETCONN* rtxNetCreateConn (OSCTXT* pctxt, const char* url);

/**
 * This function closes a network connection.
 *
 * @param pNetConn - Pointer to network connection structure.  This is
 *   assumed to have been created using the rtxNetCreateNewConn.
 * @return - Operation status: 0 if success, negative code if error.
 */
EXTERNRT int rtxNetCloseConn (OSRTNETCONN* pNetConn);

/**
 * This function creates a network connection.  The network entity is
 * described by a network connection structure.  The network structure
 * may have been created from a URL using the \c rtxNetCreateConn function
 * or may have been initialized manually.
 *
 * @param pNetConn - Pointer to network connection structure.
 * @return - Operation status: 0 if success, negative code if error.
 * @see rtxNetCreateConn
 */
EXTERNRT int rtxNetConnect (OSRTNETCONN* pNetConn);

#ifdef __cplusplus
}
#endif

#endif /* _OS_NOSOCKET */

#endif
