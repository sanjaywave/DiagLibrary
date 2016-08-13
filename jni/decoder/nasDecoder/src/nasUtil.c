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

#include "nasUtil.h"
#include "TS24008IES.h"
#include "rtxsrc/rtxError.h"

int NAS_Set_TS24007L3_Header
(OSCTXT* pctxt, TS24007L3_Header* pheader, TS24007L3_ProtoDiscr protoDiscr,
 OSUINT8 msgType, OSUINT8 transId, OSBOOL tiFlag)
{
   if (0 == pheader) return RTERR_NULLPTR;
   OSCRTLMEMSET (pheader, 0, sizeof(TS24007L3_Header));
   pheader->protoDiscr = protoDiscr;
   pheader->msgType = msgType;
   if (protoDiscr == TS24007L3_ProtoDiscr_callCtrl ||
       protoDiscr == TS24007L3_ProtoDiscr_sessMgmt) {
      pheader->l3HdrOpts.t = T_TS24007L3_L3HdrOptions_transId;
      pheader->l3HdrOpts.u.transId.flag = tiFlag;
      pheader->l3HdrOpts.u.transId.value = transId;
   }
   else {
      pheader->l3HdrOpts.t = T_TS24007L3_L3HdrOptions_skipInd;
   }
   return 0;
}

int NAS_Get_TS24008IE_EmergencyNumberLength
(const TS24008IE_EmergencyNumber* pvalue, OSUINT8* plength)
{
   if (0 == pvalue || 0 == plength) return RTERR_NULLPTR;
   if (0 != pvalue->digits) {
      /* Calculate length in bytes */
      OSSIZE digitsLen = OSCRTLSTRLEN (pvalue->digits);
      OSSIZE nbytes = (digitsLen/2 + 1);

      /* If odd digits length, add extra byte to record length */
      if (digitsLen % 2 != 0) nbytes++;

      if (nbytes <= 255) {
         *plength = (OSUINT8) nbytes;
      }
      else return RTERR_TOOBIG;
   }
   else {
      *plength = 1; /* length of catvalue */
   }

   return 0;
}



