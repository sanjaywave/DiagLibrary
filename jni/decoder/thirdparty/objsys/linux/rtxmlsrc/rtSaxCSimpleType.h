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
 * @file rtSaxCSimpleType.h
 */
#ifndef RTSAXCSIMPLETYPESAX_H
#define RTSAXCSIMPLETYPESAX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rtxmlsrc/osrtxml.h"
#include "rtxmlsrc/rtSaxCParser.h"

/**************************************************************/
/*                                                            */
/*  simpleType                                                  */
/*                                                            */
/**************************************************************/
typedef EXTERN struct SimpleTypeSaxHandler {
   OSSAXHandlerBase mSaxBase;
   OSRTMEMBUF mCurrElemValue;
} SimpleTypeSaxHandler;

EXTERNXML int SAXSimpleTypeStartElement
   (void *userData, const OSUTF8CHAR* localname,
    const OSUTF8CHAR* qname, const OSUTF8CHAR* const* attrs);

EXTERNXML int SAXSimpleTypeEndElement
   (void *userData, const OSUTF8CHAR* localname, const OSUTF8CHAR* qname);

EXTERNXML int SAXSimpleTypeCharacters
   (void *userData, const OSUTF8CHAR* chars, int length);

EXTERNXML int SAXSimpleTypeInit
   (OSCTXT* pctxt, SimpleTypeSaxHandler* pSaxHandler,
    const OSUTF8CHAR* elemName);

EXTERNXML void SAXSimpleTypeFree
   (OSCTXT* pctxt, SimpleTypeSaxHandler* pSaxHandler);

#ifdef __cplusplus
}
#endif

#endif /* RTSAXCSIMPLETYPESAX_H */
