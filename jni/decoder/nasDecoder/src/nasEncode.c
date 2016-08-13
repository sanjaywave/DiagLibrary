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

#include "nasEncode.h"
#include "rtsrc/rtBCD.h"
#include "rtxsrc/rtxCtype.h"
#include "rtxsrc/rtxDiagBitTrace.h"
#include "rtxsrc/rtxError.h"

int nasEncBCDDigit (OSCTXT* pctxt, char digit)
{
   OSUINT8 ub;

   int ret = rtTBCDCharToBin (digit, &ub);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   return rtxEncBits (pctxt, ub, 4);
}

int nasEnc_MCC_MNC (OSCTXT* pctxt, const char* mcc, const char* mnc)
{
   int ret;

   /* Encode MCC and MNC digits */
   RTDIAG_NEWBITFIELD (pctxt, "mcc/mnc");

   ret = nasEncBCDDigit (pctxt, mcc[1]);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   ret = nasEncBCDDigit (pctxt, mcc[0]);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   if (mnc[2] == 0x0F || mnc[2] == 0) {
      ret = rtxEncBits (pctxt, 0x0F, 4);
   }
   else {
      ret = nasEncBCDDigit (pctxt, mnc[2]);
   }
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   ret = nasEncBCDDigit (pctxt, mcc[2]);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   ret = nasEncBCDDigit (pctxt, mnc[1]);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   ret = nasEncBCDDigit (pctxt, mnc[0]);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   RTDIAG_SETBITFLDCOUNT (pctxt);

   return 0;
}

/* Common function for encoding several alternatives of MobileIdentity */

int nasEncMobileIdentityType1 (OSCTXT* pctxt, OSUINT8 type, const char* pstr)
{
   OSSIZE  len;
   OSBOOL  odd;
   OSUINT8 ub;
   int     ret;

   if (0 == pstr) return LOG_RTERR (pctxt, RTERR_NULLPTR);

   len = OSCRTLSTRLEN (pstr);
   odd = ((len % 2) == 1);

   /* Encode length */
   RTDIAG_NEWBITFIELD (pctxt, "length");

   ret = rtxEncBits (pctxt, (OSUINT32)(len/2 + 1), 8);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   RTDIAG_SETBITFLDCOUNT (pctxt);

   /* Encode first digit */
   RTDIAG_NEWBITFIELD (pctxt, "digit1");

   ret = nasEncBCDDigit (pctxt, *pstr++);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   RTDIAG_SETBITFLDCOUNT (pctxt);

   /* Encode odd/even indicator */
   RTDIAG_NEWBITFIELD (pctxt, "odd");

   ret = rtxEncBit (pctxt, odd);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   RTDIAG_SETBITFLDCOUNT (pctxt);

   /* Encode type of identity */
   RTDIAG_NEWBITFIELD (pctxt, "type");

   ret = rtxEncBits (pctxt, type, 3);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   RTDIAG_SETBITFLDCOUNT (pctxt);

   /* Encode remaining digits */
   RTDIAG_NEWBITFIELD (pctxt, "");

   while (*pstr != '\0') {
      ret = rtTBCDCharToBin (*pstr++, &ub);
      if (0 != ret) return LOG_RTERR (pctxt, ret);

      if (*pstr != '\0') {
         OSOCTET ub2;

         ret = rtTBCDCharToBin (*pstr++, &ub2);
         if (0 != ret) return LOG_RTERR (pctxt, ret);

         ub |= (OSOCTET)(ub2 << 4);
      }
      else { ub |= 0x0F0; }

      ret = rtxEncBits (pctxt, ub, 8);
      if (0 != ret) return LOG_RTERR (pctxt, ret);
   }

   RTDIAG_SETBITFLDCOUNT (pctxt);

   return 0;
}
