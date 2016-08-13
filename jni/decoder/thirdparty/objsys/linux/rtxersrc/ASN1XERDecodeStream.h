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
 * @file ASN1XERDecodeStream.h
 * The C++ definitions for ASN.1 XER input streams.
 */

#ifndef _ASN1XERDECODESTREAM_H_
#define _ASN1XERDECODESTREAM_H_

#include "rtsrc/asn1CppTypes.h"
#include "rtxsrc/OSRTInputStreamIF.h"
#include "rtxersrc/asn1XerCppTypes.h"

/** @defgroup ixerstrmclas C++ classes for streaming XER decoding.
 * @ingroup xercppruntime
 * These classes are used to perform XER decoding directly from a stream (file,
 * network, memory). @{
 */

/**
 * This class is a base class for other ASN.1 XER input stream's classes. It is
 * derived from the ASN1Stream base class. It contains variables and methods
 * specific to streaming decoding of XER messages. It is used to manage the
 * input stream containing the ASN.1 message to be decoded.
 */
class EXTERNXER ASN1XERDecodeStream : public ASN1XERDecodeBuffer
{
 protected:
   OSRTInputStreamIF* mpStream;
   OSBOOL mbOwnStream;

 public:
   /**
    * A default constructor. Use getStatus() method to determine
    * has error occured during the initialization or not.
    */
   ASN1XERDecodeStream (OSRTInputStreamIF& is, OSBOOL openType = FALSE);

   ASN1XERDecodeStream (OSRTInputStreamIF* pis, OSBOOL bOwnStream = TRUE,
      OSBOOL openType = FALSE);

   ~ASN1XERDecodeStream ();

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
    * This method returns the current position in the stream (in octets).
    *
    * @return             The number of octets already read from the stream.
    */
   virtual size_t currentPos () { return mpStream->currentPos (); }

   /**
    * Tests if this input stream supports the mark and reset methods. Whether
    * or not mark and reset are supported is an invariant property of a
    * particular input stream instance. By default, it returns FALSE.
    *
    * @return             TRUE if this stream instance supports the mark and
    *                       reset methods; FALSE otherwise.
    * @see               ::rtxStreamIsMarkSupported
    */
   virtual OSBOOL markSupported () {
      return TRUE;
   }

   /**
    * This method marks the current position in this input stream. A subsequent
    * call to the ASN1XERDecodeStream::reset function repositions this stream at
    * the last marked position so that subsequent reads re-read the same bytes.
    * The readAheadLimit argument tells this input stream to allow that many
    * bytes to be read before the mark position gets invalidated.
    *
    * @param readAheadLimit            the maximum limit of bytes that can be
    *                                    read before the mark position becomes
    *                                    invalid.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::rtxStreamBufMark, ::rtxStreamBufReset
    */
   int mark (size_t readAheadLimit) {
      return mpStream->mark (readAheadLimit);
   }

   /**
    * Read data from the stream. This method reads up to \c maxToRead bytes
    * from the stream. It may return a value less then this if the mamimum
    * number of bytes is not available.
    *
    * @param pDestBuf                 Pointer to a buffer to receive a data.
    * @param maxToRead                Size of the buffer.
    * @return             The total number of octets read into the buffer, or
    *                       negative value with error code if any error is
    *                       occurred.
    * @see                           ::rtxStreamRead
    */
   virtual long read (OSOCTET* pDestBuf, size_t maxToRead) {
      return mpStream->read (pDestBuf, maxToRead);
   }


   /**
    * Read data from the stream. This method reads up to \c maxToRead bytes
    * from the stream. It may return a value less then this if the mamimum
    * number of bytes is not available.
    *
    * @param pDestBuf                 Pointer to a buffer to receive a data.
    * @param toReadBytes              Number of bytes to be read.
    * @return             The total number of octets read into the buffer, or
    *                       negative value with error code if any error is
    *                       occurred.
    * @see                           ::rtxStreamRead
    */
   virtual long readBlocking (OSOCTET* pDestBuf, size_t toReadBytes) {
      return mpStream->readBlocking (pDestBuf, toReadBytes);
   }

   /**
    * Repositions this stream to the position at the time the mark method was
    * last called on this input stream.
    *
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::rtxStreamBufMark, ::rtxStreamBufReset
    */
   int reset () { return mpStream->reset (); }

   /**
    * Skips over and discards the specified amount of data octets from this
    * input stream.
    *
    * @param n           The number of octets to be skipped.
    * @return            Completion status of operation:
    *                      - 0 = success,
    *                      - negative return value is error.
    * @see                           ::rtxStreamSkip
    */
   virtual int skip (size_t n) { return mpStream->skip (n); }


   /**
    * Decodes an ASN.1 constructed object from the stream.
    * Use getStatus() method to determine has error occured during the
    * operation or not.
    *
    * @param val          A reference to an object to be decoded.
    * @return             reference to this class to perform sequential
    *                       decoding.
    */
   ASN1XERDecodeStream& operator >> (ASN1CType& val);

   /**
    * This method decodes an ASN.1 constructed object from the stream.
    *
    * @param val                       A reference to an object to be decoded.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    */
   int decodeObj (ASN1CType& val);

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

#endif /* _ASN1XERDECODESTREAM_H_ */

