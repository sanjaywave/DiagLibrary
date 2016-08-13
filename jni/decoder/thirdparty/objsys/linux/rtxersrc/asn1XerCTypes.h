/*
 * Copyright (c) 1997-2013 Objective Systems, Inc.
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
 * @file asn1XerCTypes.h
 */

#ifndef _ASN1XERCTYPES_H_
#define _ASN1XERCTYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include "asn1xer.h"
#include "rtxmlsrc/rtSaxCParser.h"

typedef void (*ASN1XERStartElementHandler)(void *userData,
                                           const XMLCHAR *name,
                                           const XMLCHAR **atts);

typedef void (*ASN1XEREndElementHandler)(void *userData,
                                         const XMLCHAR *name);

/* s is not 0 terminated. */
typedef void (*ASN1XERCharacterHandler)(void *userData,
                                        const XMLCHAR *s,
                                        int len);

/*
typedef struct ASN1SAXCDecodeHandlerBase {
   ASN1XERStartElementHandler mpStartElement;
   ASN1XEREndElementHandler mpEndElement;
   ASN1XERCharacterHandler mpCharacters;

   OSCTXT* mpCtxt;
   const char* mpTypeName;
   ASN1XERState mCurrState;
   int mLevel, mStartLevel;
} ASN1SAXCDecodeHandlerBase;
*/
typedef OSSAXHandlerBase ASN1SAXCDecodeHandlerBase;
/*
#define ISCOMPLETE(e) (e->mSaxBase.mLevel == e->mSaxBase.mStartLevel)
*/

#define ASN1SAXCTRY(pctxt,stat)
/* if ((stat = setjmp((pctxt)->jmpMark)) == 0) */

#define ASN1SAXCTHROW(pctxt,stat) do { LOG_RTERR ((pctxt), stat); \
/* longjmp((pctxt)->jmpMark, stat); */ } while (0)

#define ASN1SAXCCATCH else

#define STRX(pctxt,pWideStr) xerTextToCStr ((pctxt), (pWideStr))

#if defined (XML_UNICODE) && defined(XML_UNICODE_WCHAR_T)
#define LSTRX(pctxt,pLStr)   xerTextToCStr ((pctxt), (pLStr))
#else
#define LSTRX(pctxt,pLStr)   \
strcpy ((char*)rtxMemAlloc (pctxt, strlen (pLStr) + 1), (pLStr))
#endif

#define XERCDIAGSTRM2(pctxt,a)     RTDIAGSTRM2(pctxt,a)
#define XERCDIAGSTRM3(pctxt,a,b)   RTDIAGSTRM3(pctxt,a,b)
#define XERCDIAGSTRM4(pctxt,a,b,c) RTDIAGSTRM4(pctxt,a,b,c)

#ifndef _COMPACT
#define DECLARE_NON_COMPACT_VAR(type,var) type var
#else
#define DECLARE_NON_COMPACT_VAR(type,var)
#endif

#ifdef __cplusplus
}
#endif
#endif /* _ASN1XERCTYPES_H_ */
