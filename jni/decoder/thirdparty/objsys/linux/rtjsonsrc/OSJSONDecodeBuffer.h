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
 * @file OSJSONDecodeBuffer.h
 * JSON decode buffer or stream class definition.
 */
#ifndef _OSJSONDECODEBUFFER_H_
#define _OSJSONDECODEBUFFER_H_

#include "rtxsrc/OSRTInputStream.h"
#include "rtjsonsrc/OSJSONMessageBuffer.h"

/**
 * The OSJSONDecodeBuffer class is derived from the OSJSONMessageBuffer base
 * class. It contains variables and methods specific to decoding JSON
 * messages. It is used to manage an input buffer or stream containing a
 * message to be decoded.
 */
class EXTJSONCLASS OSJSONDecodeBuffer : public OSJSONMessageBuffer {
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
    * This version of the OSJSONDecodeBuffer constructor takes a name of a
    * file that contains JSON data to be decoded and constructs a buffer.
    *
    * @param jsonFile  A pointer to name of file to be decoded.
    */
   OSJSONDecodeBuffer (const char* jsonFile);

   /**
    * This version of the OSJSONDecodeBuffer constructor takes parameters
    * describing a message in memory to be decoded and constructs a buffer.
    *
    * @param msgbuf    A pointer to a buffer containing an JSON message.
    * @param bufsiz    Size of the message buffer.
    */
   OSJSONDecodeBuffer (const OSOCTET* msgbuf, size_t bufsiz);

   /**
    * This version of the OSJSONDecodeBuffer constructor takes a reference
    * to the OSInputStream object.  The stream is assumed to have been
    * previuously initialized to point at an encoded JSON message.
    *
    * @param inputStream  reference to the OSInputStream object
    */
   OSJSONDecodeBuffer (OSRTInputStream& inputStream);

   ~OSJSONDecodeBuffer ();

   /**
    * This method initializes the decode message buffer.
    *
    * @return            Completion status of operation:
    *                      - 0 (0) = success,
    *                      - negative return value is error.
    */
   EXTJSONMETHOD virtual int init ();

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
    *                     JSONEncode, and JSONDecode.
    *
    * @return             Boolean result of the match operation. True if the
    *                     \c bufferType argument is \c JSONDecode.
    *                     argument.
    */
   virtual OSBOOL isA (int bufferType) {
      return bufferType == JSONDecode;
   }

} ;

#endif

