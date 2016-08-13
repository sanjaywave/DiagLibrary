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
/**
 * @file rtxBigNumber.h
 */

#ifndef _RTXBIGNUMBER_H_
#define _RTXBIGNUMBER_H_

#include "rtxsrc/rtxContext.h"

#ifdef __cplusplus
extern "C" {
#endif

/* BigNumber functions */

/**
 * Addition big numbers: a + b = c.
 *
 * @param a            First addend.
 * @param szA          Length of first addend in octets.
 * @param b            Second addend.
 * @param szB          Length of second addend in octets.
 * @param c            Sum.
 * @param szC          Length of sum buffer in octets.
 * @return             Status of the operation.  Zero if successful; a
 *                       negative status code if overflow.
 */
int rtxAddBigNum (const OSOCTET* a, OSSIZE szA,
                  const OSOCTET* b, OSSIZE szB,
                  OSOCTET* c, OSSIZE szC);

/**
 * Substraction big numbers: a - b = c.
 *
 * @param a            Minuend.
 * @param szA          Length of minuend in octets.
 * @param b            Substrahend.
 * @param szB          Length of substrahend in octets.
 * @param c            Difference.
 * @param szC          Length of difference buffer in octets.
 * @return             Status of the operation.  Zero if successful; a
 *                       negative status code if overflow.
 */
int rtxSubBigNum (const OSOCTET* a, OSSIZE szA,
                  const OSOCTET* b, OSSIZE szB,
                  OSOCTET* c, OSSIZE szC);

/**
 * Multiplication big numbers: a * b = c.
 *
 * @param a            Multiplicand.
 * @param szA          Length of multiplicand in octets.
 * @param b            Multiplier.
 * @param szB          Length of multiplier in octets.
 * @param c            Product.
 * @param szC          Length of product buffer in octets.
 * @return             Status of the operation.  Zero if successful; a
 *                       negative status code if overflow.
 */
int rtxMulBigNum (const OSOCTET* a, OSSIZE szA,
                  const OSOCTET* b, OSSIZE szB,
                  OSOCTET* c, OSSIZE szC);

/**
 *  Division big numbers with reminder: a / b = c.
 *
 * @param a            Divident.
 * @param szA          Length of divident in octets.
 * @param b            Divisor.
 * @param szB          Length of divisor in octets.
 * @param c            Quotient.
 * @param szC          Length of quotient buffer in octets.
 * @param rem          Reminder.
 * @param szRem        Length of reminder buffer in octets.
 * @return             Status of the operation.  Zero if successful; a
 *                       negative status code if overflow.
 */
int rtxDivRemBigNum (const OSOCTET* a, OSSIZE szA,
                     const OSOCTET* b, OSSIZE szB,
                     OSOCTET* c, OSSIZE szC,
                     OSOCTET* rem, OSSIZE szRem);

/**
 *  Division big numbers: a / b = c.
 *
 * @param a            Divident.
 * @param szA          Length of divident in octets.
 * @param b            Divisor.
 * @param szB          Length of divisor in octets.
 * @param c            Quotient.
 * @param szC          Length of quotient buffer in octets.
 * @return             Status of the operation.  Zero if successful; a
 *                       negative status code if overflow.
 */
int rtxDivBigNum (const OSOCTET* a, OSSIZE szA,
                  const OSOCTET* b, OSSIZE szB,
                  OSOCTET* c, OSSIZE szC);

/**
 *  Division by module big numbers: a % b = rem.
 *
 * @param a            Divident.
 * @param szA          Length of divident in octets.
 * @param b            Divisor.
 * @param szB          Length of divisor in octets.
 * @param rem          Reminder.
 * @param szRem        Length of reminder buffer in octets.
 * @return             Status of the operation.  Zero if successful; a
 *                       negative status code if overflow.
 */
int rtxModBigNum (const OSOCTET* a, OSSIZE szA,
                  const OSOCTET* b, OSSIZE szB,
                  OSOCTET* rem, OSSIZE szRem);

/**
 *  Convert big number to string.
 *
 * @param a            Number.
 * @param szA          Length of number in octets.
 * @param str          Result string.
 * @param szStr        Length of string buffer in octets.
 * @return             Status of the operation.  Zero if successful; a
 *                       negative status code if overflow.
 */
int rtxBigNumToStr (const OSOCTET* a, OSSIZE szA,
                    char* str, OSSIZE szStr);

/**
 *  Convert string to big number.
 *
 * @param str          Input null terminated string.
 * @param a            Result number.
 * @param szA          Length of number buffer in octets.
 * @return             Status of the operation.  Zero if successful; a
 *                       negative status code if overflow.
 */
int rtxStrToBigNum (const char* str, OSOCTET* a, OSSIZE szA);

#ifdef __cplusplus
}
#endif

#endif /* RTXBIGINT */
