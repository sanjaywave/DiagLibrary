/*
* Copyright (c) 2003-2010 Objective Systems, Inc.
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
/*
* @file rtxRandTest.h
*/
#ifndef _RTXRANDTEST_H_
#define _RTXRANDTEST_H_

#ifndef _NO_STREAM

#include "rtxsrc/osSysTypes.h"
#include "rtxsrc/rtxExternDefs.h"
#include "rtxsrc/rtxStream.h"

#ifdef __cplusplus
extern "C" {
#endif

   /*
   * This function used for runtime random functions initialization.
   * Has to be called one time in the beginning of the program.
   *
   * @param randSeed      Seed for srand random-number generation.
   */
   EXTERNRT void rtxRandInit(unsigned int randSeed);

   /*
   * This function used for runtime random int32 data generation in interval
   *
   * @param pctxt        Pointer to context structure variable.
   * @param lower        Minimum number in interval
   * @param upper        Maximum number in interval
   *
   * @return             int32 generated number
   */
   EXTERNRT OSINT32 rtxRandInt32 (OSCTXT* pctxt, OSINT32 lower, OSINT32 upper);

   /*
   * This function used for runtime random int64 data generation in interval
   *
   * @param pctxt        Pointer to context structure variable.
   * @param lower        Minimum number in interval
   * @param upper        Maximum number in interval
   *
   * @return             int64 generated number
   */
   EXTERNRT OSINT64 rtxRandInt64 (OSCTXT* pctxt, OSINT64 lower, OSINT64 upper);

   /*
   * This function used for runtime random unsigned int32 data generation in interval
   *
   * @param pctxt        Pointer to context structure variable.
   * @param lower        Minimum number in interval
   * @param upper        Maximum number in interval
   *
   * @return             Unsigned int32 generated number
   */
   EXTERNRT OSUINT32 rtxRandUInt32 (OSCTXT* pctxt, OSUINT32 lower, OSUINT32 upper);

   /*
   * This function used for runtime random unsigned int64 data generation in interval
   *
   * @param pctxt        Pointer to context structure variable.
   * @param lower        Minimum number in interval
   * @param upper        Maximum number in interval
   *
   * @return             Unsigned int64 generated number
   */
   EXTERNRT OSUINT64 rtxRandUInt64 (OSCTXT* pctxt, OSUINT64 lower, OSUINT64 upper);

   /*
   * This function used for runtime random string generation
   *
   * @param pctxt        Pointer to context structure variable.
   * @param lower        Minimum size of string
   * @param upper        Maximum size of string
   * @param charSet      Char set for string
   *
   * @return             Generated string
   */
   EXTERNRT char* rtxRandString (OSCTXT* pctxt, size_t lower, size_t upper,
      const char* charSet);

   /*
   * This function used for runtime random Unicode string generation
   *
   * @param pctxt        Pointer to context structure variable.
   * @param lower        Minimum size of string
   * @param upper        Maximum size of string
   * @param charSet      Char set for string
   *
   * @return             Generated Unicode string
   */
   EXTERNRT OSUTF8CHAR* rtxRandUnicodeString (OSCTXT* pctxt, size_t lower,
      size_t upper, const char* charSet);

   /*
   * This function used for runtime random float data generation in interval
   *
   * @param pctxt        Pointer to context structure variable.
   * @param lower        Minimum number in interval
   * @param upper        Maximum number in interval
   *
   * @return             Float generated number
   */
   EXTERNRT OSREAL rtxRandFloat (OSCTXT* pctxt, OSREAL lower, OSREAL upper);

   /*
   * This function used for runtime random octet string generation
   *
   * @param pctxt        Pointer to context structure variable.
   * @pvalue             Generated string
   * @param lower        Minimum size of string
   * @param upper        Maximum size of string
   */
   EXTERNRT void rtxRandOctetString (OSCTXT* pctxt, OSDynOctStr* pvalue, size_t
      lower, size_t upper);

   /*
   * This function used for runtime random date generation
   *
   * @param pctxt        Pointer to context structure variable.
   * @param pvalue       Generated date
   */
   EXTERNRT void rtxRandDateTime (OSCTXT* pctxt, OSNumDateTime* pvalue, OSUINT32 lower, OSUINT32 upper);

#ifdef __cplusplus
}
#endif

#endif /* _NO_STREAM */
#endif /* _RTXRANDTEST_H_ */
