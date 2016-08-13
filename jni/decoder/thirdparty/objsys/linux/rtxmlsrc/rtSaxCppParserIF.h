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

#ifndef __RTSAXCPPPARSERIF_H_
#define __RTSAXCPPPARSERIF_H_

#include "rtxsrc/rtxErrCodes.h"
#include "rtxsrc/OSRTInputStreamIF.h"
#include "rtxmlsrc/osrtxml.h"
#include "rtxmlsrc/rtSaxDefs.h"


class EXTXMLCLASS OSXMLErrorInfo {
 public:
   virtual ~OSXMLErrorInfo () {}
   virtual void resetErrorInfo () = 0;
   virtual void setErrorInfo (int status, const char* file = 0, int line = 0) = 0;
   virtual int  getErrorInfo (int* status, const char** file, int* line) = 0;
};

class EXTERNXML OSXMLErrorHandler
{
 public:
    virtual ~OSXMLErrorHandler () {}

   /** @name The error handler interface */
   //@{
  /**
   * Receive notification of a warning.
   *
   * <p>SAX parsers will use this method to report conditions that
   * are not errors or fatal errors as defined by the XML 1.0
   * recommendation.  The default behaviour is to take no action.</p>
   *
   * <p>The SAX parser must continue to provide normal parsing events
   * after invoking this method: it should still be possible for the
   * application to process the document through to the end.</p>
   *
   */
   virtual void warning() = 0;

 /**
   * Receive notification of a recoverable error.
   *
   * <p>This corresponds to the definition of "error" in section 1.2
   * of the W3C XML 1.0 Recommendation.  For example, a validating
   * parser would use this callback to report the violation of a
   * validity constraint.  The default behaviour is to take no
   * action.</p>
   *
   * <p>The SAX parser must continue to provide normal parsing events
   * after invoking this method: it should still be possible for the
   * application to process the document through to the end.  If the
   * application cannot do so, then the parser should report a fatal
   * error even if the XML 1.0 recommendation does not require it to
   * do so.</p>
   */
   virtual void error() = 0;

 /**
   * Receive notification of a non-recoverable error.
   *
   * <p>This corresponds to the definition of "fatal error" in
   * section 1.2 of the W3C XML 1.0 Recommendation.  For example, a
   * parser would use this callback to report the violation of a
   * well-formedness constraint.</p>
   *
   * <p>The application must assume that the document is unusable
   * after the parser has invoked this method, and should continue
   * (if at all) only for the sake of collecting addition error
   * messages: in fact, SAX parsers are free to stop reporting any
   * other events once this method has been invoked.</p>
   */
   virtual void fatalError() = 0;

   /**
   * Reset the Error handler object on its reuse
   *
   * <p>This method helps in reseting the Error handler object
   * implementational defaults each time the Error handler is begun.</p>
   *
   */
   virtual void resetErrors() = 0;
   //@}
};

/**
  * Receive notification of general document events.
  *
  */
class EXTERNXML OSXMLContentHandler
{
 public:
   virtual ~OSXMLContentHandler() {}

   /** @name The virtual document handler interface */

   //@{
  /**
   * Receive notification of character data.
   *
   * <p>The Parser will call this method to report each chunk of
   * character data.  SAX parsers may return all contiguous character
   * data in a single chunk, or they may split it into several
   * chunks; however, all of the characters in any single event
   * must come from the same external entity, so that the Locator
   * provides useful information.</p>
   *
   * @param chars The characters from the XML document.
   * @param length The length of chars.
   */
   virtual int characters
   (
       const OSUTF8CHAR* const chars,
       unsigned int      length
   ) = 0;

 /**
   * Receive notification of the end of an element.
   *
   * <p>The SAX parser will invoke this method at the end of every
   * element in the XML document; there will be a corresponding
   * startElement() event for every endElement() event (even when the
   * element is empty).</p>
   *
   * @param uri The URI of the asscioated namespace for this element
   * @param localname The local part of the element name
   * @param qname The QName of this element
   */
   virtual int endElement
   (
      const OSUTF8CHAR* const uri,
      const OSUTF8CHAR* const localname,
      const OSUTF8CHAR* const qname
   ) = 0;

 /**
   * Receive notification of the beginning of an element.
   *
   * <p>The Parser will invoke this method at the beginning of every
   * element in the XML document; there will be a corresponding
   * endElement() event for every startElement() event (even when the
   * element is empty). All of the element's content will be
   * reported, in order, before the corresponding endElement()
   * event.</p>
   *
   * @param uri The URI of the asscioated namespace for this element
   * @param localname The local part of the element name
   * @param qname The QName of this element
   * @param attrs The attributes name/value pairs attached to the element,
   *                if any.
   * @see #endElement
   */
   virtual int startElement
   (
      const   OSUTF8CHAR* const  uri,
      const   OSUTF8CHAR* const  localname,
      const   OSUTF8CHAR* const  qname,
      const   OSUTF8CHAR* const* attrs
   ) = 0;

   //@}
};

class EXTXMLCLASS OSXMLBase {
protected:
   OSXMLBase () {}

   virtual ~OSXMLBase () {}
public:
   virtual void release () = 0;
};

#define DECLARE_XMLBASEIMP \
public: void release () { delete this; }

class EXTERNXML OSXMLReaderClass : public OSXMLBase
{
 public:

   virtual int parse () = 0;
 /**
   * Parse an XML document.
   *
   * The application can use this method to instruct the SAX parser
   * to begin parsing an XML document from any valid input
   * source (a character stream, a byte stream, or a URI).
   *
   * Applications may not invoke this method while a parse is in
   * progress (they should create a new Parser instead for each
   * additional XML document).  Once a parse is complete, an
   * application may reuse the same Parser object, possibly with a
   * different input source.
   *
   * @param source The input source for the top-level of the
   *               XML document.
   */
   virtual int parse (OSRTInputStreamIF& source) = 0;

 /**
   * Parse an XML document from memory buffer.
   *
   * @param pBuffer    Buffer containg the XML data to be parsed.
   * @param bufSize    Buffer size, in octets.
   */
   virtual int parse
   (
      const char* const pBuffer,
      size_t            bufSize
   ) = 0;

 /**
   * Parse an XML document from a system identifier (URI).
   *
   * This method is a shortcut for the common case of reading a
   * document from a system identifier.  It is the exact equivalent
   * of the following:
   *
   * parse(new URLInputSource(systemId));
   *
   * If the system identifier is a URL, it must be fully resolved
   * by the application before it is passed to the parser.
   *
   * @param systemId The system identifier (URI).
   */
   virtual int parse
   (
       const char* const systemId
   ) = 0;
};

/**
 * This class is derived from the SAX class DefaultHandler base class.
 * It contains variables and methods specific to decoding XML messages.
 * It is used to intercept standard SAX parser events, such as startElement,
 * characters, endElement. This class is used as a base class for XBinder
 * generated global element control classes (&lt;elem&gt;_CC).
 */
class EXTERNXML OSXMLDefaultHandlerIF :
   public OSXMLContentHandler, public OSXMLErrorInfo
{
 public:
   virtual ~OSXMLDefaultHandlerIF () {}
#if 0
   // These are already defined in OSXMLContentHandler
   virtual int startElement
      (const OSUTF8CHAR* const uri,
       const OSUTF8CHAR* const localname,
       const OSUTF8CHAR* const qname,
       const OSUTF8CHAR* const* attrs) = 0;

   virtual int characters
      (const OSUTF8CHAR* const chars, unsigned int length) = 0;

   virtual int endElement
      (const OSUTF8CHAR* const uri,
       const OSUTF8CHAR* const localname,
       const OSUTF8CHAR* const qname) = 0;
#endif
   virtual void startDocument() = 0;
   virtual void endDocument () = 0;

   virtual int finalize () = 0;
} ;

class EXTXMLCLASS OSXMLParserCtxtIF {
 public:
   virtual ~OSXMLParserCtxtIF () {}
   virtual OSRTInputStreamIF* createInputStream () = 0;
   virtual OSRTInputStreamIF* createFileInputStream
      (const char* const filename) = 0;
   virtual OSRTInputStreamIF* createMemoryInputStream
      (OSOCTET* pMemBuf, size_t bufSize) = 0;
   virtual OSCTXT* getContext () = 0;
   virtual const OSUTF8CHAR* parseQName (const OSUTF8CHAR* const qname) = 0;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function creates XML reader instance.
 *
 * @param pContext     Pointer to XML Parser Context instance.
 * @param pSaxHandler  Pointer to default SAX handler instance.
 * @return OSXMLReaderClass instance.
 */
OSXMLReaderClass* rtSaxCppCreateXmlReader
(OSXMLParserCtxtIF* pContext, OSXMLDefaultHandlerIF* pSaxHandler);

/**
 * Enables the parser's thread-safe mode (if applicable).
 *
 * @return          0 - if success, negative value is error.
 */
int rtSaxCppEnableThreadSafety ();

/**
 * This function locks XML library. It increments an internal counter to
 * prevent unloading and freeing of the XML parser library. Usually, the XML
 * parser library is being initialized on demand at the beginning of the
 * \c 'parse' method and is freed at the end (if necessary, for example
 * Xerces requires initialization/termination, but Expat does not). It
 * may lead to serious performance impact, if multiple calls to 'parse'
 * are being done.  If \c rtSaxCppLockXmlLibrary was called, XML the parser
 * library won't be freed until \c rtSaxCppUnlockXmlLibrary is called.
 */
void rtSaxCppLockXmlLibrary ();

/**
 * This function unlocks XML library. It decrements an internal counter and if
 * it is zero XML parser library will be freed (if necessary). See
 * \c rtSaxCppLockXmlLibrary for further details.
 */
void rtSaxCppUnlockXmlLibrary ();

#ifdef __cplusplus
}
#endif

#endif /* __RTSAXCPPPARSERIF_H_ */

