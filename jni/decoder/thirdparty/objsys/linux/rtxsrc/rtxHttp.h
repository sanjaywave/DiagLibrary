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
 * @file rtxHttp.h
 */
#ifndef _RTXHTTP_H_
#define _RTXHTTP_H_

#include "rtxsrc/rtxArrayList.h"
#include "rtxsrc/rtxNetUtil.h"

typedef struct OSRTHttpHeader {
   OSUINT8      majorVersion;
   OSUINT8      minorVersion;
   OSINT32      status;
   OSRTArrayList fields;
} OSRTHttpHeader;

typedef struct OSRTHttpContent {
   size_t length;
   OSOCTET* data;
} OSRTHttpContent;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function sends an HTTP request to a network connection.
 *
 * @param pNetConn - Pointer to network connection structure.
 * @param method - HTTP method to be used for request (GET or POST)
 * @param content - Content to be sent after header.
 * @param contentType - Type of content.
 * @return - Operation status: 0 if success, negative code if error.
 */
EXTERNRT int rtxHttpSendRequest (OSRTNETCONN* pNetConn,
   const char* method, const char* content, const char* contentType);

/**
 * This function receives the initial header returned from an HTTP
 * request.  The header response information is returned in the header
 * structure.
 *
 * @param pNetConn - Pointer to network connection structure.
 * @param pHeader - Pointer to header structure to receive returned data.
 * @return - Operation status: 0 if success, negative code if error.
 */
EXTERNRT int rtxHttpRecvRespHdr
(OSRTNETCONN* pNetConn, OSRTHttpHeader* pHeader);

/**
 * This function receives HTTP content.  All content associated with
 * the response header is stored in the given memory buffer.
 *
 * @param pNetConn - Pointer to network connection structure.
 * @param pHeader - Pointer to response header structure describing content.
 * @param pContent - Buffer to receive content.  Dynamic memory is allocated
 *                     for the content using the rtxMemAlloc function.
 * @return - Operation status: 0 if success, negative code if error.
 */
EXTERNRT int rtxHttpRecvContent
(OSRTNETCONN* pNetConn, OSRTHttpHeader* pHeader, OSRTHttpContent* pContent);

void rtxHttpPrintHeader (const OSRTHttpHeader* pHeader);

#ifdef __cplusplus
}
#endif

#endif
