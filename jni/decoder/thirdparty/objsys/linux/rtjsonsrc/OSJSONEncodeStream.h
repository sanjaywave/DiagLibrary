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
 * @file OSJSONEncodeStream.h
 * JSON encode stream class definition.
 */
#ifndef _OSJSONENCODESTREAM_H_
#define _OSJSONENCODESTREAM_H_

#include "rtxsrc/OSRTOutputStream.h"
#include "rtjsonsrc/OSJSONMessageBuffer.h"

/**
 * The OSJSONEncodeStream class is derived from the OSJSONMessageBuffer base
 * class. It contains variables and methods specific to streaming encoding
 * JSON messages.  It is used to manage the stream into which a message is
 * to be encoded.
 */
class EXTJSONCLASS OSJSONEncodeStream : public OSJSONMessageBuffer {
 protected:
   /** A pointer to an OSRTOutputStream object. */
   OSRTOutputStream* mpStream;

   /** TRUE if the OSJSONEncodeStream object will close and free the stream
    * in the destructor. */
   OSBOOL mbOwnStream;

   /** Internal pointer to the context structure associated with the
    *  stream for making C function calls. */
   OSCTXT* mpCtxt;

 public:
   /**
    * This version of the OSJSONEncodeStream constructor takes a reference
    * to the OSOutputStream object. The stream is assumed to have been
    * previously initialized.
    *
    * @param outputStream  reference to the OSOutputStream object
    */
   EXTJSONMETHOD OSJSONEncodeStream (OSRTOutputStream& outputStream);

   /**
    * This version of the OSJSONEncodeStream constructor takes a pointer
    * to the OSRTOutputStream object. The stream is assumed to have been
    * previously initialized. If ownStream is set to TRUE, then stream
    * will be closed and freed in the destructor.
    *
    * @param pOutputStream  reference to the OSOutputStream object
    * @param ownStream      set ownership for the passed stream object.
    */
   OSJSONEncodeStream (OSRTOutputStream* pOutputStream,
                      OSBOOL ownStream = TRUE);

   ~OSJSONEncodeStream ();

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
   EXTJSONMETHOD int encodeAttr
      (const OSUTF8CHAR* name, const OSUTF8CHAR* value);

   /**
    * This method encodes JSON textual content.  JSON metadata characters
    * are escaped. The input value is specified in UTF-8
    * character format.
    *
    * @param value        UTF-8 string value to be encoded.
    * @return             Completion status of operation:
    *                       - 0 = success,
    *                       - negative return value is error.
    */
   EXTJSONMETHOD int encodeText (const OSUTF8CHAR* value);

   /**
    * This method reinitializes the encode stream to allow a new message
    * to be encoded.  This makes it possible to reuse one stream
    * object in a loop to encode multiple messages.
    */
   virtual EXTJSONMETHOD int init ();

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
    *                       JSONEncode, and JSONDecode.
    * @return             Boolean result of the match operation. True if the
    *                       \c bufferType argument is \c JSONEncode.
    *                       argument.
    */
   virtual OSBOOL isA (int bufferType) {
      return bufferType == JSONEncode;
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

} ;

#endif

