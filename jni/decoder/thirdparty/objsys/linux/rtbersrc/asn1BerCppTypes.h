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
//
// CHANGE LOG
// Date         Init    Description
// 07/14/02     ED      Changed all methods to being with lowercase
//                      char for consistency.  Added deprecated inline
//                      methods for those that did not to maintain
//                      backward compatibility.
// 10/30/03     CZ      Added Doxygen code.
//
////////////////////////////////////////////////////////////////////////////

/**
 * @file asn1BerCppTypes.h
 * BER/DER/CER C++ type and class definitions.
 */
#ifndef _ASN1BERCPPTYPES_H_
#define _ASN1BERCPPTYPES_H_

#include "rtsrc/asn1CppTypes.h"
#include "rtbersrc/asn1ber.h"
#include "rtxsrc/rtxPrint.h"

/**
 * @defgroup bercppruntime BER/DER/CER C++ Run-Time Classes.
 * @{
 */


/** @defgroup bermsgbuffclas BER Message Buffer Classes
 * These classes manage the buffers for encoding and decoding ASN.1 BER/DER
 * messages. @{
 */

// ASN.1 BER message buffer class

/**
 * The ASN1BERMessageBuffer class is derived from the ASN1MessageBuffer base
 * class. It is the base class for the ASN1BEREncodeBuffer and
 * ASN1BERDecodeBuffer derived classes. It contains variables and methods
 * specific to encoding or decoding ASN.1 messages using the Basic Encoding
 * Rules(BER) and Distinguished Encoding Rules (DER). It is used to manage the
 * buffer into which an ASN.1 message is to be encoded or decoded.
*/

class EXTERNBER ASN1BERMessageBuffer : public ASN1MessageBuffer {
 protected:
   /**
    * The protected constructor creates a new context and sets the buffer class
    * type. Use getStatus() method to determine has error occured during the
    * initialization or not.
    *
    * @param bufferType   Type of message buffer that is being created (for
    *                       example, BEREncode or BERDecode).
    */
   ASN1BERMessageBuffer (Type bufferType) : ASN1MessageBuffer(bufferType) {}

   /**
    * The protected constructor uses an existing context and sets the buffer class
    * type. Use getStatus() method to determine has error occured during the
    * initialization or not.
    *
    * @param bufferType   Type of message buffer that is being created (for
    *                       example, BEREncode or BERDecode).
    *
    * @param pContext     A pointer to the context structure with which this
    *                       buffer is associated.
    */
   ASN1BERMessageBuffer (Type bufferType, OSRTContext *pContext) :
      ASN1MessageBuffer (bufferType, pContext) { }

 public:
   /**
    * This method calculates the actual length of an indefinite length message
    * component.
    *
    * Calling Sequence:
    *
    * <TT>len=messageBuffer.calcIndefLen (buf_p);</TT>
    *
    * where messageBuffer is an ASN1BERMessageBuffer derived class object.
    *
    * @param buf_p        A pointer to a message component encoded using
    *                       indefinite length encoding.
    * @param bufSize      Size of the buffer \c buf_p (in octets).
    * @return             Length, in octets, of message component, as int.
    */
   inline int calcIndefLen (OSOCTET* buf_p, int bufSize = INT_MAX) {
      return xd_indeflen_ex (buf_p, bufSize);
   }

   /**
    * This method outputs a formatted binary dump of the current buffer
    * contents to stdout.
    *
    * Calling Sequence:
    *
    * <TT>messageBuffer.binDump ();</TT>
    *
    * where messageBuffer is an ASN1BERMessageBuffer derived class object.
    */
   inline void binDump () {
      int stat = xu_dump ((OSOCTET*)getMsgPtr(), 0, 0);
      if (stat != 0) setStatus (stat);
   }

   /**
    * This method outputs a hexadecimal dump of the current buffer contents to
    * stdout.
    *
    * Calling sequence:
    *
    * <TT>messageBuffer.hexDump ();</TT>
    *
    * where messageBuffer is an ASN1BERMessageBuffer derived class object.
    */
   inline void hexDump (int numocts) {
      rtxHexDump (getMsgPtr(), numocts);
   }

   // deprecated methods
   inline int  CalcIndefLen (OSOCTET* buf_p) { return calcIndefLen(buf_p); }
   inline void BinDump () { binDump(); }
   inline void HexDump (int numocts) { hexDump (numocts); }
} ;

// ASN.1 BER encode message buffer class
 /**
  * The ANS1BEREncodeBuffer class is derived from the ASN1BERMessageBuffer base
  * class. It contains variables and methods specific to encoding ASN.1
  * messages using the Basic Encoding Rules (BER). It is used to manage the
  * buffer into which an ASN.1 message is to be encoded.
 */
class EXTERNBER ASN1BEREncodeBuffer : public ASN1BERMessageBuffer {
 public:
   /**
    * Default constructor.  This sets all internal variables to their
    * initial values.  Use the getStatus() method to determine if an error
    * occured during initialization or not.
    */
   ASN1BEREncodeBuffer ();

   /**
    * Parameterized constructor.  This version takes a message buffer and
    * size argument (static encoding version). Use the getStatus() method
    * to determine if an error occured during initialization or not.
    *
    * @param pMsgBuf      A pointer to a fixed size message buffer to recieve
    *                       the encoded message.
    * @param msgBufLen    Size of the fixed-size message buffer.
    */
   ASN1BEREncodeBuffer (OSOCTET* pMsgBuf, size_t msgBufLen);

   /**
    * Parameterized constructor.  This version takes a message buffer, a
    * size argument (static encoding version), and a context. Use the
    * getStatus() method to determine if an error occured during initialization
    * or not.
    *
    * @param pMsgBuf      A pointer to a fixed size message buffer to recieve
    *                       the encoded message.
    * @param msgBufLen    Size of the fixed-size message buffer.
    * @param pContext     A pointer to an existing OSRTContext structure.
    */
   ASN1BEREncodeBuffer
      (OSOCTET* pMsgBuf, size_t msgBufLen, OSRTContext* pContext);

   /**
    * This method releases memory that was allocated for a dynamic
    * encode buffer.
    */
   inline void freeBuffer() { xe_free (getCtxtPtr()); }

   /**
    * This method returns a copy of the current encoded message. Memory is
    * allocated for the message using the 'new []' operation. It is the
    * users's responsibility to free the memory using 'delete []'.
    *
    * Calling Sequence:
    *
    * <TT>ptr = encodeBuffer.getMsgCopy ();</TT>
    *
    * where encodeBuffer is an ASN1BEREncodeBuffer object.
    *
    * @return             Pointer to copy of encoded message. It is the users's
    *                       responsibility to free the memory using 'delete []'
    *                       (i.e., delete [] ptr;).
    */
   virtual OSOCTET* getMsgCopy ();

   /**
    * This method returns the internal pointer to the current encoded message.
    * Calling Sequence:
    *
    * <TT>ptr = encodeBuffer.getMsgPtr ();</TT>
    *
    * where encodeBuffer is an ASN1BEREncodeBuffer object.
    *
    * @return             Pointer to encoded message.
    */
   virtual const OSOCTET* getMsgPtr ();

   /**
    * This method reinitializes the encode buffer pointer to allow a new
    * message to be encoded. This makes it possible to reuse one message buffer
    * object in a loop to encode multiple messages. After this method is
    * called, any previously encoded message in the buffer will be overwritten
    * on the next encode call.
    *
    * Calling Sequence:
    *
    * <TT>encodeBuffer.init ();</TT>
    *
    * where encodeBuffer is an ASN1BEREncodeBuffer object.
    *
    * @return             Completion status of operation:
    *                       - 0 (0) = success,
    *                       - negative return value is error.
    */
   int init ();

   /**
    * This method checks the type of the message buffer.
    *
    * @param bufferType   Enumerated identifier specifying a derived class. The
    *                       only possible value for this class is BEREncode.
    * @return             Boolean result of the match operation. True if this
    *                       is the class corresponding to the identifier
    *                       argument.
    */
   virtual OSBOOL isA (int bufferType) {
      return bufferType == BEREncode;
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
   int setBuffer (OSOCTET* pMsgBuf, size_t msgBufLen);

   /**
    * This operator encodes instance of ASN1CType derived class. Use
    * getStatus() method to determine has error occured during the operation or
    * not.
    */
   ASN1BEREncodeBuffer& operator << (ASN1CType& val);
} ;


// ASN.1 BER decode message buffer class
/**
 * The ASN1BERDecodeBuffer class is derived from the ASN1BERMessageBuffer base
 * class. It contains variables and methods specific to decoding ASN.1 BER/DER
 * messages. It is used to manage the input buffer containing an ASN.1 message
 * to be decoded.
*/

class EXTERNBER ASN1BERDecodeBuffer : public ASN1BERMessageBuffer {
 protected:
   const OSOCTET* mpMsgBuf;
   size_t mMsgBufLen;
   OSBOOL mBufSetFlag;

 public:
   /**
    * Default constructor. Use the getStatus() method to determine if an
    * error occured during initialization or not.
    */
   ASN1BERDecodeBuffer ();

  /**
   * Parameterized constructor.  This constructor constructs a buffer
   * describing an encoded ASN.1 message. Parameters describing the message
   * to be decoded are passed as arguments.
   *
   * @param pMsgBuf      A pointer to a buffer containing an encoded ASN.1
   *                       message.
   * @param msgBufLen    Size of the message buffer. This does not have to be
   *                       equal to the length of the message. The message
   *                       length can be determined from the outer
   *                       tag-length-value in the message. This parameter is
   *                       used to determine if the length of the message is
   *                       valid; therefore it must be greater than or equal to
   *                       the actual length. Typically, the size of the buffer
   *                       the message was read into is passed.
   */
   ASN1BERDecodeBuffer (const OSOCTET* pMsgBuf, size_t msgBufLen);

  /**
   * Parameterized constructor.  This constructor constructs a buffer
   * describing an encoded ASN.1 message. Parameters describing the message
   * to be decoded are passed as arguments.
   *
   * @param pMsgBuf      A pointer to a buffer containing an encoded ASN.1
   *                       message.
   * @param msgBufLen    Size of the message buffer. This does not have to be
   *                       equal to the length of the message. The message
   *                       length can be determined from the outer
   *                       tag-length-value in the message. This parameter is
   *                       used to determine if the length of the message is
   *                       valid; therefore it must be greater than or equal to
   *                       the actual length. Typically, the size of the buffer
   *                       the message was read into is passed.
   * @param pContext     A pointer to an existing OSRTContext structure.
   */
   ASN1BERDecodeBuffer
      (const OSOCTET* pMsgBuf, size_t msgBufLen, OSRTContext* pContext);

   /**
    * This method finds a tagged element within a message.
    *
    * Calling Sequence:
    *
    * <TT>ptr = decodeBuffer.findElement (tag, elemLen, firstFlag);</TT>
    *
    * where decodeBuffer is an ASN1BERDecodeBuffer object.
    *
    * @return             Pointer to tagged component in message or NULL if
    *                       component not found.
    * @param tag          ASN.1 tag value to search for.
    * @param firstFlag    Flag indicating if this the first time this search is
    *                       being done. If true, internal pointers will be set
    *                       to start the search from the beginning of the
    *                       message. If false, the search will be resumed from
    *                       the point at which the last matching tag was found.
    *                       This makes it possible to find all instances of a
    *                       particular tagged element within a message
    * @param elemLen      Reference to an integer value to receive the length
    *                       of the found element.
    */
   OSOCTET* findElement
      (ASN1TAG tag, OSINT32& elemLen, OSBOOL firstFlag = TRUE);

   /**
    * This method returns the internal pointer to the current message that
    * has been set to be decoded.
    *
    * @return             Pointer to message.
    */
   virtual const OSOCTET* getMsgPtr() {
      // Note: return message buffer pointer from object, not from context,
      // because may not yet be set in context (ED, 9/30/04)..
      return mpMsgBuf;
   }

   /**
    * Initializes message buffer.
    *
    * @return             Completion status of operation:
    *                       - 0 (0) = success,
    *                       - negative return value is error.
    */
   int init ();

   /**
    * This method checks the type of the message buffer.
    *
    * @param bufferType   Enumerated identifier specifying a derived class. The
    *                       only possible value for this class is BERDecode.
    * @return             Boolean result of the match operation. True if this
    *                       is the class corresponding to the identifier
    *                       argument.
    */
   virtual OSBOOL isA (int bufferType) {
      return bufferType == BERDecode;
   }
   /**
    * This method will parse the initial tag-length pair from the message.
    *
    * Calling Sequence:
    *
    * <TT>stat = decodeBuffer.parseTagLen (tag, msglen);</TT>
    *
    * where decodeBuffer is an ASN1BERDecodeBuffer object.
    *
    * @return             Status of the operation. Possible values are 0
    *                       if successful or one of the negative error status
    *                       codes defined in Appendix A of the C/C++ Common
    *                       Functions Reference Manual.
    * @param tag          Reference to a tag structure to receive the outer
    *                       level tag value parsed from the message.
    * @param msglen       Length of the message. This is the total length of
    *                       the message obtained by adding the number of bytes
    *                       in initial tag-length to the parsed length value.
    */
   inline int parseTagLen (ASN1TAG& tag, int& msglen) {
      mBufSetFlag = TRUE;
      return xd_setp (getCtxtPtr(),
                      mpMsgBuf, (int)mMsgBufLen, &tag, &msglen);
   }

   /**
    * This method reads a file containing a single BER/DER/CER encoded
    * data record into the buffer for decoding.
    *
    * @param filePath     The zero-terminated string containing the path to the
    *                       file.
    * @return             Completion status of operation:
    *                       - 0 (0) = success,
    *                       - negative return value is error.
    */
   int readBinaryFile (const char* filePath);

   /**
    * This method sets a buffer containing a message to be decoded.
    *
    * @param pMsgBuf      A pointer to a memory buffer containing a message to
    *                       be decoded. The buffer should be declared as an
    *                       array of unsigned characters (OSOCTETs).
    * @param msgBufLen    The length of the memory buffer in bytes.
    * @return             Completion status of operation:
    *                       - 0 (0) = success,
    *                       - negative return value is error.
    */
   int setBuffer (const OSOCTET* pMsgBuf, size_t msgBufLen);

   // deprecated methods
   inline OSOCTET* FindElement (ASN1TAG tag, int& elemLen,
                                int firstFlag = 1) {
      return findElement (tag, elemLen, (OSBOOL)firstFlag);
   }
   inline int ParseTagLen (ASN1TAG& tag, int& msglen) {
      return parseTagLen (tag, msglen);
   }

   /**
    * This operator decodes an instance of an ASN1CType derived class. Use
    * the getStatus() method to determine if an error occured during the
    * operation or not.
    */
   ASN1BERDecodeBuffer& operator >> (ASN1CType& val);
} ;

/**
 * @} bermsgbuffclas
*/

/**
 * @} bercppruntime
*/
#endif
