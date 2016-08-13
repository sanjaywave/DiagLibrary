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

#ifndef INITCTXTAPPINFOFUNC
#define INITCTXTAPPINFOFUNC rtXmlInitCtxtAppInfo
#define FREECTXTAPPINFOFUNC rtXmlFreeCtxtAppInfo
#define RESETCTXTAPPINFOFUNC rtXmlResetCtxtAppInfo
#endif

EXTXMLMETHOD int INITCTXTAPPINFOFUNC (OSCTXT* pctxt)
{
   OSXMLCtxtInfo* pCtxtInfo;
   pctxt->pXMLInfo = (OSXMLCtxtInfo*)
      rtxMemSysAlloc (pctxt, sizeof(OSXMLCtxtInfo));
   if (pctxt->pXMLInfo == 0) return LOG_RTERRNEW (pctxt, RTERR_NOMEM);
   pCtxtInfo = (OSXMLCtxtInfo*)pctxt->pXMLInfo;
   pCtxtInfo->pFreeFunc = FREECTXTAPPINFOFUNC;
   pCtxtInfo->pResetFunc = RESETCTXTAPPINFOFUNC;
   pCtxtInfo->schemaLocation = 0;
   pCtxtInfo->noNSSchemaLoc = 0;
   pCtxtInfo->xsiTypeAttr = 0;
   pCtxtInfo->encoding = OSXMLUTF8;
   pCtxtInfo->mbCdataProcessed = FALSE;
   pCtxtInfo->mSaxLevel = pCtxtInfo->mSkipLevel = 0;
   pCtxtInfo->maxSaxErrors = pCtxtInfo->errorsCnt = 0;
   pCtxtInfo->facets.totalDigits = -1;
   pCtxtInfo->facets.fractionDigits = -1;
   pCtxtInfo->indent = OSXMLINDENT;
   pCtxtInfo->indentChar = ' ';
   pCtxtInfo->soapVersion = 12;
   pCtxtInfo->encodingStr = 0;
   rtxDListFastInit (&pCtxtInfo->namespaceList);
   rtxDListFastInit (&pCtxtInfo->encodedNSList);
   rtxDListFastInit (&pCtxtInfo->sortedAttrList);
   pCtxtInfo->nsPfxLinkStack.count = 0;
   pCtxtInfo->nsPfxLinkStack.top = 0;
   pCtxtInfo->nsURITable.nrows = 0;
   pCtxtInfo->nsURITable.data = 0;
   pCtxtInfo->byteOrderMark = OSXMLBOM_NO_BOM;
   rtxMemBufInit (pctxt, &pCtxtInfo->memBuf, OSRTMEMBUFSEG);

   /* Note: the pull-parser reader is not created here because an XML
      stream or buffer must first be assigned so that the initial item
      can be parsed. */
   pCtxtInfo->pXmlPPReader = 0;
   pCtxtInfo->attrsBuff = 0;
   pCtxtInfo->attrsBuffSize = 0;
   pCtxtInfo->attrStartPos = 0;

   /* Add XML error codes to global table */
#ifndef XERCONTEXT /* to avoid an undefined symbol error */
   rtErrXmlInit ();
#endif

   pctxt->state = OSXMLINIT;

   return 0;
}

EXTXMLMETHOD int FREECTXTAPPINFOFUNC (OSCTXT* pctxt)
{
   if (0 != pctxt->pXMLInfo) {
      OSXMLCtxtInfo* pCtxtInfo = (OSXMLCtxtInfo*)pctxt->pXMLInfo;
      OSCTXT* pXMLInfoCtxt = pctxt;
      if (0 != pCtxtInfo->pFreeFunc) {
         /* Call free using the context used to allocate XML Info.
            It may be different from current context in the case
            where getAppInfo() and setAppInfo() are used. */
         if (0 != pCtxtInfo->memBuf.pctxt)
            pXMLInfoCtxt = pCtxtInfo->memBuf.pctxt;
         rtxMemBufFree (&pCtxtInfo->memBuf);
#ifndef XERCONTEXT
         rtXmlNSRemoveAll (pctxt);
#endif
         rtxMemFreeArray (pXMLInfoCtxt, pCtxtInfo->schemaLocation);
         rtxMemFreeArray (pXMLInfoCtxt, pCtxtInfo->noNSSchemaLoc);
         rtxMemFreeArray (pXMLInfoCtxt, pCtxtInfo->xsiTypeAttr);
         pCtxtInfo->pFreeFunc = 0;
         pCtxtInfo->noNSSchemaLoc = 0;
         pCtxtInfo->schemaLocation = 0;
         pCtxtInfo->nsURITable.nrows = 0;
         pCtxtInfo->nsURITable.data = 0;

         /* Free nodes in namespace lists (note: we can't free namespace
            record memory because they might be held in local lists) */
         rtxDListFreeNodes (pXMLInfoCtxt, &pCtxtInfo->namespaceList);
         rtxDListFreeNodes (pXMLInfoCtxt, &pCtxtInfo->encodedNSList);
         rtxDListFreeNodes (pXMLInfoCtxt, &pCtxtInfo->sortedAttrList);

         rtxMemFreeArray (pctxt, pCtxtInfo->attrsBuff);
         pCtxtInfo->attrsBuff = 0;
         pCtxtInfo->attrsBuffSize = 0;
         pCtxtInfo->attrStartPos = 0;

#ifndef XERCONTEXT /* This is a missing symbol in the asn1xer library! */
         rtXmlpFreeReader (pctxt, pCtxtInfo);
#endif
      }
      pctxt->pXMLInfo = 0;
      return 0;
   }
   return LOG_RTERRNEW (pctxt, RTERR_NOTINIT);
}

EXTXMLMETHOD int RESETCTXTAPPINFOFUNC (OSCTXT* pctxt)
{
   if (0 != pctxt->pXMLInfo) {
      OSXMLCtxtInfo* pCtxtInfo = (OSXMLCtxtInfo*)pctxt->pXMLInfo;
      rtxMemBufReset (&pCtxtInfo->memBuf);
#ifndef XERCONTEXT
      rtXmlNSRemoveAll (pctxt);
#endif
      rtxMemFreeArray (pctxt, pCtxtInfo->schemaLocation);
      rtxMemFreeArray (pctxt, pCtxtInfo->noNSSchemaLoc);
      rtxMemFreeArray (pctxt, pCtxtInfo->xsiTypeAttr);
      pCtxtInfo->noNSSchemaLoc = 0;
      pCtxtInfo->schemaLocation = 0;
      pCtxtInfo->xsiTypeAttr = 0;
      pCtxtInfo->pFreeFunc = FREECTXTAPPINFOFUNC;
      pCtxtInfo->pResetFunc = RESETCTXTAPPINFOFUNC;
      pCtxtInfo->encoding = OSXMLUTF8;
      pCtxtInfo->indent = OSXMLINDENT;
      pCtxtInfo->indentChar = ' ';
      pCtxtInfo->encodingStr = 0;
      pCtxtInfo->nsURITable.nrows = 0;
      pCtxtInfo->nsURITable.data = 0;
      pCtxtInfo->pXmlPPReader = 0;
      rtxMemFreeArray (pctxt, pCtxtInfo->attrsBuff);
      rtxDListFastInit (&pCtxtInfo->sortedAttrList);
      pCtxtInfo->attrsBuff = 0;
      pCtxtInfo->attrsBuffSize = 0;
      pCtxtInfo->attrStartPos = 0;
      return 0;
   }
   return LOG_RTERRNEW (pctxt, RTERR_NOTINIT);
}


