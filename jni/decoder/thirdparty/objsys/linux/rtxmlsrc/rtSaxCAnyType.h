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
 * @file rtSaxCAnyType.h
 */

#ifndef _RTSAXCANYTYPE_H_
#define _RTSAXCANYTYPE_H_

#include "rtxmlsrc/osrtxml.h"
#include "rtxmlsrc/rtSaxCParser.h"

#ifdef __cplusplus
extern "C" {
#endif

/* For some reason, Borland does not like the EXTERNXML definition; I believe
 * this fixes the problem. */

#ifndef __BORLANDC__
typedef struct EXTERNXML AnyTypeSaxHandler {
#else
EXTERNXML typedef struct AnyTypeSaxHandler {
#endif
   OSSAXHandlerBase mSaxBase;
   OSXSDAnyType* mpMsgData;
   OSCTXT mEncCtxt;
   OSBOOL bInitialized;
} AnyTypeSaxHandler;

EXTERNXML int SAXAnyTypeStartElement
   (void *userData, const OSUTF8CHAR* localname,
    const OSUTF8CHAR* qname, const OSUTF8CHAR* const* attrs);

EXTERNXML int SAXAnyTypeCharacters
   (void *userData, const OSUTF8CHAR* chars, int length);

EXTERNXML int SAXAnyTypeEndElement
   (void *userData, const OSUTF8CHAR* localname, const OSUTF8CHAR* qname);

EXTERNXML void SAXAnyTypeInit
   (OSCTXT* pctxt, AnyTypeSaxHandler* pSaxHandler,
    OSXSDAnyType* pvalue, int level);

EXTERNXML int  SAXAnyTypeElementInit
   (OSCTXT* pctxt, AnyTypeSaxHandler* pSaxHandler,
    OSXSDAnyType* pvalue, const OSUTF8CHAR* elemName);

EXTERNXML void SAXAnyTypeFree
   (OSCTXT* pctxt, AnyTypeSaxHandler* pSaxHandler);

#ifdef __cplusplus
}
#endif

#endif /* _RTSAXCANYTYPE_H_ */
