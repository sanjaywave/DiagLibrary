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
 * @file rtx3GPP.h
 * Functions for implementing various algorithms described in various
 * 3GPP standards documents.
 */
#ifndef _RTX3GPP_H_
#define _RTX3GPP_H_

#include "rtxsrc/rtxContext.h"
#include "rtxsrc/rtxTBCD.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SHIFT_IEI 9

/**
 * This function converts a value from one of the bit range formats defined
 * in 3GPP TS 44.018 sections 10.5.2.13.3 through 10.5.2.13.6 into an
 * unsigned integer value.
 *
 * @param rfarray      Array of range format values.
 * @param frcount      Number of items in array on which conversion is
 *                       to be done.
 * @param range        Range value (1024, 512, 256, 128).
 * @param presult      Pointer to unsigned integer value to receive
 *                       result of the conversion.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNRT int rtx3GPPFromRangeFormat
(OSUINT32 rfarray[], OSUINT32 rfcount, OSUINT32 range, OSUINT32* presult);

/**
 * This function encodes an 8-bit ASCII character string
 * to GSM 7-bit format as specified in 3GPP TS 23.038.
 *
 * @param pctxt        Pointer to context structure
 * @param value        Character string to be encoded
 * @return             Completion status of operation:
 *                       - 0 = success
 *                       - negative return value is error.
 */
EXTERNRT int rtx3GPPEncGSMCharStr (OSCTXT* pctxt, const char* value);

/**
 * This function decodes characters encoded in GSM 7-bit format as
 * specified in 3GPP TS 23.038 to a standard ASCII character array.
 *
 * @param pctxt        Pointer to context structure
 * @param outbuf       Output array to receive decoded characters
 * @param outbufsiz    Size of output buffer
 * @param nchars       Number of characters to be decoded
 * @return             Completion status of operation:
 *                       - 0 = success
 *                       - negative return value is error.
 */
EXTERNRT int rtx3GPPDecGSMCharStr
(OSCTXT* pctxt, char* outbuf, OSSIZE outbufsiz, OSSIZE nchars);

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
EXTERNRT int rtx3GPPEnc_MCC_MNC 
(OSCTXT* pctxt, const char* mcc, const char* mnc);

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
EXTERNRT int rtx3GPPDec_MCC_MNC (OSCTXT* pctxt, char mcc[4], char mnc[4]);

/**
 * This function decodes the non-imperative part of an L3 message having
 * an empty non-imperative part.  This is not for use with circuit-switched
 * call control messages (3GPP TS 24.008 section 9.3) as it does not handle 
 * the SHIFT IE case.
 *
 * This will report an error if a comprehension required IE is found.
 *
 * @param pctxt        Pointer to context structure.
 * @param callCtrl     Boolean indicating this is a call control message.
 * @return             Completion status of operation:
 *                       - zero (0) = success,
 *                       - negative return value is error.
 */
EXTERNRT int rtx3GPPDecL3NonImperative (OSCTXT* pctxt, OSBOOL callCtrl);

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
 */
EXTERNRT int rtx3GPPEncMobileIdentityType1 
(OSCTXT* pctxt, OSUINT8 type, const char* pstr);

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
EXTERNRT int rtx3GPPDecMobileIdentityType1
(OSCTXT* pctxt, OSUINT8 len, OSBOOL odd, OSUINT8 digit1,
 char* strbuf, OSSIZE bufsize);

#ifdef __cplusplus
}
#endif

#endif
