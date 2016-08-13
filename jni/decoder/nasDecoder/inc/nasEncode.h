/*
 * Copyright (c) 2013 Objective Systems, Inc.
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

#ifndef NAS_ENCODE_H
#define NAS_ENCODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rtxsrc/rtxContext.h"
#include "rtxsrc/rtxBitEncode.h"
#include "rtxsrc/rtxIntEncode.h"
#include "rtsrc/rtBCD.h"
#include "nasUtil.h"

/**
 * This function encodes a BCD digit (character 0-9, *, #, a, b, c) into
 * a 4-bit field.
 *
 * @param pctxt        Pointer to context structure.
 * @param digit        BCD digit to be encoded.
 * @return             Completion status of operation:
 *                       - zero (0) = success,
 *                       - negative return value is error.
 * @ return
 */
int nasEncBCDDigit (OSCTXT* pctxt, char digit);

/**
 * This function is used to encode an MCC/MNC pair.
 *
 * @param pctxt        Pointer to context structure.
 * @param mcc          MCC value (3 digits).
 * @param mnc          MNC value (3 digits).
 * @return             Completion status of operation:
 *                       - zero (0) = success,
 *                       - negative return value is error.
 * @ return
 */
int nasEnc_MCC_MNC (OSCTXT* pctxt, const char* mcc, const char* mnc);

/**
 * This function is used to encode different alternatives within the
 * MobileIdentify CHOICE type.  These include the IMSI, IMEI, and
 * IMEISV alternatives.
 *
 * @param pctxt        Pointer to context structure.
 * @param type         Choice type alternative (t) value.
 * @param pstr         Numeric string content of IMSI, IMEI, etc.
 * @return             Completion status of operation:
 *                       - zero (0) = success,
 *                       - negative return value is error.
 * @ return
 */
int nasEncMobileIdentityType1 (OSCTXT* pctxt, OSUINT8 type, const char* pstr);

#ifdef __cplusplus
}
#endif

#endif
