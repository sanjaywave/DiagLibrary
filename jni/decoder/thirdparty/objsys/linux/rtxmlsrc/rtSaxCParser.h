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
 * @file rtSaxCParser.h
 */
#ifndef __RTSAXCPARSER_H_
#define __RTSAXCPARSER_H_

#include "rtxsrc/rtxCommon.h"
#include "rtxsrc/rtxUnicode.h"
#include "rtxmlsrc/osrtxml.h"
#include "rtxmlsrc/rtSaxDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSXMLREADER {
   OSCTXT* pctxt;
} OSXMLREADER;

/* atts is array of name/value pairs, terminated by 0;
   names and values are 0 terminated.
*/
typedef int (*CSAX_StartElementHandler)(void *userData,
                                        const OSUTF8CHAR *localname,
                                        const OSUTF8CHAR *qname,
                                        const OSUTF8CHAR * const* atts);

typedef int (*CSAX_EndElementHandler)(void *userData,
                                      const OSUTF8CHAR *localname,
                                      const OSUTF8CHAR *qname);


/* s is not 0 terminated. */
typedef int (*CSAX_CharacterDataHandler)(void *userData,
                                         const OSUTF8CHAR *s,
                                         int len);

typedef void (*CSAX_FreeHandler)(OSCTXT* pctxt, void* pSaxHandler);

typedef struct OSSAXHandlerBase {
   CSAX_StartElementHandler mpStartElement;
   CSAX_EndElementHandler mpEndElement;
   CSAX_CharacterDataHandler mpCharacters;
   CSAX_FreeHandler mpFree; /* can be NULL */

   OSOCTET* mpSaxHandlerBuff;
   OSCTXT* mpCtxt;
   const OSUTF8CHAR* mpElemName;
   OSINT32 mLevel;
   OSINT16 mStartLevel;
   OSINT16 mReqElemCnt;
   OSINT16 mCurrElemIdx;
   OSINT16 mState;
} OSSAXHandlerBase;

#define ISCOMPLETE(e) \
(e->mSaxBase.mLevel <= e->mSaxBase.mStartLevel)

#ifndef _COMPACT
#define SAXDIAGFUNCSTART(psaxb,funcName,localName) \
rtSaxDiagStartFunc (psaxb, funcName, localName)

#define SAXDIAGFUNCEND(psaxb,funcName,localName) \
rtSaxDiagEndFunc (psaxb, funcName, localName)

#else /* COMPACT */
#define SAXDIAGFUNCSTART(pctxt,funcName,localName)
#define SAXDIAGFUNCEND(pctxt,funcName,localName)
#endif

#define RELEASE_SAX_HANDLER(pctxt, pSaxBase) do {\
if ((pSaxBase)->mpFree != 0) (pSaxBase)->mpFree (pctxt, pSaxBase); \
rtxMemFreePtr (pctxt, (pSaxBase)->mpSaxHandlerBuff); \
} while (0)

/**
 * This function creates an XML reader instance which hold all data required
 * by the SAX parser.
 *
 * @param pctxt             Pointer to OSCTXT structure
 * @param pSaxHandlerData   Pointer to a user data structure.  This pointer
 *                            will be passed to each SAX callback in the
 *                            userData argument.
 * @param pStartElementProc Pointer to the start-element handler function.
 * @param pEndElementProc   Pointer to the end-element handler function.
 * @param pCharactersProc   Pointer to the characters handler function.
 * @return A pointer to OSXMLREADER structure.
 */
OSXMLREADER* rtSaxCCreateXmlReader
   (OSCTXT* pctxt, void* pSaxHandlerData,
    CSAX_StartElementHandler  pStartElementProc,
    CSAX_EndElementHandler    pEndElementProc,
    CSAX_CharacterDataHandler pCharactersProc);

/**
 * Enables the parser's thread-safe mode.
 *
 * @return          0 - if success, negative value is error.
 */
int rtSaxCEnableThreadSafety (void);

/**
 * Starts the parser.
 *
 * @param pReader    A pointer to OSXMLREADER structure
 * @return           0 - if success, negative value is error.
 */
int rtSaxCParse (OSXMLREADER* pReader);

/**
 * Releases the parser.
 *
 * @param pReader    A pointer to OSXMLREADER structure
 * @return           0 - if success, negative value is error.
 */
int rtSaxCReleaseReader (OSXMLREADER* pReader);

/**
 * This function outputs a diagnostic message at the start of
 * a SAX handler function.  It is added by the compiler to the
 * generated code when the "-trace" option is specified.
 *
 * @param pSaxBase      SAX handler common base structure
 * @param funcName      Name of function
 * @param localName     Local name of XML element
 */
EXTERNXML void rtSaxDiagStartFunc
(OSSAXHandlerBase* pSaxBase, const char* funcName,
 const OSUTF8CHAR* localName);

/**
 * This function outputs a diagnostic message at the end of
 * a SAX handler function.  It is added by the compiler to the
 * generated code when the "-trace" option is specified.
 *
 * @param pSaxBase      SAX handler common base structure
 * @param funcName      Name of function
 * @param localName     Local name of XML element
 */
EXTERNXML void rtSaxDiagEndFunc
(OSSAXHandlerBase* pSaxBase, const char* funcName,
 const OSUTF8CHAR* localName);

#ifdef __cplusplus
}
#endif

#endif /* __RTSAXCPARSER_H_ */
