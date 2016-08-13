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
/*
// CHANGE LOG
// Date         Init    Description
// 11/03/03	CZ	Expand documentation comments.
//
///////////////////////////////////////////////////////////////
*/

/**
 * @file asn1XerCppTypes.h
 * XER C++ type and class definitions.
 */
#ifndef _ASN1XERCPPTYPES_H_
#define _ASN1XERCPPTYPES_H_

#include "rtxmlsrc/rtSaxCppParser.h"
#include "rtsrc/asn1CppTypes.h"
#include "rtxersrc/asn1xer.h"
#include "rtxsrc/rtxMemBuf.h"

class OSRTInputStream;

/** @defgroup xercppruntime XER C++ Runtime Classes.
 * @{
 */
/** @defgroup xermsgbuffclas XER Message Buffer Classes
 * These classes manage the buffers for encoding and decoding ASN.1 XER
 * messages. @{
*/

// ASN.1 XER message buffer class

/**
 * The XER message buffer class is derived from the ASN1MessageBuffer base
 * class. It is the base class for the ASN1XEREncodeBuffer and
 * ASN1XERDecodeBuffer classes. It contains variables and methods specific to
 * encoding or decoding ASN.1 messages using the XML Encoding Rules (XER). It
 * is used to manage the buffer into which an ASN.1 message is to be encoded or
 * decoded.
 *

 */
class EXTERNXER ASN1XERMessageBuffer : public ASN1MessageBuffer {
 protected:
   /**
    * This protected constructor creates a new context.
    * Use the getStatus() method to determine if an error occured during
    * initialization or not.
    *
    * @param bufferType   Type of message buffer that is being created (for
    *                       example, XEREncode or XERDecode).
    */
   ASN1XERMessageBuffer (Type bufferType);

   /**
    * This protected constructor creates uses an existing context to construct
    * a message buffer.  Use the getStatus() method to determine if an error
    * occured during initialization or not.
    *
    * @param bufferType   Type of message buffer that is being created (for
    *                       example, XEREncode or XERDecode).
    * @param pContext     Pointer to context to use.
    */
   ASN1XERMessageBuffer (Type bufferType, OSRTContext* pContext) :
      ASN1MessageBuffer (bufferType, pContext) {}

 public:
} ;

// ASN.1 XER encode message buffer class
/**
 * The ASN1XEREncodeBuffer class is derived from the ASN1XERMessageBuffer base
 * class. It contains variables and methods specific to encoding ASN.1 messages
 * using the XML Encoding Rules (XER). It is used to manage the buffer into
 * which an ASN.1 message is to be encoded.
 *
 */
class EXTERNXER ASN1XEREncodeBuffer : public ASN1XERMessageBuffer {
 public:
   /**
    * Default constructor Use getStatus() method to determine has error occured
    * during the initialization or not.
    */
   ASN1XEREncodeBuffer ();

   /**
    * A constructor. Use getStatus() method to determine has error occured
    * during the initialization or not.
    *
    * @param canonical    Indicates the usage of Canonical XER(CXER).
    */
   ASN1XEREncodeBuffer (OSBOOL canonical);

   /**
    * A constructor. Use getStatus() method to determine has error occured
    * during the initialization or not.
    *
    * @param canonical    Indicates the usage fo Canonical XER(CXER).
    * @param pContext     Pointer to existing context to reference.
    * @param initBufSize  Initial size of encode buffer
   */
   ASN1XEREncodeBuffer (OSBOOL canonical, OSRTContext* pContext,
      size_t initBufSize = 0);

   /**
    * A constructor. Use getStatus() method to determine has error occured
    * during the initialization or not.
    *
    * @param pMsgBuf      A pointer to a fixed size message buffer to receive
    *                       the encoded message.
    * @param msgBufLen    Size of the fixed-size message buffer.
    */
   ASN1XEREncodeBuffer (OSOCTET* pMsgBuf, size_t msgBufLen);

   /**
    * A constructor. Use getStatus() method to determine has error occured
    * during the initialization or not.
    *
    * @param pMsgBuf      A pointer to a fixed size message buffer to receive
    *                       the encoded message.
    * @param msgBufLen    Size of the fixed-size message buffer.
    * @param canonical    Indicates the usage fo Canonical XER(CXER).
    * @param openType     Open Type encoding indicator. Set this argument as
    *                       TRUE to encode an open type.
    */

   ASN1XEREncodeBuffer (OSOCTET* pMsgBuf, size_t msgBufLen,
                        OSBOOL canonical, OSBOOL openType = FALSE);
   /**
    * This method returns the length of a previously encoded XER message.
    *
    * @return             len Length of the XER message encapsulated within
    *                       this buffer object.
    */
   inline size_t getMsgLen () {
      return (getCtxtPtr()->buffer.byteIndex);
   }

   /**
    * This method reinitializes the encode buffer pointer to allow a new
    * message to be encoded. This makes it possible to reuse one message buffer
    * object in a loop to encode multiple messages. After this method is
    * called, any previously encoded message in the buffer will be overwritten
    * on the next encode call.
    *
    * @return             Completion status of operation:
    *                       - 0 (0) = success,
    *                       - negative return value is error.
    */
   int init ();

   /**
    * This method checks the type of the message buffer.
    *
    * @param bufferType   Enumerated identifier specifying a derived class. The
    *                       only possible value for this class is XEREncode.
    * @return             Boolean result of the match operation. True if this
    *                       is the class corresponding to the identifier
    *                       argument.
    */
   virtual OSBOOL isA (int bufferType) {
      return bufferType == XEREncode;
   }
   /**

    *
    * This method turns Canonical XML Encoding Rules (CXER) usage on.
    */
   inline void setCanonical() { rtxCtxtSetFlag (getCtxtPtr(), ASN1CANXER); }

   /**

    *
    * This method turns Open Type encoding on.
    *

    */
   inline void setOpenType () { rtxCtxtSetFlag (getCtxtPtr(), ASN1OPENTYPE); }

   /**
    * There are two versions of this method. The first version writes to a file
    * that is specified by file name.
    *

    *
    * @param filename     The name of file for writing the encoded message.
    * @return             Number of octets actually written, which may be less
    *                       than real message length if an error occurs.
    */
   long write (const char* filename);

   /**
    * The second version writes to a file that is specified by a FILE pointer.
    *
    * @param fp           The pointer to FILE structure for writing the encoded
    *                       message.
    * @return             Number of octets actually written, which may be less
    *                       than real message length if an error occurs.
    */
   long write (FILE* fp);
} ;

/**
 * The ASN1XERShdMemHpEncBuf (shared memory heap) class is derived from
 * the ASN1XEREncodeBuffer class. It is designed to use the memory heap
 * of the given context object. Its primary use is for decoding open
 * type data.
 */
class EXTERNXER ASN1XERShdMemHpEncBuf : public ASN1XEREncodeBuffer {
 protected:
   void* mpSavedMemHeap;  // memory heap from original context

 public:
   /**
    * This constructor creates a shared memory heap encode buffer
    * by using the memory heap in the pContext object.
    *
    * @param canonical    Indicates the usage fo Canonical XER(CXER).
    * @param pContext     Pointer to existing context to reference.
    * @param initBufSize  Initial size of encode buffer
   */
   ASN1XERShdMemHpEncBuf (OSBOOL canonical, OSRTContext* pContext,
      size_t initBufSize = 0);

   /**
    * This constructor is a passthru to the underlying XER encode buffer
    * constructor.
    *
    * @param canonical    Indicates the usage of Canonical XER(CXER).
    */
   ASN1XERShdMemHpEncBuf (OSBOOL canonical);

   /**
    * The destructor frees the original context memory heap.
    */
   ~ASN1XERShdMemHpEncBuf();
} ;

//XER SAX decode handler base class. ASN1XERSAXDecodeHandler derived class.
/**
 * This class is derived from the SAX class DefaultHandler base class. It
 * contains variables and methods specific to XML decoding ASN.1 messages. It
 * is used to intercept standard SAX parser events, such as startElement,
 * characters, endElement. This class is used as a base class for XER ASN1C
 * generated ASN1C_* classes.
 */
class EXTERNXER ASN1XERSAXDecodeHandler : public OSXMLDefaultHandlerIF
{
 protected:
   ASN1XERState mCurrState;
   int mCurrElemID;
   int mLevel, mStartLevel;
   const char* mpTypeName;
   struct ErrorInfo {
      int stat;
      const char* file;
      int line;
   } errorInfo;
 public:

   /**
    * Default constructor without parameters.
    */
   ASN1XERSAXDecodeHandler () :
      mCurrState(XERINIT), mCurrElemID(0), mLevel(0), mpTypeName(0) {}

   virtual int startElement
      (const OSUTF8CHAR* const uri,
       const OSUTF8CHAR* const localname,
       const OSUTF8CHAR* const qname,
       const OSUTF8CHAR* const* attrs);

   virtual int characters
      (const OSUTF8CHAR* const chars, unsigned int length);

   virtual int endElement
      (const OSUTF8CHAR* const uri,
       const OSUTF8CHAR* const localname,
       const OSUTF8CHAR* const qname);

   virtual void startDocument();
   virtual void endDocument ();

   // SAX Error Info handlers
   virtual void resetErrorInfo ();
   virtual void setErrorInfo (int status, const char* file = 0, int line = 0);
   virtual int  getErrorInfo (int* status, const char** file, int* line);

   // SAX ErrorHandler interface

   // void warning (const SAXParseException& exception);
   // void error (const SAXParseException& exception);
   // void fatalError (const SAXParseException& exception);
   // void resetErrors ();

   /**
    * This method returns the current state of the decoding process.
    *
    * @return             The state of the decoding process as type
    *                       ASN1XERState. Can be XERINIT, XERSTART, XERDATA, or
    *                       XEREND.
    */
   inline ASN1XERState getState() { return mCurrState; }
   virtual int finalize() { return 0; }
   OSCTXT* finalizeMemBuf (OSRTMessageBufferIF* msgBuf, OSRTMEMBUF& memBuf);
   virtual void init (int level = 0) {
      mCurrState = XERINIT; mStartLevel = mLevel = level;
   }

   /**
    * This method returns the completion status of the decoding process.
    *
    * @return             The completion state of decoding process, as
    *                       OSBOOL. Can be TRUE (completed) or FALSE (not
    *                       completed).
    */
   inline OSBOOL isComplete ()  { return mLevel == mStartLevel; }
   void setTypeName (const char* typeName) {
      mpTypeName = typeName;
   }
} ;


// ASN.1 XER Decode Message Buffer Class
/**
 * The ANS1XERDecodeBuffer class is derived from the ASN1XERMessageBuffer base
 * class. It contains variables and methods specific to decoding ASN.1 XER
 * messages. It is used to manage the input buffer containing an ASN.1 message
 * to be decoded.
 *
 * Note that the XER decode buffer object does not take a message buffer
 * argument because buffer management is handled by the XML parser.
 *

 */
class EXTERNXER ASN1XERDecodeBuffer : public ASN1XERMessageBuffer {
 protected:
   union {
      const char* fileName;
      OSRTInputStreamIF* pInputStream;
      struct {
         const OSOCTET* pMemBuf;
         int            bufSize;
      } memBuf;
   } mInput;
   enum {
      INPUT_FILE,
      INPUT_STREAM,
      INPUT_STREAM_ATTACHED,
      INPUT_MEMORY
   } mInputId;
 public:

   /**
    * The ASN1XERDecodeBuffer class has three overloaded constructors. This
    * version of the ASN1XERDecodeBuffer constructor takes a name of a file
    * that contains the XML data to be decoded, and constructs a buffer. Use
    * getStatus() method to determine has error occured during the
    * initialization or not.
    *
    * @param xmlFile      A pointer to name of file to be decoded.
    */
   ASN1XERDecodeBuffer (const char* xmlFile);

   /**
    * This version of the ASN1XERDecodeBuffer constructor takes parameters
    * describing the message to be decoded, and constructs a buffer describing
    * an encoded ASN.1 message. Use getStatus() method to determine has error
    * occured during the initialization or not.
    *
    * @param msgbuf       A pointer to a buffer containing an encoded ASN.1
    *                       message.
    * @param numocts      Size of the message buffer.
    * @param openType     Open Type decoding indicator. Set this argument as
    *                       TRUE to decode an open type.
    */
   ASN1XERDecodeBuffer (const OSOCTET* msgbuf,
                        size_t numocts,
                        OSBOOL openType = FALSE);

   /**
    * This version of the ASN1XERDecodeBuffer constructor takes a reference to
    * the OSRTInputStream object. Use getStatus() method to determine has error
    * occured during the initialization or not.
    *
    * @param inputStream  reference to the OSRTInputStream object
    * @param openType     Open Type decoding indicator. Set this argument as
    *                       TRUE to decode an open type.
    */
   ASN1XERDecodeBuffer (OSRTInputStreamIF& inputStream,
                        OSBOOL openType = FALSE);

   ~ASN1XERDecodeBuffer ();

   /**
    * This method decodes an XML message associated with this buffer.
    *
    * @return             stat Status of the operation. Possible values are
    *                       0 if successful or one of the negative error
    *                       status codes defined in Appendix A of the C/C++
    *                       runtime Common Functions Reference Manual.
    * @param pReader      Pointer to OSXMLReaderClass object.
    */
   virtual int decodeXML (OSXMLReaderClass* pReader);

   /**
    * This method returns the name of the XML file that is associated with the
    * current buffer.
    *
    * @return             Name of the XML file that is associated with the
    *                       current buffer.
    */
   inline const char* getXmlFileName() {
      return (mInputId == INPUT_FILE) ? mInput.fileName : 0;
   }

   /**
    * This method reinitializes the decode buffer pointer to allow new data to
    * be decoded. This makes it possible to reuse one message buffer object in
    * a loop to decode multiple data messages.
    *
    * @return             Status of the operation. Possible values are 0
    *                       if successful or one of the negative error status
    *                       codes defined in Appendix A of the C/C++ Run-time
    *                       Common Functions Reference Manual.
    * @param membuf       reference to OSMemBuf object.
    */
   virtual int initBuffer (OSRTMEMBUF& membuf) {
      return ASN1XERMessageBuffer::initBuffer (membuf);
   }

   /**
    * This version of the overloaded initBuffer method initializes the message
    * buffer to point at the given null-terminated UTF-8 character string.
    *
    * @param str         Pointer to a null-terminated ASCII character
    *                       string.
    * @return            Completion status of operation:
    *                      - 0 (0) = success,
    *                      - negative return value is error.
    */
   virtual int initBuffer (const OSUTF8CHAR* str);

   /**
    * This version of the overloaded initBuffer method initializes the message
    * buffer to point at the given Unicode string. This is used mainly for XER
    * (XML) message decoding.
    *
    * @param str      Pointer to a Unicode character string.
    * @return            Completion status of operation:
    *                      - 0 (0) = success,
    *                      - negative return value is error.
    */
   virtual int initBuffer (OSUNICHAR* str) { return RTERR_NOTSUPP; }

   virtual OSBOOL isA (int bufferType) {
      return bufferType == XERDecode;
   }

   inline void setOpenType () { rtxCtxtSetFlag (getCtxtPtr(), ASN1OPENTYPE); }
} ;

/**
 * @} xermsgbuffclas
*/

/**
 * @}asn1XerCppTypes
 */


#endif // _ASN1XERCPPTYPES_H_
