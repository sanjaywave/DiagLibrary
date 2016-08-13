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
 * @file rtxIntDecode.h
 * General purpose integer decode functions.  These decode integer value
 * contents that are encoded in big-endian form.  This is a common format
 * for a number of different encoding rules.
 */
#ifndef _RTXINTDECODE_H_
#define _RTXINTDECODE_H_

#include "rtxsrc/rtxContext.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This macro decodes an 8-bit signed integer at the current
 * message buffer/stream location and advances the pointer to the next field.
 *
 * @param pctxt       Pointer to context block structure.
 * @param pvalue      Pointer to decoded 8-bit integer value.
 * @return            Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
#define rtxDecInt8(pctxt,pvalue) rtxReadBytes(pctxt,pvalue,1)

/**
 * This function decodes an 16-bit signed integer at the current
 * message buffer/stream location and advances the pointer to the next field.
 *
 * @param pctxt       Pointer to context block structure.
 * @param pvalue      Pointer to decoded 16-bit integer value.
 * @param nbytes      Number of bytes to decode (2 or less).
 * @return            Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNRT int rtxDecInt16 (OSCTXT* pctxt, OSINT16* pvalue, OSSIZE nbytes);

/**
 * This function decodes an 32-bit signed integer at the current
 * message buffer/stream location and advances the pointer to the next field.
 *
 * @param pctxt       Pointer to context block structure.
 * @param pvalue      Pointer to decoded 32-bit integer value.
 * @param nbytes      Number of bytes to decode (4 or less).
 * @return            Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNRT int rtxDecInt32 (OSCTXT* pctxt, OSINT32* pvalue, OSSIZE nbytes);

/**
 * This macro decodes an 8-bit unsigned integer at the current
 * message buffer/stream location and advances the pointer to the next field.
 *
 * @param pctxt       Pointer to context block structure.
 * @param pvalue      Pointer to decoded 8-bit integer value.
 * @return            Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
#define rtxDecUInt8(pctxt,pvalue) rtxReadBytes(pctxt,pvalue,1)

/**
 * This function decodes an 16-bit unsigned integer at the current
 * message buffer/stream location and advances the pointer to the next field.
 *
 * @param pctxt       Pointer to context block structure.
 * @param pvalue      Pointer to decoded 16-bit integer value.
 * @param nbytes      Number of bytes to decode (2 or less).
 * @return            Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNRT int rtxDecUInt16 (OSCTXT* pctxt, OSUINT16* pvalue, OSSIZE nbytes);

/**
 * This function decodes an 32-bit unsigned integer at the current
 * message buffer/stream location and advances the pointer to the next field.
 *
 * @param pctxt       Pointer to context block structure.
 * @param pvalue      Pointer to decoded 32-bit integer value.
 * @param nbytes      Number of bytes to decode (4 or less).
 * @return            Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNRT int rtxDecUInt32 (OSCTXT* pctxt, OSUINT32* pvalue, OSSIZE nbytes);

#ifdef __cplusplus
}
#endif

#endif
