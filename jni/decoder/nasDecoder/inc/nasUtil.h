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

#ifndef NAS_UTIL_H
#define NAS_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TS24007L3.h"

struct TS24008IE_EmergencyNumber;

/**
 * This function is used to populate the contents of a NAS 24.007
 * L3 header structure.
 *
 * @param pctxt        Pointer to context structure.
 * @param pheader      Pointer to header stucture to be populated.
 * @param protoDiscr   Protocol discriminator value.
 * @param msgType      Message type.
 * @param transId      Transaction ID (see 3GPP TS 24.007 11.2.3.1.3).
 * @param tiFlag       Transaction ID flag.
 * @return             Completion status of operation:
 *                       - zero (0) = success,
 *                       - negative return value is error.
 */
EXTERN int NAS_Set_TS24007L3_Header
(OSCTXT* pctxt, TS24007L3_Header* pheader, TS24007L3_ProtoDiscr protoDiscr,
 OSUINT8 msgType, OSUINT8 transId, OSBOOL tiFlag);

/**
 * This function calculates the length of an emergency number record
 * (TS 24.008 10.5.3.13).
 *
 * @param pvalue      Pointer to EmergencyNumber structure.
 * @param plength     Pointer to octet to receive calculated length.
 * @return             Completion status of operation:
 *                       - zero (0) = success,
 *                       - RTERR_TOOBIG if length value will not fit in octet.
 */
EXTERN int NAS_Get_TS24008IE_EmergencyNumberLength
(const struct TS24008IE_EmergencyNumber* pvalue, OSUINT8* plength);

#ifdef __cplusplus
}
#endif

#endif
