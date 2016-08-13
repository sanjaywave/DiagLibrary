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
 * @file OSXMLEncodeStream.h
 * XML encode stream class definition.
 */
#ifndef _OSXMLENCODESTREAM_H_
#define _OSXMLENCODESTREAM_H_

#include "rtxsrc/OSRTOutputStream.h"
#include "rtxmlsrc/OSXMLMessageBuffer.h"

/**
 * The OSXMLEncodeStream class is derived from the OSXMLMessageBuffer base
 * class. It contains variables and methods specific to streaming encoding
 * XML messages.  It is used to manage the stream into which a message is
 * to be encoded.
 */
class EXTXMLCLASS OSXMLEncodeStream : public OSXMLMessageBuffer {
 protected:
   /** A pointer to an OSRTOutputStream object. */
   OSRTOutputStream* mpStream;

   /** TRUE if the OSXMLEncodeStream object will close and free the stream
    * in the destructor. */
   OSBOOL mbOwnStream;

   /** Internal pointer to the context structure associated with the
    *  stream for making C function calls. */
   OSCTXT* mpCtxt;

 public:
   /**
    * This version of the OSXMLEncodeStream constructor takes a reference
    * to the OSOutputStream object. The stream is assumed to have been
    * previously initialized.
    *
    * @param outputStream  reference to the OSOutputStream object
    */
   EXTXMLMETHOD OSXMLEncodeStream (OSRTOutputStream& outputStream);

   /**
    * This version of the OSXMLEncodeStream constructor takes a pointer
    * to the OSRTOutputStream object. The stream is assumed to have been
    * previously initialized. If ownStream is set to TRUE, then stream
    * will be closed and freed in the destructor.
    *
    * @param pOutputStream  reference to the OSOutputStream object
    * @param ownStream      set ownership for the passed stream object.
    */
   OSXMLEncodeStream (OSRTOutputStream* pOutputStream,
                      OSBOOL ownStream = TRUE);

   ~OSXMLEncodeStream ();

   /**
    * This function encodes an attribute in which the name and value
    * are given as null-terminated UTF-8 strings.
    *
    * @param name         Attribute name.
    * @param value        UTF-8 string value to be encoded.
    * @return             Completion status of operation:
    *                       - 0 = success,
    *                       - negative return value is error.
    */
   EXTXMLMETHOD int encodeAttr
      (const OSUTF8CHAR* name, const OSUTF8CHAR* value);

   /**
    * This method encodes XML textual content.  XML metadata characters
    * such as '<' are escaped. The input value is specified in UTF-8
    * character format but may be transformed if a different character
    * encoding is enabled.
    *
    * @param value        UTF-8 string value to be encoded.
    * @return             Completion status of operation:
    *                       - 0 = success,
    *                       - negative return value is error.
    */
   EXTXMLMETHOD int encodeText (const OSUTF8CHAR* value);

   /**
    * This method ends an XML document by flushing any remaining data
    * to the stream.
    */
   EXTXMLMETHOD int endDocument ();

   /**
    * This method encodes an end element tag value (\</elemName\>).
    *
    * @param elemName     XML element name.
    * @param pNS          XML namespace information (prefix and URI).
    * @return             Completion status of operation:
    *                       - 0 = success,
    *                       - negative return value is error.
    */
   EXTXMLMETHOD int endElement
      (const OSUTF8CHAR* elemName, OSXMLNamespace* pNS = 0);

   /**
    * This method reinitializes the encode stream to allow a new message
    * to be encoded.  This makes it possible to reuse one stream
    * object in a loop to encode multiple messages.
    */
   virtual EXTXMLMETHOD int init ();

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
    * This is a virtual method that must be overridden by derived classes
    * to allow access to the stored message.  The base class
    * implementation returns a null value.
    *
    * @return              A pointer to the stored message.
    */
   virtual const OSOCTET* getMsgPtr () { return 0; }

   /**
    * This method returns the output stream associated with the object.
    *
    * @return              A pointer to the output stream.
    */
   inline OSRTOutputStream* getStream () const { return mpStream; }

   /**
    * This method writes information to start an XML document to the
    * encode stream.  This includes the XML header declaration.
    */
   EXTXMLMETHOD int startDocument ();

   /**
    * This method writes information to start an XML element to the
    * encode stream.  It can leave the element open so that attributes
    * can be added.
    *
    * @param elemName     XML element name.
    * @param pNS          XML namespace information (prefix and URI).  If the
    *                     prefix is NULL, this method will search the context's
    *                     namespace stack for a prefix to use.
    * @param pNSAttrs     List of namespace attributes to be added to element.
    * @param terminate    Add closing '>' character.
    * @return             Completion status of operation:
    *                       - 0 = success,
    *                       - negative return value is error.
    */
   EXTXMLMETHOD int startElement
      (const OSUTF8CHAR* elemName, OSXMLNamespace* pNS = 0,
       OSRTDList* pNSAttrs = 0, OSBOOL terminate = FALSE);

   /**
    * This metod terminates a currently open XML start element by adding
    * either a '>' or '/>' (if empty) terminator.  It will also add XSI
    * attributes to the element.  Note that is important to use this
    * method to terminate the element rather than writng a closing angle
    * bracket text to the stream directly due to the way state is maintained
    * in the context.
    *
    * @return             Completion status of operation:
    *                       - 0 = success,
    *                       - negative return value is error.
    */
   EXTXMLMETHOD int termStartElement ();

} ;

#endif

