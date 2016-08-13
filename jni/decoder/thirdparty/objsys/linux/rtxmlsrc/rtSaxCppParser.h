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

#ifndef __RTSAXCPPPARSER_H_
#define __RTSAXCPPPARSER_H_

#include <string.h>
#include "rtxsrc/rtxErrCodes.h"
#include "rtxsrc/OSRTContext.h"
#include "rtxmlsrc/rtSaxCppParserIF.h"
#include "rtxmlsrc/osrtxml.h"
#include "rtxmlsrc/rtSaxDefs.h"

class EXTXMLCLASS OSXMLBasePtr {
   OSXMLBase* mPtr;
 public:
   OSXMLBasePtr ():mPtr (0) {}
   OSXMLBasePtr (OSXMLBase* ptr):mPtr (ptr) {}
   ~OSXMLBasePtr () { if (mPtr) mPtr->release (); }

   operator OSXMLBase* () const  { return mPtr; }

   OSXMLBase* operator= (OSXMLBase* ptr) {
      if (mPtr) mPtr->release ();
      mPtr = ptr;
      return mPtr;
   }
};

/**
 * This class is derived from the SAX class DefaultHandler base class.
 * It contains variables and methods specific to decoding XML messages.
 * It is used to intercept standard SAX parser events, such as startElement,
 * characters, endElement. This class is used as a base class for XBinder
 * generated global element control classes (&lt;elem&gt;_CC).
 */
class EXTERNXML OSXMLDefaultHandler : public OSXMLDefaultHandlerIF
{
 protected:
   OSRTCtxtPtr mpContext;
   const OSUTF8CHAR* mpElemName;
   OSINT32 mLevel;
   OSINT16 mStartLevel;
   OSINT16 mReqElemCnt;
   OSINT16 mCurrElemIdx;
   OSINT16 mState;

   struct EXTXMLCLASS ErrorInfo {
      int stat;
      const char* file;
      int line;

      inline ErrorInfo ():stat (0),file (0), line (0) {}
   } errorInfo;

 public:
   OSXMLDefaultHandler (OSRTContext* pContext,
      const OSUTF8CHAR* elemName = 0, OSINT32 level = 0) :
      mpContext (pContext),
      mpElemName(elemName), mLevel(level),
      mStartLevel (0), mReqElemCnt (0), mCurrElemIdx(0), mState (0)
   { }

   virtual ~OSXMLDefaultHandler () {}

   EXTXMLMETHOD virtual int startElement
      (const OSUTF8CHAR* const uri,
       const OSUTF8CHAR* const localname,
       const OSUTF8CHAR* const qname,
       const OSUTF8CHAR* const* attrs);

   EXTXMLMETHOD virtual int characters
      (const OSUTF8CHAR* const chars, unsigned int length);

   EXTXMLMETHOD virtual int endElement
      (const OSUTF8CHAR* const uri,
       const OSUTF8CHAR* const localname,
       const OSUTF8CHAR* const qname);

   EXTXMLMETHOD virtual void startDocument();
   EXTXMLMETHOD virtual void endDocument ();

   EXTXMLMETHOD virtual int finalize ();

   // SAX Error Info handlers
   EXTXMLMETHOD virtual void resetErrorInfo ();
   EXTXMLMETHOD virtual void setErrorInfo (int status, const char* file = 0, int line = 0);
   EXTXMLMETHOD virtual int  getErrorInfo (int* status, const char** file, int* line);

   // SAX ErrorHandler interface

   // void warning (const SAXParseException& exception);
   // void error (const SAXParseException& exception);
   // void fatalError (const SAXParseException& exception);
   // void resetErrors ();

   /**
    * This method returns the current state of the decoding process.
    *
    * @return   The state of the decoding process as type OSXMLState. Can be
    *            XMLINIT, XMLSTART, XMLDATA, or XMLEND.
    */
   inline OSINT16 getState() { return mState; }

   virtual void init (int level = 0) {
      mState = 0; mLevel = (OSINT16)level;
   }

   void setElemName (const OSUTF8CHAR* elemName) {
      mpElemName = elemName;
   }

   inline OSBOOL isComplete () { return (mLevel <= mStartLevel); }

#ifndef _COMPACT
   EXTXMLMETHOD void traceStartElement (const char* funcName, const OSUTF8CHAR* localName);
   EXTXMLMETHOD void traceEndElement (const char* funcName, const OSUTF8CHAR* localName);
#else
   inline void traceStartElement
      (const char* funcName, const OSUTF8CHAR* localName) {}
   inline void traceEndElement
      (const char* funcName, const OSUTF8CHAR* localName) {}
#endif
} ;

class EXTXMLCLASS OSXMLDefaultHandlerPtr {
   OSXMLDefaultHandler* mPtr;
 public:
   inline OSXMLDefaultHandlerPtr ():mPtr (0) {}
   inline OSXMLDefaultHandlerPtr (OSXMLDefaultHandler* ptr):mPtr (ptr) {}
   inline ~OSXMLDefaultHandlerPtr () { delete mPtr; }

   inline operator OSXMLDefaultHandler* () { return mPtr; }
   inline operator const OSXMLDefaultHandler* () const { return mPtr; }
   inline OSXMLDefaultHandler* operator-> () const { return mPtr; }

   inline OSXMLDefaultHandler* operator= (OSXMLDefaultHandler* ptr) {
      delete mPtr;
      mPtr = ptr;
      return mPtr;
   }
   inline int operator == (const OSXMLDefaultHandler* ptr) const
      { return ptr == mPtr; }
   inline int operator != (const OSXMLDefaultHandler* ptr) const
      { return ptr != mPtr; }
   inline int operator ! () const { return mPtr == 0; }

   friend int operator != (const void* ptr, const OSXMLDefaultHandlerPtr& ptr2);
   friend int operator == (const void* ptr, const OSXMLDefaultHandlerPtr& ptr2);
} ;

inline int operator != (const void* ptr, const OSXMLDefaultHandlerPtr& ptr2)
  { return ptr != (const void*)ptr2.mPtr; }

inline int operator == (const void* ptr, const OSXMLDefaultHandlerPtr& ptr2)
  { return ptr == (const void*)ptr2.mPtr; }

class EXTXMLCLASS OSXMLParserCtxt : public OSXMLParserCtxtIF {
   OSRTCtxtPtr mpContext;

 public:
   OSXMLParserCtxt (OSRTContext *pContext);

   EXTXMLMETHOD virtual OSRTInputStreamIF* createInputStream ();
   EXTXMLMETHOD virtual OSRTInputStreamIF* createFileInputStream
      (const char* const filename);
   EXTXMLMETHOD virtual OSRTInputStreamIF* createMemoryInputStream
      (OSOCTET* pMemBuf, size_t bufSize);
   EXTXMLMETHOD virtual OSCTXT* getContext ();
   EXTXMLMETHOD virtual const OSUTF8CHAR* parseQName (const OSUTF8CHAR* const qname);
} ;

#ifndef _COMPACT
#define OSCPPSAXDIAGSTART(funcName,localName) \
traceStartElement (funcName, localName)

#define OSCPPSAXDIAGEND(funcName,localName) \
traceEndElement (funcName, localName)

#else /* COMPACT */
#define OSCPPSAXDIAGSTART(funcName,localName)
#define OSCPPSAXDIAGEND(funcName,localName)
#endif

#endif /* __RTSAXCPPPARSER_H_ */

