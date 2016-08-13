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

#ifndef _ASN1XML_H_
#define _ASN1XML_H_

#include "rtsrc/asn1type.h"
#include "rtxmlsrc/osrtxml.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup asn1xml ASN.1-XML encode/decode functions.
 * @{
 */


/**
 * This function decodes the contents of an ASN.1 Generalized type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.  The encoding is expected to be in xsd:dateTime
 * format (not in the XER format).
 *
 * @param pctxt        Pointer to context block structure.
 * @param outdata      Pointer to a pointer to receive decoded UTF-8
 *                       string.  Memory is allocated for this string
 *                       using the run-time memory manager.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtAsn1XmlpDecGenTime
(OSCTXT* pctxt, const char** outdata);

/**
 * This function decodes the contents of an ASN.1 REAL type.
 * Input is expected to be returned by an XML pull parser.
 *
 * This method recognizes the basic-XER encoding for a REAL, which consists of
 * the elements <PLUS-INFINITY/>, <MINUS-INFINITY/>, and <NOT-A-NUMBER/>, and
 * realnumber with an optional negative sign.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to OSREAL to value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtAsn1XmlpDecReal (OSCTXT* pctxt, OSREAL* pvalue);

/**
 * This function decodes the contents of an ASN.1 OBJECT IDENTIFIER type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to ASN.1 object identifier value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtAsn1XmlpDecObjId (OSCTXT* pctxt, ASN1OBJID* pvalue);

/**
 * This function decodes the contents of an ASN.1 UNIVERSAL string type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param ppdata       Pointer to 32-bit character string value to receive
 *                       decoded result.
 * @param pnchars      Pointer to length value to recieve decoded length
 *                       in characters.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtAsn1XmlpDecUnivStr
(OSCTXT* pctxt, const OS32BITCHAR** ppdata, OSUINT32* pnchars);


/**
 * This function decodes the contents of an ASN.1 UTCTime type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.  The encoding is expected to be in xsd:dateTime
 * format (not in the XER format).
 *
 * @param pctxt        Pointer to context block structure.
 * @param outdata      Pointer to a pointer to receive decoded UTF-8
 *                       string.  Memory is allocated for this string
 *                       using the run-time memory manager.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtAsn1XmlpDecUTCTime
(OSCTXT* pctxt, const char** outdata);

/**
 * This function encodes a variable of the ASN.1 GeneralizedTime type.
 * It performs conversion from ASN.1 time format into the XML dateTime
 * format.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param value        A pointer to a null-terminated C character string to be
 *                       encoded. It should contain UTCTime in ASN.1 format.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param nsPrefix     XML namespace prefix.  If not null, will be
 *                       prepended to elemName to form a qualified name.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtAsn1XmlEncGenTime
(OSCTXT *pctxt, const char* value, const OSUTF8CHAR* elemName,
 const OSUTF8CHAR* nsPrefix);

/**
 * This function encodes a variable of the ASN.1 UTCTime type. It performs
 * conversion from ASN.1 time format into the XML dateTime format.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param value        A pointer to a null-terminated C character string to be
 *                       encoded. It should contain UTCTime in ASN.1 format.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param nsPrefix     XML namespace prefix.  If not null, will be
 *                       prepended to elemName to form a qualified name.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtAsn1XmlEncUTCTime
(OSCTXT *pctxt, const char* value, const OSUTF8CHAR* elemName,
 const OSUTF8CHAR* nsPrefix);

/**
 * This function encodes a variable of the ASN.1 OBJECT IDENTIFIER type.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pvalue       A pointer to an object identifier structure. This
 *                       structure contains an integer to hold the number of
 *                       subidentifers in the object and an array to hold the
 *                       subidentifier values.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param nsPrefix     XML namespace prefix.  If not null, will be
 *                       prepended to elemName to form a qualified name.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtAsn1XmlEncObjId
(OSCTXT *pctxt, const ASN1OBJID *pvalue, const OSUTF8CHAR* elemName,
 const OSUTF8CHAR* nsPrefix);

/**
 * This function encodes a variable of the ASN.1 REAL type.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pvalue       A pointer to OSREAL value.
 *
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param nsPrefix     XML namespace prefix.  If not null, will be
 *                       prepended to elemName to form a qualified name.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtAsn1XmlEncReal
(OSCTXT *pctxt, OSREAL value, const OSUTF8CHAR* elemName,
 const OSUTF8CHAR* nsPrefix);


/**
 * This function encodes a variable of the ASN.1 RELATIVE-OID type.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pvalue       A pointer to an object identifier structure. This
 *                       structure contains an integer to hold the number of
 *                       subidentifers in the object and an array to hold the
 *                       subidentifier values.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param nsPrefix     XML namespace prefix.  If not null, will be
 *                       prepended to elemName to form a qualified name.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtAsn1XmlEncRelOID
(OSCTXT *pctxt, const ASN1OBJID *pvalue, const OSUTF8CHAR* elemName,
 const OSUTF8CHAR* nsPrefix);

/**
 * This function encodes a variable of the ASN.1 open type. It copies
 * the data as it exists in the structure to the encode buffer or stream.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param data         A pointer to a buffer containing the open type data.
 * @param nocts        Number of bytes in the data buffer to encode.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param nsPrefix     XML namespace prefix.  If not null, will be
 *                       prepended to elemName to form a qualified name.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtAsn1XmlEncOpenType
(OSCTXT *pctxt, const OSOCTET* data, OSUINT32 nocts,
 const OSUTF8CHAR* elemName, const OSUTF8CHAR* nsPrefix);

/**
 * This function encodes an ASN.1 open type extension. This occurs in a
 * SEQUENCE or SET type when a ... is present.  The type is represented
 * as a list of open type structures.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pElemList    Linked list of ASN.1 open type structures.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtAsn1XmlEncOpenTypeExt (OSCTXT* pctxt, OSRTDList* pElemList);

/**
 * This function encodes a variable of the ASN.1 UNIVERSAL string type.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param value        Array of universal characters to be encoded.  Each
 *                       character is represented as a 32-bit integer.
 * @param nchars       Number of characters to encode.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          XML namespace information (prefix and URI).
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtAsn1XmlEncUnivStr
(OSCTXT* pctxt, const OS32BITCHAR* value, OSUINT32 nchars,
 const OSUTF8CHAR* elemName, OSXMLNamespace* pNS);



/**
 * This function formats a name-value XML pair into a name="value" attribute
 * string.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param name         A pointer to an XML element name.  A name must be
 *                       provided.
 * @param value        A pointer to the corresponding element value.
 * @param pAttrStr     The resulting name="value" string.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtAsn1XmlFmtAttrStr
(OSCTXT* pctxt, const OSUTF8CHAR* name, const OSUTF8CHAR* value,
 OSUTF8CHAR** pAttrStr);

/**
 * This function parses an XML name-value pair from an attribute string.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pAttrStr     The name-value string to be parsed.
 * @param pNVPair      A pointer to an XML name-value pair structure filled in
 *                       by invoking this method.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtAsn1XmlParseAttrStr
(OSCTXT* pctxt, const OSUTF8CHAR* pAttrStr, OSUTF8NVP* pNVPair);

/**
 * This function formats an attribute string and adds it to the attribute
 * list.
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 *
 * @param name         The name of the new attribute.
 *
 * @param value        The value of the new attribute.
 *
 * @param plist        The attribute list.
 */
EXTERNXML int rtAsn1XmlAddAnyAttr
 (OSCTXT* pctxt, const OSUTF8CHAR *name, const OSUTF8CHAR *value,
  OSRTDList* plist);

/**
 * This function decodes a bit string value.  The string consists of a
 * series of '1' and '0' characters.  This is the dynamic version in which
 * memory is allocated for the returned binary string variable.  Bits are
 * stored from MSB to LSB order.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to a variable to receive the decoded boolean
 *                       value.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtAsn1XmlpDecDynBitStr (OSCTXT* pctxt, ASN1DynBitStr* pvalue);

/**
 * This function decodes a list of space-separated bit strings and returns
 * each bit string as a separate item on the given list. The string consists
 * of a series of '1' and '0' characters. Memory is allocated
 * for the list nodes and token values using the rtx memory management
 * functions. Bits are stored from MSB to LSB order.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param plist        A pointer to a linked list structure to which the
 *                       parsed bit string values will be added.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecListOfASN1DynBitStr (OSCTXT* pctxt, OSRTDList* plist);

/**
 * This function decodes the contents of an ASN.1 RELATIVE-OID type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to ASN.1 object identifier value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtAsn1XmlpDecRelOID (OSCTXT* pctxt, ASN1OBJID *pvalue);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif
