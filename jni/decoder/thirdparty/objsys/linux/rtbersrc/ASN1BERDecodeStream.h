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
 * @file ASN1BERDecodeStream.h
 * The C++ definitions for ASN.1 BER input streams.
 */

#ifndef _ASN1BERDECODESTREAM_H_
#define _ASN1BERDECODESTREAM_H_

#include "rtsrc/asn1CppTypes.h"
#include "rtxsrc/OSRTInputStreamIF.h"
#include "rtbersrc/asn1berStream.h"

/** @defgroup iberstrmclas C++ classes for streaming BER decoding.
 * @ingroup bercppruntime
 * These classes are used to perform BER decoding directly from a stream (file,
 * network, memory). @{
 */

/**
 * This class is a base class for other ASN.1 BER input stream's classes. It is
 * derived from the ASN1Stream base class. It contains variables and methods
 * specific to streaming decoding of BER messages. It is used to manage the
 * input stream containing the ASN.1 message to be decoded.
 */
class EXTERNBER ASN1BERDecodeStream : public ASN1MessageBuffer
{
 protected:
   OSRTInputStreamIF* mpStream;
   OSBOOL mbOwnStream;

 public:
   /**
    * A default constructor. Use getStatus() method to determine if
    * an error occured during the initialization or not.
    */
   ASN1BERDecodeStream (OSRTInputStreamIF& is);

   ASN1BERDecodeStream (OSRTInputStreamIF* pis, OSBOOL bOwnStream = TRUE);

   ~ASN1BERDecodeStream ();

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
   virtual OSRTCtxtPtr getContext () {
      return ASN1MessageBuffer::getContext ();
   }

   /**
    * The getCtxtPtr method returns the underlying C runtime context. This
    * context can be used in calls to C runtime functions.
    *
    * @return The pointer to C runtime context.
    */
   virtual OSCTXT* getCtxtPtr () { return ASN1MessageBuffer::getCtxtPtr (); }

   /**
    * Returns error text in a dynamic memory buffer. The buffer will be
    * allocated by 'operator new []'. The calling routine is responsible
    * to free the memory by using 'operator delete []'.
    *
    * @return    A pointer to a newly allocated buffer with error text.
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
    *                       Otherwise, returns newly allocated buffer with
    *                       error text or NULL, if error.
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
    * Returns the current stream position.  This may be used with the
    * <code>setPosition</code> method to reset back to an arbitrary point
    * in the input stream.
    *
    * @param ppos         Pointer to a variable to receive position.
    * @return             Completion status of operation: 0 = success,
    *                       negative return value is error.
    */
   virtual int getPosition (size_t* ppos) {
      return mpStream->getPosition (ppos);
   }

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
    * call to the ASN1BERDecodeStream::reset method repositions this stream at
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
    * from the stream. It may return a value less then this if the maximum
    * number of bytes is not available.
    *
    * @param pDestBuf     Pointer to a buffer to receive a data.
    * @param maxToRead    Size of the buffer.
    * @return             The total number of octets read into the buffer, or
    *                       negative value with error code if any error is
    *                       occurred.
    * @see                ::rtxStreamRead
    */
   virtual long read (OSOCTET* pDestBuf, size_t maxToRead) {
      return mpStream->read (pDestBuf, maxToRead);
   }

   /**
    * Read data from the stream. This method reads \c maxToRead bytes
    * from the stream.  It will block until either the bytes are available
    * or an error occurs.
    *
    * @param pDestBuf     Pointer to a buffer to receive a data.
    * @param toReadBytes  Number of bytes to be read.
    * @return             The total number of octets read into the buffer, or
    *                       negative error code.
    * @see                ::rtxStreamReadBlocking
    */
   virtual long readBlocking (OSOCTET* pDestBuf, size_t toReadBytes) {
      return mpStream->readBlocking (pDestBuf, toReadBytes);
   }

   /**
    * Repositions this stream to the position at the time the mark method was
    * last called on this input stream.
    *
    * @return            Completion status of operation:
    *                      - 0 (0) = success,
    *                      - negative return value is error.
    * @see               ::rtxStreamBufMark, ::rtxStreamBufReset
    */
   int reset () { return mpStream->reset (); }

   /**
    * Sets the current stream position to the given offset.
    *
    * @param pos          Position stream is to be reset to.  This
    *                       is normally obtained via a call to
    *                       <code>getPosition</code>, although in most
    *                       cases it is a zero-based offset.
    * @return             Completion status of operation: 0 = success,
    *                       negative return value is error.
    */
   virtual int setPosition (size_t pos) {
      return mpStream->setPosition (pos);
   }

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
    * Use getStatus() method to determine if an error occured during the
    * operation or not.
    *
    * @param val          A reference to an object to be decoded.
    * @return             reference to this class to perform sequential
    *                       decoding.
    */
   ASN1BERDecodeStream& operator >> (ASN1CType& val);

   /**
    * This method returns the total number of octets (bytes) already decoded
    * from the stream.
    *
    * @return             Number of octets (bytes) already decoded from the
    *                       stream.
    */
   size_t byteIndex ();

   /**
    * This method determines if the decoder has reached the end of a message
    * context block. This method could be called when decoding a SET or
    * SEQUENCE OF/SET OF construct.
    *
    * @param ccb          Reference to a 'context control block' structure.
    *                       This is basically a loop control mechanism to keep
    *                       the variable associated with parsing a nested
    *                       constructed element straight.
    * @return             Boolean value indicating whether or not the
    *                       end-of-context has been reached.
    */
   OSBOOL chkend (ASN1CCB& ccb);

   /**
    * This method decodes a variable of the ASN.1 INTEGER type. In this case,
    * the integer is assumed to be of a larger size than can fit in a C or C++
    * long type (normally 32 or 64 bits).
    *
    * @param pval                      Reference to a pointer to a variable to
    *                                    receive a decoded big integer value.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmBigInt
    */
   int decodeBigInt (const char*& pval, ASN1TagType tagging = ASN1EXPL,
                     int length = 0);

   /**
    * This method decodes a variable of the ASN.1 BIT STRING type into a static
    * memory structure. It is used to decode a sized bit string production.
    *
    * @param pbits                     Pointer to a variable to receive the
    *                                    decoded bit string. This is assumed to
    *                                    be a static array large enough to hold
    *                                    the number of bits specified in the
    *                                    numbits input parameter.
    * @param numbits                   As input parameter it is a reference to
    *                                    an integer variable containing the
    *                                    size (in bits) of the sized ASN.1 bit
    *                                    string. An error will occur if the
    *                                    number of bits in the decoded string
    *                                    is larger than this value. Note that
    *                                    this is also used as an output
    *                                    variable - the actual number of
    *                                    decoded bits will be returned in this
    *                                    variable.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmBitStr
    */
   int decodeBitStr (OSOCTET* pbits, OSUINT32& numbits,
                     ASN1TagType tagging = ASN1EXPL, int length = 0);

   /**
    * This method decodes a variable of the ASN.1 BIT STRING type. It will
    * allocate dynamic memory to store the decoded result.
    *
    * @param val                       Reference to an ASN1DynBitStr variable
    *                                    to receive the decoded bit string.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmDynBitStr
    */
   int decodeBitStr (ASN1DynBitStr& val, ASN1TagType tagging = ASN1EXPL,
                     int length = 0);

   /**
    * This method decodes a variable of the ASN.1 BMPString type.
    *
    * @param val                       Reference to a variable to receive the
    *                                    decoded BMPString value.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmBMPStr
    */
   int decodeBMPStr (Asn116BitCharString& val, ASN1TagType tagging = ASN1EXPL,
                     int length = 0);

   /**
    * This method decodes a variable of the ASN.1 BOOLEAN type.
    *
    * @param val                       Reference to a variable to receive the
    *                                    decoded boolean value.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmBool
    */
   int decodeBool (OSBOOL& val, ASN1TagType tagging = ASN1EXPL,
                   int length = 0);

   /**
    * This method decodes a variable of one of the ASN.1 8-bit character string
    * types. These types include IA5String, VisibleString, PrintableString, and
    * NumericString.
    *
    * @param pval                      Reference to a character string pointer
    *                                    variable to receive the decoded
    *                                    string. The string is stored as a
    *                                    standard null-terminated C string.
    *                                    Memory is allocated for the string by
    *                                    the ::rtxMemAlloc function.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param tag                       The ASN.1 tag to be decoded. This
    *                                    parameter is passed using the ASN1C
    *                                    internal tag representation. It is
    *                                    passed as an unsigned 32-bit integer.
    *                                    This parameter only has meaning if the
    *                                    tagging parameter specifies explicit
    *                                    decoding.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmCharStr
    */
   int decodeCharStr (const char*& pval, ASN1TagType tagging = ASN1EXPL,
                      ASN1TAG tag = 0, int length = 0);

   /**
    * This method decodes a variable of the ASN.1 ENUMERATED type.
    *
    * @param val                       Reference to a variable to receive the
    *                                    decoded enumerated value.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmEnum
    */
   int decodeEnum (OSINT32& val, ASN1TagType tagging = ASN1EXPL,
                   int length = 0);

   /**
    * This method decodes the end-of-contents octets.
    *
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmMatchEOC
    */
   int decodeEoc ();

   /**
    * This method decodes a variable of the ASN.1 INTEGER type.
    *
    * @param val                       Reference to a variable to receive a
    *                                    decoded 32-bit integer value.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmInt
    */
   int decodeInt (OSINT32& val, ASN1TagType tagging = ASN1EXPL,
                  int length = 0);

   /**
    * This method decodes an 8-bit variable of the ASN.1 INTEGER type.
    *
    * @param val                       Reference to a variable to receive a
    *                                    decoded 8-bit integer value.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmInt8
    */
   int decodeInt8 (OSINT8& val, ASN1TagType tagging = ASN1EXPL,
                   int length = 0);

   /**
    * This method decodes a 16-bit variable of the ASN.1 INTEGER type.
    *
    * @param val                       Reference to a variable to receive a
    *                                    decoded 16-bit integer value.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmInt16
    */
   int decodeInt16 (OSINT16& val, ASN1TagType tagging = ASN1EXPL,
                    int length = 0);

   /**
    * This method decodes a 64-bit variable of the ASN.1 INTEGER type.
    *
    * @param val                       Reference to a variable to receive a
    *                                    decoded 64-bit integer value.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmInt64
    */
   int decodeInt64 (OSINT64& val, ASN1TagType tagging = ASN1EXPL,
                    int length = 0);

   /**
    * This method decodes a BER length determinant value.
    *
    * @param length                    Reference to a variable to receive the
    *                                    decoded length of the tagged
    *                                    component. The returned value will
    *                                    either be the actual length or the
    *                                    special constant 'ASN_K_INDEFLEN',
    *                                    which indicates indefinite length.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmLength
    */
   int decodeLength (OSINT32& length);

   /**
    * This method decodes a variable of the ASN.1 NULL type.
    *
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmNull
    */
   int decodeNull (ASN1TagType tagging = ASN1EXPL);

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
    * This method decodes a variable of the ASN.1 OBJECT IDENTIFIER type.
    *
    * @param val                       Reference to a variable to receive the
    *                                    decoded OBJECT IDENTIFIER value.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmObjId
    */
   int decodeObjId (ASN1OBJID& val, ASN1TagType tagging = ASN1EXPL,
                    int length = 0);

   /**
    * This method decodes a variable of the ASN.1 OBJECT IDENTIFIER type using
    * 64-bit subidentifiers..
    *
    * @param val                       Reference to a variable to receive the
    *                                    decoded 64-bit OBJECT IDENTIFIER
    *                                    value.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmObjId64
    */
   int decodeObjId64 (ASN1OID64& val, ASN1TagType tagging = ASN1EXPL,
                      int length = 0);

   /**
    * This method decodes a variable of the ASN.1 OCTET STRING type into a
    * static memory structure. It is used to decode a sized octet string
    * production.
    *
    * @param pocts                     Pointer to a variable to receive the
    *                                    decoded octet string. This is assumed
    *                                    to be a static array large enough to
    *                                    hold the number of octets specified in
    *                                    the numocts input parameter.
    * @param numocts                   Reference to an integer variable
    *                                    containing the size (in octets) of the
    *                                    sized ASN.1 octet string. An error
    *                                    will occur if the number of octets in
    *                                    the decoded string is larger than this
    *                                    value. Note that this is also used as
    *                                    an output variable - the actual number
    *                                    of decoded octets will be returned in
    *                                    this variable.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmOctStr
    */
   int decodeOctStr (OSOCTET* pocts, OSUINT32& numocts,
                     ASN1TagType tagging = ASN1EXPL, int length = 0);

   /**
    * This method decodes a variable of the ASN.1 OCTET STRING type. It will
    * allocate dynamic memory to store the decoded result.
    *
    * @param val                       Reference to an ASN1DynOctStr variable
    *                                    to receive the decoded octet string.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmDynOctStr
    */
   int decodeOctStr (ASN1DynOctStr& val, ASN1TagType tagging = ASN1EXPL,
                     int length = 0);

   /**
    * This method decodes an ASN.1 open type value.  This is a value of
    * any ASN.1 data type.  It may be constructed and contain multiple
    * elements including elements encoded with indefinite lengths.
    * This method will allocate dynamic memory to store the decoded result.
    *
    * @param val                       Reference to an ASN1OpenType variable
    *                                    to receive the decoded open type data.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmDynOctStr
    */
   int decodeOpenType (ASN1OpenType& val);

   /**
    * This method decodes a variable of the ASN.1 REAL type.
    *
    * @param val                       Reference to a variable to receive the
    *                                    decoded REAL value.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmReal
    */
   int decodeReal (OSREAL& val, ASN1TagType tagging = ASN1EXPL,
                   int length = 0);

   /**
    * This method decodes a variable of the ASN.1 RELATIVE-OID type.
    *
    * @param val                       Reference to a variable to receive the
    *                                    decoded RELATIVE-OID value.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmRelativeOID
    */
   int decodeRelativeOID (ASN1OBJID& val, ASN1TagType tagging = ASN1EXPL,
                          int length = 0);

   /**
    * This method decodes the tag at the current decode pointer location and
    * returns the results.
    *
    * @param tag                       Reference to a variable to receive the
    *                                    decoded ASN.1 tag value.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmTag
    */
   int decodeTag (ASN1TAG& tag);

   /**
    * This method decodes the tag and length at the current decode pointer
    * location and returns the results.
    *
    * @param tag                       Reference to a variable to receive the
    *                                    decoded ASN.1 tag value.
    * @param len                       Reference to a variable to receive the
    *                                    decoded length of the tagged
    *                                    component. The returned value will
    *                                    either be the actual length or the
    *                                    special constant 'ASN_K_INDEFLEN',
    *                                    which indicates indefinite length.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmTagAndLen
    */
   int decodeTagAndLen (ASN1TAG& tag, OSINT32& len);

   /**
    * This method decodes a variable of the unsigned variant of ASN.1 INTEGER
    * type.
    *
    * @param val                       Reference to a variable to receive a
    *                                    decoded unsigned 32-bit integer value.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmUInt
    */
   int decodeUInt (OSUINT32& val, ASN1TagType tagging = ASN1EXPL,
                   int length = 0);

   /**
    * This method decodes an 8-bit variable of the unsigned variant of ASN.1
    * INTEGER type.
    *
    * @param val                       Reference to a variable to receive a
    *                                    decoded unsigned 8-bit integer value.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmUInt8
    */
   int decodeUInt8 (OSUINT8& val, ASN1TagType tagging = ASN1EXPL,
                    int length = 0);

   /**
    * This method decodes a 16-bit variable of the unsigned variant of ASN.1
    * INTEGER type.
    *
    * @param val                       Reference to a variable to receive a
    *                                    decoded unsigned 16-bit integer value.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmUInt16
    */
   int decodeUInt16 (OSUINT16& val, ASN1TagType tagging = ASN1EXPL,
                     int length = 0);

   /**
    * This method decodes a 64-bit variable of the unsigned variant of ASN.1
    * INTEGER type.
    *
    * @param val                       Reference to a variable to receive a
    *                                    decoded unsigned 64-bit integer value.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmUInt64
    */
   int decodeUInt64 (OSUINT64& val, ASN1TagType tagging = ASN1EXPL,
                     int length = 0);

   /**
    * This method decodes a variable of the ASN.1 UniversalString type.
    *
    * @param val                       Reference to a variable to receive the
    *                                    decoded UniversalString value.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param length                    The length, in octets, of the contents
    *                                    field to be decoded. This parameter
    *                                    only has meaning if the tagging
    *                                    parameter specifies implicit decoding.
    *                                    If explicit, the length is obtained
    *                                    from the decoded length field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmUnivStr
    */
   int decodeUnivStr (Asn132BitCharString& val,
                      ASN1TagType tagging = ASN1EXPL,
                      int length = 0);

   /**
    * Get the total length of a tag-length-value (TLV) component.
    * This is not the length stored in the L field.  It is the total
    * length of the component which is equal to the parsed length plus
    * the number of bytes in the tag and length fields.
    *
    * @return             The total number of octets in the TLV or a
    *                       negative error code.
    * @see                ::berDecStrmGetTLVLength
    */
   int getTLVLength ();

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

   /**
    * This method "peeks" the tag and length at the current decode pointer
    * location and returns the results. The decode pointer location is left as
    * it was before call to this function.
    *
    * @param tag                       Reference to a variable to receive the
    *                                    decoded ASN.1 tag value.
    * @param len                       Reference to a variable to receive the
    *                                    decoded length of the tagged
    *                                    component. The returned value will
    *                                    either be the actual length or the
    *                                    special constant 'ASN_K_INDEFLEN',
    *                                    which indicates indefinite length.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berDecStrmTagAndLen
    */
   int peekTagAndLen (ASN1TAG& tag, int& len);

   /**
    * Read a complete tag-length-value (TLV) from the decode stream into
    * the given memory buffer.
    *
    * @param pDestBuf     Pointer to a buffer to receive a data.
    * @param bufsiz       Size of the buffer.
    * @return             The total number of octets read into the buffer, or
    *                       negative error code.
    * @see                ::berDecStrmReadTLV
    */
   int readTLV (OSOCTET* pDestBuf, size_t bufsiz);

} ;

/** @} */

#endif /* _ASN1BERDECODESTREAM_H_ */

