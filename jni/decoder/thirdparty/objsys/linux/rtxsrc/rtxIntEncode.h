/*
 * Copyright (c) 2013-2013 Objective Systems, Inc.
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
 * @file rtxIntEncode.h
 * General purpose integer encode functions.  These encode integer value
 * contents into big-endian form which is a common format for a number
 * of different encoding rules.
 */
#ifndef _RTXINTENCODE_H_
#define _RTXINTENCODE_H_

#include "rtxsrc/rtxContext.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function will encode the given unsigned integer into big-endian
 * form.  One, two, and four byte fixed sizes are supported.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value       The value to be encoded.
 * @param size        Size of the field in bytes into which the value should
 *                       be encoded (1, 2, or 4).
 */
EXTERNRT int rtxEncUInt32 (OSCTXT* pctxt, OSUINT32 value, OSSIZE size);

#ifdef __cplusplus
}
#endif

#endif
