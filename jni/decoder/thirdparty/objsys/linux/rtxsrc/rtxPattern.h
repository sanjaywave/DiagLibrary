/*
 * Copyright (c) 2003-2013 Objective Systems, Inc.
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
 * @file rtxPattern.h
 * Pattern matching functions.
 */
#ifndef _RTXPATTERN_H_
#define _RTXPATTERN_H_

#include "rtxsrc/osSysTypes.h"
#include "rtxsrc/rtxExternDefs.h"
#include "rtxsrc/rtxContext.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ccfPattern Pattern matching functions
 * @{
 *
 * These functions handle pattern matching which is required to to process XML
 * schema pattern constraints.
 */
/**
 * This function compares the given string to the given pattern. It returns
 * true if match, false otherwise.
 *
 * @param pctxt        Pointer to context structure.
 * @param text         Text to be matched.
 * @param pattern      Regular expression.
 * @return             Boolean result.
 */
EXTERNRT OSBOOL rtxMatchPattern (OSCTXT* pctxt,
                                  const OSUTF8CHAR* text,
                                  const OSUTF8CHAR* pattern);

EXTERNRT OSBOOL rtxMatchPattern2 (OSCTXT* pctxt,
                                   const OSUTF8CHAR* pattern);

/**
 * This function frees the memory associated with the regular expression cache.
 * The regular expression cache is designed to use memory that survives calls
 * to rtxMemFree and rtxMemReset, therefore it is necessary to call this
 * function to free that memory. (Note that \c rtxFreeContext invokes this.)
 */
EXTERNRT void rtxFreeRegexpCache(OSCTXT* pctxt);

/**
 * @} ccfPattern
 */

#ifdef __cplusplus
}
#endif

#endif
