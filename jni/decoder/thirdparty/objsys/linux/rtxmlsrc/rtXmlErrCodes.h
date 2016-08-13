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
 * @file rtXmlErrCodes.h
 * List of numeric status codes that can be returned by ASN1C run-time
 * functions and generated code.
 */
#ifndef _RTXMLERRCODES_H_
#define _RTXMLERRCODES_H_

#include "rtxsrc/rtxErrCodes.h"

/**
 * @defgroup xmlErrCodes XML run-time error status codes.
 * This is a list of status codes that can be returned by XML
 * run-time functions and generated code.  In many cases, additional
 * information and parameters for the different errors are stored in
 * the context structure at the time the error in raised.  This
 * additional information can be output using the \c rtxErrPrint or
 * \c rtxErrLogUsingCB run-time functions.
 * @{
 */

/*
   When adding error codes here, see also g_status_text in rtXmlError.c, where
   message formats for each code are defined.
*/

/** End of block marker. */
#define XML_OK_EOB      0x7fffffff

/** Maintained for backward compatibility. */
#define XML_OK_FRAG     XML_OK_EOB

/**
 * Error base.  XML specific errors start at this base number to
 * distinguish them from common and other error types.
 */
#define XML_E_BASE      -200

/**
 * General error
 */
#define XML_E_GENERR  (XML_E_BASE)

/**
 * An invalid XML symbol (character) was detected at the given point
 * in the parse stream.
 */
/* Message:
 * Invalid symbol '%s' detected at position %s in XML stream
 * Parameters:
 * 1. symbol name (string)
 * 2. position in XML stream (string)
 */
#define XML_E_INVSYMBOL (XML_E_BASE-1)

/**
 * Start/end tag mismatch.  The parsed end tag does not match the start
 * tag that was parsed earlier at this level.  Indicates document is not
 * well-formed.
 */
/* Message:
 * Start/end XML tags mismatch: parsed '%s', expected '%s'
 * Parameters:
 * 1. parsed end tag value (string)
 * 2. parsed start tag value (string)
 */
#define XML_E_TAGMISMATCH (XML_E_BASE-2)

/**
 * Duplicate attribute found.
 */
#define XML_E_DUPLATTR (XML_E_BASE-3)

/**
 * Bad character reference found.
 */
#define XML_E_BADCHARREF (XML_E_BASE-4)

/**
 * Invalid mode.
 */
#define XML_E_INVMODE  (XML_E_BASE-5)

/**
 * Unexpected end of file (document).
 */
#define XML_E_UNEXPEOF  (XML_E_BASE-6)

/**
 * Current tag is not matched to specified one. Informational code.
 */
#define XML_E_NOMATCH    (XML_E_BASE-7)

/**
 * Missing required element. This status code is returned by the decoder
 * when the decoder knows exactly which element is absent.
 */
#define XML_E_ELEMMISRQ (XML_E_BASE-8)

/**
 * Missing required elements.  This status code is returned by the decoder
 * when the number of elements decoded for a given content model group is
 * less then the required number of elements as specified in the schema.
 */
#define XML_E_ELEMSMISRQ (XML_E_BASE-9)

/**
 * The number of elements in a repeating collection was less than the
 * number of elements specified in the XSD minOccurs facet for this
 * type or element.
 */
#define XML_E_TOOFEWELEMS (XML_E_BASE-10)

/**
 * Unexpected start tag.
 */
#define XML_E_UNEXPSTARTTAG (XML_E_BASE-11)

/**
 * Unexpected end tag.
 */
#define XML_E_UNEXPENDTAG (XML_E_BASE-12)

/**
 * Expected identifier not found.
 */
#define XML_E_IDNOTFOU (XML_E_BASE-13)

/**
 * Unknown xsi:type.
 */
#define XML_E_INVTYPEINFO (XML_E_BASE-14)

/**
 * Namespace URI not defined for given prefix.  A namespace URI was not
 * defined using an xmlns attribute for the given prefix.
 */
/* Message:
 * Namespace URI for prefix '%s' not found
 * Parameters:
 * 1. namespace prefix (string)
 */
#define XML_E_NSURINOTFOU (XML_E_BASE-15)

/**
 * Keyref constraint has some key that not present in refered constraint.
 */
/* Message:
 * Key not found in '%s', fields: %s %s %s %s
 * Parameters:
 * 1. name of keyref constraint
 * 2 - 5. field values of failed key
 */
#define XML_E_KEYNOTFOU (XML_E_BASE-16)

/**
 * Key or unique constraint has duplicated key.
 */
/* Message:
 * Duplicate key found in '%s', fields: %s %s %s %s
 * Parameters:
 * 1. name of key/unique constraint
 * 2 - 5. field values of failed key
 */
#define XML_E_DUPLKEY (XML_E_BASE-17)

/**
 * Some key has no full set of fields. It is not valid for key constraint.
 */
/* Message:
 * Field #%s absent in some key of '%s'
 * Parameters:
 * 1. index of absent field
 * 2. name of key constraint
 */
#define XML_E_FLDABSENT (XML_E_BASE-18)

/**
 * Some key has more than one value for field.
 */
/* Message:
 * Key has duplicate field #%s in '%s''
 * Parameters:
 * 1. index of duplicate field
 * 2. name of constraint
 */
#define XML_E_DUPLFLD (XML_E_BASE-19)

/**
 * An element was not empty when expected.
 */
#define XML_E_NOTEMPTY (XML_E_BASE-20)

/**
 * @} xmlErrCodes
 */
#endif
