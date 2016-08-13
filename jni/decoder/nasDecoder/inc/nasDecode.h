/*
 * Copyright (c) 2011-2013 Objective Systems, Inc.
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

#ifndef NAS_DECODE_H
#define NAS_DECODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxContext.h"
#include "rtxsrc/rtxBitDecode.h"
#include "rtxsrc/rtxIntDecode.h"
#include "rtsrc/rtBCD.h"

#define SHIFT_IEI 9

/**
 * This function decodes a BCD digit by reading the next 4-bit field at the
 * current stream position and converting to a character (0-9, *, #, a, b, c).
 *
 * @param pctxt        Pointer to context structure.
 * @param pdigit       Pointer to character to receive decoded digit.
 * @return             Completion status of operation:
 *                       - zero (0) = success,
 *                       - negative return value is error.
 * @ return
 */
int nasDecTBCDigit (OSCTXT* pctxt, char* pdigit);

/**
 * This function is used to decode an MCC/MNC pair.
 *
 * @param pctxt        Pointer to context structure.
 * @param mcc          Buffer to receive MCC value (3 digits).
 * @param mnc          Buffer to receive MNC value (3 digits).
 * @return             Completion status of operation:
 *                       - zero (0) = success,
 *                       - negative return value is error.
 * @ return
 */
int nasDec_MCC_MNC (OSCTXT* pctxt, char mcc[4], char mnc[4]);

/**
 * This function is used to decode common data present in different
 * alternatives within the MobileIdentify CHOICE type.  These include
 * the IMSI, IMEI, and IMEISV alternatives.
 *
 * @param pctxt        Pointer to context structure.
 * @param len          Length in bytes of the data to be decoded.
 * @param odd          Odd/even indicator boolean decoded from header.
 * @param digit1       First BCD character decoded from header.
 * @param strbuf       Character array to hold decoded value.  This must
 *                       be at least one byte larger to hold the maximum
 *                       sized string value to account for the null
 *                       terminator character.
 * @param bufsize      Size in bytes of strbuf.
 * @return             Completion status of operation:
 *                       - zero (0) = success,
 *                       - negative return value is error.
 */
int nasDecMobileIdentityType1
(OSCTXT* pctxt, OSUINT8 len, OSBOOL odd, OSUINT8 digit1,
 char* strbuf, OSSIZE bufsize);

/**
 * This function decodes the non-imperative part of an L3 message having
 * an empty non-imperative part.  This is not for use with circuit-switched
 * call control messages (section 9.3) as it does not handle the SHIFT IE
 * case.
 *
 * This will report an error if a comprehension required IE is found.
 *
 * @param pctxt        Pointer to context structure.
 * @return             Completion status of operation:
 *                       - zero (0) = success,
 *                       - negative return value is error.
 */
int ts24008Dec_NonImperative (OSCTXT* pctxt);

/**
 * This function decodes the non-imperative part of a circuit-switched
 * call control message (section 9.3) having an empty non-imperative part,
 * excepting the SHIFT IE which is defined as known.
 *
 * This will report an error if a comprehension required IE is found.
 *
 * @param pctxt        Pointer to context structure.
 * @return             Completion status of operation:
 *                       - zero (0) = success,
 *                       - negative return value is error.
 */
int ts24008Dec_CC_NonImperative (OSCTXT* pctxt);

#ifdef __cplusplus
}
#endif

#endif
