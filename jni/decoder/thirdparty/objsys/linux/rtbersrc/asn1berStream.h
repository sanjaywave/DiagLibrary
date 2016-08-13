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
 * @file asn1berStream.h
 * ASN.1 runtime constants, data structure definitions, and functions to
 * support the streaming encoding/decoding of Basic Encoding Rules (BER) as
 * defined in the ITU-T X.690 standard.
 */
#ifndef _ASN1BERSTREAM_H_
#define _ASN1BERSTREAM_H_

#include "rtbersrc/asn1ber.h"
#include "rtxsrc/rtxStream.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup berstream Streaming BER Runtime Library Functions.
 * @ingroup berruntime
 * The streaming BER functions handle the BER encode of primitive ASN.1 data
 * types. Calls to these functions are assembled in the C source code generated
 * by the ASN1C compiler (with -stream option) to accomplish the encoding of
 * complex ASN.1 structures. These functions are also directly callable from
 * within a user's application program if the need to accomplish a low level
 * encoding function exists. In contrast to the non-streaming C BER functions,
 * these operate with streams, rather than the memory buffer, the sources or
 * destination.  Thus, the data may be encoded directly to the file stream
 * or socket output stream and may be decoded directly from the file or
 * socket input stream. @{
 */

/**
 * This function initializes an OSCTXT block for further streaming operations.
 * It makes sure that if the block was not previoiusly initialized, that all
 * key working parameters are set to their correct initial state values
 * (i.e. declared within a function as a
 * normal working variable). It also initialize stream block.
 *
 * @param pctxt  Pointer to context structure variable to be initialized.
 *
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
#ifndef berStrmInitContext
EXTERNBER int berStrmInitContext (OSCTXT* pctxt);
#endif

EXTERNBER int berStrmInitContextUsingKey
   (OSCTXT* pctxt, const OSOCTET* key, size_t keylen);

/**
 * This function closes the stream and frees all dynamic memory associated with
 * a context.
 *
 * @param pctxt        A pointer to a context structure.
 *
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNBER int berStrmFreeContext (OSCTXT* pctxt);

/** @defgroup berstreamenc C Streaming BER Encode Functions.
 * The C streaming BER encode functions encode ASN.1 primitive data types
 * directly to the output stream. It must be already
 * opened by using any of the following functions: ::rtxStreamFileOpen,
 * ::rtxStreamFileAttach, ::rtxStreamSocketAttach, ::rtxStreamMemoryCreate,
 * ::rtxStreamMemoryAttach.\n The streaming BER encoding uses indefinite
 * length form for encode complex ASN.1 types. @{
 */

/**
 * This function encodes a variable of the ASN.1 INTEGER type. In this case,
 * the integer is assumed to be of a larger size than can fit in a C or C++
 * long type (normally 32 or 64 bits). For example, parameters used to
 * calculate security values are typically larger than these sizes.\n Items
 * of this type are stored in character string constant variables. They can be
 * represented as decimal strings (with no prefixes), as hexadecimal strings
 * starting with a "0x" prefix, as octal strings starting with a "0o" prefix or
 * as binary strings starting with a "0b" prefix. Other radixes currently are
 * not supported. It is highly recommended to use the hexadecimal or binary
 * strings for better performance.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pvalue       A pointer to a character string containing the value to
 *                       be encoded.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmBigInt (OSCTXT* pctxt,
                                const char* pvalue,
                                ASN1TagType tagging);

/**
 * This function is similar to the \c berEncStrmBigInt function except
 * that only the given number of characters from the character string
 * are used as the value to be encoded.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pvalue       A pointer to a character string containing the value to
 *                       be encoded.
 * @param nchars       Number of characters from pvalue to encode.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmBigIntNchars
(OSCTXT* pctxt, const char* pvalue, size_t nchars, ASN1TagType tagging);

/**
 * This function encodes a variable of the ASN.1 BIT STRING type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     A pointer to an OCTET string containing the bit data to
 *                       be encoded. This string contains bytes having the
 *                       actual bit settings as they are to be encoded in the
 *                       message.
 * @param numbits      The number of bits within the bit string to be encoded.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmBitStr (OSCTXT*  pctxt,
                                const OSOCTET* object_p,
                                OSUINT32   numbits,
                                ASN1TagType tagging);

/**
 * This function encodes a variable of the ASN.1 BMPString type that is based
 * on a 16-bit character sets.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     A pointer to a structure representing a 16-bit character
 *                       string to be encoded. This structure contains a
 *                       character count element and a pointer to an array of
 *                       16-bit character elements represented as 16-bit short
 *                       integers.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmBMPStr (OSCTXT* pctxt,
                                const Asn116BitCharString* object_p,
                                ASN1TagType tagging);

/**
 * This function encodes a variable of the ASN.1 BOOLEAN type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value        A BOOLEAN value to be encoded. A BOOLEAN is defined as a
 *                       single OCTET whose value is 0 for FALSE and any other
 *                       value for TRUE.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmBool (OSCTXT* pctxt, OSBOOL value,
                              ASN1TagType tagging);

/**
 * This function encodes a variable one of the ASN.1 character string types
 * that are based on 8-bit character sets. This includes IA5String,
 * VisibleString, PrintableString, and NumericString.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     A pointer to a null-terminated C character string to be
 *                       encoded.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param tag          The ASN.1 tag to be encoded in the message. This
 *                       parameter is passed using the ASN1C internal tag
 *                       representation. It is passed as an unsigned 32-bit
 *                       integer.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmCharStr (OSCTXT* pctxt, const char* object_p,
                                 ASN1TagType tagging, ASN1TAG tag);

/**
 * This function encodes a definite length field to the stream.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param length       Length to be encoded.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmDefLength (OSCTXT* pctxt, size_t length);

/**
 * This function encodes end-of-contents octets (EOC) into the stream. EOC is
 * two zero octets (it is documented in the X.690 standard). This function must
 * be called when the encoding of the complex type with indefinite length is
 * finishing (see ::berEncStrmTagAndIndefLen).
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmEOC (OSCTXT *pctxt);

/**
 * This function encodes a variable of the ASN.1 ENUMERATED type. The
 * enumerated encoding is identical to that of an integer. The compiler adds
 * additional checks to the generated code to ensure the value is within the
 * given set.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value        An integer containing the enumerated value to be
 *                       encoded.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmEnum (OSCTXT* pctxt, OSINT32 value,
                              ASN1TagType tagging);

/**
 * This function encodes a variable of the ASN.1 INTEGER type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value        An INTEGER value to be encoded. The OSINT32 type is set
 *                       to the C type 'int' in the asn1type.h file. This is
 *                       assumed to represent a 32-bit integer value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmInt (OSCTXT* pctxt, OSINT32 value,
                             ASN1TagType tagging);

/**
 * This function encodes an 8-bit variable of the ASN.1 INTEGER type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value        An 8-bit INTEGER value to be encoded. The OSINT8 type
 *                       is set to the C type 'signed char' in the asn1type.h
 *                       file. This is assumed to represent an 8-bit integer
 *                       value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmInt8 (OSCTXT* pctxt, OSINT8 value,
                              ASN1TagType tagging);

/**
 * This function encodes a 16-bit variable of the ASN.1 INTEGER type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value        A 16-bit INTEGER value to be encoded. The OSINT16 type
 *                       is set to the C type 'signed short' in the asn1type.h
 *                       file. This is assumed to represent a 16-bit integer
 *                       value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmInt16 (OSCTXT* pctxt, OSINT16 value,
                               ASN1TagType tagging);

/**
 * This function encodes a 64-bit variable of the ASN.1 INTEGER type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value        A 64-bit INTEGER value to be encoded. The OSINT64 type
 *                       is set to the C type '__int64', 'long long' or 'long'
 *                       in the asn1type.h file (depends on the used platform
 *                       and the compiler). This is assumed to represent a
 *                       64-bit integer value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmInt64 (OSCTXT* pctxt, OSINT64 value,
                               ASN1TagType tagging);

/**
 * This function is used to encode a BER length determinant value.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param length       The length variable to encode. An ASN_K_INDEFLEN
 *                       constant is interpreted that an indefinite length
 *                       identifier should be encoded.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmLength (OSCTXT *pctxt, int length);

/**
 * This function encodes an ASN.1 NULL placeholder.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmNull (OSCTXT* pctxt, ASN1TagType tagging);

/**
 * This function encodes a variable of the ASN.1 OBJECT IDENTIFIER type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     A pointer to an object identifier structure. This
 *                       structure contains an integer to hold the number of
 *                       subidentifers in the object and an array to hold the
 *                       subidentifier values.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmObjId (OSCTXT* pctxt, const ASN1OBJID *object_p,
                               ASN1TagType tagging);

/**
 * This function encodes a variable of the ASN.1 OBJECT IDENTIFIER type using
 * 64-bit subidentifiers.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     A pointer to a 64-bit object identifier structure. This
 *                       structure contains an integer to hold the number of
 *                       subidentifers in the object and an array of 64-bit
 *                       unsigned integers to hold the subidentifier values.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmObjId64 (OSCTXT* pctxt, const ASN1OID64 *object_p,
                                 ASN1TagType tagging);

/**
 * This function encodes a variable of the ASN.1 OCTET STRING type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     A pointer to an OCTET STRING containing the octet data
 *                       to be encoded.
 * @param numocts      The number of octets (bytes) within the OCTET STRING to
 *                       be encoded.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmOctStr (OSCTXT* pctxt,
                                const OSOCTET* object_p,
                                OSUINT32 numocts,
                                ASN1TagType tagging);

/**
 * This function encodes an ASN.1 open type extension. An open type extension
 * is defined as an extensibility marker on a constructed type without any
 * extension elements defined (for example, SEQUENCE { a INTEGER, : }). The
 * difference is that this is an implicit field that can span one or more
 * elements whereas the standard Open Type is assumed to be a single tagged
 * field.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pElemList    The pointer to linked list structure. The list will
 *                       contain elements of ASN1OpenType type.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmOpenTypeExt (OSCTXT* pctxt, OSRTDList* pElemList);

/**
 * This function encodes a variable of the REAL data type. This function
 * provides support for the plus-infinity and minus-infinity special real
 * values. Use the ::rtxGetPlusInfinity or ::rtxGetMinusInfinity functions to get
 * these special values.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value        An OSREAL data type. This is defined to be the C
 *                       double type. Special real values plus and minus
 *                       infinity are encoded by using the ::rtxGetPlusInfinity
 *                       and ::rtxGetMinusInfinity functions to set the real
 *                       value to be encoded.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmReal (OSCTXT* pctxt, OSREAL value,
                              ASN1TagType tagging);

/**
 * This function will encode a number from character string to ASN.1 real type
 * using decimal encoding. Number may be represented in integer, decimal, and
 * exponent formats.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store al working variables
 *                       that must be maintained between function calls.
 * @param object_p     Value to be encoded.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmReal10 (OSCTXT *pctxt, const char* object_p,
                                ASN1TagType tagging);

/**
 * This function will encode a number from character string to ASN.1 real type
 * with using CER/DER decimal encoding. Number may be represented in integer,
 * decimal, and exponent formats.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store al working variables
 *                       that must be maintained between function calls.
 * @param object_p     Value to be encoded.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int cerEncStrmReal10 (OSCTXT *pctxt, const char* object_p,
                                ASN1TagType tagging);

/**
 * This function encodes a variable of the ASN.1 RELATIVE-OID type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     A pointer to an object identifier structure. This
 *                       structure contains an integer to hold the number of
 *                       subidentifers in the object and an array to hold the
 *                       subidentifier values.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmRelativeOID (OSCTXT* pctxt,
                                     const ASN1OBJID *object_p,
                                     ASN1TagType tagging);

/**
 * This function is used to encode the ASN.1 tag field that preface each block
 * of message data. The ASN1C compiler generates calls to this function to
 * handle the encoding of user-defined tags within an ASN.1 specification.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param tag          The ASN.1 tag to be encoded in the message. This
 *                       parameter is passed using the ASN1C internal tag
 *                       representation. It is passed as an unsigned 32-bit
 *                       integer.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmTag (OSCTXT *pctxt, ASN1TAG tag);

/**
 * This function is used to encode the ASN.1 tag and length fields that preface
 * each block of message data. The ASN1C compiler generates calls to this
 * function to handle the encoding of user-defined tags within an ASN.1
 * specification. This function is also called from within the run-time library
 * functions to handle the addition of the universal tags defined for each of
 * the ASN.1 primitive data types.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param tag          The ASN.1 tag to be encoded in the message. This
 *                       parameter is passed using the ASN1C internal tag
 *                       representation. It is passed as an unsigned 32-bit
 *                       integer.
 * @param length       The length of the contents field. This parameter can be
 *                       used to specify the actual length, or the special
 *                       constant 'ASN_K_INDEFLEN' can be used to specify that
 *                       an indefinite length specification should be encoded.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmTagAndLen (OSCTXT *pctxt, ASN1TAG tag, int length);

/**
 * This function is used to encode a tag value and an indefinite length. This
 * can be used to manually create an indefinite length wrapper around long or
 * constructed records.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param tag          The ASN.1 tag to be encoded in the message. This
 *                       parameter is passed using the ASN1C internal tag
 *                       representation. It is passed as an unsigned 32-bit
 *                       integer.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmTagAndIndefLen (OSCTXT *pctxt, ASN1TAG tag);

/**
 * This function encodes an unsigned variable of the ASN.1 INTEGER type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value        An unsigned INTEGER value to be encoded. The OSUINT32
 *                       type is set to the C type 'unsigned int' in the
 *                       asn1type.h file. This is assumed to represent a 32-bit
 *                       integer value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmUInt (OSCTXT* pctxt, OSUINT32 value,
                              ASN1TagType tagging);

/**
 * This function encodes an unsigned 8-bit variable of the ASN.1 INTEGER type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value        An unsigned 8-bit INTEGER value to be encoded. The
 *                       OSOCTET type is set to the C type 'unsigned char' in
 *                       the asn1type.h file. This is assumed to represent an
 *                       8-bit integer value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmUInt8 (OSCTXT* pctxt, OSUINT8 value,
                               ASN1TagType tagging);

/**
 * This function encodes an unsigned 16-bit variable of the ASN.1 INTEGER type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value        An unsigned 16-bit INTEGER value to be encoded. The
 *                       OSUINT16 type is set to the C type 'unsigned short'
 *                       in the asn1type.h file. This is assumed to represent a
 *                       16-bit integer value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmUInt16 (OSCTXT* pctxt, OSUINT16 value,
                                ASN1TagType tagging);

/**
 * This function encodes an unsigned 64-bit variable of the ASN.1 INTEGER type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value        An unsigned 64-bit INTEGER value to be encoded. The
 *                       OSUINT64 type is set to the C type 'unsigned
 *                       __int64', 'unsigned long long' or 'unsigned long' in
 *                       the asn1type.h file (depends on the used platform and
 *                       the compiler). This is assumed to represent a 64-bit
 *                       integer value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmUInt64 (OSCTXT* pctxt, OSUINT64 value,
                                ASN1TagType tagging);

/**
 * This function encodes a variable of the ASN.1 UniversalString type that is
 * based on a 32-bit character sets.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     A pointer to a structure representing a 32-bit character
 *                       string to be encoded. This structure contains a
 *                       character count element and a pointer to an array of
 *                       32-bit character elements represented as 32-bit
 *                       unsigned integers.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmUnivStr (OSCTXT* pctxt,
                                 const Asn132BitCharString* object_p,
                                 ASN1TagType tagging);

/**
 * This function encodes a variable of the XSD any element wildcard type.
 * It is only used in generated code when and XSD file is compiled.  It
 * provides the option to encode the wildcard element as XML text or in
 * binary form if ASN.1 binary encoding rules are being used.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pvalue       A pointer to a structure representing an XSD Any
 *                       (xsd:any) type to be encoded.  The structure
 *                       contains a union of XML text or binary data.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmXSDAny
(OSCTXT* pctxt, OSXSDAny* pvalue, ASN1TagType tagging);

/**
 * This function puts one octet into the output stream. It is used inside the
 * run-time library or may be used by user to encode indicator of indefinite
 * length (0x80, as defined in ITU-T X.690 standard).
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param octet        The octet to be encoded.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmWriteOctet (OSCTXT* pctxt, OSOCTET octet);

/**
 * This function puts an array of octets into the output stream. It is used
 * inside the run-time library or may be used by user to encode end-of-contents
 * octets (EOC) or open type's content.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param poctets      The array of octets to be encoded.
 * @param numocts      The number of octets in the array.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berEncStrmWriteOctets (OSCTXT* pctxt,
                                     const OSOCTET* poctets,
                                     size_t numocts);


/**
 * This function encodes a variable of the ASN.1 BMPString type with using
 * Canonical Encoding Rules (CER). BMPString type is based on a 16-bit
 * character sets.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     A pointer to a structure representing a 16-bit character
 *                       string to be encoded. This structure contains a
 *                       character count element and a pointer to an array of
 *                       16-bit character elements represented as 16-bit short
 *                       integers.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int cerEncStrmBMPStr (OSCTXT* pctxt,
                                const Asn116BitCharString* object_p,
                                ASN1TagType tagging);

/**
 * This function encodes a variable of the ASN.1 BIT STRING type with using
 * Canonical Encoding Rules (CER).
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     A pointer to an OCTET string containing the bit data to
 *                       be encoded. This string contains bytes having the
 *                       actual bit settings as they are to be encoded in the
 *                       message.
 * @param numbits      The number of bits within the bit string to be encoded.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int cerEncStrmBitStr (OSCTXT*  pctxt,
                                const OSOCTET* object_p,
                                OSUINT32   numbits,
                                ASN1TagType tagging);

/**
 * This function encodes a variable one of the ASN.1 character string types
 * that are based on 8-bit character sets with using
 * Canonical Encoding Rules (CER). This includes IA5String,
 * VisibleString, PrintableString, and NumericString.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     A pointer to a null-terminated C character string to be
 *                       encoded.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param tag          The ASN.1 tag to be encoded in the message. This
 *                       parameter is passed using the ASN1C internal tag
 *                       representation. It is passed as an unsigned 32-bit
 *                       integer.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int cerEncStrmCharStr (OSCTXT* pctxt, const char* object_p,
                                 ASN1TagType tagging, ASN1TAG tag);

/**
 * This function encodes a variable of the ASN.1 OCTET STRING type with using
 * Canonical Encoding Rules (CER).
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     A pointer to an OCTET STRING containing the octet data
 *                       to be encoded.
 * @param numocts      The number of octets (bytes) within the OCTET STRING to
 *                       be encoded.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int cerEncStrmOctStr (OSCTXT*  pctxt,
                                const OSOCTET* object_p,
                                OSUINT32   numocts,
                                ASN1TagType tagging);

/**
 * This function encodes a variable of the ASN.1 UniversalString type with using
 * Canonical Encoding Rules (CER). UniversalString type is based on a 32-bit
 * character sets.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     A pointer to a structure representing a 32-bit character
 *                       string to be encoded. This structure contains a
 *                       character count element and a pointer to an array of
 *                       32-bit character elements represented as 32-bit
 *                       unsigned integers.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int cerEncStrmUnivStr (OSCTXT* pctxt,
                                 const Asn132BitCharString* object_p,
                                 ASN1TagType tagging);

/** @} */

/** @defgroup berstreamdec C Streaming BER Decode Functions.
 * The C streaming BER Decode Functions decode ASN.1 primitive data types
 * directly from the input stream. The input stream should be initialized as
 * buffered stream by using ::rtxStreamInit function. It also must be already
 * opened by using any of the following functions: ::rtxStreamFileOpen,
 * ::rtxStreamFileAttach, ::rtxStreamSocketAttach, ::rtxStreamMemoryCreate,
 * ::rtxStreamMemoryAttach. @{
 */

/**
 * This function decodes a variable an ASN.1 16-bit character string type. This
 * includes the BMPString type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     Pointer to a structure variable to receive the decoded
 *                       string. The string is stored as an array of short
 *                       integer characters. Memory is allocated for the string
 *                       by the ::rtxMemAlloc function.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmBMPStr (OSCTXT* pctxt, Asn116BitCharString* object_p,
                                ASN1TagType tagging, int length);

/**
 * This function decodes a variable of the ASN.1 INTEGER type. In this case,
 * the integer is assumed to be of a larger size than can fit in a C or C++
 * long type (normally 32 or 64 bits). For example, parameters used to
 * calculate security values are typically larger than these sizes.\n These
 * variables are stored in character string constant variables. They are
 * represented as hexadecimal strings starting with a "0x" prefix. If it is
 * necessary to convert a hexadecimal string to another radix, then use the
 * ::rtxBigIntSetStr / ::rtxBigIntToString functions.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     Pointer to a character pointer variable to receive the
 *                       decoded value. Dynamic memory is allocated for the
 *                       variable using the ::rtxMemAlloc function. The decoded
 *                       variable is represented as a hexadecimal string
 *                       starting with a "0x" prefix.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmBigInt (OSCTXT *pctxt, const char** object_p,
                                ASN1TagType tagging, int length);

EXTERNBER int berDecStrmBigEnum (OSCTXT *pctxt, const char** object_p,
                                 ASN1TagType tagging, int length);

/**
 * This function decodes a variable of the ASN.1 BIT STRING type into a static
 * memory structure. This function call is generated by ASN1C to decode a sized
 * bit string production.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pvalue       Pointer to a variable to receive the decoded bit string.
 *                       This is assumed to be a static array large enough to
 *                       hold the number of bits specified in the *pnbits input
 *                       parameter.
 * @param pnbits       As input parameter it is a pointer to an integer
 *                       variable containing the size (in bits) of the sized
 *                       ASN.1 bit string. An error will occur if the number of
 *                       bits in the decoded string is larger than this value.
 *                       Note that this is also used as an output variable -
 *                       the actual number of decoded bits will be returned in
 *                       this variable.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmBitStr (OSCTXT *pctxt, OSOCTET* pvalue,
                                OSUINT32* pnbits,
                                ASN1TagType tagging,
                                int length);

/**
 * This function decodes a variable of the ASN.1 BOOLEAN type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     Pointer to a variable to receive the decoded BOOLEAN
 *                       value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmBool (OSCTXT *pctxt, OSBOOL *object_p,
                              ASN1TagType tagging, int length);

/**
 * This function decodes a variable of one of the ASN.1 8-bit character string
 * types. These types include IA5String, VisibleString, PrintableString, and
 * NumericString.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param ppvalue      Pointer to a character string pointer variable to
 *                       receive the decoded string. The string is stored as a
 *                       standard null-terminated C string. Memory is allocated
 *                       for the string by the ::rtxMemAlloc function.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param tag          The ASN.1 tag to be decoded. This parameter is passed
 *                       using the ASN1C internal tag representation. It is
 *                       passed as an unsigned 32-bit integer. This parameter
 *                       only has meaning if the tagging parameter specifies
 *                       explicit decoding.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmCharStr (OSCTXT *pctxt, const char** ppvalue,
                                 ASN1TagType tagging, ASN1TAG tag, int length);

/**
 * This function tests for the end of a constructed context.  It is used
 * in loop control statements to determine when a block of repeating
 * elements has ended.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pccb         Pointer to a 'context control block' structure. This is
 *                       basically a loop control mechanism to keep the
 *                       variable associated with parsing a nested constructed
 *                       element straight.
 * @return             A result of testing:
 *                       - TRUE, if end of current context has been reached;
 *                       - FALSE, otherwise.
 */
EXTERNBER OSBOOL berDecStrmCheckEnd (OSCTXT* pctxt, ASN1CCB* pccb);

/**
 * This function decodes a variable of the ASN.1 BIT STRING type. It will
 * allocate dynamic memory to store the decoded result.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param ppvalue      Pointer to a pointer variable to receive the decoded bit
 *                       string. Dynamic memory is allocated to hold the
 *                       string.
 * @param pnbits       Pointer to an integer value to receive the decoded
 *                       number of bits.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmDynBitStr (OSCTXT *pctxt, const OSOCTET** ppvalue,
                                   OSUINT32* pnbits,
                                   ASN1TagType tagging, int length);

/**
 * This function decodes a variable of the ASN.1 OCTET STRING type. It will
 * allocate dynamic memory to store the decoded result.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param ppvalue      Pointer to a pointer variable to receive the decoded
 *                       octet string. Dynamic memory is allocated to hold the
 *                       string.
 * @param pnocts       Pointer to an integer value to receive the decoded
 *                       number of octets.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmDynOctStr (OSCTXT *pctxt, const OSOCTET** ppvalue,
                                   OSUINT32* pnocts,
                                   ASN1TagType tagging, int length);

/**
 * This function decodes a variable of the ASN.1 ENUMERATED type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     Pointer to a variable to receive the decoded enumerated
 *                       value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmEnum (OSCTXT *pctxt, OSINT32 *object_p,
                              ASN1TagType tagging, int length);

/**
 * This function returns the length of the tag-length-value at the current
 * position in the stream.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @return             Total length (in bytes) of TLV component or
 *                       negative error code.
 */
EXTERNBER int berDecStrmGetTLVLength (OSCTXT* pctxt);

/**
 * This function decodes a variable of the ASN.1 INTEGER type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     Pointer to a variable to receive a decoded 32-bit
 *                       integer value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmInt (OSCTXT *pctxt, OSINT32 *object_p,
                             ASN1TagType tagging, int length);

/**
 * This function decodes an 8-bit variable of the ASN.1 INTEGER type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     Pointer to a variable to receive a decoded 8-bit integer
 *                       value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmInt8 (OSCTXT *pctxt, OSINT8 *object_p,
                              ASN1TagType tagging, int length);

/**
 * This function decodes a 16-bit variable of the ASN.1 INTEGER type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     Pointer to a variable to receive a decoded 16-bit
 *                       integer value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmInt16 (OSCTXT *pctxt, OSINT16 *object_p,
                               ASN1TagType tagging, int length);

/**
 * This function decodes a 64-bit variable of the ASN.1 INTEGER type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     Pointer to a variable to receive a decoded 64-bit
 *                       integer value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmInt64 (OSCTXT *pctxt, OSINT64 *object_p,
                               ASN1TagType tagging, int length);

/**
 * This function decodes a BER length determinant value.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param len_p        Pointer to a variable to receive the decoded length of
 *                       the tagged component. The returned value will either
 *                       be the actual length or the special constant
 *                       'ASN_K_INDEFLEN', which indicates indefinite length.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmLength (OSCTXT *pctxt, int *len_p);

/**
 * This function does a comparison between the end-of-contents octets (EOC) and
 * the tag at the current decode pointer position to determine if they match.
 * It then returns the result of the match operation. If match is not
 * successful, the decode pointer will be unchanged; otherwise the pointer will
 * be moved behind the EOC.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @return             Completion status of operation:
 *                       - 0 (0) - match is successful;
 *                       - RTERR_IDNOTFOU - match is not successful;
 *                       - negative value - error occurred.
 */
EXTERNBER int berDecStrmMatchEOC (OSCTXT* pctxt);

/**
 * This function does a comparison between the given tag and the tag at the
 * current decode pointer position to determine if they match. It then returns
 * the result of the match operation.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param tag          Tag variable to match.
 * @param len_p        Pointer to a variable to receive the decoded length of
 *                       the tagged component. The returned value will either
 *                       be the actual length or the special constant
 *                       'ASN_K_INDEFLEN', which indicates indefinite length.
 * @param advance      The boolean value indicates the behaviour of the decode
 *                       pointer. If it is set to TRUE and match is successful,
 *                       then the pointer will be moved behind the tag.
 *                       Otherwise, it will be left unchanged.
 * @return             Completion status of operation:
 *                       - 0 (0) - match is successful;
 *                       - RTERR_IDNOTFOU - match is not successful;
 *                       - negative value - error occurred.
 */
EXTERNBER int berDecStrmMatchTag (OSCTXT* pctxt, ASN1TAG tag, int *len_p,
                                  OSBOOL advance);

/**
 * This function moves the decode pointer to the next tagged element in the
 * decode stream. It is useful for use in an error handling callback function
 * because it allows an unknown or bogus element to be skipped.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmNextElement (OSCTXT *pctxt);

/**
 * This function decodes an ASN.1 NULL placeholder.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmNull (OSCTXT *pctxt, ASN1TagType tagging);

/**
 * This function decodes a value of the ASN.1 OBJECT IDENTIFIER type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     Pointer to value to receive decoded result. The
 *                       ASN1OBJID structure contains an integer to hold the
 *                       number of subidentifiers and an array to hold the
 *                       subidentifier values.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmObjId (OSCTXT *pctxt, ASN1OBJID *object_p,
                               ASN1TagType tagging,
                               int length);

/**
 * This function decodes a value of the ASN.1 OBJECT IDENTIFIER type using
 * 64-bit subidentifiers.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     Pointer to value to receive decoded result. The
 *                       ASN1OID64 structure contains an integer to hold the
 *                       number of subidentifiers and an array of 64-bit
 *                       unsigned integers to hold the subidentifier values.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmObjId64 (OSCTXT *pctxt, ASN1OID64 *object_p,
                                 ASN1TagType tagging,
                                 int length);

/**
 * This function decodes a variable of the ASN.1 OCTET STRING type into a
 * static memory structure. This function call is generated by ASN1C to decode
 * a sized octet string production.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pvalue       Pointer to a variable to receive the decoded octet
 *                       string. This is assumed to be a static array large
 *                       enough to hold the number of octets specified in the
 *                       *pnocts input parameter.
 * @param pnocts       Pointer to an integer variable containing the size (in
 *                       octets) of the sized ASN.1 octet string. An error will
 *                       occur if the number of octets in the decoded string is
 *                       larger than this value. Note that this is also used as
 *                       an output variable - the actual number of decoded
 *                       octets will be returned in this variable.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmOctStr (OSCTXT *pctxt, OSOCTET* pvalue,
                                OSUINT32* pnocts,
                                ASN1TagType tagging,
                                int length);

/**
 * This function decodes a variable of an ASN.1 open type. This includes the
 * now deprecated ANY and ANY DEFINED BY types from the 1990 standard as well
 * as other types defined to be open in the new standards (for example, a
 * variable type declaration in an X.681 Information Object Class definition).
 * \n Decoding is accomplished by returning a pointer to the encoded message
 * component at the current decode pointer location and skipping to the next
 * field. The caller must then call additional decode functions to further
 * decode the component.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p2    A pointer to a pointer (**) to hold the address of a
 *                       byte buffer. This buffer will contain a copy of the
 *                       encoded message component located at the current
 *                       decode pointer location.
 * @param pnumocts    Pointer to an integer value to receive the decoded
 *                       number of octets.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmOpenType (OSCTXT *pctxt,
                                  const OSOCTET** object_p2,
                                  OSUINT32* pnumocts);

/**
 * This function is similar to the ::berDecStrmOpenType. The difference is that
 * after it decodes the open type data into an ASN1OpenType structure, it
 * appends the structure to a doubly-linked list. This function is typically
 * used for decoding extension items in extensible types. The user is provided
 * with a list of each extension item in the message.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pElemList    The pointer to linked list structure. The decoded
 *                       ASN1OpenType structure will be appended to this list.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmOpenTypeAppend (OSCTXT *pctxt,
                                        OSRTDList *pElemList);

/**
 * This function is similar to the ::berDecStrmOpenType function except that it
 * is used in places where open type extensions are specified. An open type
 * extension is defined as an extensibility marker on a constructed type
 * without any extension elements defined (for example, SEQUENCE { a INTEGER, :
 * }). The difference is that this is an implicit field that can span one or
 * more elements whereas the standard Open Type is assumed to be a single
 * tagged field.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param ccb_p        Pointer to a 'context control block' structure. This is
 *                       basically a loop control mechanism to keep the
 *                       variable associated with parsing a nested constructed
 *                       element straight.
 * @param tag          Next expected tag value (or ASN_K_NOTAG value if last
 *                       field). The routine will loop through elements until
 *                       matching tag found or some other error occurs.
 * @param pElemList    The pointer to linked list structure. The decoded
 *                       ASN1OpenType structure will be appended to this list.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmOpenTypeExt (OSCTXT *pctxt, ASN1CCB* ccb_p,
                                     ASN1TAG tag,
                                     OSRTDList *pElemList);

/**
 * This function "peeks" the tag and length at the current decode pointer
 * location and returns the results. The decode pointer location is left as
 * it was before call to this function.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param ptag         Pointer to a variable to receive the decoded ASN.1 tag
 *                       value.
 * @param plen         Pointer to a variable to receive the decoded length of
 *                       the tagged component. The returned value will either
 *                       be the actual length or the special constant
 *                       'ASN_K_INDEFLEN', which indicates indefinite length.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmPeekTagAndLen
   (OSCTXT* pctxt, ASN1TAG* ptag, int* plen);

/**
 * This function reads a complete tag-length-value (TLV) from the decode
 * stream into a dynamic memory buffer.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param ppbuf        Pointer to a pointer to a buffer to receive the
 *                       allocated memory pointer.  The memory is allocated
 *                       using the rtxMem* memory management functions.  This
 *                       may be freed using rtxMemFree or rtxMemFreePtr.
 * @return             The total number of octets read into the buffer, or
 *                       negative error code.
 */
EXTERNBER int berDecStrmReadDynTLV (OSCTXT* pctxt, OSOCTET** ppbuf);

/**
 * This function reads a complete tag-length-value (TLV) from the decode
 * stream into the given memory buffer.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param buf          Pointer to a buffer to receive a data.
 * @param bufsiz       Size of the buffer.
 * @return             The total number of octets read into the buffer, or
 *                       negative error code.
 */
EXTERNBER int berDecStrmReadTLV (OSCTXT* pctxt, OSOCTET* buf, size_t bufsiz);

/**
 * This function decodes a variable of the binary encoded ASN.1 REAL type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     Pointer to a variable to receive the decoded real value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmReal (OSCTXT *pctxt, OSREAL *object_p,
                              ASN1TagType tagging, int length);

/**
 * This function decodes a value of the decimal encoded ASN.1 REAL type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     Pointer to a character pointer variable to receive the
 *                       decoded result. Dynamic memory is allocated
 *                       for the variable using the ::rtxMemAlloc function.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmReal10 (OSCTXT *pctxt, const char** object_p,
                                ASN1TagType tagging, int length);

/**
 * This function decodes a value of the ASN.1 RELATIVE-OID type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     Pointer to value to receive decoded result. The
 *                       ASN1OBJID structure contains an integer to hold the
 *                       number of subidentifiers and an array to hold the
 *                       subidentifier values.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmRelativeOID (OSCTXT *pctxt, ASN1OBJID *object_p,
                                     ASN1TagType tagging,
                                     int length);

/**
 * This function decodes the tag at the current decode pointer location and
 * returns the results.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param tag_p        Pointer to a variable to receive the decoded ASN.1 tag
 *                       value.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmTag (OSCTXT* pctxt, ASN1TAG *tag_p);

/**
 * This function decodes the tag and length at the current decode pointer
 * location and returns the results.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param tag_p        Pointer to a variable to receive the decoded ASN.1 tag
 *                       value.
 * @param len_p        Pointer to a variable to receive the decoded length of
 *                       the tagged component. The returned value will either
 *                       be the actual length or the special constant
 *                       'ASN_K_INDEFLEN', which indicates indefinite length.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmTagAndLen (OSCTXT *pctxt, ASN1TAG *tag_p,
                                   int *len_p);

/**
 * This function does a quick test on end-of-contents octets at the current
 * decode pointer. In contrast to the ::berDecStrmMatchEOC this function never
 * moves the decode pointer and it returns a boolean result of testing.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param ccb_p        Pointer to a 'context control block' structure. This is
 *                       basically a loop control mechanism to keep the
 *                       variable associated with parsing a nested constructed
 *                       element straight.
 * @return             A result of testing:
 *                       - TRUE, if EOC at the current decode pointer;
 *                       - FALSE, otherwise.
 */
EXTERNBER OSBOOL berDecStrmTestEOC (OSCTXT* pctxt, ASN1CCB* ccb_p);

/**
 * This function does a comparison between the given tag and the tag at the
 * current decode pointer position to determine if they match. It then returns
 * the result of the match operation. In contrary to the
 * \c berDecStrmMatchTag function this one does NOT log error, if tags are
 * not matched.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param tag          Tag variable to match.
 * @param len_p        Pointer to a variable to receive the decoded length of
 *                       the tagged component. The returned value will either
 *                       be the actual length or the special constant
 *                       'ASN_K_INDEFLEN', which indicates indefinite length.
 * @param advance      The boolean value indicates the behaviour of the decode
 *                       pointer. If it is set to TRUE and match is successful,
 *                       then the pointer will be moved behind the tag.
 *                       Otherwise, it will be left unchanged.
 * @return             Completion status of operation:
 *                       - 0 (0) - match is successful;
 *                       - RTERR_IDNOTFOU - match is not successful;
 *                       - another negative value - error occurred.
 */
EXTERNBER int berDecStrmTestTag (OSCTXT* pctxt, ASN1TAG tag, int *len_p,
                                 OSBOOL advance);

/**
 * This function decodes a variable of the unsigned variant of ASN.1 INTEGER
 * type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     Pointer to a variable to receive a decoded unsigned
 *                       32-bit integer value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmUInt (OSCTXT *pctxt, OSUINT32 *object_p,
                              ASN1TagType tagging, int length);

/**
 * This function decodes an 8-bit variable of the unsigned variant of ASN.1
 * INTEGER type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     Pointer to a variable to receive a decoded unsigned
 *                       8-bit integer value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmUInt8 (OSCTXT *pctxt, OSUINT8 *object_p,
                               ASN1TagType tagging, int length);

/**
 * This function decodes a 16-bit variable of the unsigned variant of ASN.1
 * INTEGER type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     Pointer to a variable to receive a decoded unsigned
 *                       16-bit integer value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmUInt16 (OSCTXT *pctxt, OSUINT16 *object_p,
                                ASN1TagType tagging, int length);

/**
 * This function decodes a 64-bit variable of the unsigned variant of ASN.1
 * INTEGER type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     Pointer to a variable to receive a decoded unsigned
 *                       64-bit integer value.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmUInt64 (OSCTXT *pctxt, OSUINT64 *object_p,
                                ASN1TagType tagging, int length);

/**
 * This function decodes a variable an ASN.1 32-bit character UniversalString
 * type.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param object_p     Pointer to a structure variable to receive the decoded
 *                       string. The string is stored as an array of unsigned
 *                       integer characters. Memory is allocated for the string
 *                       by the ::rtxMemAlloc function.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param length       The length, in octets, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNBER int berDecStrmUnivStr (OSCTXT* pctxt,
                                 Asn132BitCharString* object_p,
                                 ASN1TagType tagging, int length);
/** @} */

EXTERNBER int cerEncCanonicalSort (OSCTXT* pctxt,
                                   OSCTXT* pMemCtxt,
                                   OSRTSList* pList);
EXTERNBER void cerGetBufLocDescr (OSCTXT *pctxt, Asn1BufLocDescr* pDescr);
EXTERNBER void cerAddBufLocDescr (OSCTXT *pctxt, OSRTSList* pElemList,
                                  Asn1BufLocDescr* pDescr);

#ifdef __cplusplus
}
#endif

/* This macro will test for EOB */

#define BS_CHKEOB(pctxt) \
(((pctxt)->buffer.byteIndex + 2 > (pctxt)->buffer.size) ? TRUE : \
(((pctxt)->buffer.data[(pctxt)->buffer.byteIndex] == 0 && \
(pctxt)->buffer.data[(pctxt)->buffer.byteIndex + 1] == 0 ) ? \
TRUE : FALSE))

/* This macro will test for length overrun */

#define BS_CHKEND(pctxt,ccb_p) \
((ccb_p)->stat = 0, \
(((ccb_p)->len == ASN_K_INDEFLEN) ? berDecStrmTestEOC (pctxt,ccb_p) : \
(((int)(OSRTSTREAM_BYTEINDEX(pctxt) - (ccb_p)->bytes) >= (ccb_p)->len))))

/** @} */

#endif /* _ASN1BERSTREAM_H_ */
