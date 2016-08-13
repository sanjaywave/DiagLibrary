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
 * @file rtxStreamSocket.h
 */
#ifndef _RTXSTREAMSOCKET_H_
#define _RTXSTREAMSOCKET_H_

#ifndef _OS_NOSOCKET

#ifndef _NO_STREAM

#include "rtxsrc/rtxStream.h"
#include "rtxsrc/rtxSocket.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup rtxStreamSocket Socket stream functions.
 * Socket stream functions are used for socket stream operations.
 * @{
 */
/**
 * Attaches the existing socket handle to the stream. The socket should be
 * already opened and connected. The 'flags' parameter specifies the access
 * mode for the stream - input or output.
 *
 * @param pctxt        Pointer to a context structure variable that has
 *                       been initialized for stream operations.
 * @param socket       The socket handle created by \c rtxSocketCreate.
 * @param flags        Specifies the access mode for the stream:
 *                       - OSRTSTRMF_INPUT = input (reading) stream;
 *                       - OSRTSTRMF_OUTPUT = output (writing) stream.
 * @return             Completion status of operation: 0 = success,
 *                       negative return value is error.
 */
EXTERNRT int rtxStreamSocketAttach
(OSCTXT* pctxt, OSRTSOCKET socket, OSUINT16 flags);

/**
 * This function closes a socket stream.
 *
 * @param pctxt        Pointer to a context structure variable that has
 *                       been initialized for stream operations.
 * @return             Completion status of operation: 0 = success,
 *                       negative return value is error.
 */
EXTERNRT int rtxStreamSocketClose (OSCTXT* pctxt);

/**
 * This function opens a socket stream for writing.
 *
 * @param pctxt        Pointer to a context structure variable that has
 *                       been initialized for stream operations.
 * @param host         Name of host or IP address to which to connect.
 * @param port         Port number to which to connect.
 * @return             Completion status of operation: 0 = success,
 *                       negative return value is error.
 */
EXTERNRT int rtxStreamSocketCreateWriter
(OSCTXT* pctxt, const char* host, int port);

/**
 * This function transfers ownership of the socket to or from the
 * stream instance. The socket will be closed and deleted when the stream
 * is closed or goes out of scope. By default stream socket owns the socket.
 *
 * @param pctxt        Pointer to a context structure variable that has
 *                       been initialized for stream operations.
 * @param ownSocket    Boolean value.
 */
EXTERNRT int rtxStreamSocketSetOwnership (OSCTXT* pctxt, OSBOOL ownSocket);

#ifdef __cplusplus
}
#endif
/**
 * @} rtxStreamSocket
 */

#endif /* _OS_NOSOCKET */

#endif /* _NO_STREAM */
#endif /* _RTXSTREAMSOCKET_H_ */
