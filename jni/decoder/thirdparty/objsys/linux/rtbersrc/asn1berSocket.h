/*
 * Copyright (c) 1997-2013 Objective Systems, Inc.
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
 * @file asn1berSocket.h
 */


#ifndef _ASN1BERSOCKET_H_
#define _ASN1BERSOCKET_H_

#ifndef _OS_NOSOCKET

#include "rtbersrc/asn1ber.h"
#include "rtxsrc/rtxSocket.h"

#ifdef __cplusplus
extern "C" {
#endif

/* utility functions */

/** @addtogroup berruntime
 * @{
 */
/** @addtogroup berperUtil
 * @{
 */
/**
 * This routine reads the BER message into the
 * given buffer. The TLV can be of indefinite length. If buffer is NULL,
 * dynamic buffer will be allocated and returned as ppDestBuffer. Length of
 * the message will be returned in pMessageSize.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param socket       The socket to read from. It should be already connected
 *                       TCP socket.
 * @param buffer       The static buffer to receive the parsed data. Can be NULL.
 * @param bufsiz       The size of the buffer to receive the parsed data.
 *                        Should be 0, if \c buffer is NULL.
 * @param ppDestBuffer The pointer to receive the destination buffer pointer. If
 *                        \c buffer is static, this parameter can be NULL.
 * @param pMessageSize The pointer to integer to receive the size of read
 *                        message.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNBER int berReadMsgFromSocket (OSCTXT* pctxt, OSRTSOCKET socket,
                                    OSOCTET* buffer, int bufsiz,
                                    OSOCTET** ppDestBuffer, int* pMessageSize);
/** @} berperUtil */

#ifdef __cplusplus
}
#endif

#endif /* _OS_NOSOCKET */

/** @} berruntime */
#endif /* _ASN1BERSOCKET_H_ */
