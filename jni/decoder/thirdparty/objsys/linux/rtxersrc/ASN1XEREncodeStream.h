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

/**
 * @file ASN1XEREncodeStream.h
 * The C++ definitions for ASN.1 XER output streams.
 */

#ifndef _ASN1XERENCODESTREAM_H_
#define _ASN1XERENCODESTREAM_H_

#include "rtsrc/asn1CppTypes.h"
#include "rtxsrc/OSRTOutputStreamIF.h"
#include "rtxersrc/asn1XerCppTypes.h"

/** @defgroup oxerstrmclas C++ classes for streaming XER encoding.
 * @ingroup xercppruntime
 * These classes are used to perform XER encoding directly to a stream (file,
 * network, memory). @{
 */

/**
 * This class is a base class for other ASN.1 XER output stream's classes. It
 * is derived from the ASN1Stream base class. It contains variables and methods
 * specific to streaming encoding of XER messages.
 */
class EXTERNXER ASN1XEREncodeStream : public ASN1XEREncodeBuffer
{
 protected:
   OSRTOutputStreamIF* mpStream;
   OSBOOL mbOwnStream;
 public:
   /**
    * A default constructor. Use getStatus() method to determine
    * has error occured during the initialization or not.
    */
   ASN1XEREncodeStream (OSRTOutputStreamIF& os, OSBOOL canonical = FALSE);

   ASN1XEREncodeStream (OSRTOutputStreamIF* pos, OSBOOL bOwnStream = TRUE,
      OSBOOL canonical = FALSE);

   ~ASN1XEREncodeStream ();

   /**
    * Returns a pointer to application-specific information block
    */
   virtual void* getAppInfo () { return ASN1MessageBuffer::getAppInfo (); }

   /**
    * The getContext method returns the underlying context smart-pointer
    * object.
    *
    * @return Context smart pointer object.
    */

   virtual OSRTCtxtPtr getContext () { return ASN1MessageBuffer::getContext (); }

   /**
    * The getCtxtPtr method returns the underlying C runtime context. This
    * context can be used in calls to C runtime functions.
    *
    * @return The pointer to C runtime context.
    */
   virtual OSCTXT* getCtxtPtr () { return ASN1MessageBuffer::getCtxtPtr (); }

   /**
    * Returns error text in a dynamic memory buffer. Buffer will be allocated by
    * 'operator new []'. The calling routine is responsible to free the memory
    * by using 'operator delete []'.
    *
    * @return             A pointer to a newly allocated buffer with error text.
    */
   virtual char* getErrorInfo () { return ASN1MessageBuffer::getErrorInfo (); }

   /**
    * Returns error text in a memory buffer. If buffer pointer is specified in
    * parameters (not NULL) then error text will be copied in the passed
    * buffer. Otherwise, this method allocates memory using the
    * 'operator new []' function. The calling routine is responsible to free
    * the memory by using 'operator delete []'.
    *
    * @param pBuf         A pointer to a destination buffer to obtain the error
    *                       text. If NULL, dynamic buffer will be allocated.
    * @param bufSize      A reference to buffer size. If pBuf is NULL it will
    *                       receive the size of allocated dynamic buffer.
    * @return             A pointer to a buffer with error text. If pBuf is not
    *                       NULL, the return pointer will be equal to it.
    *                       Otherwise, returns newly allocated buffer with error
    *                       text. NULL, if error occured.
    */
   virtual char* getErrorInfo (char* pBuf, size_t& bufSize) {
      return ASN1MessageBuffer::getErrorInfo (pBuf, bufSize);
   }

   /**
    * This method returns the completion status of previous operation.
    * It can be used to check completion status of constructors or methods,
    * which do not return completion status. If error occurs, use
    * printErrorInfo method to print out the error's description and
    * stack trace.
    * Method resetError can be used to reset error to continue operations
    * after recovering from the error.
    *
    * @return            Runtime status code:
    *                      - 0 (0) = success,
    *                      - negative return value is error.
    */
   virtual int getStatus () const { return ASN1MessageBuffer::getStatus (); }

   /**
    * The printErrorInfo method prints information on errors contained within
    * the context.
    */
   virtual void printErrorInfo () { ASN1MessageBuffer::printErrorInfo (); }

   /**
    * The resetErrorInfo method resets information on errors contained within
    * the context.
    */
   virtual void resetErrorInfo () { ASN1MessageBuffer::resetErrorInfo (); }

   /**
    * Sets the application-specific information block.
    */
   virtual void setAppInfo (void* pAppInfo) {
      ASN1MessageBuffer::setAppInfo (pAppInfo);
   }

   /**
    * The setDiag method will turn diagnostic tracing on or off.
    *
    * @param value - Boolean value (default = TRUE = on)
    */
   virtual void setDiag (OSBOOL value=TRUE) {
      ASN1MessageBuffer::setDiag (value);
   }


   /**
    * Closes the input or output stream and releases any system resources
    * associated with the stream. For output streams this function also flushes
    * all internal buffers to the stream.
    *
    * @return            Completion status of operation:
    *                      - 0 = success,
    *                      - negative return value is error.
    * @see       ::rtxStreamClose
    */
   virtual int close () { return mpStream->close (); }

   /**
    * Flushes the buffered data to the stream.
    *
    * @return            Completion status of operation:
    *                      - 0 = success,
    *                      - negative return value is error.
    * @see       ::rtxStreamFlush
    */
   virtual int flush () { return mpStream->flush (); }

   /**
    * Checks, is the stream opened or not.
    * @returns TRUE, if the stream is opened, FALSE otherwise.
    * @see       ::rtxStreamIsOpened
    */
   virtual OSBOOL isOpened () { return mpStream->isOpened (); }

   /**
    * Write data to the stream.  This method writes the given number of
    * octets from the given array to the output stream.
    *
    * @param pdata       Pointer to the data to be written.
    * @param size        The number of octets to write.
    * @return            The total number of octets written into the stream, or
    *                       negative value with error code if any error is
    *                       occurred.
    * @see               ::rtxStreamWrite
    */
   virtual long write (const OSOCTET* pdata, size_t size) {
      return mpStream->write (pdata, size);
   }

   /**
    * Encodes an ASN.1 constructed object to the stream.
    * Use getStatus() method to determine has error occured during the
    * operation or not.
    *
    * @param val          A reference to an object to be encoded.
    * @return             reference to this class to perform sequential
    *                       encoding.
    */
   ASN1XEREncodeStream& operator << (ASN1CType& val);

   /**
    * This method encodes an ASN.1 constructed object to the stream.
    *
    * @param val          A reference to an object to be encoded.
    * @return             Completion status of operation:
    *                       - 0 (0) = success,
    *                       - negative return value is error.
    */
   int encodeObj (ASN1CType& val);

   /**
    * This method matches an enumerated identifier defined in the base class.
    * One identifier is declared for each of the derived classes.
    *
    * @param bufferType   Enumerated identifier specifying a derived class.
    *                       This type is defined as a public access type in the
    *                       ASN1MessageBufferIF base interface. Possible values
    *                         are: BEREncode, BERDecode, PEREncode, PERDecode,
    *                         XEREncode, XERDecode, XMLEncode, XMLDecode,
    *                         Stream.
    * @return             Boolean result of the match operation. True if this
    *                       is the class corresponding to the identifier
    *                       argument.
    */
   OSBOOL isA (int bufferType);
} ;

/** @} */
#endif /* _ASN1XERENCODESTREAM_H_ */

