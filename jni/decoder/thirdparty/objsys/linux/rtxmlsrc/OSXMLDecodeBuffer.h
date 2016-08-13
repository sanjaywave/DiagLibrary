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
 * @file OSXMLDecodeBuffer.h
 * XML decode buffer or stream class definition.
 */
#ifndef _OSXMLDECODEBUFFER_H_
#define _OSXMLDECODEBUFFER_H_

#include "rtxsrc/OSRTInputStream.h"
#include "rtxmlsrc/OSXMLMessageBuffer.h"
#include "rtxmlsrc/rtSaxCppParserIF.h"

/**
 * The OSXMLDecodeBuffer class is derived from the OSXMLMessageBuffer base
 * class. It contains variables and methods specific to decoding XML
 * messages. It is used to manage an input buffer or stream containing a
 * message to be decoded.
 *
 * Note that the XML decode buffer object does not take a message
 * buffer argument because buffer management is handled by the XML parser.
 *
*/
class EXTXMLCLASS OSXMLDecodeBuffer : public OSXMLMessageBuffer {
 protected:
   /**
    * Input source for message to be decoded.
    */
   OSRTInputStream* mpInputStream;

   /**
    * This is set to true if this object creates the underlying stream
    * object.  In this case, the stream will be deleted in the object's
    * destructor.
    */
   OSBOOL mbOwnStream;

 public:
   /**
    * This version of the OSXMLDecodeBuffer constructor takes a name of a
    * file that contains XML data to be decoded and constructs a buffer.
    *
    * @param xmlFile  A pointer to name of file to be decoded.
    */
   OSXMLDecodeBuffer (const char* xmlFile);

   /**
    * This version of the OSXMLDecodeBuffer constructor takes parameters
    * describing a message in memory to be decoded and constructs a buffer.
    *
    * @param msgbuf    A pointer to a buffer containing an XML message.
    * @param bufsiz    Size of the message buffer.
    */
   OSXMLDecodeBuffer (const OSOCTET* msgbuf, size_t bufsiz);

   /**
    * This version of the OSXMLDecodeBuffer constructor takes a reference
    * to the OSInputStream object.  The stream is assumed to have been
    * previuously initialized to point at an encoded XML message.
    *
    * @param inputStream  reference to the OSInputStream object
    */
   OSXMLDecodeBuffer (OSRTInputStream& inputStream);

   ~OSXMLDecodeBuffer ();

   /**
    * This method decodes an XML message associated with this buffer.
    *
    * @return stat             Status of the operation.  Possible values
    *                          are 0 if successful or one of the negative
    *                          error status codes defined in Appendix A of the
    *                          C/C++ runtime Common Functions Reference Manual.
    * @param pReader          Pointer to OSXMLReaderClass object.
    *
    * @return Completion status.
    */
   EXTXMLMETHOD int decodeXML (OSXMLReaderClass* pReader);

   /**
    * This method initializes the decode message buffer.
    *
    * @return            Completion status of operation:
    *                      - 0 (0) = success,
    *                      - negative return value is error.
    */
   EXTXMLMETHOD virtual int init ();

   /**
    * This method parses the initial tag from an XML message.  If the
    * tag is a QName, only the local part of the name is returned.
    *
    * @param ppName        Pointer to a pointer to receive decoded UTF-8
    *                       string. Dynamic memory is allocated
    *                       for the variable using the rtxMemAlloc function.
    * @return             Completion status of operation:
    *                       - 0 = success,
    *                       - negative return value is error.
    */
   EXTXMLMETHOD int parseElementName (OSUTF8CHAR** ppName);

   /**
    * This method parses the initial tag from an XML message.
    *
    * @param pQName        Pointer to a QName structure to receive parsed
    *                       name prefix and local name. Dynamic memory is
    *                       allocated for both name parts using the
    *                       rtxMemAlloc function.
    * @return             Completion status of operation:
    *                       - 0 = success,
    *                       - negative return value is error.
    */
   EXTXMLMETHOD int parseElemQName (OSXMLQName* pQName);

   /**
    * This method sets the maximum number of errors returned by the
    * SAX parser.
    *
    * @param   maxErrors   The desired number of maximum errors.
    *
    * @return              The previously set maximum number of errors. */
   EXTXMLMETHOD OSUINT32 setMaxErrors (OSUINT32 maxErrors);

   /**
    * This is a virtual method that must be overridden by derived classes to
    * allow identification of the class. The base class variant is abstract.
    * This method matches an enumerated identifier defined in the base class.
    * One identifier is declared for each of the derived classes.
    *
    * @param bufferType   Enumerated identifier specifying a derived class.
    *                     This type is defined as a public access type in the
    *                     OSRTMessageBufferIF base interface. Possible values
    *                     include BEREncode, BERDecode, PEREncode, PERDecode,
    *                     XMLEncode, and XMLDecode.
    *
    * @return             Boolean result of the match operation. True if the
    *                     \c bufferType argument is \c XMLDecode.
    *                     argument.
    */
   virtual OSBOOL isA (int bufferType) {
      return bufferType == XMLDecode;
   }

} ;

#endif

