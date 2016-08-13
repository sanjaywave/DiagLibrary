/*
 * Copyright (c) 2013-2013 Objective Systems, Inc.
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

/*! \mainpage
 * <h1>C JSON Runtime Library Functions</h1>
 *
 * The <b>C run-time JSON library</b> contains functions used to encode/decode
 * data in Javascript object notation (JSON).  These functions are identified
 * by their <i>rtJson</i> prefixes.
 *
 */
/**
 * @file osrtjson.h
 * JSON low-level C encode/decode functions.
 */
#ifndef _OSRTJSON_H_
#define _OSRTJSON_H_

#include "rtxsrc/osMacros.h"
#include "rtxsrc/osSysTypes.h"
#include "rtxsrc/rtxCommonDefs.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxMemory.h"

#include "rtjsonsrc/rtJsonExternDefs.h"

#ifndef EXTERNJSON
#ifdef BUILDJSONDLL
#define EXTERNJSON __declspec(dllexport)
#elif defined (USEJSONLDLL)
#define EXTERNJSON __declspec(dllimport)
#else
#define EXTERNJSON
#endif /* BUILDJSONDLL */
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct OSXMLElemIDRec;

/* JSON specific context flags */

#define OSJSONATTR      0x00008000  /* encode string value as attribute */

/* JSON macros */

#define OSJSONPUTCOMMA(pctxt) \
if ((pctxt)->lastChar != '{' && (pctxt)->lastChar != ',' \
    && (pctxt)->lastChar != '\0') OSRTSAFEPUTCHAR (pctxt, ',');

extern const signed char decodeTable[];
#define BASE64TOINT(c) ((((unsigned)(c)) < 128) ? decodeTable [(c) - 40] : -1)

/* JSON states, if needed; this references the XML states used in the XML
 * runtime; this shouldn't pose a problem. */

#ifndef OSXMLState
typedef enum {
   OSXMLINIT,
   OSXMLHEADER,
   OSXMLSTART,
   OSXMLATTR,
   OSXMLDATA,
   OSXMLEND
} OSXMLState;
#endif

/* Other definitions if needed */

#ifndef DEFAULT_DOUBLE_PRECISION
#define DEFAULT_DOUBLE_PRECISION 11
#endif

#ifndef DEFAULT_FLOAT_PRECISION
#define DEFAULT_FLOAT_PRECISION 6
#endif

#ifndef OSUPCASE
/** The upper-case flag:  if set, hex strings will be encoded in upper case. */
#define OSUPCASE 0x00008000
#endif

#ifndef OSDecimalFmt
typedef struct OSDecimalFmt {
   OSINT8 totalDigits;        /* total significiant digits */
   OSINT8 fractionDigits;     /* maximum signficiant digits in fraction,
                                 precision */
   OSINT8 fractionMinDigits;  /* minimum digits in fraction, if
                                 fraction digits less than this value
                                 trailing zeros will be added. */
   OSINT8 integerMaxDigits;   /* Maximum digits in integer part; if it is 0
                                 and integer part is 0 then integer part
                                 will be omitted, like .3, or -.3 */
   OSINT8 integerMinDigits;   /* Minimum digits in integer part, leading zeros
                                 will be added if necessary. */
   OSBOOL signPresent;        /* Indicates, sign must present, even if value
                                 is positive. */
   OSBOOL pointPresent;       /* Indicates, decimal point must present, even
                                 if value's fraction is 0 */
   OSUINT8 nPatterns;         /* number of patterns stored in 'patterns' */
   const char* const* patterns; /* patterns, used to verify value format is
                                 correct */
} OSDecimalFmt;
#endif

#ifndef OSDoubleFmt
typedef struct OSDoubleFmt {
   OSINT8 totalDigits;        /* total significiant digits */
   OSINT8 fractionDigits;     /* maximum signficiant digits in fraction,
                                 precision */
   OSINT8 fractionMinDigits;  /* minimum digits in fraction, if
                                 fraction digits less than this value
                                 trailing zeros will be added. */
   OSINT8 integerMaxDigits;   /* Maximum digits in integer part; if it is 0
                                 and integer part is 0 then integer part
                                 will be omitted, like .3, or -.3 */
   OSINT8 integerMinDigits;   /* Minimum digits in integer part, leading zeros
                                 will be added if necessary. */
   OSINT8 expSymbol;          /* 'E' or 'e' only; 0 if no exp is expected,
                                 -1 - default ('E') */
   OSINT16 expMinValue;       /* Minimum exponent value. By default - -infinity */
   OSINT16 expMaxValue;       /* Maximum exponent value. By default - infinity */
   OSINT8 expDigits;          /* Total digits in exponent part; if exponent's
                                 value is not enough, trailing zeros will be
                                 added */
   OSBOOL signPresent;        /* Indicates, sign must present, even if value
                                 is positive. */
   OSBOOL pointPresent;       /* Indicates, decimal point must present, even
                                 if value's fraction is 0 */
   OSBOOL expPresent;         /* Indicates, exponent must present, even
                                 if its value is 0 */
   OSBOOL expSignPresent;     /* Indicates, exponent sign must present, even
                                 if its value is > 0 */
} OSDoubleFmt;
#endif

/* run-time JSON function prototypes */

/**
 * @defgroup rtJsonEnc JSON encode functions.
 * @{
 */

/**
 * This function encodes a list of OSAnyAttr attributes in which
 * the name and value are given as a UTF-8 string.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       List of attributes.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncAnyAttr (OSCTXT* pctxt, const OSRTDList* pvalue);

/**
 * This function encodes a variable of the XSD integer type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncIntValue (OSCTXT* pctxt, OSINT32 value);

/**
 * This function encodes a variable of the XSD integer type. This version
 * of the function is used for 64-bit integer values.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncInt64Value (OSCTXT* pctxt, OSINT64 value);

/**
 * This function encodes a variable of the XSD base64Binary type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param nocts        Number of octets in the value string.
 * @param value        Value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncBase64StrValue (OSCTXT* pctxt, OSUINT32 nocts,
                            const OSOCTET* value);

/**
 * This function encodes a variable of the XSD boolean type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Boolean value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncBoolValue (OSCTXT* pctxt, OSBOOL value);

/**
 * This function encodes a numeric gYear value into a JSON string
 * representation.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncGYear (OSCTXT* pctxt, const OSXSDDateTime* pvalue);

/**
 * This function encodes a numeric gYearMonth value into a JSON string
 * representation.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncGYearMonth (OSCTXT* pctxt, const OSXSDDateTime* pvalue);

/**
 * This function encodes a numeric gMonth value into a JSON string
 * representation.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncGMonth (OSCTXT* pctxt, const OSXSDDateTime* pvalue);

/**
 * This function encodes a numeric gMonthDay value into a JSON string
 * representation.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncGMonthDay (OSCTXT* pctxt, const OSXSDDateTime* pvalue);

/**
 * This function encodes a numeric gDay value into a JSON string
 * representation.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncGDay (OSCTXT* pctxt, const OSXSDDateTime* pvalue);

/**
 * This function encodes a variable of the XSD 'date' type as a
 * string. This version of the function is used to encode an OSXSDDateTime
 * value into CCYY-MM-DD format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncDate (OSCTXT* pctxt, const OSXSDDateTime* pvalue);

/**
 * This function encodes a variable of the XSD 'time' type as a JSON
 * string.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncTime (OSCTXT* pctxt, const OSXSDDateTime* pvalue);

/**
 * This function encodes a numeric date/time value into a string
 * representation.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncDateTime (OSCTXT* pctxt, const OSXSDDateTime* pvalue);

/**
 * This function encodes a value of the XSD decimal type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param pFmtSpec     Pointer to format specification structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncDecimalValue (OSCTXT* pctxt, OSREAL value,
                        const OSDecimalFmt* pFmtSpec);

/**
 * This function encodes a value of the XSD double or float type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param pFmtSpec     Pointer to format specification structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncDoubleValue (OSCTXT* pctxt, OSREAL value,
                        const OSDoubleFmt* pFmtSpec);

/**
 * This function encodes a variable of the XSD float type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param pFmtSpec     Pointer to format specification structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncFloatValue (OSCTXT* pctxt, OSREAL value,
                        const OSDoubleFmt* pFmtSpec);

/**
 * This function encodes a variable of the XSD hexBinary type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param nocts        Number of octets in the value string.
 * @param data         Value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncHexStr (OSCTXT* pctxt, OSUINT32 nocts,
                         const OSOCTET* data);

/**
 * This function encodes a variable of the ASN.1 Bit string type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param nbits        Number of bits in the value string.
 * @param data         Value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncBitStrValue (OSCTXT* pctxt, OSUINT32 nbits,
                         const OSOCTET* data);
/**
 * This function adds indentation whitespace to the output stream.
 * The amount of indentation to add is determined by the level member
 * variable in the context structure and the OSXMLINDENT constant value.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncIndent (OSCTXT* pctxt);

/**
 * This function encodes a JSON object containing a string value.
 *
 * @param pctxt        Pointer to context block structure.
 * @param name         Name token to be encoded.
 * @param value        Value as a character string to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncStringObject (OSCTXT* pctxt,
                        const OSUTF8CHAR* name, const OSUTF8CHAR* value);

/**
 * This function encodes a JSON object containing a string value.
 *
 * @param pctxt        Pointer to context block structure.
 * @param name         Name token to be encoded.
 * @param nameLen      Length of the name token to be encoded.
 * @param value        Value as a character string to be encoded.
 * @param valueLen     Length of the value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncStringObject2 (OSCTXT* pctxt,
                        const OSUTF8CHAR* name, size_t nameLen,
                        const OSUTF8CHAR* value, size_t valueLen);

/**
 * This function encodes a name/value pair. The value is a character string.
 *
 * @param pctxt        Pointer to context block structure.
 * @param name         Name token to be encoded.
 * @param value        Value as a character string to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncStringPair (OSCTXT* pctxt,
                        const OSUTF8CHAR* name, const OSUTF8CHAR* value);

/**
 * This function encodes a name/value pair. The value is a character string.
 *
 * @param pctxt        Pointer to context block structure.
 * @param name         Name token to be encoded.
 * @param nameLen      Length of the name token to be encoded.
 * @param value        Value as a character string to be encoded.
 * @param valueLen     Length of the value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncStringPair2 (OSCTXT* pctxt,
                        const OSUTF8CHAR* name, size_t nameLen,
                        const OSUTF8CHAR* value, size_t valueLen);

/**
 * This function encodes a variable of the XSD string type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        XML string value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncStringValue (OSCTXT* pctxt, const OSUTF8CHAR* value);

/**
 * This function encodes a variable of the XSD string type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        XML string value to be encoded.
 * @param valueLen     Length of the XML string to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncStringValue2 (OSCTXT* pctxt,
                        const OSUTF8CHAR* value, size_t valueLen);

/**
 * This function encodes a variable of Unicode value.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Unicode characters to be encoded.
 * @param nchars       Number of Unicode characters to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncUnicodeData (OSCTXT* pctxt,
                        const OSUNICHAR* value, OSUINT32 nchars);

/**
 * This function encodes a variable of UCS-4 value.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        UCS-4 characters to be encoded.
 * @param nchars       Number of characters to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncUCS4Data (OSCTXT* pctxt,
                        const OS32BITCHAR* value, OSUINT32 nchars);


/**
 * This function encodes a variable of the XSD unsigned integer type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncUIntValue (OSCTXT* pctxt, OSUINT32 value);

/**
 * This function encodes a variable of the XSD integer type. This version
 * of the function is used when constraints cause an unsigned 64-bit integer
 * variable to be used.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonEncUInt64Value (OSCTXT* pctxt, OSUINT64 value);

/**
 * @}
 */

/**
 * @defgroup rtJsonDec JSON decode functions.
 * @{
 */

/**
 * This function decodes an arbitrary block of JSON-encoded data into
 * a string variable.  In this case, the expected format is element name
 * : JSON encoded data.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param ppvalue      A pointer to a variable to receive the decoded
 *                       JSON text.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecAnyElem (OSCTXT* pctxt, OSUTF8CHAR** ppvalue);

/**
 * This function decodes an arbitrary block of JSON-encoded data into
 * a string variable.  In this case, the expected format is a complete
 * JSON encoded data fragment.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param ppvalue      A pointer to a variable to receive the decoded
 *                       JSON text.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecAnyType (OSCTXT* pctxt, OSUTF8CHAR** ppvalue);

/**
 * This function decodes a contents of a Base64-encode binary string into a
 * static memory structure. The octet string must be Base64 encoded. This
 * function call is used to decode a sized base64Binary string production.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pvalue       A pointer to a variable to receive the decoded bit
 *                       string. This is assumed to be a static array large
 *                       enough to hold the number of octets specified in the
 *                       bufsize input parameter.
 * @param pnocts       A pointer to an integer value to receive the decoded
 *                       number of octets.
 * @param bufsize      The size (in octets) of
 *                       the sized octet string. An error will occur if
 *                       the number of octets in the decoded string is larger
 *                       than this value.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecBase64Str (OSCTXT* pctxt,
                        OSOCTET* pvalue, OSUINT32* pnocts,
                        size_t bufsize);

/**
 * This function decodes a contents of a Base64-encode binary string.
 * The octet string must be Base64 encoded. This
 * function will allocate dynamic memory to store the decoded result.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pvalue       A pointer to a dynamic octet string structure to receive
 *                       the decoded octet string. Dynamic memory is allocated
 *                       for the string using the ::rtxMemAlloc function.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecDynBase64Str (OSCTXT* pctxt, OSDynOctStr* pvalue);

/**
 * This function decodes a variable of the boolean type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to a variable to receive the decoded boolean
 *                       value.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecBool (OSCTXT* pctxt, OSBOOL* pvalue);

/**
 * This function decodes a variable of the XSD 'date' type.
 * Input is expected to be a string of characters returned
 * by a JSON parser. The string should have CCYY-MM-DD format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecDate (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes a variable of the XSD 'time' type.
 * Input is expected to be a string of characters returned
 * by a JSON parser. The string should have one of following formats:
 *
 *                    (1) hh-mm-ss.ss  used if tz_flag = false
 *                    (2) hh-mm-ss.ssZ used if tz_flag = false and tzo = 0
 *                    (3) hh-mm-ss.ss+HH:MM if tz_flag = false and tzo > 0
 *                    (4) hh-mm-ss.ss-HH:MM-HH:MM
 *                                          if tz_flag = false and tzo < 0
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecTime (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes a variable of the XSD 'dateTime' type.
 * Input is expected to be a string of characters returned
 * by an JSON parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecDateTime (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes a variable of the XSD 'gYear' type.
 * Input is expected to be a string of characters returned
 * by a JSON parser. The string should have CCYY[-+hh:mm|Z] format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecGYear (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes a variable of the XSD 'gYearMonth' type.
 * Input is expected to be a string of characters returned
 * by a JSON parser. The string should have CCYY-MM[-+hh:mm|Z] format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecGYearMonth (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes a variable of the XSD 'gMonth' type.
 * Input is expected to be a string of characters returned
 * by a JSON parser. The string should have --MM[-+hh:mm|Z] format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecGMonth (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes a variable of the XSD 'gMonthDay' type.
 * Input is expected to be a string of characters returned
 * by a JSON parser. The string should have --MM-DD[-+hh:mm|Z] format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecGMonthDay (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes a variable of the XSD 'gDay' type.
 * Input is expected to be a string of characters returned
 * by a JSON parser. The string should have ---DD[-+hh:mm|Z] format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecGDay (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes the contents of a decimal data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by a JSON parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to 64-bit double value to receive
 *                       decoded result.
 * @param totalDigits  The total number of digits in the decimal value.
 * @param fractionDigits The number of fractional digits in the decimal value.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecDecimal (OSCTXT* pctxt, OSREAL* pvalue,
                        int totalDigits, int fractionDigits);

/**
 * This function decodes the contents of a float or double data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by a JSON parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to 64-bit double value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecDouble (OSCTXT* pctxt, OSREAL* pvalue);

/**
 * This function decodes the contents of a hexBinary string into a static
 * memory structure.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by a JSON parser.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pvalue       A pointer to a variable to receive the decoded bit
 *                       string. This is assumed to be a static array large
 *                       enough to hold the number of octets specified in the
 *                       bufsize input parameter.
 * @param pnocts       A pointer to an integer value to receive the decoded
 *                       number of octets.
 * @param bufsize      The size (in octets) of
 *                       the sized octet string. An error will occur if
 *                       the number of octets in the decoded string is larger
 *                       than this value.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecHexStr (OSCTXT* pctxt, OSOCTET* pvalue,
                        OSUINT32* pnocts, size_t bufsize);

/**
 * This function decodes a contents of a hexBinary string. This
 * function will allocate dynamic memory to store the decoded result.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by a JSON parser.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pvalue       A pointer to a dynamic octet string structure to receive
 *                       the decoded octet string. Dynamic memory is allocated
 *                       to hold the string using the ::rtxMemAlloc function.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecDynHexStr (OSCTXT* pctxt, OSDynOctStr* pvalue);

/**
 * This function decodes a variable of the ASN.1 Bit string type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param nbits        A pointer to an unsigned integer to receive the number
 *                      of bits in the bit string.
 * @param data         A pointer to an OSOCTET array that will receive the
 *                      decoded bit string; the array will be allocated by
 *                      the decoding function.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecDynBitStr
   (OSCTXT* pctxt, OSUINT32 *nbits, OSOCTET **data);

/**
 * This function decodes a variable of the ASN.1 Bit string type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param nbits        A pointer to an unsigned integer to receive the number
 *                      of bits in the bit string.
 * @param data         A pointer to an OSOCTET array that will receive the
 *                      decoded bit string.  The array must be preallocated.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecBitStrValue
   (OSCTXT* pctxt, OSUINT32 *nbits, OSOCTET *data);

/**
 * This function decodes the contents of an 8-bit integer data type
 * (i.e. a signed byte type in the range of -128 to 127).
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by a JSON parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to 8-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecInt8Value (OSCTXT* pctxt, OSINT8* pvalue);

/**
 * This function decodes the contents of a 16-bit integer data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by a JSON parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to 16-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecInt16Value (OSCTXT* pctxt, OSINT16* pvalue);

/**
 * This function decodes the contents of a 32-bit integer data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by a JSON parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to 32-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecInt32Value (OSCTXT* pctxt, OSINT32* pvalue);
#define rtJsonDecIntValue rtJsonDecInt32Value

/**
 * This function decodes the contents of a 64-bit integer data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by a JSON parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to 64-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecInt64Value (OSCTXT* pctxt, OSINT64* pvalue);

/**
 * This function decodes the contents of an unsigned 8-bit integer data type
 * (i.e. a signed byte type in the range of 0 to 255).
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by a JSON parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to unsigned 8-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecUInt8Value (OSCTXT* pctxt, OSUINT8* pvalue);

/**
 * This function decodes the contents of an unsigned 16-bit integer data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by a JSON parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to unsigned 16-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecUInt16Value (OSCTXT* pctxt, OSUINT16* pvalue);

/**
 * This function decodes the contents of an unsigned 32-bit integer data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by a JSON parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to unsigned 32-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecUInt32Value (OSCTXT* pctxt, OSUINT32* pvalue);
#define rtJsonDecUIntValue rtJsonDecUInt32Value

/**
 * This function decodes the contents of an unsigned 64-bit integer data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by a JSON parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to unsigned 64-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecUInt64Value (OSCTXT* pctxt, OSUINT64* pvalue);

/**
 * This function parses the next byte and matches it with given character.
 *
 * @param pctxt        Pointer to context block structure.
 * @param ch           The character to be matched.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecMatchChar (OSCTXT* pctxt, OSUTF8CHAR ch);

/**
 * This function parses the next object start tag that matches with
 * one of the names in the given name array. It also matches the
 * the leading '{' of the object and ':' character after the name.
 *
 * @param pctxt        Pointer to context block structure.
 * @param nameArray    Array of names to be matched.
 * @param numNames     Number of names in the name array
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecMatchObjectStart
(OSCTXT* pctxt, const OSUTF8NameAndLen* nameArray, size_t numNames);

/**
 * This function decodes a JSON string and matches with a given token.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param token        The token to be matched.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecMatchToken (OSCTXT* pctxt, const OSUTF8CHAR* token);

/**
 * This function decodes a JSON string and matches with a given token.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param token        The token to be matched.
 * @param tokenLen     The length of the token to be matched.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecMatchToken2
(OSCTXT* pctxt, const OSUTF8CHAR* token, size_t tokenLen);

/**
 * This function decodes a name/value pair.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to an structure to receive the
 *                       decoded name and value.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecNameValuePair (OSCTXT* pctxt, OSUTF8NVP* pvalue);

/**
 * This function read the next character.
 * It does not decode the value.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param pch           A pointer to a variable to receive the next character.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecPeekChar (OSCTXT* pctxt, OSUTF8CHAR* pch);

/**
 * This function read the next character. It does not decode the value.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @return              The peeked character, or null if there is a failure.
 *                         The error will be logged in the context.
 */
EXTERNJSON char rtJsonDecPeekChar2 (OSCTXT* pctxt);

/**
 * This function decodes the contents of a JSON object containing a
 * character string.
 * Input is expected to be a complete JSON object returned by
 * a JSON parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param name         The name token.
 * @param ppvalue      Pointer to an string structure to receive the
 *                       decoded string.  Memory is allocated for the string
 *                       using the run-time memory manager.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecStringObject
(OSCTXT* pctxt, const OSUTF8CHAR* name, OSUTF8CHAR** ppvalue);

/**
 * This function decodes the contents of a string data type.
 * This type contains a pointer to a UTF-8 characer string.
 * Input is expected to be a string of UTF-8 characters returned by
 * a JSON parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param ppvalue      Pointer to an string structure to receive the
 *                       decoded string.  Memory is allocated for the string
 *                       using the run-time memory manager.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecStringValue (OSCTXT* pctxt, OSUTF8CHAR** ppvalue);

/**
 * This function decodes the contents of an XML string data type.
 * This type contains a pointer to a UTF-8 characer string plus flags
 * that can be set to alter the encoding of the string (for example, the
 * cdata flag allows the string to be encoded in a CDATA section).
 * Input is expected to be a string of UTF-8 characters returned by
 * a JSON parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to an XML string structure to receive the
 *                       decoded string.  Memory is allocated for the string
 *                       using the run-time memory manager.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNJSON int rtJsonDecXmlStringValue (OSCTXT* pctxt, OSXMLSTRING* pvalue);

EXTERNJSON int rtJsonDecSkipWhitespace (OSCTXT* pctxt);

/**
 * This function parses the next start tag and finds the index of
 * the element name in the descriptor table.
 *
 * @param pctxt        Pointer to context block structure.
 * @param nameArray    Elements descriptor table.
 * @param nrows        Number of descriptors in table.
 * @return             Completion status of operation:
 *                       - positive or zero value is element identifier,
 *                       - negative return value is error.
 */
EXTERNJSON size_t rtJsonGetElemIdx
(OSCTXT* pctxt, const OSUTF8NameAndLen nameArray[], size_t nrows);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif
