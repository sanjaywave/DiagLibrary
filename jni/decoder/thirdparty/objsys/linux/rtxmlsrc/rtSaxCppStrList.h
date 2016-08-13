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
 * @file rtSaxCppStrList.h
 */
#ifndef _RTSAXCPPSTRLIST_H_
#define _RTSAXCPPSTRLIST_H_

#ifdef ASN1RT
#include "rtxsrc/rtxDList.h"
#else
#include "rtxsrc/rtxCppDList.h"
#endif
#include "rtxmlsrc/osrtxml.h"
#include "rtxmlsrc/rtSaxCppParser.h"

/**
 * OSXMLStrListHandler
 */
class EXTXMLCLASS OSXMLStrListHandler {
   inline OSXMLStrListHandler () {}
 public:
   EXTXMLMETHOD static int parse
      (OSCTXT* pctxt, OSRTMEMBUF *pMemBuf, OSRTDList* pStrList);
#ifndef ASN1RT
   EXTXMLMETHOD static int parse
      (OSCTXT* pctxt, OSRTMEMBUF *pMemBuf, OSRTObjListClass* pStrList,
       OSBOOL useSTL = FALSE);

   static int parseSTL
      (OSCTXT* pctxt, OSRTMEMBUF *pMemBuf, OSRTObjListClass* pStrList)
   {
         return parse (pctxt, pMemBuf, pStrList, TRUE);
   }
#endif
   inline static int match (OSCTXT*) { return 0; }

} ;

#endif // _RTSAXCPPSTRLIST_H_

