/*
 * Copyright (c) 2003-2012 Objective Systems, Inc.
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
 * @file rtSaxCppAny.h
 */
#ifndef _RTSAXCPPANY_H_
#define _RTSAXCPPANY_H_

#include "rtxsrc/OSRTContext.h"
#include "rtxmlsrc/osrtxml.h"
#include "rtxmlsrc/rtSaxCppParser.h"
#include "rtxmlsrc/rtXmlCppMsgBuf.h"
#ifndef ASN1RT
#include "rtxsrc/rtxCppXmlString.h"
#include "rtxmlsrc/OSXSDAnyTypeClass.h"
#include "rtxmlsrc/rtSaxCppAnyType.h"
#endif

class EXTXMLCLASS OSXMLAnyHandler : public OSXMLDefaultHandler {
 private:
   OSRTContext mEncCtxt;
#ifndef ASN1RT
   OSXMLStringClass* mpAnyMsgData;
   OSXSDAnyTypeClass* mpAnyTypeMsgData;
#else
   const OSUTF8CHAR** mppMsgData;
#endif
   EXTXMLMETHOD void   localInit (OSRTContext* pContext);
   EXTXMLMETHOD OSBOOL isEmptyElement (const OSUTF8CHAR* qname);
   EXTXMLMETHOD OSXMLAnyHandler& operator= (const OSXMLAnyHandler&);

 public:
#ifndef ASN1RT
   EXTXMLMETHOD OSXMLAnyHandler
      (OSXSDAnyTypeClass& msgData, OSRTContext* pContext, int level = 0);

   EXTXMLMETHOD OSXMLAnyHandler
      (OSXSDAnyTypeClass& msgData, OSRTContext* pContext,
       const OSUTF8CHAR* elemName);

   // xsd:any
   EXTXMLMETHOD OSXMLAnyHandler
      (OSXMLStringClass& msgData, OSRTContext* pContext, int level = 0);

   EXTXMLMETHOD OSXMLAnyHandler
      (OSXMLStringClass& msgData, OSRTContext* pContext,
       const OSUTF8CHAR* elemName);
#else
   EXTXMLMETHOD OSXMLAnyHandler
      (const OSUTF8CHAR** ppMsgData, OSRTContext* pContext, int level = 0);

   EXTXMLMETHOD OSXMLAnyHandler
      (const OSUTF8CHAR** ppMsgData, OSRTContext* pContext,
       const OSUTF8CHAR* elemName);
#endif
   EXTXMLMETHOD ~OSXMLAnyHandler ();

   EXTXMLMETHOD virtual int startElement
      (const OSUTF8CHAR* const uri,
       const OSUTF8CHAR* const localname,
       const OSUTF8CHAR* const qname,
       const OSUTF8CHAR* const* attrs);

   EXTXMLMETHOD virtual int characters
      (const OSUTF8CHAR* const chars, OSUINT32 length);

   EXTXMLMETHOD virtual int endElement
      (const OSUTF8CHAR* const uri,
       const OSUTF8CHAR* const localname,
       const OSUTF8CHAR* const qname);
} ;

#endif /* _RTSAXCPPANY_H_ */

