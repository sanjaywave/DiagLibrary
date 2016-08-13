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
 * @file ASN1BEREncodeStream.h
 * The C++ definitions for ASN.1 BER output streams.
 */

#ifndef _ASN1BERENCODESTREAM_H_
#define _ASN1BERENCODESTREAM_H_

#include "rtsrc/asn1CppTypes.h"
#include "rtxsrc/OSRTOutputStreamIF.h"
#include "rtbersrc/asn1berStream.h"

/** @defgroup oberstrmclas C++ classes for streaming BER encoding.
 * @ingroup bercppruntime
 * These classes are used to perform BER encoding directly to a stream (file,
 * network, memory). @{
 */

/**
 * This class is a base class for other ASN.1 BER output stream's classes. It
 * is derived from the ASN1Stream base class. It contains variables and methods
 * specific to streaming encoding of BER messages.
 */
class EXTERNBER ASN1BEREncodeStream : public ASN1MessageBuffer
{
 protected:
   OSRTOutputStreamIF* mpStream;
   OSBOOL mbOwnStream;
 public:
   /**
    * A default constructor. Use getStatus() method to determine
    * has error occured during the initialization or not.
    */
   ASN1BEREncodeStream (OSRTOutputStreamIF& os);

   ASN1BEREncodeStream (OSRTOutputStreamIF* pos, OSBOOL bOwnStream = TRUE);

   ~ASN1BEREncodeStream ();

   /**
    * Returns a pointer to application-specific information block
    */
   virtual void* getAppInfo() { return ASN1MessageBuffer::getAppInfo (); }

   /**
    * The getContext method returns the underlying context smart-pointer
    * object.
    *
    * @return Context smart pointer object.
    */

   virtual OSRTCtxtPtr getContext() { return ASN1MessageBuffer::getContext(); }

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
   ASN1BEREncodeStream& operator << (ASN1CType& val);



   /**
    * This method encodes a variable of the ASN.1 BMPString type that is based
    * on a 16-bit character sets.
    *
    * @param val                       A reference to a structure representing
    *                                    a 16-bit character string to be
    *                                    encoded. This structure contains a
    *                                    character count element and a pointer
    *                                    to an array of 16-bit character
    *                                    elements represented as 16-bit short
    *                                    integers.
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
    * @see                            ::berEncStrmBMPStr
    */
   int encodeBMPStr
      (const Asn116BitCharString& val, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes a variable of the ASN.1 INTEGER type. In this case,
    * the integer is assumed to be of a larger size than can fit in a C or C++
    * long type (normally 32 or 64 bits).
    *
    * @param *pval                     A pointer to a character string
    *                                    containing the value to be encoded.
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
    * @see                            ::berEncStrmBigInt
    */
   int encodeBigInt (const char *pval, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes a variable of the ASN.1 INTEGER type. In this case,
    * the integer is assumed to be of a larger size than can fit in a C or C++
    * long type (normally 32 or 64 bits).
    *
    * @param *pval                     A pointer to a character string
    *                                    containing the value to be encoded.
    * @param nchars                    Number of characters from pval to encode.
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
    * @see                            ::berEncStrmBigInt
    */
   int encodeBigIntNchars
      (const char *pval, size_t nchars, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes a variable of the ASN.1 BIT STRING type.
    *
    * @param pbits                     A pointer to an OCTET string containing
    *                                    the bit data to be encoded. This
    *                                    string contains bytes having the
    *                                    actual bit settings as they are to be
    *                                    encoded in the message.
    * @param numbits                   The number of bits within the bit string
    *                                    to be encoded.
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
    * @see                            ::berEncStrmBitStr
    */
   int encodeBitStr (const OSOCTET* pbits, OSUINT32 numbits,
                      ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes a variable of the ASN.1 BIT STRING type.
    *
    * @param val                       A reference to the ASN1DynBitStr
    *                                    structure containing a bit data and
    *                                    number of bits to be encoded.
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
    * @see                            ::berEncStrmBitStr
    */
   int encodeBitStr (const ASN1DynBitStr& val, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes a variable of the ASN.1 BOOLEAN type.
    *
    * @param val                       A BOOLEAN value to be encoded. A BOOLEAN
    *                                    is defined as a single OCTET whose
    *                                    value is 0 for FALSE and any other
    *                                    value for TRUE.
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
    * @see                            ::berEncStrmBool
    */
   int encodeBool (OSBOOL val, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes a variable of the ASN.1 character string type.
    *
    * @param pval                      A pointer to a null-terminated C
    *                                    character string to be encoded.
    * @param tagging                   An enumerated type whose value is set to
    *                                    either 'ASN1EXPL' (for explicit
    *                                    tagging) or 'ASN1IMPL' (for implicit).
    *                                    Controls whether the universal tag
    *                                    value for this type is added or not.
    *                                    Users will generally always set this
    *                                    value to 'ASN1EXPL'.
    * @param tag                       The ASN.1 tag to be encoded in the
    *                                    message. This parameter is passed
    *                                    using the ASN1C internal tag
    *                                    representation. It is passed as an
    *                                    unsigned 32-bit integer.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berEncStrmCharStr
    */
   int encodeCharStr (const char* pval, ASN1TagType tagging = ASN1EXPL,
                       ASN1TAG tag = 0);

   /**
    * This method encodes a variable of the ASN.1 ENUMERATED type. The
    * enumerated encoding is identical to that of an integer. The compiler adds
    * additional checks to the generated code to ensure the value is within the
    * given set.
    *
    * @param val                       An integer containing the enumerated
    *                                    value to be encoded.
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
    * @see                            ::berEncStrmEnum
    */
   int encodeEnum (OSINT32 val, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes end-of-contents octets (EOC) into the stream. EOC is
    * two zero octets (it is documented in the X.690 standard). This method
    * must be called when the encoding of the complex type with indefinite
    * length is finishing.
    *
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berEncStrmEOC,
    *                                    ::berEncStrmTagAndIndefLen,
    */
   int encodeEoc ();

   /**
    * This method is used to encode the indefinite length indicator. This can
    * be used to manually create an indefinite length wrapper around long or
    * constructed records.
    *
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berEncStrmWriteOctet
    */
   int encodeIndefLen ();

   /**
    * This method encodes a variable of the ASN.1 INTEGER type.
    *
    * @param val                       A 32-bit INTEGER value to be encoded.
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
    * @see                            ::berEncStrmInt
    */
   int encodeInt (OSINT32 val, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes an 8-bit variable of the ASN.1 INTEGER type.
    *
    * @param val                       An 8-bit INTEGER value to be encoded.
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
    * @see                            ::berEncStrmInt8
    */
   int encodeInt8 (OSINT8 val, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes a 16-bit variable of the ASN.1 INTEGER type.
    *
    * @param val                       A 16-bit INTEGER value to be encoded.
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
    * @see                            ::berEncStrmInt16
    */
   int encodeInt16 (OSINT16 val, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes a 64-bit variable of the ASN.1 INTEGER type.
    *
    * @param val                       A 64-bit INTEGER value to be encoded.
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
    * @see                            ::berEncStrmInt64
    */
   int encodeInt64 (OSINT64 val, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method is used to encode a length in BER format.
    *
    * @param len                       The length of the contents field.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berEncStrmLength
    */
   int encodeLen (size_t len);

   /**
    * This method encodes a variable of the ASN.1 NULL type.
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
    * @see                            ::berEncStrmRelativeOID
    */
   int encodeNull (ASN1TagType tagging = ASN1EXPL);

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
    * This method encodes a variable of the ASN.1 OBJECT IDENTIFIER type.
    *
    * @param val                       A reference to an object identifier
    *                                    structure. This structure contains an
    *                                    integer to hold the number of
    *                                    subidentifers in the object and an
    *                                    array to hold the subidentifier
    *                                    values.
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
    * @see                            ::berEncStrmObjId
    */
   int encodeObjId (const ASN1OBJID& val, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes a variable of the ASN.1 OBJECT IDENTIFIER type using
    * 64-bit subidentifiers.
    *
    * @param val                       A reference to a 64-bit object
    *                                    identifier structure. This structure
    *                                    contains an integer to hold the number
    *                                    of subidentifers in the object and an
    *                                    array of 64-bit unsigned integers to
    *                                    hold the subidentifier values.
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
    * @see                            ::berEncStrmObjId64
    */
   int encodeObjId64 (const ASN1OID64& val, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes a variable of the ASN.1 OCTET STRING type.
    *
    * @param pocts                     A pointer to an OCTET STRING containing
    *                                    the octet data to be encoded.
    * @param numocts                   The number of octets (bytes) within the
    *                                    OCTET STRING to be encoded.
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
    * @see                            ::berEncStrmOctStr
    */
   int encodeOctStr (const OSOCTET* pocts, OSUINT32 numocts,
                      ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes a variable of the ASN.1 OCTET STRING type.
    *
    * @param val                       A reference to the ASN1DynOctStr
    *                                    structure containing an octet data and
    *                                    number of octets to be encoded.
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
    * @see                            ::berEncStrmOctStr
    */
   int encodeOctStr (const ASN1DynOctStr& val, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes a variable of the REAL data type. It provides support
    * for the plus-infinity and minus-infinity special real values. Use the
    * ::rtxGetPlusInfinity or ::rtxGetMinusInfinity functions to get these
    * special values.
    *
    * @param val                       An OSREAL data type. This is defined
    *                                    to be the C double type. Special real
    *                                    values plus and minus infinity are
    *                                    encoded by using the
    *                                    ::rtxGetPlusInfinity and
    *                                    ::rtxGetMinusInfinity functions to set
    *                                    the real value to be encoded.
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
    * @see                            ::berEncStrmReal
    */
   int encodeReal (OSREAL val, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes a variable of the ASN.1 RELATIVE-OID type.
    *
    * @param val                       A reference to an object identifier
    *                                    structure. This structure contains an
    *                                    integer to hold the number of
    *                                    subidentifers in the object and an
    *                                    array to hold the subidentifier
    *                                    values.
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
    * @see                            ::berEncStrmRelativeOID
    */
   int encodeRelativeOID (const ASN1OBJID& val, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method is used to encode the ASN.1 tag field that preface each block
    * of message data. The ASN1C compiler generates calls to this function to
    * handle the encoding of user-defined tags within an ASN.1 specification.
    *
    * @param tag                       The ASN.1 tag to be encoded in the
    *                                    message. This parameter is passed
    *                                    using the ASN1C internal tag
    *                                    representation. It is passed as an
    *                                    unsigned 32-bit integer.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berEncStrmTag
    */
   int encodeTag (ASN1TAG tag);

   /**
    * This method is used to encode a tag value and an indefinite length. This
    * can be used to manually create an indefinite length wrapper around long
    * or constructed records.
    *
    * @param tag                       The ASN.1 tag to be encoded in the
    *                                    message. This parameter is passed
    *                                    using the ASN1C internal tag
    *                                    representation. It is passed as an
    *                                    unsigned 32-bit integer.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berEncStrmTagAndIndefLen
    */
   int encodeTagAndIndefLen (ASN1TAG tag);

   /**
    * This method is used to encode the ASN.1 tag and length fields that
    * preface each block of message data.
    *
    * @param tag                       The ASN.1 tag to be encoded in the
    *                                    message. This parameter is passed
    *                                    using the ASN1C internal tag
    *                                    representation. It is passed as an
    *                                    unsigned 32-bit integer.
    * @param len                       The length of the contents field. This
    *                                    parameter can be used to specify the
    *                                    actual length, or the special constant
    *                                    'ASN_K_INDEFLEN' can be used to
    *                                    specify that an indefinite length
    *                                    specification should be encoded.
    * @return                          Completion status of operation:
    *                                    - 0 (0) = success,
    *                                    - negative return value is error.
    * @see                            ::berEncStrmTagAndLen
    */
   int encodeTagAndLen (ASN1TAG tag, OSINT32 len);

   /**
    * This method encodes an unsigned variable of the ASN.1 INTEGER type.
    *
    * @param val                     An unsigned INTEGER value to be encoded.
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
    * @see                            ::berEncStrmUInt
    */
   int encodeUInt (OSUINT32 val, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes an 8-bit unsigned variable of the ASN.1 INTEGER type.
    *
    * @param val                     An 8-bit unsigned INTEGER value to be
    *                                    encoded.
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
    * @see                            ::berEncStrmUInt8
    */
   int encodeUInt8 (OSUINT8 val, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes a 16-bit unsigned variable of the ASN.1 INTEGER type.
    *
    * @param val                     A 16-bit unsigned INTEGER value to be
    *                                    encoded.
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
    * @see                            ::berEncStrmUInt16
    */
   int encodeUInt16 (OSUINT16 val, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes a 64-bit unsigned variable of the ASN.1 INTEGER type.
    *
    * @param val                     A 64-bit unsigned INTEGER value to be
    *                                    encoded.
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
    * @see                            ::berEncStrmUInt64
    */
   int encodeUInt64 (OSUINT64 val, ASN1TagType tagging = ASN1EXPL);

   /**
    * This method encodes a variable of the ASN.1 UniversalString type that is
    * based on a 32-bit character sets.
    *
    * @param val                       A reference to a structure representing
    *                                    a 32-bit character string to be
    *                                    encoded. This structure contains a
    *                                    character count element and a pointer
    *                                    to an array of 32-bit character
    *                                    elements represented as 32-bit
    *                                    unsigned integers.
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
    * @see                            ::berEncStrmUnivStr
    */
   int encodeUnivStr
      (const Asn132BitCharString& val, ASN1TagType tagging = ASN1EXPL);

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
#endif /* _ASN1BERENCODESTREAM_H_ */

