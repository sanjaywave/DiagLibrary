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
 * @file OSJSONEncodeBuffer.h
 * JSON encode message buffer class definition.
 */
#ifndef _OSJSONENCODEBUFFER_H_
#define _OSJSONENCODEBUFFER_H_

#include "rtjsonsrc/OSJSONMessageBuffer.h"

/**
 * The OSJSONEncodeBuffer class is derived from the OSJSONMessageBuffer base
 * class. It contains variables and methods specific to encoding JSON messages.
 * It is used to manage the buffer into which a message is to be encoded.
 */
class EXTJSONCLASS OSJSONEncodeBuffer : public OSJSONMessageBuffer {
 protected:
   OSJSONEncodeBuffer (OSRTContext* pContext) :
      OSJSONMessageBuffer (JSONEncode, pContext) {}

 public:
   /**
    * Default constructor
    */
   EXTJSONMETHOD OSJSONEncodeBuffer ();

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
   EXTJSONMETHOD OSJSONEncodeBuffer (OSOCTET* pMsgBuf, size_t msgBufLen);

   /**
    * This method returns the length of a previously encoded JSON message.
    *
    * @return Length of the JSON message encapsulated within this
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
   EXTJSONMETHOD virtual int init ();

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
    * This method adds a null-terminator character ('\0') at the current
    * buffer position.
    */
   inline void nullTerminate () {
      OSRTZTERM (getCtxtPtr());
   }

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
   EXTJSONMETHOD virtual long write (const char* filename);

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
   EXTJSONMETHOD virtual long write (FILE* fp);
} ;

#endif

