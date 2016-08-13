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

#include "nasDecode.h"
#include "rtxsrc/rtxCtype.h"
#include "rtxsrc/rtxDiagBitTrace.h"
#include "rtxsrc/rtxError.h"

int nasDecTBCDigit (OSCTXT* pctxt, char* pdigit)
{
   int ret;
   OSUINT8 ub;

   ret = rtxDecBitsToByte (pctxt, &ub, 4);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   return rtTBCDBinToChar (ub, pdigit);
}

int nasDec_MCC_MNC (OSCTXT* pctxt, char mcc[4], char mnc[4])
{
   int ret;

   /* Decode MCC and MNC digits */
   RTDIAG_NEWBITFIELD (pctxt, "mcc/mnc");

   ret = nasDecTBCDigit (pctxt, &mcc[1]);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   ret = nasDecTBCDigit (pctxt, &mcc[0]);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   ret = nasDecTBCDigit (pctxt, &mnc[2]);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   ret = nasDecTBCDigit (pctxt, &mcc[2]);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   ret = nasDecTBCDigit (pctxt, &mnc[1]);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   ret = nasDecTBCDigit (pctxt, &mnc[0]);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   RTDIAG_SETBITFLDCOUNT (pctxt);

   mcc[3] = mnc[3] = '\0';

   return 0;
}

int nasDecMobileIdentityType1
(OSCTXT* pctxt, OSUINT8 len, OSBOOL odd, OSUINT8 digit1,
 char* strbuf, OSSIZE bufsize)
{
   OSUINT8 i, ndigits;
   OSUINT8 digit2;
   char*   pstr = strbuf;
   int     ret;

   len--;
   ndigits = len * 2;
   if (odd) ndigits++;

   if (ndigits >= bufsize) {
      return LOG_RTERR (pctxt, RTERR_TOOBIG);
   }

   if (digit1 <= 9) {
      *pstr++ = digit1 + '0';
   }
   else return LOG_RTERR (pctxt, RTERR_INVCHAR);

   RTDIAG_NEWBITFIELD (pctxt, "");

   for (i = 0; i < len - 1; i++) {
      /* Decode 2nd digit (p + 1) */
      ret = rtxDecBitsToByte (pctxt, &digit2, 4);
      if (0 != ret) return LOG_RTERR (pctxt, ret);

      /* Decode first digit (p) */
      ret = rtxDecBitsToByte (pctxt, &digit1, 4);
      if (0 != ret) return LOG_RTERR (pctxt, ret);

      /* Convert digits to text */
      ret = rtTBCDBinToChar (digit1, pstr++);
      if (0 != ret) return LOG_RTERR (pctxt, ret);

      ret = rtTBCDBinToChar (digit2, pstr++);
      if (0 != ret) return LOG_RTERR (pctxt, ret);
   }

   /* Decode 2nd digit (p + 1) */
   ret = rtxDecBitsToByte (pctxt, &digit2, 4);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   /* Decode first digit (p) */
   ret = rtxDecBitsToByte (pctxt, &digit1, 4);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   /* Convert digits to text */
   ret = rtTBCDBinToChar (digit1, pstr++);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   if (odd) {
      ret = rtTBCDBinToChar (digit2, pstr++);
      if (0 != ret) return LOG_RTERR (pctxt, ret);
   }

   RTDIAG_SETBITFLDCOUNT (pctxt);

   *pstr = '\0';

   return 0;
}

/*
Decode the non-imperative part of a message having an empty non-imperative
part.  This is not for use with circuit-switched call control messages
(section 9.3) as it does not handle the SHIFT IE specially.

This will report an error if a comprehension required IE is found.
*/
int ts24008Dec_NonImperative (OSCTXT* pctxt)
{
   /*
   Decode non-imperative part.  Just look for comprehension required IEs
   and report an error if there are any.
   */
   int ret;

   while (!OSRTENDOFBUF(pctxt)) {
      OSUINT8 iei;              /* the current iei */
      OSUINT8 skipBytes = 0;    /* number of bytes to skip to get past
                                   unprocessed IE */

      /* Decode IEI */
      RTDIAG_NEWBITFIELD (pctxt, "IEI");

      ret = rtxDecBitsToByte (pctxt, &iei, 8);
      if (ret == RTERR_ENDOFBUF) break;
      else if (0 != ret) return LOG_RTERR (pctxt, ret);

      RTDIAG_SETBITFLDCOUNT (pctxt);

      if ( !(iei & 0x80 ) ) {
         /* bit 8 is 0; assuming TLV type 4 */
         ret = rtxDecBitsToByte (pctxt, &skipBytes, 8);
         if (0 != ret) return LOG_RTERR (pctxt, ret);

         /* if comprehension scheme applies then set comprehension required
            high nibble all zeros <==> comprehension required */
         if (( iei & 0x0F0 ) == 0) {
            rtxErrAddStrParm (pctxt, "comprehension required on unknown IEI");
            return LOG_RTERR (pctxt, RTERR_NOTSUPP);
         }

         if ( skipBytes != 0 ) {
            ret = rtxSkipBits (pctxt, skipBytes * 8);
            if (0 != ret) return LOG_RTERR (pctxt, ret);
         }
      }
      /* else bit 8 set to 1; 1-octet IE - nothing to skip */
   }

   return 0;
}

/*
Decode the non-imperative part of a circuit-switched call control message
(section 9.3) having an empty non-imperative part, excepting the SHIFT IE
which is defined as known.

This will report an error if a comprehension required IE is found.
*/
int ts24008Dec_CC_NonImperative (OSCTXT* pctxt)
{
   /* All known IEIs are assumed to be in codeset 0, and IEI's outside
      codeset 0 are unknown.
   */
   OSUINT8 lockedCodeset = 0;   /* locked-in IEI codeset */

   int ret;

   while (!OSRTENDOFBUF(pctxt)) {
      OSUINT8 nibble;           /* used for the high/low nibble of 1 octet IE */
      OSUINT8 iei;              /* the current iei */
      OSUINT8 skipBytes = 0;    /* number of bytes to skip to get past
                                   unprocessed IE */

      OSUINT8 activeCodeset = lockedCodeset;

      /* Decode IEI */
      RTDIAG_NEWBITFIELD (pctxt, "IEI");

      ret = rtxDecBitsToByte (pctxt, &iei, 8);
      if (ret == RTERR_ENDOFBUF) break;
      else if (0 != ret) return LOG_RTERR (pctxt, ret);

      RTDIAG_SETBITFLDCOUNT (pctxt);

      nibble = iei >> 4;      //nibble == high nibble

      /* Handle any series of shift IEs */
      while ( nibble == SHIFT_IEI ) {
         OSUINT8 codeset = iei & 0x07;  /* lowest 3 bits */
         OSBOOL locking = (iei & 0x08 ) == 0;   /* bit 4 clear => locking */
         if ( locking && codeset > lockedCodeset ) {
            lockedCodeset = codeset;
            activeCodeset = codeset;
         }
         else if ( !locking ) activeCodeset = codeset;

         /* move to next IEI */
         RTDIAG_NEWBITFIELD (pctxt, "IEI");

         ret = rtxDecBitsToByte (pctxt, &iei, 8);
         if (ret == RTERR_ENDOFBUF) return 0;
         else if (0 != ret) return LOG_RTERR (pctxt, ret);

         RTDIAG_SETBITFLDCOUNT (pctxt);
         nibble = iei >> 4;   //nibble == high nibble
      }

      /* Note: comprehension is not required for TV type 1 or T type 2 */
      if ( !(iei & 0x80 ) ) {
         //bit 8 is set 0; assuming TLV type 4
         ret = rtxDecBitsToByte (pctxt, &skipBytes, 8);
         if (0 != ret) return LOG_RTERR (pctxt, ret);

         /* if comprehension scheme applies then set comprehension required
            active codeset of 0 && high nibble all zeros <==> comprehension
            required */
         if (activeCodeset == 0 && (( iei & 0x0F0 ) == 0)) {
            rtxErrAddStrParm (pctxt, "comprehension required on unknown IEI");
            return LOG_RTERR (pctxt, RTERR_NOTSUPP);
         }

         if ( skipBytes != 0 ) {
            ret = rtxSkipBits (pctxt, skipBytes * 8);
            if (0 != ret) return LOG_RTERR (pctxt, ret);
         }
      }
      /* else bit 8 set to 1; leave skipBytes == 0 for 1-octet IE */
   }

   return 0;
}
