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

#ifndef __SAXPARSER_H_
#define __SAXPARSER_H_

#include <string.h>
#ifndef _NO_EXCEPTIONS

#if (defined(_HP_UX) || defined(__hpux) || defined(_HPUX_SOURCE)) && \
__cplusplus >= 199707L && defined (__HP_aCC) && __HP_aCC >= 55000L
/* HP-UX aCC, version 5.5 and above. Maybe, earlier versions also
   require new stream headers - will change if it is so. */
#define __NEW_STREAM_HEADERS__ 1
#endif

#if defined (__NEW_STREAM_HEADERS__) || defined (_MSC_VER) || \
(defined (__GNUC__) && __GNUC__ >= 3)
#include <iostream>
using namespace std;
#else
#include <iostream.h>
#endif
#endif // _NO_EXCEPTION
#include <stdlib.h>

#include "rtxersrc/asn1xer.h"
/* #include "rtsrc/OSCInputStream.h" */
#include "rtxersrc/rtSAXDefs.h"

#ifndef _NO_EXCEPTIONS
#define OSRTSAXEXCEPTION0 throw OSRTSAXExceptionImpl ()
#define OSRTSAXEXCEPTION1(str) throw OSRTSAXExceptionImpl (str)
#else  // _NO_EXCEPTIONS
#define OSRTSAXEXCEPTION0 { \
fprintf (stderr, "Fatal Error, OSRTSAXExceptionImpl is thrown, in %s at %i\n", \
__FILE__, __LINE__); \
exit (-1); \
}
#define OSRTSAXEXCEPTION1(str) { \
fprintf (stderr, "Fatal Error, OSRTSAXExceptionImpl(%s) is thrown, in %s at %i\n", \
str, __FILE__, __LINE__); \
exit (-1); \
}
#endif // _NO_EXCEPTIONS
/**
  * Encapsulate a general SAX error or warning.
  *
  * <p>This class can contain basic error or warning information from
  * either the XML SAX parser or the application: a parser writer or
  * application writer can subclass it to provide additional
  * functionality.  SAX handlers may throw this exception or
  * any exception subclassed from it.</p>
  */
class EXTERNXER OSRTSAXException
{
 public:
   virtual ~OSRTSAXException () {}
   /**
     * Get the contents of the message.
     */
   virtual const XMLCHAR* getMessage() = 0;

   virtual int getStatus () = 0;
   virtual const XMLCHAR* getSrcFileName () = 0;
   virtual int getSrcLineNum() = 0;
};

class EXTERNXER OSXMLErrorInfo {
 public:
   virtual void resetErrorInfo () = 0;
   virtual void setErrorInfo (int status, const char* file = 0, int line = 0) = 0;
   virtual int  getErrorInfo (int* status, const char** file, int* line) = 0;
};

class EXTERNXER OSXMLErrorHandler
{
 public:

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
   */
   virtual void warning(/*const SAXParseException& exception*/) = 0;

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
   virtual void error(/*const SAXParseException& exception*/) = 0;

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
   virtual void fatalError(/*const SAXParseException& exception*/) = 0;

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
  * <p>This is the main interface that most SAX2 applications
  * implement: if the application needs to be informed of basic parsing
  * events, it implements this interface and registers an instance with
  * the SAX2 parser using the setDocumentHandler method.  The parser
  * uses the instance to report basic document-related events like
  * the start and end of elements and character data.</p>
  *
  * <p>The order of events in this interface is very important, and
  * mirrors the order of information in the document itself.  For
  * example, all of an element's content (character data, processing
  * instructions, and/or subelements) will appear, in order, between
  * the startElement event and the corresponding endElement event.</p>
  */
class EXTERNXER OSXMLContentHandler
{
 public:
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
   * <p>The application must not attempt to read from the array
   * outside of the specified range.</p>
   *
   * <p>Note that some parsers will report whitespace using the
   * ignorableWhitespace() method rather than this one (validating
   * parsers must do so).</p>
   *
   * @param chars The characters from the XML document.
   * @param length The number of characters to read from the array.
   */
   virtual void characters
   (
       const XMLCHAR* const chars,
       const unsigned int   length
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
   virtual void endElement
   (
      const XMLCHAR* const uri,
      const XMLCHAR* const localname,
      const XMLCHAR* const qname
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
   * <p>Note that the attribute list provided will
   * contain only attributes with explicit values (specified or
   * defaulted): IMPLIED attributes will be omitted.</p>
   *
   * @param uri The URI of the asscioated namespace for this element
   * @param localname The local part of the element name
   * @param qname The QName of this element
   * @param attrs The attributes attached to the element, if any.
   */
   virtual void startElement
   (
      const   XMLCHAR* const  uri,
      const   XMLCHAR* const  localname,
      const   XMLCHAR* const  qname,
      const   XMLCHAR* const* attrs
   ) = 0;

   //@}
};

class EXTERNXER OSXMLBase {
protected:
   OSXMLBase () {}

   virtual ~OSXMLBase ();
public:
   virtual void release () = 0;
};

#define DECLARE_XMLBASEIMP \
public: void release () { delete this; }

class EXTERNXER OSXMLReader : public OSXMLBase
{
public:

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
   * @exception OSRTSAXException Any SAX exception, possibly
   *            wrapping another exception.
   */
#if 0
   virtual int parse
   (
      OSCInputStream& source
   ) = 0;
#endif
 /**
   * Parse an XML document from memory buffer.
   */
   virtual int parse
   (
       const char* const pBuffer,
       int               bufSize
   ) = 0;

 /**
   * Parse an XML document from a system identifier (URI).
   *
   * This method is a shortcut for the common case of reading a
   * document from a system identifier.
   *
   * If the system identifier is a URL, it must be fully resolved
   * by the application before it is passed to the parser.
   *
   * @param systemId The system identifier (URI).
   * @exception OSRTSAXException Any SAX exception, possibly
   *            wrapping another exception.
   */
   virtual int parse
   (
       const char* const systemId
   ) = 0;

 /**
   * Allow an application to register a document event handler.
   *
   * If the application does not register a document handler, all
   * document events reported by the SAX parser will be silently
   * ignored (this is the default behaviour implemented by
   * HandlerBase).
   *
   * Applications may register a new or different handler in the
   * middle of a parse, and the SAX parser must begin using the new
   * handler immediately.
   *
   * @param handler The document handler.
   */
   virtual void setContentHandler(OSXMLContentHandler* const handler) = 0;

 /**
   * Allow an application to register an error event handler.
   *
   * If the application does not register an error event handler,
   * all error events reported by the SAX parser will be silently
   * ignored, except for fatalError, which will throw a SAXException
   * (this is the default behaviour implemented by HandlerBase).
   *
   * Applications may register a new or different handler in the
   * middle of a parse, and the SAX parser must begin using the new
   * handler immediately.
   *
   * @param handler The error handler.
   */
   virtual void setErrorHandler(OSXMLErrorHandler* const handler) = 0;

   virtual void setErrorInfoHandler (OSXMLErrorInfo* const handler) = 0;
};

class EXTERNXER StrX {
 private :
   char* fLocalForm;
 public :
   // -----------------------------------------------------------------------
   //  Constructors and Destructor
   // -----------------------------------------------------------------------
   StrX (const XMLCHAR* const toTranscode);
   ~StrX();

   // -----------------------------------------------------------------------
   //  Getter methods
   // -----------------------------------------------------------------------
   const char* localForm() const
   {
      return fLocalForm;
   }

   operator const char* () const
   {
      return fLocalForm;
   }
};

#ifndef _NO_EXCEPTIONS
inline ostream& operator<<(ostream& target, const StrX& toDump)
{
   target << toDump.localForm();
   return target;
}
#endif // _NO_EXCEPTION

// Exception implementation

class EXTERNXER OSRTSAXExceptionImpl : public OSRTSAXException
{
   DECLARE_XMLBASEIMP
 public:
   OSRTSAXExceptionImpl();

   OSRTSAXExceptionImpl(const char* const msg);

   OSRTSAXExceptionImpl(const char* const msg,
      int stat_, const char* const file_, int line_);

 /**
   * Create a new OSRTSAXExceptionImpl.
   *
   * @param msg The error or warning message.
   */
   OSRTSAXExceptionImpl(int stat_, const char* file_, int line_);

 /**
   * Copy constructor
   *
   * @param toCopy The exception to be copy constructed
   */
   OSRTSAXExceptionImpl(const OSRTSAXExceptionImpl& toCopy);

   /** Destructor */
   virtual ~OSRTSAXExceptionImpl();

   OSRTSAXExceptionImpl& operator=(const OSRTSAXExceptionImpl& toCopy);

   virtual const XMLCHAR* getMessage();

   virtual int getStatus () { return stat; }
   virtual const XMLCHAR* getSrcFileName () { return file; }
   virtual int getSrcLineNum() { return line; }

protected :
   // -----------------------------------------------------------------------
   //  Protected data members
   //
   //  fMsg
   //      This is the text of the error that is being thrown.
   // -----------------------------------------------------------------------
   XMLCHAR* fMsg;
   int      stat;
   XMLCHAR* file;
   int      line;
};

class OSXMLBasePtr {
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

#if 0
// this function should be custom
OSXMLReader* saxCreateXmlReader (ASN1CtxtPtr ctxt,
                                 OSXMLContentHandler* pSaxHandler,
                                 OSXMLErrorInfo* pErrorInfoHandler);
#endif

// common public functions
EXTERNXER OSRTSAXException* createSAXException ();

EXTERNXER OSRTSAXException* createSAXException (const char* const msg, int stat,
   const char* const fileName, int line);

#endif /* __SAXPARSER_H_ */
