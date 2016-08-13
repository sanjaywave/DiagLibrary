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
 * @file rtSaxCSoap.h
 */
#ifndef RTSAXCSOAP_H
#define RTSAXCSOAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rtxmlsrc/osrtxml.h"
#include "rtxmlsrc/rtSaxCParser.h"

/**************************************************************/
/*                                                            */
/*  soap                                                      */
/*                                                            */
/**************************************************************/
typedef EXTERN struct SoapSaxHandler {
   OSSAXHandlerBase mSaxBase;
   void* mpMsgSaxHandler;
   void* mpFaultMsgSaxHandler;
   OSRTMEMBUF mCurrElemValue;
   OSBOOL mbEnvelopeParsed;
   OSBOOL mbFault;
   OSBOOL mbProcessingDetailFault;
   OSBOOL mbParsingHeader;
} SoapSaxHandler;

EXTERNXML int SAXSoapStartElement
   (void *userData, const OSUTF8CHAR* localname,
    const OSUTF8CHAR* qname, const OSUTF8CHAR* const* atts);

EXTERNXML int SAXSoapEndElement
   (void *userData, const OSUTF8CHAR* localname, const OSUTF8CHAR* qname);

EXTERNXML int SAXSoapCharacters
   (void *userData, const OSUTF8CHAR* chars, int length);

EXTERNXML int SAXSoapInit
   (OSCTXT* pctxt, SoapSaxHandler* pSaxHandler,
    void* pMsgSaxHandler, void* pFaultMsgSaxHandler);

EXTERNXML void SAXSoapFree
   (OSCTXT* pctxt, SoapSaxHandler* pSaxHandler);

OSBOOL isSoapEnv(const OSUTF8CHAR* uri);
#ifdef __cplusplus
}
#endif

#endif /* RTSAXCSOAP_H */
