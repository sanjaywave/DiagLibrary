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
 * @file ASN1SAX_XEROpenType.h
 */

#ifndef _ASN1SAX_XEROPENTYPE_H_
#define _ASN1SAX_XEROPENTYPE_H_

#include "asn1XerCTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct EXTERNXER ASN1SAX_XEROpenType {
   ASN1SAXCDecodeHandlerBase mSaxBase;
   ASN1OpenType* mpMsgData;
   OSCTXT mEncCtxt;
} ASN1SAX_XEROpenType;

EXTERNXER int asn1Sax_XEROpenType_startElement
   (void *userData, const OSUTF8CHAR* localname,
    const OSUTF8CHAR* qname, const OSUTF8CHAR* const* atts);

EXTERNXER int asn1Sax_XEROpenType_characters
   (void *userData, const OSUTF8CHAR* chars, int length);

EXTERNXER int asn1Sax_XEROpenType_endElement
   (void *userData, const OSUTF8CHAR* localname, const OSUTF8CHAR* qname);

EXTERNXER void asn1Sax_XEROpenType_init
   (OSCTXT* pctxt, ASN1SAX_XEROpenType* pSaxHandler,
    ASN1OpenType* pvalue, OSINT16 level);

EXTERNXER void asn1Sax_XEROpenType_free
   (OSCTXT* pctxt, ASN1SAX_XEROpenType* pSaxHandler);

#ifdef __cplusplus
}
#endif

#endif /* _ASN1SAX_XEROPENTYPE_H_ */
