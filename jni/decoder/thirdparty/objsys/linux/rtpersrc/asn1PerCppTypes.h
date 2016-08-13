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
 * @file asn1PerCppTypes.h
 * PER C++ type and class definitions.
 */
#ifndef _ASN1PERCPPTYPES_H_
#define _ASN1PERCPPTYPES_H_

#include "rtpersrc/asn1per.h"
#include "rtsrc/asn1CppTypes.h"
#include "rtxsrc/rtxBitEncode.h"

class OSRTStream;
class OSRTInputStream;
class OSRTOutputStream;

/** @defgroup percppruntime PER C++ Runtime Classes.
 * @{
 */
/**
 * The ASN.1 C++ runtime classes are wrapper classes that provide an
 * object-oriented interface to the ASN.1 C runtime library functions. These
 * classes are derived from the common classes documented in the ASN1C C/C++
 * Common Runtime Functions manual and are specific the Packed Encoding Rules
 * (PER).
*/

/** @defgroup permsgbuffclas PER Message Buffer Classes
 * These classes manage the buffers for encoding and decoding ASN.1 PER
 * messages. @{
*/

// ASN.1 PER message buffer class
/**
 * The ASN1PERMessageBuffer class is derived from the ASN1MessageBuffer base
 * class. It is the base class for the ASN1PEREncodeBuffer and
 * ASN1PERDecodeBuffer derived classes. It contains variables and methods
 * specific to encoding or decoding ASN.1 messages using the Packed Encoding
 * Rules (PER). It is used to manage the buffer into which an ASN.1 message is
 * to be encoded or decoded.
*/
class EXTPERCLASS ASN1PERMessageBuffer : public ASN1MessageBuffer {
 protected:

   /**
    * This constructor does not set a PER input source.  It is used by
    * the derived encode buffer classes.  Use the getStatus() method to
    * determine if an error has occured during initialization.
    *
    * @param bufferType   Type of message buffer that is being created (for
    *                       example, PEREncode or PERDecode).
    * @param aligned      Flag indicating if aligned (TRUE) or unaligned
    *                       (FALSE) encoding should be done.
    */
   EXTPERMETHOD ASN1PERMessageBuffer (Type bufferType, OSBOOL aligned);

   /**
    * This constructor associates a stream with a PER encode or decode
    * buffer.  It is used by the derived encode buffer classes to create
    * a stream-based PER encoder or decoder.
    *
    * @param stream Stream class reference.
    * @param aligned      Flag indicating if aligned (TRUE) or unaligned
    *                       (FALSE) encoding should be done.
    */
   EXTPERMETHOD ASN1PERMessageBuffer (OSRTStream& stream, OSBOOL aligned);

   /**
    * This constructor allows a memory buffer holding a binary PER message
    * to be specified. Use the getStatus() method to determine if an error
    * has occured during initialization.
    *
    * @param bufferType   Type of message buffer that is being created (for
    *                       example, PEREncode or PERDecode).
    * @param pMsgBuf      A pointer to a fixed size message buffer to recieve
    *                       the encoded message.
    * @param msgBufLen    Size of the fixed-size message buffer.
    * @param aligned      Flag indicating if aligned (TRUE) or unaligned
    *                       (FALSE) encoding should be done.
    */
   EXTPERMETHOD ASN1PERMessageBuffer
      (Type bufferType, OSOCTET* pMsgBuf, size_t msgBufLen, OSBOOL aligned);

   /**
    * This constructor allows a memory buffer holding a binary PER message
    * to be specified. It also allows a pre-existing context to be associated
    * with this buffer. Use the getStatus() method to determine if an error
    * has occured during initialization.
    *
    * @param bufferType   Type of message buffer that is being created (for
    *                       example, PEREncode or PERDecode).
    * @param pMsgBuf      A pointer to a fixed size message buffer to recieve
    *                       the encoded message.
    * @param msgBufLen    Size of the fixed-size message buffer.
    * @param aligned      Flag indicating if aligned (TRUE) or unaligned
    *                       (FALSE) encoding should be done.
    * @param pContext     A pointer to an OSRTContext structure.
    */
   EXTPERMETHOD ASN1PERMessageBuffer
      (Type bufferType, OSOCTET* pMsgBuf, size_t msgBufLen,
       OSBOOL aligned, OSRTContext *pContext);

 public:

   /**
    * This method outputs a binary dump of the current buffer contents to
    * stdout.
    *
    * @param varname      char pointer to current buffer
    */
   inline void binDump (const char* varname) {
      pu_bindump (getCtxtPtr(), (char*)varname);
   }

   /**
    * This method outputs a hexadecimal dump of the current buffer contents to
    * stdout.
    *
    * @param             - none
    */
   inline void hexDump () {
      pu_hexdump (getCtxtPtr());
   }

   /**
    * This method returns the length of a previously encoded PER message.
    *
    * @param             - none
    */
   inline size_t getMsgLen () {
      return pu_getMsgLen (getCtxtPtr());
   }

   /**
    * This method indicates if PER aligned encoding is in effect.
    *
    * @param             - none
    * @return Boolean result: true if aligned; false if unaligned.
    */
   inline OSBOOL isAligned () {
      return (getCtxtPtr()->buffer.aligned);
   }

   /**
    * This method turns PER diagnostic tracing on or off.
    *
    * This enables the collection of the bit statistics inside the PER library
    * functions that can be displayed using the binDump method.
    *
    * @param value        Boolean value indicating whether tracing should be on
    *                       (true) or off (false).
    */
   inline void setTrace (OSBOOL value) {
      pu_setTrace (getCtxtPtr(), value);
   }

   /**
    * This method sets a buffer to receive the encoded message.
    *
    * @param pMsgBuf      A pointer to a memory buffer to use to encode a
    *                       message. The buffer should be declared as an array
    *                       of unsigned characters (OSOCTETs). This parameter
    *                       can be set to NULL to specify dynamic encoding
    *                       (i.e., the encode functions will dynamically
    *                       allocate a buffer for the message).
    * @param msgBufLen    The length of the memory buffer in bytes. If pMsgBuf
    *                       is NULL, this parameter specifies the initial size
    *                       of the dynamic buffer; if 0 - the default size will
    *                       be used.
    * @return             Completion status of operation:
    *                       - 0 (0) = success,
    *                       - negative return value is error.
    */
   EXTPERMETHOD int setBuffer (const OSOCTET* pMsgBuf, size_t msgBufLen);

   // deprecated methods
   inline void BinDump (const char* varname) { binDump (varname); }
   inline void HexDump () { hexDump (); }
   inline int GetMsgLen () { return (int)getMsgLen (); }
   inline void SetTrace (OSBOOL value) { setTrace (value); }
} ;

// ASN.1 PER encode message buffer class
/**
 * The ASN1PEREncodeBuffer class is derived from the ASN1PERMessageBuffer base
 * class. It contains variables and methods specific to encoding ASN.1
 * messages. It is used to manage the buffer into which an ASN.1 PER message
 * is to be encoded.
 */
class EXTPERCLASS ASN1PEREncodeBuffer : public ASN1PERMessageBuffer {
 public:
   /**
    * This version of the ASN1PEREncodeBuffer constructor that takes one
    * argument, aligned flag.  It creates a dynamic memory buffer into which
    * a PER message is encoded.
    *
    * @param aligned      Flag indicating if aligned (TRUE) or unaligned
    *                       (FALSE) encoding should be done.
    */
   ASN1PEREncodeBuffer (OSBOOL aligned) :
      ASN1PERMessageBuffer (PEREncode, aligned) {}

   /**
    * This version of the ASN1PEREncodeBuffer constructor takes a message
    * buffer and size argument and an aligned flag argument (static encoding
    * version).
    *
    * @param pMsgBuf      A pointer to a fixed-size message buffer to receive
    *                       the encoded message.
    * @param msgBufLen    Size of the fixed-size message buffer.
    * @param aligned      Flag indicating if aligned (TRUE) or unaligned
    *                       (FALSE) encoding should be done.
    */
   ASN1PEREncodeBuffer (OSOCTET* pMsgBuf, size_t msgBufLen, OSBOOL aligned)
      : ASN1PERMessageBuffer (PEREncode, pMsgBuf, msgBufLen, aligned) {}


   /**
    * This version of the ASN1PEREncodeBuffer constructor takes a message
    * buffer and size argument and an aligned flag argument (static encoding
    * version) as well as a pointer to an existing context object.
    *
    * @param pMsgBuf      A pointer to a fixed-size message buffer to receive
    *                       the encoded message.
    * @param msgBufLen    Size of the fixed-size message buffer.
    * @param aligned      Flag indicating if aligned (TRUE) or unaligned
    *                       (FALSE) encoding should be done.
    * @param pContext     A pointer to an OSRTContext structure created by
    *                         the user.
    */
   ASN1PEREncodeBuffer (OSOCTET *pMsgBuf, size_t msgBufLen, OSBOOL aligned,
      OSRTContext *pContext) : ASN1PERMessageBuffer (PEREncode, pMsgBuf,
         msgBufLen, aligned, pContext) { }

   /**
    * This version of the ASN1PEREncodeBuffer constructor takes a
    * reference to an output stream object and an aligned flag argument
    * (stream encoding version).
    *
    * @param ostream      A reference to an output stream object.
    * @param aligned      Flag indicating if aligned (TRUE) or unaligned
    *                       (FALSE) encoding should be done.
    */
   EXTPERMETHOD ASN1PEREncodeBuffer
      (OSRTOutputStream& ostream, OSBOOL aligned);

   /**
    * This method aligns the output buffer or stream to the next octet boundary.
    *
    * @return           Status of operation: 0 = success, negative
    *                     value if error occurred.
    */
   inline int byteAlign () {
      return pe_byte_align (getCtxtPtr());
   }

   /**
    * This method writes a single encoded bit value to the output buffer
    * or stream.
    *
    * @param value      Boolean value of bit to be written.
    * @return           Status of operation: 0 = success, negative
    *                     value if error occurred.
    */
   inline int encodeBit (OSBOOL value) {
      return rtxEncBit (getCtxtPtr(), value);
   }

   /**
    * This method writes the given number of bits from the byte array
    * to the output buffer or stream starting from the given bit offset.
    *
    * @param pvalue     Pointer to byte array containing data to
    *                     be encoded.
    * @param nbits      Number of bits to copy from byte array to
    *                     encode buffer.
    * @param bitOffset  Starting bit offset from which bits are to be copied.
    * @return           Status of operation: 0 = success, negative
    *                     value if error occurred.
    */
   inline int encodeBits
      (const OSOCTET* pvalue, size_t nbits, OSUINT32 bitOffset = 0) {
      return (0 == bitOffset) ?
         rtxEncBitsFromByteArray (getCtxtPtr(), pvalue, nbits) :
         rtxCopyBits (getCtxtPtr(), pvalue, nbits, bitOffset);
   }

   /**
    * This method returns the length (in bits) of the encoded message.
    *
    * @return             Length(in bits)of encoded message
    */
   inline size_t getMsgBitCnt () {
      return pe_GetMsgBitCnt (getCtxtPtr());
   }

   /**
    * This method returns a copy of the current encoded message. Memory is
    * allocated for the message using the 'new' operation. It is the user's
    * responsibility to free the memory using 'delete'.
    *
    * @return             Pointer to copy of encoded message. It is the user's
    *                       responsibility to release the memory using the
    *                       'delete' operator (i.e., delete [] ptr;)
    */
   virtual EXTPERMETHOD OSOCTET* getMsgCopy ();

   /**
    * This method returns the internal pointer to the current encoded message.
    *
    * @return             Pointer to encoded message.
    */
   virtual EXTPERMETHOD const OSOCTET* getMsgPtr ();

   /**
    * This method reinitializes the encode buffer pointer to allow a new
    * message to be encoded. This makes it possible to reuse one message buffer
    * object in a loop to encode multiple messages. After this method is
    * called, any previously encoded message in the buffer will be overwritten
    * on the next encode call.
    *
    * @return             Completion status of operation:
    *                       - 0 (0) = success,
    *                       - negative return value is error.
    */
   EXTPERMETHOD int init ();

   /**
    * This method checks the type of the message buffer.
    *
    * @param bufferType   Enumerated identifier specifying a derived class. The
    *                       only possible value for this class is PEREncode.
    * @return             Boolean result of the match operation. True if this
    *                       is the class corresponding to the identifier
    *                       argument.
    */
   virtual OSBOOL isA (int bufferType) {
      return bufferType == PEREncode;
   }

   // deprecated methods
   inline int GetMsgBitCnt () { return (int)getMsgBitCnt(); }

} ;

// ASN.1 PER decode message buffer class
/**
 * The ASN1PERDecodeBuffer class is derived from the ASN1PERMessageBuffer base
 * class. It contains variables and methods specific to decoding ASN.1 PER
 * messages. It is used to manage the input buffer containing the ASN.1 message
 * to be decoded. This class has 3 overloaded constructors.
 *
 */
class EXTPERCLASS ASN1PERDecodeBuffer : public ASN1PERMessageBuffer {
 public:
   /**
    * This is a default constructor. Use getStatus() method to determine has
    * error occured during the initialization or not.
    *
    * @param aligned      Flag indicating if the message was encoded using
    *                       aligned (TRUE)* or unaligned (FALSE) encoding.
    */
   ASN1PERDecodeBuffer (OSBOOL aligned) :
      ASN1PERMessageBuffer (PERDecode, aligned)
      {}

   /**
    * This constructor is used to describe the message to be decoded. Use
    * getStatus() method to determine has error occured during the
    * initialization or not.
    *
    * @param pMsgBuf      A pointer to the message to be decoded.
    * @param msgBufLen    Length of the message buffer.
    * @param aligned      Flag indicating if the message was encoded using
    *                       aligned (TRUE) * or unaligned (FALSE) encoding.
    */
   ASN1PERDecodeBuffer (const OSOCTET* pMsgBuf,
                        size_t msgBufLen, OSBOOL aligned) :
      ASN1PERMessageBuffer (PERDecode, (OSOCTET*)pMsgBuf, msgBufLen, aligned)
      {}

   /**
    * This constructor is used to describe the message to be decoded. Use
    * getStatus() method to determine has error occured during the
    * initialization or not.
    *
    * @param pMsgBuf      A pointer to the message to be decoded.
    * @param msgBufLen    Length of the message buffer.
    * @param aligned      Flag indicating if the message was encoded using
    *                       aligned (TRUE) * or unaligned (FALSE) encoding.
    * @param pContext     A pointer to an OSRTContext structure created by
    *                       the user.
    */
   ASN1PERDecodeBuffer (const OSOCTET* pMsgBuf,
                        size_t msgBufLen, OSBOOL aligned,
                        OSRTContext *pContext) :
      ASN1PERMessageBuffer (PERDecode, (OSOCTET*)pMsgBuf, msgBufLen, aligned,
            pContext) {}

   /**
    * This version of the ASN1PERDecodeBuffer constructor takes a
    * reference to an input stream object and an aligned flag argument
    * (stream decoding version).
    *
    * @param istream      A reference to an input stream object.
    * @param aligned      Flag indicating if aligned (TRUE) or unaligned
    *                       (FALSE) encoding should be done.
    */
   EXTPERMETHOD ASN1PERDecodeBuffer (OSRTInputStream& istream, OSBOOL aligned);

   /**
    * This constructor takes a pointer to the path of a file containing
    * a binary PER message to be decoded.
    *
    * @param filePath     Complete file path and name of file to read.
    * @param aligned      Flag indicating if the message was encoded using
    *                       aligned (TRUE) * or unaligned (FALSE) encoding.
    */
   EXTPERMETHOD ASN1PERDecodeBuffer (const char* filePath, OSBOOL aligned);

   /**
    * This method aligns the input buffer or stream to the next octet boundary.
    *
    * @return           Status of operation: 0 = success, negative
    *                     value if error occurred.
    */
   inline int byteAlign () {
      return pd_byte_align (getCtxtPtr());
   }

   /**
    * This method checks the type of the message buffer.
    *
    * @param bufferType   Enumerated identifier specifying a derived class. The
    *                       only possible value for this class is PERDecode.
    * @return             Boolean result of the match operation. True if this
    *                       is the class corresponding to the identifier
    *                       argument.
    */
   virtual OSBOOL isA (int bufferType) {
      return bufferType == PERDecode;
   }

   /**
    * This method is used to peek at the next available byte in the
    * decode buffer/stream without advancing the cursor.
    *
    * @param ub         Single byte buffer to receive peeked byte.
    * @return             Completion status of operation:
    *                       - 0 (0) = success,
    *                       - negative return value is error.
    */
   EXTPERMETHOD int peekByte (OSOCTET& ub);

   /**
    * This method reads the file into the buffer to decode.
    *
    * @param filePath     The zero-terminated string containing the path to the
    *                       file.
    * @return             Completion status of operation:
    *                       - 0 (0) = success,
    *                       - negative return value is error.
    */
   EXTPERMETHOD int readBinaryFile (const char* filePath);

   /**
    * This method is used to read the given number of bytes from
    * the underlying buffer/stream into the given buffer.
    *
    * @param buffer       Buffer into which data should be read.
    * @param bufsize      Size of the buffer
    * @param nbytes       Number of bytes to read.  Must be <= bufsize.
    * @return             Completion status of operation:
    *                       - 0 (0) = success,
    *                       - negative return value is error.
    */
   EXTPERMETHOD int readBytes (OSOCTET* buffer, size_t bufsize, size_t nbytes);

} ;


/** @}permsgbuffclas

*/
/** @}percppruntime

 */
#endif
