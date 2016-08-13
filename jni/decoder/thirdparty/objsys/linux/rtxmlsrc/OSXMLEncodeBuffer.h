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
 * @file OSXMLEncodeBuffer.h
 * XML encode message buffer class definition.
 */
#ifndef _OSXMLENCODEBUFFER_H_
#define _OSXMLENCODEBUFFER_H_

#include "rtxmlsrc/OSXMLMessageBuffer.h"

/**
 * The OSXMLEncodeBuffer class is derived from the OSXMLMessageBuffer base
 * class. It contains variables and methods specific to encoding XML messages.
 * It is used to manage the buffer into which a message is to be encoded.
 */
class EXTXMLCLASS OSXMLEncodeBuffer : public OSXMLMessageBuffer {
 protected:
   OSXMLEncodeBuffer (OSRTContext* pContext) :
      OSXMLMessageBuffer (XMLEncode, pContext) {}
 public:
   /**
    * Default constructor
    */
   EXTXMLMETHOD OSXMLEncodeBuffer ();

   /**
    * This constructor allows a static message buffer to be specified to
    * receive the encoded message.
    *
    * @param pMsgBuf  A pointer to a fixed size message buffer to receive
    *                 the encoded message.
    *
    * @param msgBufLen Size of the fixed-size message buffer.
    *
    */
   EXTXMLMETHOD OSXMLEncodeBuffer (OSOCTET* pMsgBuf, size_t msgBufLen);

   /**
    * This method adds XML header text to the output buffer with the
    * given version number and encoding attributes.
    *
    * @param version    XML version (default is 1.0)
    * @param encoding   Character encoding (default is UTF-8)
    * @param newLine    Add newline char at end of header
    * @return           Zero if success or negative error code.
    */
   int addXMLHeader (const OSUTF8CHAR* version = OSUTF8("1.0"),
                     const OSUTF8CHAR* encoding = OSUTF8(OSXMLHDRUTF8),
                     OSBOOL newLine = TRUE);

   /**
    * This method adds encoded XML text to the encode buffer.  It is
    * assumed that the user has already processed the text to do
    * character escaping, etc..  The text is copied directly to
    * the buffer as-is.
    *
    * @param text       Encoded XML text to be added to the buffer.
    * @return           Zero if success or negative error code.
    */
   EXTXMLMETHOD int addXMLText (const OSUTF8CHAR* text);

   /**
    * This method returns the length of a previously encoded XML message.
    *
    * @return Length of the XML message encapsulated within this
    * buffer object.
    */
   virtual size_t getMsgLen () {
      return getByteIndex();
   }

   /**
    * This method reinitializes the encode buffer to allow a new message
    * to be encoded.  This makes it possible to reuse one message buffer
    * object in a loop to encode multiple messages.  After this method
    * is called, any previously encoded message in the buffer will be
    * overwritten on the next encode call.
    */
   EXTXMLMETHOD virtual int init ();

   /**
    * This is a virtual method that must be overridden by derived classes to
    * allow identification of the class. The base class variant is abstract.
    * This method matches an enumerated identifier defined in the base class.
    * One identifier is declared for each of the derived classes.
    *
    * @param bufferType   Enumerated identifier specifying a derived class.
    *                       This type is defined as a public access type in the
    *                       OSRTMessageBufferIF base interface. Possible values
    *                       include BEREncode, BERDecode, PEREncode, PERDecode,
    *                       XMLEncode, and XMLDecode.
    * @return             Boolean result of the match operation. True if the
    *                       \c bufferType argument is \c XMLEncode.
    *                       argument.
    */
   virtual OSBOOL isA (int bufferType) {
      return bufferType == XMLEncode;
   }

   /**
    * This method adds a null-terminator character ('\0') at the current
    * buffer position.
    */
   inline void nullTerminate () {
      OSRTZTERM (getCtxtPtr());
   }

   /**
    * This method sets a flag indicating that the data is to be
    * encoded as ax XML fragment instead of as a complete XML document
    * (i.e. an XML header will not be added).
    */
   EXTXMLMETHOD void setFragment (OSBOOL value = TRUE);

   /**
    * This method writes the encoded message to the given file.
    *
    * @param  filename  The name of file to which the encoded message will
    *                   be written.
    * @return           Number of octets actually written.  This value may be
    *                   less than the actual  message length if an error
    *                   occurs.
    *
    */
   EXTXMLMETHOD virtual long write (const char* filename);

   /**
    * This version of the write method writes to a file that is specified
    * by a FILE pointer.
    *
    * @param  fp       Pointer to FILE structure to which the encoded
    *                  message will be written.
    * @return           Number of octets actually written.  This value may be
    *                   less than the actual  message length if an error
    *                   occurs.
    *
    */
   EXTXMLMETHOD virtual long write (FILE* fp);
} ;

#endif

