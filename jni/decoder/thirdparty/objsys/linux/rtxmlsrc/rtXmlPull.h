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
#ifndef __RTXMLPULL_H
#define __RTXMLPULL_H

#include "rtxsrc/rtxCtype.h"
#include "rtxsrc/rtxStream.h"
#include "rtxsrc/rtxStreamBuffered.h"
#include "rtxsrc/rtxErrCodes.h"
#include "rtxsrc/rtxStreamDirectBuf.hh"
#include "rtxmlsrc/osrtxml.h"

/* _PULL_FROM_BUFFER - if defined, parser pull data from context buffer */
/* #define _PULL_FROM_BUFFER */

#define MAX_DELAYED_TASK_SIZE 10

/* default buffer size for qName in ElementName */
#define OSXML_DEFAULT_QNAME_BUF_SIZE 10

#define OSXMLEVT_NONE 0
#define OSXMLEVT_INITIAL OSXMLEVT_NONE
#define OSXMLEVT_START_DOCUMENT 0x1
#define OSXMLEVT_START_TAG 0x2
#define OSXMLEVT_TEXT 0x4
#define OSXMLEVT_END_TAG 0x8
#define OSXMLEVT_END_DOCUMENT 0x10
#define OSXMLEVT_COMMENT 0x20
#define OSXMLEVT_PI 0x40
#define OSXMLEVT_DTD 0x80

#define OSXMLEVT_USED_FLAG 0x80000000u
#define OSXMLEVT_ID_MASK (~OSXMLEVT_USED_FLAG)
#define OSXMLEVT_ALL_MASK  OSXMLEVT_ID_MASK

#define XMLPREADER(pctxt) ((OSXMLCtxtInfo*) pctxt->pXMLInfo)->pXmlPPReader

typedef enum {
   OSXMLS_HEADER = -1,
   OSXMLS_COMMENT = -2,
   OSXMLS_START_ELEMENT = -3,
   OSXMLS_DTD = -4,
   OSXMLS_CONTENT = -5,
   OSXMLS_PI = -6,
   OSXMLS_END_ELEMENT = -7,
   OSXMLS_CDATA = -8,

   OSXMLS_LAST = -9
} OSXMLStates;

typedef enum {
   OSXMLSI_HEADER = -OSXMLS_HEADER-1,
   OSXMLSI_COMMENT = -OSXMLS_COMMENT-1,
   OSXMLSI_START_ELEMENT = -OSXMLS_START_ELEMENT-1,
   OSXMLSI_DTD = -OSXMLS_DTD-1,
   OSXMLSI_CONTENT = -OSXMLS_CONTENT-1,
   OSXMLSI_PI = -OSXMLS_PI-1,
   OSXMLSI_END_ELEMENT = -OSXMLS_END_ELEMENT-1,
   OSXMLSI_CDATA = -OSXMLS_CDATA-1,

   OSXMLSI_LAST = -OSXMLS_LAST-1
} OSXMLStatesIndex;

typedef enum {
   OSXMLDM_NONE = 0,
   OSXMLDM_SIMULATED,
   OSXMLDM_CONTENT
} OSXMLDataMode;

typedef struct {
   size_t offset;
   size_t length;
} OSXMLStrFragOffset;

typedef struct {
   OSXMLStrFragment     mQName;
   OSXMLStrFragment     mLocalName;
   OSXMLStrFragment     mPrefix;

   OSUTF8CHAR           mBuffer[OSXML_DEFAULT_QNAME_BUF_SIZE];
} OSXMLElementName;

typedef struct OSXMLEvent {
   OSUINT32 mId;
   OSINT32 mLevel;
} OSXMLEvent;

typedef struct OSXMLDataCtxt {
   OSINT32 mDataLevel;
   OSXMLDataMode mDataMode;

   /* output */
   int mnChunk;
   OSBOOL mbLastChunk;
   OSBOOL mbCDATA;
   OSXMLStrFragment mData;

   /* for attr only */
   OSXMLStrFragment mSrcData;
   size_t           mSrcDataOffset;
   OSBOOL           mbInsTokenSeparator;
} OSXMLDataCtxt;

typedef struct {
   OSXMLStrFragOffset   mQName;
   OSXMLStrFragOffset   mLocalName;
   OSXMLStrFragOffset   mPrefix;
} OSXMLElemNameOffset;

typedef struct OSXMLStack {
   void*                mpStackArray; /* units specified in params */
   size_t               mSize;        /* allocated size in units */
   size_t               mUnitSize;    /* size of the unit, in octets */
   size_t               mCount;       /* number of used items    */
   OSBOOL               mbDynamic;    /* is dynamic memory allowed */
} OSXMLStack;

typedef struct OSXMLRewindPos {
   size_t      mPos;
   OSXMLEvent  mEvent;
   OSXMLStrFragment mData;
   OSBOOL      mbListMode;
   OSBOOL      mbLastChunk;
} OSXMLRewindPos;

typedef struct OSXMLSrcPos {
   OSUINT32    mLine;
   OSUINT32    mColumn;
   OSUINT32    mByteIdx;
   OSBOOL      mbCR;
} OSXMLSrcPos;

typedef struct OSXMLReader {
   /* input */
   OSCTXT*              mpCtxt;
   OSBOOL               mbNoTransform;
   /* OSBOOL               mbWSOnlyContext; */
   OSBOOL               mbMixedContext;
   OSBOOL               mbListMode;
   OSBOOL               mbSkipPullListElem;
   OSBOOL               mbDecodeAsGroup;
   OSXMLWhiteSpaceMode  mWhiteSpaceMode;
   OSXMLWhiteSpaceMode  mStringWhiteSpaceMode;
   OSXMLEncoding        mEncoding;  /* XML document encoding (ex. UTF-8) */
   OSXMLBOM             mBOM;

   /* output */
   OSXMLEvent           mLastEvent;

   OSXMLElemNameOffset  mElementName;
   OSBOOL               mbHasAttributes;
   OSBOOL               mbEmptyElement;

   OSXMLStack           mAttributes; /* contains OSXMLAttrOffset */

   /*OSXMLStrFragment     mCharacters;*/
   OSXMLStrFragment     mData;
   OSBOOL               mbLastChunk;
   OSBOOL               mbCDATA;
   OSINT32              mLevel;
   OSINT32              mDecodeLevel;

   int                  mError;

   OSINT16              mElementNsIndex;

   /* internals */
   int mState, mPrevState, mNewState;
   int mLocalStates [OSXMLSI_LAST];
   OSUTF8CHAR  mCharBuf[10];
   OSUTF8CHAR* mpEscapeChar;
   OSUTF8CHAR* mpChars;
   size_t      mMarkedByteIndex;
   OSXMLDataMode mDataMode;

   /* contains OSXMLStrFragment for start tags to test against end tags */
   OSXMLStack  mTagNamesStack;

   /* contains namespaces (OSXMLNamespace) */
   OSXMLStack  mNamespacesStack;
#ifdef OSXMLNS12
   const OSUTF8CHAR** mNamespaceTable;
   int         mNamespacesNumber;
   const OSUTF8CHAR** mPrevNamespaceTable;
   int         mPrevNamespacesNumber;
#else
   const char** mNamespaceTable;
   int         mNamespacesNumber;
   const char** mPrevNamespaceTable;
   int         mPrevNamespacesNumber;
#endif

   OSOCTET     mDelayedTasks [MAX_DELAYED_TASK_SIZE];
   size_t      mDelayedTaskCount;
   size_t      mDelayedTaskIndex;

   /* transport */
   OSUTF8CHAR* mpBuffer;
   size_t      mByteIndex;
   size_t      mBufSize;
   size_t      mReadSize;
   size_t      mMarkedPos;

   size_t      mLastBlockSize;
   size_t      mLastByteIndex;

   OSBOOL      mbBackoffEnabled;
   OSBOOL      mbSysMemBuf;

   OSXMLStack  mRewindPosStack;

   /* position in source XML */
   OSXMLSrcPos mSrcPos;
   size_t      mMarkedAttrCount;

} OSXMLReader;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates XML reader.
 */
EXTXMLMETHOD struct OSXMLReader* rtXmlRdCreateXmlReader (OSCTXT* pctxt);

/**
 * Sets the whitespace treatment mode. This mode affects the content
 * and attribute values reading. For example, if OSXMLWSM_COLLAPSE
 * mode is set then all spaces in returned data will be already
 * collapsed.
 *
 * @param pReader    Pointer to XMLReader context structure.
 * @return  Previously set whitespace mode.
 */
EXTXMLMETHOD OSXMLWhiteSpaceMode rtXmlRdSetWhiteSpaceMode
   (struct OSXMLReader* pReader, OSXMLWhiteSpaceMode whiteSpaceMode);

/**
 * Reads the first data chunk from content or attribute value. To start reading
 * the content the last event id (\c rtXmlRdGetLastEvent, \c OSXMLEvent)
 * should be OSXMLEVT_START_TAG. To start reading attribute values,
 * the \c rtXmlRdSelectAttribute function should be called first.
 * The data pointer will be placed in pDataCtxt->mData.value, and the
 * length of this data (in octets) will be stored in pDataCtxt->mData.length.
 *
 * @param pReader    Pointer to XMLReader context structure.
 * @param pDataCtxt  The data context that will be used for the next call to
 *                      \c rtXmlRdNextData. At this point it will be
 *                      initialized.
 *
 * @return If positive - the number of octets read;
 *         If negative - the error code;
 *         If 0        - no more data available.
 */
EXTXMLMETHOD long rtXmlRdFirstData (struct OSXMLReader* pReader, OSXMLDataCtxt* pDataCtxt);

/**
 * Reads the next data chunk from content or attribute value.
 * The data pointer will be placed to pDataCtxt->mData.value, and the
 * length of this data (in octets) will be place to pDataCtxt->mData.length.
 *
 * @param pReader    Pointer to XMLReader context structure.
 * @param pDataCtxt  The data context that was initialized by call to
 *                      \c rtXmlRdFirstData.
 *
 * @return If positive - the number of octets read;
 *         If negative - the error code;
 *         If 0        - no more data available.
 */
EXTXMLMETHOD long rtXmlRdNextData (struct OSXMLReader* pReader, OSXMLDataCtxt* pDataCtxt);

/**
 * Skip the current level of XML document.
 *
 * @param pReader    Pointer to XMLReader context structure.
 * @return 0, if success; error code otherwise.
 */
EXTXMLMETHOD int rtXmlRdSkipCurrentLevel (struct OSXMLReader* pReader);

/**
 * The most low-level call. This function reads the XML data step-by-step
 * returning after each step. Use \c rtXmlRdGetLastEvent function
 * to get the last event.
 *
 * @param pReader    Pointer to XMLReader context structure.
 * @return           Positive event ID or negative error code.
 */
EXTXMLMETHOD int rtXmlRdNext (struct OSXMLReader* pReader);

/**
 * Reads until the next start tag is found.
 *
 * @param pReader    Pointer to XMLReader context structure.
 * @return 0, if success; error code otherwise.
 */
EXTXMLMETHOD int rtXmlRdNextTag (struct OSXMLReader* pReader);

/**
 * Reads until the next end tag is found.
 *
 * @return 0, if success; error code otherwise.
 */
EXTXMLMETHOD int rtXmlRdNextEndTag (struct OSXMLReader* pReader);

/**
 * Returns information on the last processed event (ID and level).
 *
 * @return Structure containing information on the last event.
 */
EXTXMLMETHOD OSXMLEvent rtXmlRdGetLastEvent (struct OSXMLReader* pReader);

/**
 * Returns the local name and namespace URI of the current element.
 * This information is available only when the last event was a start
 * tag event (last event ID is OSXMLEVT_START_TAG).
 *
 * @param pReader    Pointer to XMLReader context structure.
 * @param localName  Local name of element.
 * @param uri        Namespace URI of element.
 * @return 0, if success; error code otherwise.
 */
EXTXMLMETHOD int rtXmlRdGetTagName (struct OSXMLReader* pReader,
                       OSXMLStrFragment* localName,
                       OSINT16* namespaceIndex);

/**
 * Returns the number of attributes defined in the current element.
 *
 * @param pReader    Pointer to XMLReader context structure.
 * @return Negative error code, if the last event type is not
 *           OSXMLEVT_START_TAG;
 *         Number of attributes otherwise.
 */
EXTXMLMETHOD int rtXmlRdGetAttributeCount (struct OSXMLReader* pReader);

/**
 * Gets the attribute name by index. If the last event is not
 * OSXMLEVT_START_TAG, an error code will be returned.
 *
 * @param pReader    Pointer to XMLReader context structure.
 * @param pAttr  Attribute name fragment
 * @param pNsidx Namespace index
 * @param index  Index of the attribute being accessed.
 * @return 0, if success; error code otherwise.
 */
EXTXMLMETHOD int rtXmlRdGetAttributeName (struct OSXMLReader* pReader,
                             OSXMLNameFragments* pAttr,
                             OSINT16* pNsidx,
                             size_t index);

/**
 * Gets the first attribute name and value. If the last event is not
 * OSXMLEVT_START_TAG, an error code will be returned.
 *
 * @param pReader    Pointer to XMLReader context structure.
 * @param pAttrName  Attribute name fragment
 * @param pAttrValue Attribute value string
 * @return 0, if success; error code otherwise.
 */
EXTXMLMETHOD int rtXmlRdFirstAttr (struct OSXMLReader* pReader,
                             OSXMLStrFragment* pAttrName,
                             OSXMLStrFragment* pAttrValue);

/**
 * Gets the second attribute name and value. If the last event is not
 * OSXMLEVT_START_TAG, an error code will be returned.
 *
 * @param pReader    Pointer to XMLReader context structure.
 * @param pAttrName  Attribute name fragment
 * @param pAttrValue Attribute value string
 * @return 0, if success; error code otherwise.
 */
EXTXMLMETHOD int rtXmlRdNextAttr (struct OSXMLReader* pReader,
                             OSXMLStrFragment* pAttrName,
                             OSXMLStrFragment* pAttrValue);

/**
 * Select attribute by index to read the attribute value. This is
 * done by calling the \c rtXmlRdFirstData and \c rtXmlRdNextData
 * functions after this function.
 *
 * @param pReader Pointer to XMLReader context structure.
 * @param index   Index of the attribute being accessed.
 * @return 0, if success; error code otherwise.
 */
EXTXMLMETHOD int rtXmlRdSelectAttribute (struct OSXMLReader* pReader, size_t index);

/**
 * Returns the current nesting level of where the pull parser currently
 * is within the XML document tree.  This might differ from the last
 * event level (\c OSXMLEvent). For example, if the event ID is
 * OSXMLEVT_END_TAG and event level is 2 then the current level returned
 * by this function would be 1 (since end tag is ALREADY processed).
 *
 * @param pReader Pointer to XMLReader context structure.
 * @return current level, if success; error code otherwise.
 */
EXTXMLMETHOD OSINT32 rtXmlRdGetCurrentLevel (struct OSXMLReader* pReader);

/**
 * This function reads XML until one of the events from eventMask happens.
 *
 * @param pReader    Pointer to XMLReader context structure.
 * @param eventMask  Or-ed OSXMLEVT_* constants.
 * @param maxLevel   Maximum nesting level of descent. If the current
 *                     level became greater then this value, then the
 *                     entire level is skipped.  Can be set to -1
 *                     to process all levels.
 * @oaram pLastEvent Pointer to variable to receive last event data. May be
 *                     set to NULL if event data is not needed.
 *
 * @return 0, if success; error code otherwise.
 */
EXTXMLMETHOD int rtXmlRdNextEvent (struct OSXMLReader* pReader,
                      OSUINT32 eventMask,
                      OSINT32 maxLevel,
                      OSXMLEvent* pLastEvent);

/**
 * Returns the current data mode.
 *
 * @return 0, if success; error code otherwise.
 */
EXTXMLMETHOD OSXMLDataMode rtXmlRdGetDataMode (struct OSXMLReader* pReader);

/**
 * Marks the last event as done.  This will cause the next read operation
 * to return the next event.
 *
 * @return Event ID value of last event.
 */
EXTXMLMETHOD int rtXmlRdMarkLastEventDone (struct OSXMLReader* pReader);

/**
 * Marks the last event as active.  This will cause the next read operation
 * to return this event again.
 *
 * @return Event ID value of last event.
 */
EXTXMLMETHOD int rtXmlRdMarkLastEventActive (struct OSXMLReader* pReader);

/**
 * Reset XML reader.
 */
EXTXMLMETHOD void rtXmlRdResetXmlReader (struct OSXMLReader* pReader);

EXTXMLMETHOD void rtXmlRdMarkPos (struct OSXMLReader* pReader);
EXTXMLMETHOD void rtXmlRdRewindToMarkedPos (struct OSXMLReader* pReader);
EXTXMLMETHOD void rtXmlRdResetMarkedPos (struct OSXMLReader* pReader);

EXTXMLMETHOD int rtXmlRdGetXSITypeAttr (struct OSXMLReader* pReader,
                           const OSUTF8CHAR** ppAttrValue,
                           OSINT16* nsidx, size_t* pLocalOffs);

/**
 * Return TRUE when next will be end tag ("</").
 */
EXTXMLMETHOD OSBOOL rtXmlRdIsEmpty (struct OSXMLReader* pReader);

EXTXMLMETHOD void rtXmlRdGetSourcePosition (struct OSXMLReader* pReader,
                               OSUINT32* pLine,
                               OSUINT32* pColumn,
                               OSUINT32* pByteIndex,
                               OSBOOL nextPos);

EXTXMLMETHOD void rtXmlRdErrAddSrcPos (struct OSXMLReader* pReader,
                          OSBOOL nextPos);

EXTXMLMETHOD void rtXmlRdErrAddDataSrcPos (struct OSXMLReader* pReader,
                              OSXMLDataCtxt* pDataCtxt,
                              OSUINT32 offset);

#ifdef __cplusplus
}
#endif

#endif /* __RTXMLPULL_H */

