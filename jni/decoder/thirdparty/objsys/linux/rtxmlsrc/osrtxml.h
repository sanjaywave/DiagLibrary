/*
 * Copyright (c) 2003-2012 Objective Systems, Inc.
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
 * <h1>C XML Runtime Library Functions</h1>
 *
 * The <b>C run-time XML library</b> contains functions used to encode/decode
 * XML data.  These functions are identified by their <i>rtXml</i> prefixes.
 *
 * The categories of functions provided are as follows:<ul>
 * <li>XML pull-parser code.</li>
 * <li>Functions functions to encode C types to XML.</li>
 * <li>Functions to decode XML to C data types.</li>
 * <li>Functions to encode XML element tags.</li>
 * <li>Functions to encode XML attributes in sorted order for C14N.</li>
 * <li>SAX parser interfaces.</li>
 * <li>Context management functions.</li>
 * </ul>
 */
/**
 * @file osrtxml.h
 * XML low-level C encode/decode functions.
 */
#ifndef _OSRTXML_H_
#define _OSRTXML_H_

#define OSXMLNS12

#include "rtxsrc/rtxCommon.h"
#include "rtxmlsrc/rtSaxDefs.h"
#include "rtxsrc/rtxDList.h"
#include "rtxsrc/rtxMemBuf.h"
#include "rtxmlsrc/rtXmlExternDefs.h"
#include "rtxmlsrc/rtXmlErrCodes.h"
#include "rtxmlsrc/rtXmlNamespace.h"

/* XML specific context flags */

#define OSUPCASE        0x00008000  /* convert characters to upper case */
#define OSTERMSTART     0x00004000  /* term for start elem (>) needed   */
#define OSEMPTYELEM     0x00002000  /* element is empty (no content)    */
#define OSQUALATTR      0x00001000  /* qualified attribute              */
#define OSXMLFRAG       0x00000800  /* XML fragment (not full doc)      */
#define OSXMLNSSET      0x00000400  /* Indicates namespaces are set     */
#define OSXMLC14N       0x00000200  /* Flag used to indicate XML canonical 
                                       encode when OSASN1XER is not set, or
                                       canonical XER when OSASN1XER is set. */
#define OSXSIATTR       0x00000100  /* add xsi ns decl to encoded msg   */
#define OSXMLNOCMPNS    0x00000080  /* match local names only           */
#define OSXSINIL        0x00000040  /* add xsi:nil decl to encoded msg  */
#define OSXMLNOBLANKS   0x00000020  /* remove blank elements            */
#define OSHASDEFAULT    0x00000010  /* decode should accept values which
                                       are empty after whitespace processing
									            because the element has a default
									            value							*/
#define OSASN1XER       0x00000008  /* if true, -xml code and runtime should
                                       produce XER encodings instead of
                                       Obj-Sys encodings                    */                                       

typedef enum {
   OSXMLUTF8,
   OSXMLUTF16,
   OSXMLUTF16BE,
   OSXMLUTF16LE,
   OSXMLLATIN1
} OSXMLEncoding;

typedef enum {
   OSSOAPNONE, OSSOAPHEADER, OSSOAPBODY, OSSOAPFAULT
} OSXMLSOAPMsgType;

typedef enum {
   OSXMLBOM_NO_BOM,
   OSXMLBOM_UTF32_BE,
   OSXMLBOM_UTF32_LE,
   OSXMLBOM_UTF16_BE,
   OSXMLBOM_UTF16_LE,
   OSXMLBOM_UTF8,
   OSXMLBOM_CHECK
} OSXMLBOM;

typedef struct OSXMLFacets {
   int totalDigits;
   int fractionDigits;
} OSXMLFacets;

typedef struct {
   const OSUTF8CHAR* value;
   size_t length;
} OSXMLStrFragment;

typedef struct {
   OSXMLStrFragment mQName;
   OSXMLStrFragment mLocalName;
   OSXMLStrFragment mPrefix;
} OSXMLNameFragments;

typedef struct OSXMLItemDescr {
   OSXMLStrFragment localName;
   OSINT16 nsidx; /* namespace index */
} OSXMLItemDescr;

typedef OSXMLItemDescr OSXMLAttrDescr;
typedef OSXMLItemDescr OSXMLElemDescr;

typedef struct OSXMLElemIDRec {
   OSXMLElemDescr descr;
   OSUINT16 id;
} OSXMLElemIDRec;

typedef struct OSXMLGroupDesc {
   int row;
   int num;
   int anyCase;
} OSXMLGroupDesc;

typedef struct OSXSDAnyType {
   OSXMLSTRING value;
   OSRTDList attrs;
} OSXSDAnyType;

typedef enum {
   OSXMLNSI_UNQUALIFIED = 0,
   OSXMLNSI_UNKNOWN = -1,
   OSXMLNSI_UNCHECKED = -2,
   OSXMLNSI_XSI = -3,
   OSXMLNSI_XMLNS = -4,
   OSXMLNSI_XML = -5,
   OSXMLNSI_SOAP_ENVELOPE = -6,
   OSXMLNSI_XSD = -7
} OSXMLNsIndex;

struct OSXMLReader;

#define OSXMLFRAGSEQUAL(frag1,frag2) \
(frag1.length==frag2.length && !memcmp(frag1.value,frag2.value,frag1.length))

#define OSXMLQNAMEEQUALS(xnamefrag,qnametext) rtxUTF8StrnEqual \
(xnamefrag.mQName.value, OSUTF8(qnametext), xnamefrag.mQName.length)

#define OSXMLSETUTF8DECPTR(pctxt,str) \
rtxInitContextBuffer (pctxt, OSRTSAFECONSTCAST (OSOCTET*, str), \
OSUTF8LEN (str))

#define IS_XMLNSATTR(name) \
((OSUTF8LEN(name) >= 5) && name[0] == 'x' && name[1] == 'm' && \
 name[2] == 'l' && name[3] == 'n' && name[4] == 's')

#define IS_XSIATTR(name) \
((OSUTF8LEN(name) >= 4) && name[0] == 'x' && name[1] == 's' && \
 name[2] == 'i' && name[3] == ':')

/* XML header declaration encoding strings */

static const char OSXMLHDRUTF8 [] = "UTF-8";
static const char OSXMLHDRUTF16 [] = "UTF-16";
static const char OSXMLHDRUTF16BE [] = "UTF-16BE";
static const char OSXMLHDRUTF16LE [] = "UTF-16LE";
static const char OSXMLHDRLATIN1 [] = "ISO-8859-1";

/*
 * NOTE: If a field is added to this structure, make sure to add code to
 * to rtXmlInitCtxtAppInfo in rtXmlContext.c to initialize it.
 */
typedef struct {
   OSFreeCtxtAppInfoPtr  pFreeFunc; /* Free function pointer            */
   OSResetCtxtAppInfoPtr pResetFunc;/* Reset function pointer           */
   OSUTF8CHAR* schemaLocation;  /* xsi:schemaLocation attribute         */
   OSUTF8CHAR* noNSSchemaLoc;   /* xsi:noNamespaceSchemaLocation attr   */
   OSUTF8CHAR* xsiTypeAttr;     /* xsi:type attribute value             */
   OSXMLEncoding encoding;      /* XML document encoding (ex. UTF-8)    */
   OSRTDList namespaceList;     /* list of OSXMLNamespace               */
   OSRTDList encodedNSList;     /* list of encoded OSXMLNamespace       */
   OSRTDList sortedAttrList;    /* list of sorted attributes            */
   OSXMLNSPfxLinkStack nsPfxLinkStack;
   OSXMLNSURITable nsURITable;
   OSRTMEMBUF  memBuf;          /* memory buffer for character method   */
   OSINT32 mSaxLevel;
   OSINT32 mSkipLevel;
   OSUINT32 maxSaxErrors;       /* maximum number of errors             */
   OSUINT32 errorsCnt;          /* counter of errors                    */
   OSUINT8 indent;              /* XML output indent                    */
   OSBOOL  mbCdataProcessed;    /* CDATA section was processed          */
   char    indentChar;          /* indent char, default is space (' ')  */
   OSUINT8 soapVersion;         /* SOAP version (11 or 12)              */
   OSXMLFacets facets;
   const OSUTF8CHAR* encodingStr;/* name of encoding ("UTF-8", "ISO-...")*/
   OSXMLBOM byteOrderMark;
   struct OSXMLReader* pXmlPPReader; /* pull-parser reader object       */
   /* fields for encode canonical XML*/
   OSRTBuffer savedBuffer;
   OSRTFLAGS savedFlags;
   OSOCTET* attrsBuff;
   size_t attrsBuffSize;
   size_t attrStartPos;
} OSXMLCtxtInfo;

/*
Options for encoding an ASN.1 REAL type to XML.
OSXMLREALOBJSYS: follow Obj-Sys XML rules
OSXMLREALBXER: follow basic-XER rules
OSXMLREALEXERMODS: follow extended-XER rules for when MODIFIED-ENCODINGS is
   given.
OSXMLREALEXERDEC: follow extended-XER rules for when DECIMAL is given.

*/
typedef enum {
   OSXMLREALOBJSYS,
   OSXMLREALBXER,
   OSXMLREALEXERMODS,
   OSXMLREALEXERDEC
} OSXMLREALEncoding;

typedef enum {
   OSXMLINIT,
   OSXMLHEADER,
   OSXMLSTART,
   OSXMLATTR,
   OSXMLDATA,
   OSXMLEND
} OSXMLState;

typedef struct OSXMLQName {
   const OSUTF8CHAR* nsPrefix;
   const OSUTF8CHAR* ncName;
} OSXMLQName;

struct OSSAXHandlerBase;

typedef struct OSIntegerFmt {
   OSINT8 integerMaxDigits;   /* Maximum digits in integer part;
                                 leading zeros will be added, if necessary. */
   OSBOOL signPresent;        /* Indicates, sign must present, even if value
                                 is positive. */
} OSIntegerFmt;

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

typedef struct {
   size_t offset;
   size_t length;
   size_t prefixLength;
   size_t nameLength;
} OSXMLSortedAttrOffset;

#define OSXMLINDENT     3

#ifdef XML_UNICODE
#define rtXmlErrAddStrParm rtxErrAddUniStrParm
#else
#define rtXmlErrAddStrParm rtxErrAddStrParm
#endif

/**
 * Whitespace treatment options.
 */
typedef enum {
   OSXMLWSM_PRESERVE = 0,
   OSXMLWSM_REPLACE,
   OSXMLWSM_COLLAPSE
} OSXMLWhiteSpaceMode;

#ifdef __cplusplus
extern "C" {
#endif

/* run-time XML function prototypes */

#ifndef rtXmlInitContext
/**
 * This function initializes a context variable for XML encoding or
 * decoding.
 *
 * @param pctxt         Pointer to OSCTXT structure
 */
EXTERNXML int rtXmlInitContext (OSCTXT* pctxt);
#endif

/**
 * This function initializes a context using a run-time key.  This form
 * is required for evaluation and limited distribution software.  The
 * compiler will generate a macro for rtXmlInitContext in the rtkey.h
 * file that will invoke this function with the generated run-time key.
 *
 * @param pctxt        The pointer to the context structure variable to be
 *                       initialized.
 * @param key          Key data generated by ASN1C compiler.
 * @param keylen       Key data field length.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlInitContextUsingKey
(OSCTXT* pctxt, const OSOCTET* key, size_t keylen);

/**
 * This function initializes the XML application info section of the
 * given context.
 *
 * @param pctxt         Pointer to OSCTXT structure
 */
EXTERNXML int rtXmlInitCtxtAppInfo (OSCTXT* pctxt);

/**
 * This function creates an XML document file input source.  The
 * document can then be decoded by invoking an XML decode function.
 *
 * @param pctxt        Pointer to context block structure.
 * @param filepath     Full pathname of XML document file to open.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlCreateFileInputSource (OSCTXT* pctxt, const char* filepath);

EXTERNXML OSBOOL rtXmlCmpQName
(const OSUTF8CHAR* qname1, const OSUTF8CHAR* name2, const OSUTF8CHAR* nsPrefix2);

EXTERNXML int rtXmlGetBase64StrDecodedLen
   (const OSUTF8CHAR* inpdata, size_t srcDataSize,
    size_t* pNumOcts, size_t* pSrcDataLen);

/**
 * This function frees a list of anyAttribute that is a member of
 * OSXSDAnyType structure.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pAnyAttrList Pointer to list of anyAttribute that is to be freed.
 */
EXTERNXML void rtXmlMemFreeAnyAttrs (OSCTXT* pctxt, OSRTDList* pAnyAttrList);

/**
 * @defgroup rtXmlDec XML decode functions.
 * @{
 */
/**
 * This function decodes the contents of a Base64-encoded binary data
 * type into a memory buffer. Input is expected to be a string of UTF-8
 * characters returned by an XML parser. The decoded data will be put
 * into the memory buffer starting from the current position and bit
 * offset. After all data is decoded the octet string may be fetched out.
 * <p>This function is normally used in the 'characters' SAX handler.
 *
 * @param pMemBuf      Memory buffer to which decoded binary data is to
 *                       be appended.
 * @param inpdata      Pointer to a source string to be decoded.
 * @param length       Length of the source string (in characters).
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecBase64Binary (OSRTMEMBUF* pMemBuf,
                                    const OSUTF8CHAR* inpdata,
                                    int length);

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
EXTERNXML int rtXmlDecBase64Str (OSCTXT* pctxt,
                                 OSOCTET* pvalue, OSUINT32* pnocts,
                                 OSINT32 bufsize);

/**
 * This function decodes a contents of a Base64-encode binary string into
 * the specified octet array. The octet string must be Base64 encoded. This
 * function call is used internally to decode both sized and non-sized
 * base64binary string production.
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
 * @param bufSize      A maximum size (in octets) of \c pvalue buffer.
 *                       An error will occur if
 *                       the number of octets in the decoded string is larger
 *                       than this value.
 * @param srcDataLen   An actual source data length (in octets) without whitespaces.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecBase64StrValue (OSCTXT* pctxt,
                                      OSOCTET* pvalue, OSUINT32* pnocts,
                                      size_t bufSize, size_t srcDataLen);

/**
 * This function will decode a variable of the XSD integer type. In this
 * case, the integer is assumed to be of a larger size than can fit in a C or
 * C++ long type (normally 32 or 64 bits). For example, parameters used to
 * calculate security values are typically larger than these sizes.
 *
 * These variables are stored in character string constant variables.
 * The radix should be 10. If it is necessary to
 * convert to another radix, then use rtxBigIntSetStr or rtxBigIntToString
 * functions.
 *
 * @param pctxt        Pointer to context block structure.
 * @param ppvalue      Pointer to a pointer to receive decoded UTF-8
 *                       string. Dynamic memory is allocated
 *                       for the variable using the rtMemAlloc function. The
 *                       decoded variable is represented as a string starting
 *                       with appropriate prefix.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecBigInt
(OSCTXT* pctxt, const OSUTF8CHAR** ppvalue);

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
EXTERNXML int rtXmlDecBool (OSCTXT* pctxt, OSBOOL* pvalue);

/**
 * This function decodes a variable of the XSD 'date' type.
 * Input is expected to be a string of characters returned
 * by an XML parser. The string should have CCYY-MM-DD format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecDate (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes a variable of the XSD 'time' type.
 * Input is expected to be a string of characters returned
 * by an XML parser. The string should have one of following formats:
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
EXTERNXML int rtXmlDecTime (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes a variable of the XSD 'dateTime' type.
 * Input is expected to be a string of characters returned
 * by an XML parser.

 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecDateTime (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes the contents of a decimal data type.
 * Input is expected to be a string of characters returned
 * by an XML parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to 64-bit double value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecDecimal (OSCTXT* pctxt, OSREAL* pvalue);

/**
 * This function decodes the contents of a float or double data type.
 * Input is expected to be a string of characters returned
 * by an XML parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to 64-bit double value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecDouble (OSCTXT* pctxt, OSREAL* pvalue);

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
 *                       the decoded octet string. Dynamic memory is allocated to
 *                       hold the string using the ::rtxMemAlloc function.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecDynBase64Str (OSCTXT* pctxt, OSDynOctStr* pvalue);

/**
 * This function decodes a contents of a hexBinary string. This
 * function will allocate dynamic memory to store the decoded result.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pvalue       A pointer to a dynamic octet string structure to receive
 *                       the decoded octet string. Dynamic memory is allocated to
 *                       hold the string using the ::rtxMemAlloc function.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecDynHexStr (OSCTXT* pctxt, OSDynOctStr* pvalue);

/**
 * This function is used to enforce a requirement that an element be
 * empty.  An error is returned in the current element has any
 * element or character children.  The last event must be the start
 * tag.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecEmptyElement (OSCTXT* pctxt);

/**
 * This function decodes the contents of a UTF-8 string data type.
 * Input is expected to be a string of UTF-8 or Unicode characters
 * returned by an XML parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param outdata      Pointer to a block of memory to receive
 *                      decoded UTF8 string.
 * @param max_len      Size of memory block.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecUTF8Str
(OSCTXT* pctxt, OSUTF8CHAR* outdata, size_t max_len);


/**
 * This function decodes the contents of a UTF-8 string data type.
 * Input is expected to be a string of UTF-8 or Unicode characters
 * returned by an XML parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param outdata      Pointer to a pointer to receive decoded UTF-8
 *                       string.  Memory is allocated for this string
 *                       using the run-time memory manager.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecDynUTF8Str (OSCTXT* pctxt, const OSUTF8CHAR** outdata);

/**
 * This function decodes the contents of a hex-encoded binary data type
 * into a memory buffer. Input is expected to be a string of UTF-8
 * characters returned by an XML parser. The decoded data will be put
 * into the given memory buffer starting from the current position and
 * bit offset. After all data is decoded the octet string may be fetched
 * out.
 *
 * <p>This function is normally used in the 'characters' SAX handler.
 *
 * @param pMemBuf      Pointer to memory buffer onto which the decoded
 *                       binary data will be appended.
 * @param inpdata      Pointer to a source string to be decoded.
 * @param length       Length of the source string (in characters).
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecHexBinary (OSRTMEMBUF* pMemBuf,
                                 const OSUTF8CHAR* inpdata,
                                 int length);

/**
 * This function decodes the contents of a hexBinary string into a static
 * memory structure.
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
EXTERNXML int rtXmlDecHexStr (OSCTXT* pctxt,
                              OSOCTET* pvalue, OSUINT32* pnocts,
                              OSINT32 bufsize);

EXTERNXML int rtXmlDecHexStrValue (OSCTXT* pctxt,
   const OSUTF8CHAR* const inpdata, size_t nbytes, OSOCTET* pvalue,
   OSUINT32* pnbits, OSINT32 bufsize);

/**
 * This function decodes a variable of the XSD 'gYear' type.
 * Input is expected to be a string of characters returned
 * by an XML parser. The string should have CCYY[-+hh:mm|Z] format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecGYear (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes a variable of the XSD 'gYearMonth' type.
 * Input is expected to be a string of characters returned
 * by an XML parser. The string should have CCYY-MM[-+hh:mm|Z] format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecGYearMonth (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes a variable of the XSD 'gMonth' type.
 * Input is expected to be a string of characters returned
 * by an XML parser. The string should have --MM[-+hh:mm|Z] format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecGMonth (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes a variable of the XSD 'gMonthDay' type.
 * Input is expected to be a string of characters returned
 * by an XML parser. The string should have --MM-DD[-+hh:mm|Z] format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecGMonthDay (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes a variable of the XSD 'gDay' type.
 * Input is expected to be a string of characters returned
 * by an XML parser. The string should have ---DD[-+hh:mm|Z] format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecGDay (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes the contents of a 32-bit integer data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to 32-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecInt (OSCTXT* pctxt, OSINT32* pvalue);

/**
 * This function decodes the contents of an 8-bit integer data type
 * (i.e. a signed byte type in the range of -128 to 127).
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to 8-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecInt8 (OSCTXT* pctxt, OSINT8* pvalue);

/**
 * This function decodes the contents of a 16-bit integer data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to 16-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecInt16 (OSCTXT* pctxt, OSINT16* pvalue);

/**
 * This function decodes the contents of a 64-bit integer data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to 64-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecInt64 (OSCTXT* pctxt, OSINT64* pvalue);

/**
 * This function decodes the contents of an unsigned 32-bit integer data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to unsigned 32-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecUInt (OSCTXT* pctxt, OSUINT32* pvalue);

/**
 * This function decodes the contents of an unsigned 8-bit integer data type
 * (i.e. a signed byte type in the range of 0 to 255).
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to unsigned 8-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecUInt8 (OSCTXT* pctxt, OSUINT8* pvalue);

/**
 * This function decodes the contents of an unsigned 16-bit integer data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to unsigned 16-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecUInt16 (OSCTXT* pctxt, OSUINT16* pvalue);

/**
 * This function decodes the contents of an unsigned 64-bit integer data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to unsigned 64-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecUInt64 (OSCTXT* pctxt, OSUINT64* pvalue);

/**
 * This function decodes an XML namespac attribute (xmlns).  It
 * is assumed that the given attribute name is either 'xmlns' or
 * 'xmlns:prefix'.  The XML namespace prefix and URI are added to
 * the given attribute list structure.  The parsed namespace is
 * also added to the namespace stack in the given context.
 *
 * @param pctxt         Pointer to context structure.
 * @param attrName      Name of the XML namespace attribute.  This is
 *                        assumed to contain either 'xmlns' (a default
 *                        namespace declaration) or 'xmlns:prefix' where
 *                        prefix is a namespace prefix.
 * @param attrValue     XML namespace attribute value (a URI).
 * @param pNSAttrs      List to receive parsed namespace values.
 * @param nsTable       Namespace URI's parsed from schema.
 * @param nsTableRowCount Number of rows (URI's) in namespace table.
 * @return              Zero if success or negative error code.
 */
EXTERNXML int rtXmlDecNSAttr
(OSCTXT* pctxt, const OSUTF8CHAR* attrName, const OSUTF8CHAR* attrValue,
 OSRTDList* pNSAttrs, const OSUTF8CHAR* nsTable[], OSUINT32 nsTableRowCount);

/**
 * This function decodes an XML qualified name string (QName) type.
 * This is a type that contains an optional namespace prefix followed by
 * a colon and the local part of the name:
 *
 * [NS 5] QName ::= (Prefix ':')? LocalPart
 *
 * [NS 6] Prefix ::= NCName
 *
 * [NS 7] LocalPart ::= NCName
 *
 * @param pctxt        Pointer to context block structure.
 * @param qname        String containing XML QName to be decoded.
 * @param prefix       Pointer to string pointer to receive decoded prefix.
 *                     This is optional.  If NULL, the prefix will not be
 *                     returned.  If not-NULL, the prefix will be returned
 *                     in memory allocated using \c rtxMemAlloc which must
 *                     be freed using one of the \c rtxMemFree functions.
 * @return             Pointer to local part of string.  This is pointer
 *                     to a location within the given string (i.e. a
 *                     pointer to the character after the ':' or to the
 *                     beginning of the string if it contains no colon).
 *                     This pointer does not need to be freed.
 */
EXTERNXML const OSUTF8CHAR* rtXmlDecQName
(OSCTXT* pctxt, const OSUTF8CHAR* qname, const OSUTF8CHAR** prefix);

/**
 * This function decodes XML schema instance (XSI) attribute. These
 * attributes include the XSI namespace declaration, the XSD schema
 * location attribute, and the XSD no namespace schema location attribute.
 *
 * @param pctxt        Pointer to context block structure.
 * @param attrName     Attribute's name to be decoded
 * @param attrValue    Attribute's value to be decoded
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecXSIAttr
   (OSCTXT* pctxt, const OSUTF8CHAR* attrName, const OSUTF8CHAR* attrValue);

/**
 * This function decodes XML schema instance (XSI) attributes. These
 * attributes include the XSI namespace declaration, the XSD schema
 * location attribute, and the XSD no namespace schema location attribute.
 *
 * @param pctxt        Pointer to context block structure.
 * @param attrs        Attributes-values array [attr, value].
 *                        Should be null-terminated.
 * @param typeName     Name of parent type to add in error log,
 *                        if would be necessary.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecXSIAttrs (OSCTXT* pctxt, const OSUTF8CHAR* const* attrs,
   const char* typeName);

/**
 * This function decodes the contents of an XML string data type.
 * This type contains a pointer to a UTF-8 characer string plus flags
 * that can be set to alter the encoding of the string (for example, the
 * cdata flag allows the string to be encoded in a CDATA section).
 * Input is expected to be a string of UTF-8 characters returned by
 * an XML parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param outdata      Pointer to an XML string structure to receive the
 *                       decoded string.  Memory is allocated for the string
 *                       using the run-time memory manager.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlDecXmlStr (OSCTXT* pctxt, OSXMLSTRING* outdata);

/**
 * This function parses the initial tag from an XML message.  If the
 * tag is a QName, only the local part of the name is returned.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param ppName        Pointer to a pointer to receive decoded UTF-8
 *                       string. Dynamic memory is allocated
 *                       for the variable using the rtxMemAlloc function.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlParseElementName (OSCTXT* pctxt, OSUTF8CHAR** ppName);

/**
 * This function parses the initial tag from an XML message.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param pQName        Pointer to a QName structure to receive parsed
 *                       name prefix and local name. Dynamic memory is
 *                       allocated for both name parts using the
 *                       rtxMemAlloc function.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlParseElemQName (OSCTXT* pctxt, OSXMLQName* pQName);

/**
 * @}
 */

/**
 * @defgroup rtXmlEnc XML encode functions.
 * @{
 */
/**
 * This function encodes a variable of the XSD any type.  This is considered
 * to be a fully-wrapped element of any type (for example:
 * \<myType\>myData\</myType\>)
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Value to be encoded. This is a string containing
 *                       the fully-encoded XML text to be copied to the
 *                       output stream.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          Pointer to namespace structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncAny (OSCTXT* pctxt, OSXMLSTRING* pvalue,
                           const OSUTF8CHAR* elemName,
                           OSXMLNamespace* pNS);

EXTERNXML int rtXmlEncAnyStr
(OSCTXT* pctxt, const OSUTF8CHAR* pvalue,
 const OSUTF8CHAR* elemName, OSXMLNamespace* pNS);

/**
 * This function encodes a variable of the XSD anyType type.
 * This is considered to be a fully-wrapped element of any type, possibly
 * containing attributes. (for example: * \<myType\>myData\</myType\>)
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncAnyTypeValue (OSCTXT* pctxt, const OSUTF8CHAR* pvalue);

/**
 * This function encodes a list of OSAnyAttr attributes in which
 * the name and value are given as a UTF-8 string.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pAnyAttrList List of attributes.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncAnyAttr (OSCTXT* pctxt, OSRTDList* pAnyAttrList);

/**
 * This function encodes a variable of the XSD base64Binary type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param nocts        Number of octets in the value string.
 * @param value        Value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          Pointer to namespace structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncBase64Binary (OSCTXT* pctxt, OSUINT32 nocts,
                                    const OSOCTET* value,
                                    const OSUTF8CHAR* elemName,
                                    OSXMLNamespace* pNS);

/**
 * This function encodes a variable of the XSD base64Binary type as an
 * attribute.
 *
 * @param pctxt        Pointer to context block structure.
 * @param nocts        Number of octets in the value string.
 * @param value        Value to be encoded.
 * @param attrName     XML attribute name.  A name must be provided.
 * @param attrNameLen  Length in bytes of the attribute name.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncBase64BinaryAttr
   (OSCTXT* pctxt, OSUINT32 nocts, const OSOCTET* value,
    const OSUTF8CHAR* attrName, size_t attrNameLen);

/**
 * This function encodes a variable of the XSD base64Binary type. It just puts
 * the encoded value in the destination buffer or stream without any tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param nocts        Number of octets in the value string.
 * @param value        Value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncBase64StrValue (OSCTXT* pctxt, OSUINT32 nocts,
                                      const OSOCTET* value);

/**
 * This function encodes a variable of the XSD integer type. In this case,
 * the integer is assumed to be of a larger size than can fit in a C or C++
 * long type (normally 32 or 64 bits). For example, parameters used to
 * calculate security values are typically larger than these sizes.
 *
 * Items of this type are stored in character string constant variables. They
 * can be represented as decimal strings (with no prefixes), as hexadecimal
 * strings starting with a "0x" prefix, as octal strings starting with a "0o"
 * prefix or as binary strings starting with a "0b" prefix. Other radixes are
 * currently not supported.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        A pointer to a character string containing the value to
 *                       be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          Pointer to namespace structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncBigInt (OSCTXT* pctxt, const OSUTF8CHAR* value,
                              const OSUTF8CHAR* elemName,
                              OSXMLNamespace* pNS);

/**
 * This function encodes an XSD integer attribute value. In this case,
 * the integer is assumed to be of a larger size than can fit in a C or C++
 * long type (normally 32 or 64 bits).
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        A pointer to a character string containing the value to
 *                       be encoded.
 * @param attrName     XML attribute name.  A name must be provided.
 * @param attrNameLen  Length in bytes of the attribute name.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncBigIntAttr (OSCTXT* pctxt, const OSUTF8CHAR* value,
                       const OSUTF8CHAR* attrName, size_t attrNameLen);

/**
 * This function encodes an XSD integer attribute value. In this case,
 * the integer is assumed to be of a larger size than can fit in a C or C++
 * long type (normally 32 or 64 bits). This function just puts
 * the encoded value in the destination buffer or stream without any tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        A pointer to a character string containing the value to
 *                       be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncBigIntValue (OSCTXT* pctxt, const OSUTF8CHAR* value);

/**
 * This function encodes a variable of the ASN.1 BIT STRING type.
 * The encoded data is a sequence of '1's and '0's.  This is only
 * used if named bits are not specified in the string
 * (@see rtXmlEncNamedBits).
 *
 * @param pctxt        Pointer to context block structure.
 * @param nbits        Number of bits in the bit string.
 * @param value        Value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          Pointer to namespace structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncBitString (OSCTXT* pctxt, OSUINT32 nbits,
                                 const OSOCTET* value,
                                 const OSUTF8CHAR* elemName,
                                 OSXMLNamespace* pNS);

/**
 * This function encodes a binary string value as a sequence of '1's
 * and '0's.
 *
 * @param pctxt        Pointer to context block structure.
 * @param nbits        Number of bits in the value string.
 * @param data         Value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncBinStrValue (OSCTXT* pctxt,
                                   OSUINT32 nbits,
                                   const OSOCTET* data);

/**
 * This function encodes a variable of the XSD boolean type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Boolean value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          Pointer to namespace structure.
 *
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncBool (OSCTXT* pctxt, OSBOOL value,
                            const OSUTF8CHAR* elemName,
                            OSXMLNamespace* pNS);

/**
 * This function encodes a variable of the XSD boolean type. It just puts
 * the encoded value in the destination buffer or stream without any tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Boolean value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncBoolValue (OSCTXT* pctxt, OSBOOL value);

/**
 * This function encodes an XSD boolean attribute value.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Boolean value to be encoded.
 * @param attrName     XML attribute name.  A name must be provided.
 * @param attrNameLen  Length in bytes of the attribute name.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncBoolAttr (OSCTXT* pctxt, OSBOOL value,
                                const OSUTF8CHAR* attrName,
                                size_t attrNameLen);

/**
 * This function encodes an XML comment.  The given text will be inserted 
 * in between XML comment start and end elements (<!-- comment -->).
 *
 * @param pctxt        Pointer to context block structure.
 * @param comment      The comment text.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncComment (OSCTXT* pctxt, const OSUTF8CHAR* comment);

/**
 * This function encodes a variable of the XSD 'date' type as a
 * string. This version of the function is used to encode an OSXSDDateTime
 * value into CCYY-MM-DD format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          Pointer to namespace structure.
 *
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncDate (OSCTXT* pctxt, const OSXSDDateTime* pvalue,
                            const OSUTF8CHAR* elemName,
                            OSXMLNamespace* pNS);

/**
 * This function encodes a variable of the XSD 'date' type as a
 * string. This version of the function is used to encode an OSXSDDateTime
 * value into CCYY-MM-DD format. This function just puts
 * the encoded value in the destination buffer or stream without any tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncDateValue
   (OSCTXT* pctxt, const OSXSDDateTime* pvalue);

/**
 * This function encodes a variable of the XSD 'time' type as an
 * string. This version of the function is used to encode OSXSDDateTime
 * value into any of following format in different condition as stated
 * below.
 *                    (1) hh-mm-ss.ss  used if tz_flag = false
 *                    (2) hh-mm-ss.ssZ used if tz_flag = false and tzo = 0
 *                    (3) hh-mm-ss.ss+HH:MM if tz_flag = false and tzo > 0
 *                    (4) hh-mm-ss.ss-HH:MM-HH:MM
 *                                          if tz_flag = false and tzo < 0
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          Pointer to namespace structure.
 *
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncTime (OSCTXT* pctxt, const OSXSDDateTime* pvalue,
                            const OSUTF8CHAR* elemName,
                            OSXMLNamespace* pNS);

/**
 * This function encodes a variable of the XSD 'time' type as an
 * string. This version of the function just puts
 * the encoded value in the destination buffer or stream without any tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncTimeValue
   (OSCTXT* pctxt, const OSXSDDateTime* pvalue);

/**
 * This function encodes a numeric date/time value into an XML string
 * representation.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          Pointer to namespace structure.
 *
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncDateTime (OSCTXT* pctxt, const OSXSDDateTime* pvalue,
                                const OSUTF8CHAR* elemName,
                                OSXMLNamespace* pNS);

/**
 * This function encodes a numeric date/time value into an XML string
 * representation. It just puts
 * the encoded value in the destination buffer or stream without any tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncDateTimeValue
   (OSCTXT* pctxt, const OSXSDDateTime* pvalue);

/**
 * This function encodes a variable of the XSD decimal type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          Pointer to namespace structure.
 * @param pFmtSpec     Pointer to format specification structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncDecimal (OSCTXT* pctxt, OSREAL value,
                               const OSUTF8CHAR* elemName,
                               OSXMLNamespace* pNS,
                               const OSDecimalFmt* pFmtSpec);

/**
 * This function encodes a variable of the XSD decimal type as an attribute.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param attrName     XML attribute name.  A name must be provided.
 * @param attrNameLen  Length of XML attribute name.
 * @param pFmtSpec     Pointer to format specification structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncDecimalAttr (OSCTXT* pctxt, OSREAL value,
                                   const OSUTF8CHAR* attrName,
                                   size_t attrNameLen,
                                   const OSDecimalFmt* pFmtSpec);

/**
 * This function encodes a value of the XSD decimal type. It just puts
 * the encoded value in the destination buffer or stream without any tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param pFmtSpec     Pointer to format specification structure.
 * @param pDestBuf     Pointer to a destination buffer. If NULL
 *                     (destBufSize should be 0) the encoded value will be
 *                     put in pctxt->buffer or in stream associated with
 *                     the pctxt.
 * @param destBufSize  The size of the destination buffer. Must be 0, if
 *                     pDestBuf is NULL.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncDecimalValue (OSCTXT* pctxt, OSREAL value,
                                    const OSDecimalFmt* pFmtSpec,
                                    char* pDestBuf, size_t destBufSize);

/**
 * This function encodes a variable of the XSD double type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          Pointer to namespace structure.
 * @param pFmtSpec     Pointer to format specification structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncDouble (OSCTXT* pctxt, OSREAL value,
                              const OSUTF8CHAR* elemName,
                              OSXMLNamespace* pNS,
                              const OSDoubleFmt* pFmtSpec);

/**
 * This function encodes a variable of the XSD double type as an attribute.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param attrName     XML attribute name.  A name must be provided.
 * @param attrNameLen  Length of XML attribute name.
 * @param pFmtSpec     Pointer to format specification structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncDoubleAttr (OSCTXT* pctxt, OSREAL value,
                                  const OSUTF8CHAR* attrName,
                                  size_t attrNameLen,
                                  const OSDoubleFmt* pFmtSpec);

/**
 * This function encodes a normal (not +/-INF or NaN) value of the XSD double 
 * or float type. It just puts the encoded value in the destination buffer or 
 * stream without any tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param pFmtSpec     Pointer to format specification structure.
 * @param defaultPrecision Default precision of the value. For float, it is 6,
 *                     for double it is 15.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncDoubleNormalValue (OSCTXT* pctxt, OSREAL value,
                                   const OSDoubleFmt* pFmtSpec,
                                   int defaultPrecision);

/**
 * This function encodes a value of the XSD double or float type. It just puts
 * the encoded value in the destination buffer or stream without any tags.
 * Special real values +/-INF and NaN are encoded as "INF", "-INF", and "NaN",
 * respectively.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param pFmtSpec     Pointer to format specification structure.
 * @param defaultPrecision Default precision of the value. For float, it is 6,
 *                     for double it is 15.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncDoubleValue (OSCTXT* pctxt, OSREAL value,
                                   const OSDoubleFmt* pFmtSpec,
                                   int defaultPrecision);

/**
 * This function encodes an enpty element tag value (\<elemName/\>).
 *
 * @param pctxt        Pointer to context block structure.
 * @param elemName     XML element name.
 * @param pNS          XML namespace information (prefix and URI).
 * @param pNSAttrs     List of namespace attributes to be added to element.
 * @param terminate    Add closing '\>' character.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncEmptyElement (OSCTXT* pctxt,
                                    const OSUTF8CHAR* elemName,
                                    OSXMLNamespace* pNS,
                                    OSRTDList* pNSAttrs,
                                    OSBOOL terminate);

/**
 * This function adds trailor information and a null terminator at
 * the end of the XML document being encoded.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncEndDocument (OSCTXT* pctxt);

/**
 * This function encodes an end element tag value (\</elemName\>).
 *
 * @param pctxt        Pointer to context block structure.
 * @param elemName     XML element name.
 * @param pNS          XML namespace information (prefix and URI).
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncEndElement (OSCTXT* pctxt,
                                  const OSUTF8CHAR* elemName,
                                  OSXMLNamespace* pNS);

/**
 * This function encodes a SOAP envelope end element tag
 * (\<SOAP-ENV:Envelope/\>).
 *
 * @param pctxt        Pointer to context block structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncEndSoapEnv (OSCTXT* pctxt);

/**
 * This function encodes SOAP end element tags.  If will add a SOAP body
 * or fault end tag based on the SOAP message type argument.  It will then
 * add an envelope end element tag.
 *
 * @param pctxt        Pointer to context block structure.
 * @param msgtype      SOAP message type (body, fault, or none)
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncEndSoapElems (OSCTXT* pctxt, OSXMLSOAPMsgType msgtype);

/**
 * This function encodes a variable of the XSD float type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          XML namespace information (prefix and URI).
 * @param pFmtSpec     Pointer to format specification structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncFloat (OSCTXT* pctxt, OSREAL value,
                             const OSUTF8CHAR* elemName,
                             OSXMLNamespace* pNS,
                             const OSDoubleFmt* pFmtSpec);

/**
 * This function encodes a variable of the XSD float type as an attribute.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param attrName     XML attribute name.  A name must be provided.
 * @param attrNameLen  Length of XML attribute name.
 * @param pFmtSpec     Pointer to format specification structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncFloatAttr (OSCTXT* pctxt, OSREAL value,
                                 const OSUTF8CHAR* attrName,
                                 size_t attrNameLen,
                                 const OSDoubleFmt* pFmtSpec);

/**
 * This function encodes a numeric gYear element into an XML string
 * representation.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          XML namespace information (prefix and URI).
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncGYear (OSCTXT* pctxt, const OSXSDDateTime* pvalue,
                             const OSUTF8CHAR* elemName,
                             OSXMLNamespace* pNS);

/**
 * This function encodes a numeric gYearMonth element into an XML string
 * representation.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          XML namespace information (prefix and URI).
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncGYearMonth (OSCTXT* pctxt, const OSXSDDateTime* pvalue,
                                  const OSUTF8CHAR* elemName,
                                  OSXMLNamespace* pNS);

/**
 * This function encodes a numeric gMonth element into an XML string
 * representation.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          XML namespace information (prefix and URI).
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncGMonth (OSCTXT* pctxt, const OSXSDDateTime* pvalue,
                              const OSUTF8CHAR* elemName,
                              OSXMLNamespace* pNS);

/**
 * This function encodes a numeric gMonthDay element into an XML string
 * representation.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          XML namespace information (prefix and URI).
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncGMonthDay (OSCTXT* pctxt, const OSXSDDateTime* pvalue,
                                 const OSUTF8CHAR* elemName,
                                 OSXMLNamespace* pNS);

/**
 * This function encodes a numeric gDay element into an XML string
 * representation.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          XML namespace information (prefix and URI).
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncGDay (OSCTXT* pctxt, const OSXSDDateTime* pvalue,
                            const OSUTF8CHAR* elemName,
                            OSXMLNamespace* pNS);

/**
 * This function encodes a numeric gYear value into an XML string
 * representation. It just puts the encoded value into the buffer or
 * stream without any tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncGYearValue
   (OSCTXT* pctxt, const OSXSDDateTime* pvalue);

/**
 * This function encodes a numeric gYearMonth value into an XML string
 * representation. It just puts the encoded value into the buffer or
 * stream without any tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncGYearMonthValue
   (OSCTXT* pctxt, const OSXSDDateTime* pvalue);

/**
 * This function encodes a numeric gMonth value into an XML string
 * representation. It just puts the encoded value into the buffer or
 * stream without any tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncGMonthValue
   (OSCTXT* pctxt, const OSXSDDateTime* pvalue);

/**
 * This function encodes a numeric gMonthDay value into an XML string
 * representation. It just puts the encoded value into the buffer or
 * stream without any tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncGMonthDayValue
   (OSCTXT* pctxt, const OSXSDDateTime* pvalue);

/**
 * This function encodes a numeric gDay value into an XML string
 * representation. It just puts the encoded value into the buffer or
 * stream without any tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncGDayValue
   (OSCTXT* pctxt, const OSXSDDateTime* pvalue);

/**
 * This function encodes a variable of the XSD hexBinary type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param nocts        Number of octets in the value string.
 * @param value        Value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          XML namespace information (prefix and URI).
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncHexBinary (OSCTXT* pctxt, OSUINT32 nocts,
                                 const OSOCTET* value,
                                 const OSUTF8CHAR* elemName,
                                 OSXMLNamespace* pNS);

/**
 * This function encodes a variable of the XSD hexBinary type as an attribute.
 *
 * @param pctxt        Pointer to context block structure.
 * @param nocts        Number of octets in the value string.
 * @param value        Value to be encoded.
 * @param attrName     XML attribute name.  A name must be provided.
 * @param attrNameLen  Length of XML attribute name.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncHexBinaryAttr
   (OSCTXT* pctxt, OSUINT32 nocts, const OSOCTET* value,
    const OSUTF8CHAR* attrName, size_t attrNameLen);

/**
 * This function encodes a variable of the XSD hexBinary type. It just puts
 * the encoded value in the destination buffer or stream without any tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param nocts        Number of octets in the value string.
 * @param data         Value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncHexStrValue (OSCTXT* pctxt,
                                   OSUINT32 nocts,
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
EXTERNXML int rtXmlEncIndent (OSCTXT* pctxt);

/**
 * This function encodes a variable of the XSD integer type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          XML namespace information (prefix and URI).
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncInt (OSCTXT* pctxt, OSINT32 value,
                           const OSUTF8CHAR* elemName,
                           OSXMLNamespace* pNS);

/**
 * This function encodes a variable of the XSD integer type. It just puts
 * the encoded value in the destination buffer or stream without any tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncIntValue (OSCTXT* pctxt, OSINT32 value);

/**
 * This function encodes a variable of the XSD integer type as an
 * attribute (name="value").
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param attrName     XML attribute name.
 * @param attrNameLen  Length (in bytes) of the attribute name.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncIntAttr (OSCTXT* pctxt, OSINT32 value,
                               const OSUTF8CHAR* attrName,
                               size_t attrNameLen);

/**
 * This function encodes a variable of the XSD integer type using a pattern
 * to specify the format of the integer value.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          XML namespace information (prefix and URI).
 * @param pattern      Pattern of the encoded value.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncIntPattern (OSCTXT* pctxt, OSINT32 value,
                                  const OSUTF8CHAR* elemName,
                                  OSXMLNamespace* pNS,
                                  const OSUTF8CHAR* pattern);

EXTERNXML int rtXmlEncIntPatternValue (OSCTXT* pctxt, OSINT32 value,
                                       const OSUTF8CHAR* pattern);

EXTERNXML int rtXmlEncUIntPattern (OSCTXT* pctxt, OSUINT32 value,
                                  const OSUTF8CHAR* elemName,
                                  OSXMLNamespace* pNS,
                                  const OSUTF8CHAR* pattern);

EXTERNXML int rtXmlEncUIntPatternValue (OSCTXT* pctxt, OSUINT32 value,
                                       const OSUTF8CHAR* pattern);

/**
 * This function encodes a variable of the XSD integer type. This version
 * of the function is used for 64-bit integer values.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          XML namespace information (prefix and URI).
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncInt64 (OSCTXT* pctxt, OSINT64 value,
                             const OSUTF8CHAR* elemName,
                             OSXMLNamespace* pNS);

EXTERNXML int rtXmlEncInt64Pattern (OSCTXT* pctxt, OSINT64 value,
                                    const OSUTF8CHAR* elemName,
                                    OSXMLNamespace* pNS,
                                    const OSUTF8CHAR* pattern);

/**
 * This function encodes a variable of the XSD integer type. This version
 * of the function is used for 64-bit integer values. It just puts
 * the encoded value in the destination buffer or stream without any tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncInt64Value (OSCTXT* pctxt, OSINT64 value);
EXTERNXML int rtXmlEncInt64PatternValue (OSCTXT* pctxt, OSINT64 value,
                                         const OSUTF8CHAR* pattern);

/**
 * This function encodes a variable of the XSD integer type as an
 * attribute (name="value").  This version of the function is used
 * for 64-bit integer values.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param attrName     XML attribute name.
 * @param attrNameLen  Length (in bytes) of the attribute name.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncInt64Attr (OSCTXT* pctxt, OSINT64 value,
                                 const OSUTF8CHAR* attrName,
                                 size_t attrNameLen);

/**
 * This function encodes a variable of the ASN.1 BIT STRING type.  In
 * this case, a set of named bits was provided in the schema for the
 * bit values.  The encoding is a list of the named bit identifers
 * corresponding to each set bit in the bit string value.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pBitMap      Bit map equating symbolic bit names to bit numbers.
 * @param nbits        Number of bits in the sit string value.
 * @param pvalue       Bit string value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          Pointer to namespace structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncNamedBits
(OSCTXT* pctxt, const OSBitMapItem* pBitMap, OSUINT32 nbits,
 const OSOCTET* pvalue, const OSUTF8CHAR* elemName, OSXMLNamespace* pNS);

EXTERNXML int rtXmlEncNamedBitsValue
(OSCTXT* pctxt, const OSBitMapItem* pBitMap, OSUINT32 nbits,
 const OSOCTET* pvalue);

/**
 * This function encodes namespace declaration attributes at the
 * beginning of an XML document.  The attributes to be encoded are
 * stored in the namespace list provided, or within the context if
 * a NULL pointer is passed for pNSAttrs.  Namespaces are added to
 * this list by using the namespace utility functions.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pNSAttrs     Pointer to list of namespace attributes.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncNSAttrs (OSCTXT* pctxt, OSRTDList* pNSAttrs);

/**
 * This function prints a list of namespace attributes.
 *
 * @param name         Name to print.
 * @param data         List of namespace attributes.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlPrintNSAttrs (const char* name, const OSRTDList* data);

/* The following is required for backward compatibility */
#define rtxPrintNSAttrs(name,data) rtXmlPrintNSAttrs(name,&data)

/**
 * This function encodes a variable of the ASN.1 REAL base 10 type.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pvalue       A pointer to an REAL base 10 value.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          XML namespace information (prefix and URI).
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncReal10
(OSCTXT *pctxt, const OSUTF8CHAR *pvalue, const OSUTF8CHAR* elemName,
 OSXMLNamespace* pNS);

/**
 * This function encodes the special SOAP encoding attrType attribute
 * which specifies the number and type of elements in a SOAP array.
 * The form of this attribute is 'attrType="\<type\>[count]"'.
 *
 * @param pctxt        Pointer to context block structure.
 * @param name         Attribute name (NS prefix + arrayType)
 * @param value        UTF-8 string value to be encoded.
 * @param itemCount    Count of the number of elements in the array.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncSoapArrayTypeAttr
(OSCTXT* pctxt, const OSUTF8CHAR* name, const OSUTF8CHAR* value,
 size_t itemCount);

EXTERNXML int rtXmlEncSoapArrayTypeAttr2
(OSCTXT* pctxt, const OSUTF8CHAR* name, size_t nameLen,
 const OSUTF8CHAR* value, size_t valueLen, size_t itemCount);

/**
 * This function encodes the XML header text at the beginning of an
 * XML document.  This is normally the following:
 *
 * <?xml version="1.0" encoding="UTF-8"?>
 *
 * @param pctxt        Pointer to context block structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncStartDocument (OSCTXT* pctxt);

/**
 * This function encodes the Unicode byte order mark header
 * at the start of the document.  It is called by rtXmlEncStartDocument
 * and does not need to be called manually.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncBOM (OSCTXT* pctxt);
/**
 * This function encodes a start element tag value (\<elemName\>).
 *
 * @param pctxt        Pointer to context block structure.
 * @param elemName     XML element name. Empty string and null are treated
 *                     equivalently.
 * @param pNS          XML namespace information (prefix and URI).  If the
 *                     prefix is NULL, this method will search the context's
 *                     namespace stack for a prefix to use.
 * @param pNSAttrs     List of namespace attributes to be added to element.
 * @param terminate    Add closing '>' character.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncStartElement (OSCTXT* pctxt,
                                    const OSUTF8CHAR* elemName,
                                    OSXMLNamespace* pNS,
                                    OSRTDList* pNSAttrs,
                                    OSBOOL terminate);

/**
 * This function encodes a SOAP envelope start element tag.   This
 * includes all of the standard SOAP namespace attributes.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pNSAttrs     List of namespace attributes to be added to
 *                       SOAP envelope.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncStartSoapEnv (OSCTXT* pctxt, OSRTDList* pNSAttrs);

/**
 * This function encodes a SOAP envelope start element tag and an
 * optional SOAP body or fault tag.   This includes all of the standard
 * SOAP namespace attributes.
 *
 * @param pctxt        Pointer to context block structure.
 * @param msgtype      SOAP message type (body, fault, or none)
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncStartSoapElems
(OSCTXT* pctxt, OSXMLSOAPMsgType msgtype);

/**
 * This function encodes a variable of the XSD string type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pxmlstr      XML string value to be encoded.
 * @param elemName     XML element name.  If either null or empty string is
 *                      passed, no element tag is added to the encoded value.
 * @param pNS          XML namespace information (prefix and URI).
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncString (OSCTXT* pctxt,
                              OSXMLSTRING* pxmlstr,
                              const OSUTF8CHAR* elemName,
                              OSXMLNamespace* pNS);

/**
 * This function encodes a variable of the XSD string type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        XML string value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncStringValue (OSCTXT* pctxt, const OSUTF8CHAR* value);

/**
 * This function encodes a variable of the XSD string type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        XML string value to be encoded.
 * @param valueLen     UTF-8 string value length (in octets).
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncStringValue2
   (OSCTXT* pctxt, const OSUTF8CHAR* value, size_t valueLen);

/**
 * This function terminates a currently open XML start element by adding
 * either a '>' or '/>' (if empty) terminator.  It will also add XSI
 * attributes to the element.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncTermStartElement (OSCTXT* pctxt);

/**
 * This function encodes a Unicode string value.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.  This is a pointer to an array
 *                       of 16-bit integer values.
 * @param nchars       Number of characters in value array.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          XML namespace information (prefix and URI).
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncUnicodeStr (OSCTXT* pctxt,
                                  const OSUNICHAR* value,
                                  OSUINT32 nchars,
                                  const OSUTF8CHAR* elemName,
                                  OSXMLNamespace* pNS);

/**
 * This function encodes an attribute in which the name and value are given as
 * a null-terminated UTF-8 strings.
 *
 * @param pctxt        Pointer to context block structure.
 * @param name         Attribute name.
 * @param value        UTF-8 string value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncUTF8Attr (OSCTXT* pctxt,
                                const OSUTF8CHAR* name,
                                const OSUTF8CHAR* value);

/**
 * This function encodes an attribute in which the name and value are given as
 * a UTF-8 strings with lengths.
 *
 * @param pctxt        Pointer to context block structure.
 * @param name         Attribute name.
 * @param nameLen      Attribute name length (in octets).
 * @param value        UTF-8 string value to be encoded.
 * @param valueLen     UTF-8 string value length (in octets).
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncUTF8Attr2 (OSCTXT* pctxt,
                                 const OSUTF8CHAR* name, size_t nameLen,
                                 const OSUTF8CHAR* value, size_t valueLen);

/**
 * This function encodes a UTF-8 string value.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          XML namespace information (prefix and URI).
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncUTF8Str (OSCTXT* pctxt,
                               const OSUTF8CHAR* value,
                               const OSUTF8CHAR* elemName,
                               OSXMLNamespace* pNS);

/**
 * This function encodes a variable of the XSD unsigned integer type.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          XML namespace information (prefix and URI).
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncUInt (OSCTXT* pctxt, OSUINT32 value,
                            const OSUTF8CHAR* elemName,
                            OSXMLNamespace* pNS);

/**
 * This function encodes a variable of the XSD unsigned integer type.
 * It just puts the encoded value in the destination buffer or
 * stream without any tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncUIntValue (OSCTXT* pctxt, OSUINT32 value);

/**
 * This function encodes a variable of the XSD unsigned integer type as an
 * attribute (name="value").
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param attrName     XML attribute name.
 * @param attrNameLen  Length (in bytes) of the attribute name.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncUIntAttr (OSCTXT* pctxt, OSUINT32 value,
                                const OSUTF8CHAR* attrName,
                                size_t attrNameLen);

/**
 * This function encodes a variable of the XSD integer type. This version
 * of the function is used when constraints cause an unsigned 64-bit integer
 * variable to be used.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param elemName     XML element name.  A name must be provided.
 *                       If an empty string is passed (""), no element tag is
 *                       added to the encoded value.
 * @param pNS          XML namespace information (prefix and URI).
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncUInt64 (OSCTXT* pctxt, OSUINT64 value,
                              const OSUTF8CHAR* elemName,
                              OSXMLNamespace* pNS);

EXTERNXML int rtXmlEncUInt64Pattern (OSCTXT* pctxt, OSUINT64 value,
                                     const OSUTF8CHAR* elemName,
                                     OSXMLNamespace* pNS,
                                     const OSUTF8CHAR* pattern);

/**
 * This function encodes a variable of the XSD integer type. This version
 * of the function is used when constraints cause an unsigned 64-bit integer
 * variable to be used. It writes the encoded value to the destination
 * buffer or stream without any tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncUInt64Value (OSCTXT* pctxt, OSUINT64 value);
EXTERNXML int rtXmlEncUInt64PatternValue (OSCTXT* pctxt, OSUINT64 value,
                                          const OSUTF8CHAR* pattern);

/**
 * This function encodes a variable of the XSD integer type as an
 * attribute (name="value").  This version of the function is used
 * for unsigned 64-bit integer values.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Value to be encoded.
 * @param attrName     XML attribute name.
 * @param attrNameLen  Length (in bytes) of the attribute name.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncUInt64Attr (OSCTXT* pctxt, OSUINT64 value,
                                  const OSUTF8CHAR* attrName,
                                  size_t attrNameLen);

/**
 * This function encodes XML schema instance (XSI) attributes at the
 * beginning of an XML document. The encoded attributes include the
 * XSI namespace declaration, the XSD schema location attribute, and
 * the XSD no namespace schema location attribute.
 *
 * <p>The XSI namespace declaration will only be added to the document
 * if schema location attributes exist or the 'needXSI' flag (see
 * below) is set.
 *
 * @param pctxt        Pointer to context block structure.
 * @param needXSI      This flag is set to true to indicate the XSI
 *                     namespace declaration is required to support
 *                     XML items in the main document body such as
 *                     xsi:type or xsi:nil.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncXSIAttrs (OSCTXT* pctxt, OSBOOL needXSI);

/**
 * This function encodes an XML schema instance (XSI) type attribute
 * value (xsi:type="value").
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        XSI type attribute value.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncXSITypeAttr (OSCTXT* pctxt, const OSUTF8CHAR* value);

/**
 * This function encodes an XML schema instance (XSI) type attribute
 * value (xsi:type="pfx:value"). This will encode namespace declarations for
 * the xsi namespace and for the typeNsUri namespace, if needed.
 *
 * @param pctxt        Pointer to context block structure.
 * @param typeNsUri    The type's namespace URI. Either null or empty if none.
 * @param typeName     The type's name.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncXSITypeAttr2 (OSCTXT* pctxt, 
                                    const OSUTF8CHAR* typeNsUri,
                                    const OSUTF8CHAR* typeName);

/**
 * This function encodes an XML nil attribute (xsi:nil="true").
 *
 * @param pctxt        Pointer to context block structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncXSINilAttr (OSCTXT* pctxt);

/**
 * This function closes an input source that was previously created
 * with one of the create input source functions such as
 * 'rtXmlCreateFileInputSource'.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlFreeInputSource (OSCTXT* pctxt);

EXTERNXML OSBOOL rtXmlStrCmpAsc (const OSUTF8CHAR* text1, const char* text2);

EXTERNXML OSBOOL rtXmlStrnCmpAsc (const OSUTF8CHAR* text1,
                                  const char* text2, size_t len);

#define rtXmlFinalizeMemBuf(pMemBuf) do { \
(pMemBuf)->pctxt->buffer.data = (pMemBuf)->buffer + (pMemBuf)->startidx; \
(pMemBuf)->pctxt->buffer.size =                                      \
((pMemBuf)->usedcnt - (pMemBuf)->startidx);                          \
(pMemBuf)->pctxt->buffer.dynamic = FALSE;                            \
(pMemBuf)->pctxt->buffer.byteIndex = 0;                              \
rtxMemBufReset (pMemBuf);                                            \
} while(0)

/**
 * This function is used to set the internal buffer within the run-time
 * library encoding context.  It must be called after the context variable
 * is initialized by the rtxInitContext function and before any other
 * compiler generated or run-time library encode function.
 *
 * @param pctxt         Pointer to a context structure.  This provides a
 *                      storage area for the function to store all working
 *                      variables that must be maintained between function
 *                      calls.
 *
 * @param bufaddr	A pointer to a memory buffer to use to encode a
 *                      message.  The buffer should be declared as an array
 *                      of unsigned characters (OCTETs).  This parameter
 *                      can be set to NULL to specify dynamic encoding (i.e.,
 * 			the encode functions will dynamically allocate a
 *                      buffer to hold the encoded message).
 *
 * @param bufsiz	The length of the memory buffer in bytes.  Should
 *                      be set to zero if NULL was specified for bufaddr
 *                      (i.e. dynamic encoding was selected).
 */
EXTERNXML int rtXmlSetEncBufPtr
(OSCTXT* pctxt, OSOCTET* bufaddr, size_t bufsiz);

/**
 * This macro returns the start address of the encoded XML message.
 * If a static buffer was used, this is simply the start address of
 * the buffer.  If dynamic encoding was done, this will return the
 * start address of the dynamic buffer allocated by the encoder.
 *
 * @param pctxt         Pointer to a context structure.
 */
#define rtXmlGetEncBufPtr(pctxt) (pctxt)->buffer.data

/**
 * This macro returns the length of the encoded XML message.
 *
 * @param pctxt         Pointer to a context structure.
 */
#define rtXmlGetEncBufLen(pctxt) (pctxt)->buffer.byteIndex


/**
 * This function returns current XML output indent value.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @return              Current indent value (>= 0) if OK, negative status
 *                      code if error.
 */
EXTERNXML int rtXmlGetIndent (OSCTXT* pctxt);


/**
 * This function returns whether the Unicode byte order mark will be
 * encoded.
 *
 * @param pctxt         Pointer to OSCTXT structure.
 * @return              TRUE if BOM is to be encoded,
 *                      FALSE if not or if context uninitialized.
 */
EXTERNXML OSBOOL rtXmlGetWriteBOM (OSCTXT* pctxt);

/**
 * This function returns current XML output indent character value
 * (default is space).
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @return              Current indent character (> 0) if OK, negative status
 *                      code if error.
 */
EXTERNXML int rtXmlGetIndentChar (OSCTXT* pctxt);
/**
 * @}
 */

/**
 * This function prepares the context for another encode
 * by setting the state back to OSXMLINIT and moving the buffer's
 * cursor back to the beginning of the buffer.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @return              0 if OK, negative status code if error.
 */
EXTERNXML int rtXmlPrepareContext (OSCTXT* pctxt);

/**
 * This function sets the option to encode in C14N mode.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param value         Boolean value: true = C14N mode enabled.
 * @return              Status of operation: 0 if OK, negative status
 *                      code if error.
 */
EXTERNXML int rtXmlSetEncC14N (OSCTXT* pctxt, OSBOOL value);

/**
 * This function sets a flag in the context that indicates the XSI
 * namespace declaration (xmlns:xsi) should be added to the encoded
 * XML instance.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param value         Boolean value: true = encode XSI namespace attribute.
 * @return              Status of operation: 0 if OK, negative status
 *                      code if error.
 */
EXTERNXML int rtXmlSetEncXSINamespace (OSCTXT* pctxt, OSBOOL value);

/**
 * This function sets a flag in the context that indicates the XSI
 * attribute declaration (xmlns:xsi) should be added to the encoded
 * XML instance.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param value         Boolean value: true = encode xsi:nil attribute.
 * @return              Status of operation: 0 if OK, negative status
 *                      code if error.
 */
EXTERNXML int rtXmlSetEncXSINilAttr (OSCTXT* pctxt, OSBOOL value);

EXTERNXML int rtXmlSetDigitsFacets
   (OSCTXT* pctxt, int totalDigits, int fractionDigits);

/**
 * This function sets the option to add the XML document header
 * (i.e. \<?xml version="1.0" encoding="UTF-8"?\>) to the XML output
 * stream.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param value         Boolean value: true = add document header
 * @return              Status of operation: 0 if OK, negative status
 *                      code if error.
 */
EXTERNXML int rtXmlSetEncDocHdr (OSCTXT* pctxt, OSBOOL value);

/**
 * This function sets the XML output encoding to the given value.
 * Currently, UTF-8/UTF-16/ISO-8859-1 encodings are supported.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param encodingStr   XML output encoding format
 * @return              Status of operation: 0 if OK, negative status
 *                      code if error.
 */
EXTERNXML int rtXmlSetEncodingStr
(OSCTXT* pctxt, const OSUTF8CHAR* encodingStr);

/**
 * This function sets XML output formatting to the given value.
 * If TRUE (the default), the XML document is formatted with indentation
 * and newlines.  If FALSE, all whitespace between elements is suppressed.
 * Turning formatting off can provide more compressed documents and also
 * a more canonical representation which is important for security
 * applications.
 * Also the function 'rtXmlSetIndent' might be used to set the exact size of
 * identation.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param doFormatting  Boolean value indicating if formatting is to be done
 * @return              Status of operation: 0 if OK, negative status
 *                      code if error.
 */
EXTERNXML int rtXmlSetFormatting (OSCTXT* pctxt, OSBOOL doFormatting);

/**
 * This function sets XML output indent to the given value.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param indent        Number of spaces per indent. Default is 3.
 * @return              Status of operation: 0 if OK, negative status
 *                      code if error.
 */
EXTERNXML int rtXmlSetIndent (OSCTXT* pctxt, OSUINT8 indent);

/**
 * This function sets XML output indent character to the given value.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param indentChar    Indent character. Default is space.
 * @return              Status of operation: 0 if OK, negative status
 *                      code if error.
 */
EXTERNXML int rtXmlSetIndentChar (OSCTXT* pctxt, char indentChar);

/**
 * This function sets the context 'namespaces are set' flag.  This
 * indicates that namespace declarations have been set either by the
 * decoder or externally by the end user.  It is used by the encoder
 * to know not to set the default namespaces specified in the schema
 * before starting encoding.
 *
 * @param pctxt         Pointer to OSCTXT structure.
 * @param value         Boolean value to which flag is to be set.
 */
EXTERNXML void rtXmlSetNamespacesSet (OSCTXT* pctxt, OSBOOL value);

/**
 * This function sets namespace prefix/URI links in the namspace
 * prefix stack in the context structure.
 *
 * @param pctxt         Pointer to OSCTXT structure.
 * @param pNSAttrs      List of namespace attributes.
 * @return              Status of operation: 0 if OK, negative status
 *                      code if error.
 */
EXTERNXML int rtXmlSetNSPrefixLinks (OSCTXT* pctxt, OSRTDList* pNSAttrs);

/**
 * This function sets the XML Schema Instance (xsi) schema location
 * attribute to be added to an encoded document.  This attribute is
 * optional: if not set, no xsi:schemaLocation attribute will be
 * added.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param schemaLocation Schema location attribute value
 * @return              Status of operation: 0 if OK, negative status
 *                      code if error.
 */
EXTERNXML int rtXmlSetSchemaLocation
(OSCTXT* pctxt, const OSUTF8CHAR* schemaLocation);

/**
 * This function sets the XML Schema Instance (xsi) no namespace schema
 * location attribute to be added to an encoded document.  This attribute is
 * optional: if not set, no xsi:noNamespaceSchemaLocation attribute will be
 * added.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param schemaLocation Schema location attribute value
 * @return              Status of operation: 0 if OK, negative status
 *                      code if error.
 */
EXTERNXML int rtXmlSetNoNSSchemaLocation
(OSCTXT* pctxt, const OSUTF8CHAR* schemaLocation);

/**
 * This function sets the SOAP version number.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param version       SOAP version number as 2 digit integer (for
 *                        example, 11 is SOAP version 1.1, 12 is
 *                        version 1.2, etc.)
 */
EXTERNXML void rtXmlSetSoapVersion (OSCTXT* pctxt, OSUINT8 version);

/**
 * This function sets the XML Schema Instance (xsi) type attribute value.
 * This will cause an xsi:type attribute to be added to the top level
 * element in an encoded XML instance.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param xsiType       xsi:type attribute value
 * @return              Status of operation: 0 if OK, negative status
 *                      code if error.
 */
EXTERNXML int rtXmlSetXSITypeAttr (OSCTXT* pctxt, const OSUTF8CHAR* xsiType);

/**
 * This function sets whether the Unicode byte order mark is encoded.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param write         TRUE to encode BOM, FALSE to not encode BOM.
 * @return              Status of operation: 0 if OK, negative status
 *                      code if error.
 */
EXTERNXML int rtXmlSetWriteBOM (OSCTXT* pctxt, OSBOOL write);

#include "rtxmlsrc/rtXmlNamespace.h"

/**
 * This function tests the context buffer for containing a correct
 * hexadecimal string.
 * It does not decode the value.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param minLength     A minimal length of expected string.
 * @param maxLength     A maximal length of expected string.
 * @return              If the string in the context buffer is a correct
 *    one, function returns zero. Error code (negative) will be
 *    returned otherwise. Note, error record will NOT be added in the
 *    error's list of the context.
 */
EXTERNXML int rtXmlMatchHexStr
(OSCTXT* pctxt, size_t minLength, size_t maxLength);

/**
 * This function tests the context buffer for containing a correct
 * base64 string.
 * It does not decode the value.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param minLength     A minimal length of expected string.
 * @param maxLength     A maximal length of expected string.
 * @return              If the string in the context buffer is a correct
 *    one, function returns zero. Error code (negative) will be
 *    returned otherwise. Note, error record will NOT be added in the
 *    error's list of the context.
 */
EXTERNXML int rtXmlMatchBase64Str
(OSCTXT* pctxt, size_t minLength, size_t maxLength);

/**
 * This function tests the context buffer for containing a correct
 * date string.
 * It does not decode the value.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @return              If the string in the context buffer is a correct
 *    one, function returns zero. Error code (negative) will be
 *    returned otherwise. Note, error record will NOT be added in the
 *    error's list of the context.
 */
EXTERNXML int rtXmlMatchDate (OSCTXT* pctxt);

/**
 * This function tests the context buffer for containing a correct
 * time string.
 * It does not decode the value.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @return              If the string in the context buffer is a correct
 *    one, function returns zero. Error code (negative) will be
 *    returned otherwise. Note, error record will NOT be added in the
 *    error's list of the context.
 */
EXTERNXML int rtXmlMatchTime (OSCTXT* pctxt);

/**
 * This function tests the context buffer for containing a correct
 * dateTime string.
 * It does not decode the value.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @return              If the string in the context buffer is a correct
 *    one, function returns zero. Error code (negative) will be
 *    returned otherwise. Note, error record will NOT be added in the
 *    error's list of the context.
 */
EXTERNXML int rtXmlMatchDateTime (OSCTXT* pctxt);

/**
 * This function tests the context buffer for containing a correct
 * gYear string.
 * It does not decode the value.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @return              If the string in the context buffer is a correct
 *    one, function returns zero. Error code (negative) will be
 *    returned otherwise. Note, error record will NOT be added in the
 *    error's list of the context.
 */
EXTERNXML int rtXmlMatchGYear (OSCTXT* pctxt);

/**
 * This function tests the context buffer for containing a correct
 * gYearMonth string.
 * It does not decode the value.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @return              If the string in the context buffer is a correct
 *    one, function returns zero. Error code (negative) will be
 *    returned otherwise. Note, error record will NOT be added in the
 *    error's list of the context.
 */
EXTERNXML int rtXmlMatchGYearMonth (OSCTXT* pctxt);

/**
 * This function tests the context buffer for containing a correct
 * gMonth string.
 * It does not decode the value.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @return              If the string in the context buffer is a correct
 *    one, function returns zero. Error code (negative) will be
 *    returned otherwise. Note, error record will NOT be added in the
 *    error's list of the context.
 */
EXTERNXML int rtXmlMatchGMonth (OSCTXT* pctxt);

/**
 * This function tests the context buffer for containing a correct
 * gMonthDay string.
 * It does not decode the value.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @return              If the string in the context buffer is a correct
 *    one, function returns zero. Error code (negative) will be
 *    returned otherwise. Note, error record will NOT be added in the
 *    error's list of the context.
 */
EXTERNXML int rtXmlMatchGMonthDay (OSCTXT* pctxt);

/**
 * This function tests the context buffer for containing a correct
 * gDay string.
 * It does not decode the value.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @return              If the string in the context buffer is a correct
 *    one, function returns zero. Error code (negative) will be
 *    returned otherwise. Note, error record will NOT be added in the
 *    error's list of the context.
 */
EXTERNXML int rtXmlMatchGDay (OSCTXT* pctxt);

/**
 * This function creates a new QName given the localName and prefix parts.
 *
 * @param pctxt         Pointer to a context structure.
 * @param localName     Element local name.
 * @param prefix        Namespace prefix.
 * @return              QName value.  Memory for the value will have been
 *                        allocated by rtxMemAlloc and thus must be freed
 *                        using one of the rtxMemFree functions.  The
 *                        value will be NULL if no dynamic memory was
 *                        available.
 */
EXTERNXML OSUTF8CHAR* rtXmlNewQName
(OSCTXT* pctxt, const OSUTF8CHAR* localName, const OSUTF8CHAR* prefix);

/**
 * This function compares an array of octets to a base64 string.
 *
 * @param nocts1        Number of octets in data1.
 * @param data1         Pointer to array of OSOCTET.
 * @param data2         Pointer to null-terminated array of OSUTF8CHAR.
 * @return              TRUE if data2 is a base64 string representation of
 *                      data1, false otherwise.
 */
EXTERNXML OSBOOL rtXmlCmpBase64Str (OSUINT32 nocts1,
                                    const OSOCTET* data1,
                                    const OSUTF8CHAR* data2);

/**
 * This function compares an array of octets to a hex string.
 *
 * @param nocts1        Number of octets in data1.
 * @param data1         Pointer to array of OSOCTET.
 * @param data2         Pointer to null-terminated array of OSUTF8CHAR.
 * @return              TRUE if data2 is a hex string representation of
 *                      data1, false otherwise.
 */
EXTERNXML OSBOOL rtXmlCmpHexStr (OSUINT32 nocts1,
                                 const OSOCTET* data1,
                                 const OSUTF8CHAR* data2);

EXTERNXML OSBOOL rtXmlCmpHexChar (OSUTF8CHAR ch, OSOCTET hexval);

EXTERNXML int rtSaxGetAttributeID (const OSUTF8CHAR* attrName,
                                   size_t nAttr,
                                   const OSUTF8CHAR* attrNames[],
                                   OSUINT32 attrPresent[]);

/**
 * This function looks up an attribute in the attribute array returned
 * by SAX to the startElement function.
 *
 * @param attrName      Name of the attribute to find.
 * @param attrs         Attribute array returned in SAX startElement
 *                        function.  This is an array of character strings
 *                        containing name1, value1, name2, value2, ...
 *                        List is terminated by a null name.
 * @return              Pointer to character string containing attribute
 *                        value or NULL if attrName not found.
 */
EXTERNXML const OSUTF8CHAR* rtSaxGetAttrValue
(const OSUTF8CHAR* attrName, const OSUTF8CHAR* const* attrs);

/**
 * This function looks up a sequence element name in the given
 * element info array.  If ensures elements are received in the
 * correct order and also sets the required element count variable.
 *
 * @param pState        The pointer to state variable to be changed.
 * @param prevElemIdx   Previous index of element. The search will be started
 *                      from this element for better performance.
 * @param localName     Local name of XML element
 * @param nsidx         Namespace index
 * @param idtab         Element ID table
 * @param fstab         Finite state table
 * @param fstabRows     Number of rows in \c fstab.
 * @param fstabCols     Number of columns in \c fstab.
 */
EXTERNXML OSINT16 rtSaxGetElemID
(OSINT16* pState, OSINT16 prevElemIdx, const OSUTF8CHAR* localName,
#ifdef OSXMLNS12
 OSINT32 nsidx, /* namespace index */
#endif
const OSSAXElemTableRec idtab[], const OSINT16* fstab,
 OSINT16 fstabRows, OSINT16 fstabCols);

/**
 * This function is a space optimized version of \c rtSaxGetElemID. It
 * operates with array of 8-bit integers (OSINT8) instead of 32-bit
 * integers (int).
 *
 * @param pState        The pointer to state variable to be changed.
 * @param prevElemIdx   Previous index of element. The search will be started
 *                      from this element + 1 for better performance.
 * @param localName     Local name of XML element
 * @param nsidx         Namespace index
 * @param idtab         Element ID table
 * @param fstab         Finite state table (array of 8-bit integers)
 * @param fstabRows     Number of rows in \c fstab.
 * @param fstabCols     Number of columns in \c fstab.
 */
EXTERNXML OSINT16 rtSaxGetElemID8
(OSINT16* pState, OSINT16 prevElemIdx, const OSUTF8CHAR* localName,
#ifdef OSXMLNS12
 OSINT32 nsidx, /* namespace index */
#endif
 const OSSAXElemTableRec idtab[], const OSINT8* fstab,
 OSINT16 fstabRows, OSINT16 fstabCols);

EXTERNXML OSINT16 rtSaxFindElemID
(OSINT16* pState, OSINT16 prevElemIdx, const OSUTF8CHAR* localName,
#ifdef OSXMLNS12
 OSINT32 nsidx, /* namespace index */
#endif
 const OSSAXElemTableRec idtab[], const OSINT16* fstab,
 OSINT16 fstabRows, OSINT16 fstabCols);

EXTERNXML OSINT16 rtSaxFindElemID8
(OSINT16* pState, OSINT16 prevElemIdx, const OSUTF8CHAR* localName,
#ifdef OSXMLNS12
 OSINT32 nsidx, /* namespace index */
#endif
 const OSSAXElemTableRec idtab[], const OSINT8* fstab,
 OSINT16 fstabRows, OSINT16 fstabCols);

/**
 * This function checks if the given attribute list contains one or
 * more XML namespace attributes (xmlns).
 *
 * @param attrs  Attribute list in form passed by parser into SAX
 *                 startElement function.
 * @return       TRUE, if xmlns attribute found in list.
 */
EXTERNXML OSBOOL rtSaxHasXMLNSAttrs (const OSUTF8CHAR* const* attrs);

/**
 * This function checks if the buffer in the context is empty or not.
 *
 * @param pctxt      Pointer to OSCTXT structure
 *
 * @return           TRUE, if the buffer contains empty string.
 */
EXTERNXML OSBOOL rtSaxIsEmptyBuffer (OSCTXT* pctxt);

EXTERNXML OSINT16 rtSaxLookupElemID
(OSCTXT* pctxt, OSINT16* pState, OSINT16 prevElemIdx,
 const OSUTF8CHAR* localName, const OSUTF8CHAR* qName,
#ifdef OSXMLNS12
 OSINT32 nsidx, /* namespace index */
#endif
 const OSSAXElemTableRec idtab[], const OSINT16* fstab,
 OSINT16 fstabRows, OSINT16 fstabCols);

EXTERNXML OSINT16 rtSaxLookupElemID8
(OSCTXT* pctxt, OSINT16* pState, OSINT16 prevElemIdx,
 const OSUTF8CHAR* localName, const OSUTF8CHAR* qName,
#ifdef OSXMLNS12
 OSINT32 nsidx, /* namespace index */
#endif
 const OSSAXElemTableRec idtab[], const OSINT8* fstab,
 OSINT16 fstabRows, OSINT16 fstabCols);

/**
 * This function parses the list of strings. It is used for parsing
 * NMTOKENS, IDREFS, NMENTITIES.
 *
 * @param pctxt         Pointer to OSCTXT structure.
 *                         Can be NULL, if pMemBuf is not NULL.
 * @param pMemBuf       Pointer to memory buffer structure.
 *                         Can be NULL, if pctxt is not NULL.
 * @param pvalue        Doubly-linked list for parsed strings.
 * @return              0 - if success, negative value is error.
 */
EXTERNXML int rtSaxStrListParse
(OSCTXT* pctxt, OSRTMEMBUF *pMemBuf, OSRTDList* pvalue);

/**
 * This function sorts a SAX attribute list in ascending order based on
 * attribute name. It currently only supports unqualified attributes.
 *
 * @param pctxt         Pointer to OSCTXT structure.
 * @param attrs         Standard SAX attribute list.  Entry i is attribute
 *                         name and i+1 is value.  List is terminated
 *                         by a null name.
 * @param order         Order array containing the order of sorted attributes.
 *                         This array is allocated using rtxMemAlloc, it
 *                         can be freed using rtxMemFreePtr or will be
 *                         freed when the context is freed.  The list
 *                         holds indicies to name items in the attribute
 *                         list that is passed in.
 * @return              If success, positive value contains number of
 *                         attributes in attrs; if failure, negative
 *                         status code.
 */
EXTERNXML int rtSaxSortAttrs
(OSCTXT* pctxt, const OSUTF8CHAR* const* attrs, OSUINT16** order);

/**
 * This function mathes the list of strings. It is used for matching
 * NMTOKENS, IDREFS, NMENTITIES.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @return              0 - if success, negative value is error.
 */
EXTERNXML int rtSaxStrListMatch (OSCTXT* pctxt);

EXTERNXML OSBOOL rtSaxTestFinal (OSINT16 state,
                                 OSINT16 currElemIdx,
                                 const int* fstab,
                                 int fstabRows, int fstabCols);

EXTERNXML OSBOOL rtSaxTestFinal8 (OSINT16 state,
                                  OSINT16 currElemIdx,
                                  const OSINT8* fstab,
                                 int fstabRows, int fstabCols);

EXTERNXML int rtSaxSetSkipLevelToCurrent (OSCTXT* pctxt, int stat);

EXTERNXML OSUINT32 rtSaxSetMaxErrors (OSCTXT* pctxt, OSUINT32 maxErrors);

EXTERNXML OSUINT32 rtSaxGetMaxErrors (OSCTXT* pctxt);

EXTERNXML int rtSaxTestAttributesPresent (OSCTXT* pctxt,
   const OSUINT32* attrPresent, const OSUINT32* reqAttrMask,
   const OSUTF8CHAR* const* attrNames, size_t numOfAttrs,
   const char* parentTypeName);

EXTERNXML OSBOOL rtSaxIncErrors (OSCTXT* pctxt);

EXTERNXML int rtSaxReportUnexpAttrs (OSCTXT* pctxt,
    const OSUTF8CHAR* const* attrs,
    const char* typeName);

/**
 * @defgroup rtXmlUtil XML utility functions.
 * @{
 */
/**
 * This function writes the encoded XML message stored in the context
 * message buffer out to a file.
 *
 * @param pctxt         Pointer to OSCTXT structure.
 * @param filename      Full path to file to which XML is to be written.
 * @return              0 - if success, negative value if error.
 */
EXTERNXML int rtXmlWriteToFile (OSCTXT* pctxt, const char* filename);

EXTERNXML int rtXmlWriteUTF16ToFile (OSCTXT* pctxt, const char* filename);

EXTERNXML void rtXmlTreatWhitespaces (OSCTXT* pctxt, int whiteSpaceType);

EXTERNXML int rtXmlCheckBuffer (OSCTXT* pctxt, size_t byte_count);

/**
 * @}
 */

EXTERNXML void rtErrXmlInit (void);
EXTERNXML int rtXmlPutChar (OSCTXT* pctxt, const OSUTF8CHAR value);
EXTERNXML int rtXmlWriteChars 
(OSCTXT* pctxt, const OSUTF8CHAR* value, size_t len);

/**
 * @defgroup rtXmlpDec XML pull-parser decode functions.
 * @{
 */
/**
 * This function decodes an arbitrary XML section of code as defined
 * by the XSD any type (xsd:any).  The decoded XML fragment is returned
 * as a string in the form as it appears in the document. Memory is
 * allocated for the string using the rtxMemAlloc function.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to UTF8 character string pointer to
 *                       receive decoded XML fragment.  Memory is
 *                       allocated for the string using the run-time
 *                       memory manager.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecAny (OSCTXT* pctxt, const OSUTF8CHAR** pvalue);

/**
 * This function decodes an any attribute string.  The full attribute
 * string (name="value") is decoded and returned on the string output
 * argument.  Memory is allocated for the string using the rtxMemAlloc
 * function.
 *
 * @param pctxt        Pointer to context block structure.
 * @param ppAttrStr    Pointer to UTF8 character string pointer to
 *                       receive decoded attribute string.  Memory is
 *                       allocated for the string using the run-time
 *                       memory manager.
 * @param index        Index of attribute.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecAnyAttrStr
(OSCTXT* pctxt, const OSUTF8CHAR** ppAttrStr, size_t index);

/**
 * This function decodes an arbitrary XML section of code as defined
 * by the XSD any type (xsd:any).  The decoded XML fragment is returned
 * as a string in the form as it appears in the document. Memory is
 * allocated for the string using the rtxMemAlloc function.  The difference
 * between this function and rtXmlpDecAny is that this function preserves
 * the full encoded XML fragment including the start and end elements tags
 * and attributes.  rtXmlpDecAny decodes the contents within the start
 * and end tags.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to UTF8 character string pointer to
 *                       receive decoded XML fragment.  Memory is
 *                       allocated for the string using the run-time
 *                       memory manager.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecAnyElem (OSCTXT* pctxt, const OSUTF8CHAR** pvalue);

/**
 * This function decodes a contents of a Base64-encode binary string into a
 * static memory structure. The octet string must be Base64 encoded. This
 * function call is used to decode a sized base64Binary string production.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
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
EXTERNXML int rtXmlpDecBase64Str (OSCTXT* pctxt, OSOCTET* pvalue,
   OSUINT32* pnocts, OSINT32 bufsize);

/**
 * This function will decode a variable of the XSD integer type. In this
 * case, the integer is assumed to be of a larger size than can fit in a C or
 * C++ long type (normally 32 or 64 bits). For example, parameters used to
 * calculate security values are typically larger than these sizes.
 *
 * These variables are stored in character string constant variables.
 * The radix should be 10. If it is necessary to
 * convert to another radix, then use rtxBigIntSetStr or rtxBigIntToString
 * functions.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to a pointer to receive decoded UTF-8
 *                       string. Dynamic memory is allocated
 *                       for the variable using the rtMemAlloc function. The
 *                       decoded variable is represented as a string starting
 *                       with appropriate prefix.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecBigInt (OSCTXT* pctxt, const OSUTF8CHAR** pvalue);

/**
 * This function decodes a bit string value.  The string consists of a
 * series of '1' and '0' characters.  This is the static version in which
 * the user provides a pre-allocated memory buffer to receive the decoded
 * data.  One byte in a memory buffer can hold 8 characters of encoded
 * data.  Bits are stored from MSB to LSB order.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to a variable to receive the decoded boolean
 *                       value.
 * @param pnbits       Pointer to hold decoded number of bits.
 * @param bufsize      Size of buffer passed in pvalue argument.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecBitString
(OSCTXT* pctxt, OSOCTET* pvalue, OSUINT32* pnbits, OSUINT32 bufsize);

/**
 * This function decodes a variable of the boolean type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to a variable to receive the decoded boolean
 *                       value.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecBool (OSCTXT* pctxt, OSBOOL* pvalue);

/**
 * This function decodes a variable of the XSD 'date' type.
 * Input is expected to be a string of characters returned
 * by an XML pull parser. The string should have CCYY-MM-DD format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecDate (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes a variable of the XSD 'dateTime' type.
 * Input is expected to be a string of characters returned
 * by an XML pull parser.

 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecDateTime (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes the contents of a decimal data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to 64-bit double value to receive
 *                       decoded result.
 * @param totalDigits  Number of total digits in the decimal number from
 *                       XSD totalDigits facet value.  Argument should be
 *                       set to -1 if facet not given.
 * @param fractionDigits Number of fraction digits in the decimal number from
 *                       XSD fractionDigits facet value. Argument should be
 *                       set to -1 if facet not given.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecDecimal (OSCTXT* pctxt, OSREAL* pvalue,
   int totalDigits, int fractionDigits);

/**
 * This function decodes the contents of a float or double data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to 64-bit double value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecDouble (OSCTXT* pctxt, OSREAL* pvalue);

#define OSXMLREALENC_OBJSYS 0x1F   //Obj-Sys XML encoding rules
#define OSXMLREALENC_BXER 0x10     //basic-XER
#define OSXMLREALENC_EXERMODS 0x1B //extended-XER with MODIFIED-ENCODINGS
#define OSXMLREALENC_EXERDECIMAL 0x03 //extended-XER with DECIMAL

/**
 * This function decodes the contents of a float or double data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param flags        Specifies alternatives allowed in the lexical value.
 *                     See OSXMLREALENC* constants.
 *                     bit 0 (rightmost) set: recognize INF, -INF, NaN text
 *                         values
 *                     bit 1 set: recognize leading '+' on normal reals
 *                     bit 2 set: recognize leading '+' on INF
 *                     bit 3 set: recognize leading zeros in exponent
 *                     bit 4 set: recognize exponents
 * @param pvalue       Pointer to 64-bit double value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecDoubleExt (OSCTXT* pctxt, OSUINT8 flags, 
                                       OSREAL* pvalue);


/**
 * This function decodes a contents of a Base64-encode binary string.
 * The octet string must be Base64 encoded. This
 * function will allocate dynamic memory to store the decoded result.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
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
EXTERNXML int rtXmlpDecDynBase64Str (OSCTXT* pctxt, OSDynOctStr* pvalue);

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
EXTERNXML int rtXmlpDecDynBitString (OSCTXT* pctxt, OSDynOctStr* pvalue);

/**
 * This function decodes a contents of a hexBinary string. This
 * function will allocate dynamic memory to store the decoded result.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
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
EXTERNXML int rtXmlpDecDynHexStr (OSCTXT* pctxt, OSDynOctStr* pvalue);

/**
 * This function decodes a Unicode string data type. The input
 * is assumed to be in UTF-8 format.  This function reads each character
 * and converts it into its Unicode equivalent.
 *
 * @param pctxt        Pointer to context block structure.
 * @param ppdata       Pointer to Unicode character string.  A Unicode
 *                       character string is represented as an array
 *                       of unsigned 16-bit integers in C.  Memory is
 *                       allocated for the string using the run-time
 *                       memory manager.
 * @param pnchars      Pointer to integer variables to receive the number
 *                       of characters in the string.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecDynUnicodeStr
(OSCTXT* pctxt, const OSUNICHAR** ppdata, OSUINT32* pnchars);

/**
 * This function decodes the contents of a UTF-8 string data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param outdata      Pointer to a pointer to receive decoded UTF-8
 *                       string.  Memory is allocated for this string
 *                       using the run-time memory manager.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecDynUTF8Str (OSCTXT* pctxt, const OSUTF8CHAR** outdata);


/**
 * This function decodes the contents of a UTF-8 string data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param out          Pointer to an array of OSUTF8CHAR to receive decoded
 *                     UTF-8 string.
 * @param max_len      Length of out array.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecUTF8Str
(OSCTXT* pctxt, OSUTF8CHAR* out, size_t max_len);

/**
 * This function decodes a variable of the XSD 'gDay' type.
 * Input is expected to be a string of characters returned
 * by an XML pull parser. The string should have ---DD[-+hh:mm|Z] format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecGDay (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes a variable of the XSD 'gMonth' type.
 * Input is expected to be a string of characters returned
 * by an XML pull parser. The string should have --MM[-+hh:mm|Z] format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecGMonth (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes a variable of the XSD 'gMonthDay' type.
 * Input is expected to be a string of characters returned
 * by an XML pull parser. The string should have --MM-DD[-+hh:mm|Z] format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecGMonthDay (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes a variable of the XSD 'gYear' type.
 * Input is expected to be a string of characters returned
 * by an XML pull parser. The string should have CCYY[-+hh:mm|Z] format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecGYear (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes a variable of the XSD 'gYearMonth' type.
 * Input is expected to be a string of characters returned
 * by an XML pull parser. The string should have CCYY-MM[-+hh:mm|Z] format.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       OSXSDDateTime type pointer points to a OSXSDDateTime
 *                     value to receive decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecGYearMonth (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes the contents of a hexBinary string into a static
 * memory structure.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
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
EXTERNXML int rtXmlpDecHexStr
(OSCTXT* pctxt, OSOCTET* pvalue, OSUINT32* pnocts, OSINT32 bufsize);

/**
 * This function decodes the contents of a 32-bit integer data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to 32-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecInt (OSCTXT* pctxt, OSINT32* pvalue);

/**
 * This function decodes the contents of an 8-bit integer data type
 * (i.e. a signed byte type in the range of -128 to 127).
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to 8-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecInt8 (OSCTXT* pctxt, OSINT8* pvalue);

/**
 * This function decodes the contents of a 16-bit integer data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to 16-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecInt16 (OSCTXT* pctxt, OSINT16* pvalue);

/**
 * This function decodes the contents of a 64-bit integer data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to 64-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecInt64 (OSCTXT* pctxt, OSINT64* pvalue);

/**
 * This function decodes the contents of a named bit field.  This
 * is a space-separated list of token values in which each token
 * corresponds to a bit field in a bit map.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pBitMap      Pointer to bit map structure that defined token to
 *                       bit mappings.
 * @param pvalue       Pointer to buffer to recieve decoded bit map.
 * @param pnbits       Number of bits in decoded bit map.
 * @param bufsize      Size of buffer passed in to received decoded bit
 *                       values.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecNamedBits
(OSCTXT* pctxt, const OSBitMapItem* pBitMap,
 OSOCTET* pvalue, OSUINT32* pnbits, OSUINT32 bufsize);

/**
 * This function decodes a list of space-separated tokens and returns
 * each token as a separate item on the given list.  Memory is allocated
 * for the list nodes and token values using the rtx memory management
 * functions.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param plist        A pointer to a linked list structure to which the
 *                       parsed token values will be added.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecStrList (OSCTXT* pctxt, OSRTDList* plist);

/**
 * This function decodes a variable of the XSD 'time' type.
 * Input is expected to be a string of characters returned
 * by an XML pull parser. The string should have one of following formats:
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
EXTERNXML int rtXmlpDecTime (OSCTXT* pctxt, OSXSDDateTime* pvalue);

/**
 * This function decodes the contents of an unsigned 32-bit integer data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to unsigned 32-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecUInt (OSCTXT* pctxt, OSUINT32* pvalue);

/**
 * This function decodes the contents of an unsigned 8-bit integer data type
 * (i.e. a signed byte type in the range of 0 to 255).
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to unsigned 8-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecUInt8 (OSCTXT* pctxt, OSOCTET* pvalue);

/**
 * This function decodes the contents of an unsigned 16-bit integer data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to unsigned 16-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecUInt16 (OSCTXT* pctxt, OSUINT16* pvalue);

/**
 * This function decodes the contents of an unsigned 64-bit integer data type.
 * Input is expected to be a string of OSUTF8CHAR characters returned
 * by an XML pull parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pvalue       Pointer to unsigned 64-bit integer value to receive
 *                       decoded result.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecUInt64 (OSCTXT* pctxt, OSUINT64* pvalue);

/**
 * This function decodes the contents of an XML string data type.
 * This type contains a pointer to a UTF-8 characer string plus flags
 * that can be set to alter the encoding of the string (for example, the
 * cdata flag allows the string to be encoded in a CDATA section).
 * Input is expected to be a string of UTF-8 characters returned by
 * an XML parser.
 *
 * @param pctxt        Pointer to context block structure.
 * @param outdata      Pointer to an XML string structure to receive the
 *                       decoded string.  Memory is allocated for the string
 *                       using the run-time memory manager.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecXmlStr (OSCTXT* pctxt, OSXMLSTRING* outdata);

/**
 * This function decodes a list of space-separated tokens and returns
 * each token as a separate item on the given list.  Memory is allocated
 * for the list nodes and token values using the rtx memory management
 * functions. List contains OSXMLSTRING structures.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param plist        A pointer to a linked list structure to which the
 *                       parsed token values will be added.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecXmlStrList (OSCTXT* pctxt, OSRTDList* plist);

/**
 * This function decodes XSI (XML Schema Instance) and XML namespace
 * attributes that may be present in any arbitrary XML element within
 * a document.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param attrName     A pointer to a structure holding various parts of
 *                       an attribute name. The parts are in the form of
 *                       string fragments meaning they are not null
 *                       terminated.  The user must be careful to use
 *                       the value and length when working with them.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecXSIAttr
(OSCTXT* pctxt, const OSXMLNameFragments* attrName);

/**
 * This function decodes the contents of an XSI (XML Schema Instance) type
 * attribute (xsi:type).
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param attrName     A pointer to a structure holding various parts of
 *                       an attribute name. The parts are in the form of
 *                       string fragments meaning they are not null
 *                       terminated.  The user must be careful to use
 *                       the value and length when working with them.
 * @param ppAttrValue  A pointer to a pointer to a UTF8 character string
 *                       to received the decoded XSI type name.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - 1 = OK, but attrName was not xsi:type (i.e. no
 *                           attribute match)
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecXSITypeAttr
(OSCTXT* pctxt, const OSXMLNameFragments* attrName,
 const OSUTF8CHAR** ppAttrValue);

/**
 * This function finds an attribute in the descriptor table.
 *
 * @param attrName     A pointer to a structure holding various parts of
 *                       an attribute name. The parts are in the form of
 *                       string fragments meaning they are not null
 *                       terminated.  The user must be careful to use
 *                       the value and length when working with them.
 * @param nsidx        Namespace index:
 *                       - 0 = attribute is unqualified,
 *                       - positive value is user namespace from
 *                          generated namespace table,
 *                       - negative value is predefined namespace
 *                          like XSD instance and ect.
 * @param nAttr        Number of descriptors in table.
 * @param attrNames    Attributes descriptor table.
 * @param attrPresent  Bit array to mark already decoded attributes. It is used
 *                       to identify duplicate attributes.
 * @return             Completion status of operation:
 *                       - positive or zero return value is
 *                          attribute index in descriptor table,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpGetAttributeID
(const OSXMLStrFragment* attrName, OSINT16 nsidx,
 size_t nAttr, const OSXMLAttrDescr attrNames[], OSUINT32 attrPresent[]);

/**
 * This function parse the next element start tag.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pElem        Pointer to a structure to receive the decoded element
 *                        descriptor.
 * @param level        Nesting level of parsed start tag. When value equal -1
 *                     parsed next start tag.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpGetNextElem
(OSCTXT* pctxt, OSXMLElemDescr* pElem, OSINT32 level);

/**
 * This function parses the next start tag and finds the index of
 * the element name in the descriptor table.
 *
 * @param pctxt        Pointer to context block structure.
 * @param tab          Elements descriptor table.
 * @param nrows        Number of descriptors in table.
 * @param level        Nesting level of parsed start tag. When value equal -1
 *                        function parse next start tag.
 * @param continueParse When value equals TRUE function skips unrecignized
 *                        elements.
 * @return             Completion status of operation:
 *                       - positive or zero value is element identifier,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpGetNextElemID
(OSCTXT* pctxt, const OSXMLElemIDRec* tab, size_t nrows, OSINT32 level,
 OSBOOL continueParse);

/**
 * This function marks current tag as unprocessed. This will cause the element
 * to be processed again in the next pull-parser function.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpMarkLastEventActive (OSCTXT* pctxt);

/**
 * This function parses the next start tag that matches with given name.
 *
 * @param pctxt        Pointer to context block structure.
 * @param elemLocalName Name of parsed element.
 * @param nsidx        Namespace index:
 *                       - 0 = attribute is unqualified,
 *                       - positive value is user namespace from
 *                          generated namespace table,
 *                       - negative value is predefined namespace
 *                          like XSD instance and ect.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpMatchStartTag
   (OSCTXT* pctxt, const OSUTF8CHAR* elemLocalName, OSINT16 nsidx);

/**
 * This function parse next end tag that matches with given name.
 *
 * @param pctxt        Pointer to context block structure.
 * @param level        Nesting level of parsed start tag. When value equal -1
 *                        function parse next end tag with current level.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpMatchEndTag (OSCTXT* pctxt, OSINT32 level);

/**
 * This function checks accessibility of attributes.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             Completion status of operation:
 *                       - TRUE = attributes are present and
 *                          access is enabled,
 *                       - FALSE = attributes are absent or
 *                          access is disabled.
 */
EXTERNXML OSBOOL rtXmlpHasAttributes (OSCTXT* pctxt);

/**
 * This function returns number of attributes in last processed start tag.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             Completion status of operation:
 *                       - zero or positive value is atributes number,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpGetAttributeCount (OSCTXT* pctxt);

/**
 * This function selects attribute to decode.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pAttr        Pointer to structure to receive the
 *                       various parts of an attribute name.
 * @param nsidx        Pointer to value to receive namespace index:
 *                       - 0 = attribute is unqualified,
 *                       - positive value is user namespace from
 *                          generated namespace table,
 *                       - negative value is predefined namespace
 *                          like XSD instance and ect (see enum OSXMLNsIndex)
 * @param index        Index of selected attribute.
 * @return             Completion status of operation:
 *                       - positive or zero return value is
 *                          attribute index in descriptor table,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpSelectAttribute
(OSCTXT* pctxt, OSXMLNameFragments* pAttr,  OSINT16* nsidx, size_t index);

/**
 * This function returns current nesting level.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             Current nesting level.
 */
EXTERNXML OSINT32 rtXmlpGetCurrentLevel (OSCTXT* pctxt);

/**
 * Sets the whitespace treatment mode. This mode affects the content
 * and attribute values reading. For example, if OSXMLWSM_COLLAPSE
 * mode is set then all spaces in returned data will be already
 * collapsed.
 *
 * @param pctxt        Pointer to context block structure.
 * @param whiteSpaceMode White space mode.
 * @return  Previously set whitespace mode.
 */
EXTERNXML void rtXmlpSetWhiteSpaceMode
   (OSCTXT* pctxt, OSXMLWhiteSpaceMode whiteSpaceMode);

/**
 * Sets mixed content mode.
 *
 * @param pctxt        Pointer to context block structure.
 * @param mixedContentMode Enable/disable mixed mode.
 * @return             Previously state of mixed mode.
 */
EXTERNXML OSBOOL rtXmlpSetMixedContentMode
   (OSCTXT* pctxt, OSBOOL mixedContentMode);

/**
 * Sets list mode. Attribute value or element content is decoded by tokens.
 *
 * @param pctxt        Pointer to context block structure.
 */
EXTERNXML void rtXmlpSetListMode (OSCTXT* pctxt);

/**
 * Check for end of decoded token list.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             State of decoded list:
 *                       - TRUE = list is not finished,
 *                       - FALSE = all tokens was decoded.
 */
EXTERNXML OSBOOL rtXmlpListHasItem (OSCTXT* pctxt);

/**
 * Count tokens in list.
 *
 * @param pctxt        Pointer to context block structure.
 * @param itemCnt      Pointer to number of elements in list.
 * @return             Token number. For element content function check
 *                       accessed part of content only. Returned value
 *                       may be below then real token number.
 */
EXTERNXML void rtXmlpCountListItems (OSCTXT* pctxt, OSSIZE* itemCnt);

/**
 * This function parses the next start tag and find index of element name in
 * descriptor table. It is used to decode sequences in strict mode.
 *
 * @param pctxt        Pointer to context block structure.
 * @param tab          Elements descriptor table.
 * @param pGroup       Decode groups table.
 * @param curID        Current decode group.
 * @param lastMandatoryID Identifier of last mandatory element.
 * @param groupMode    This parameter must be setted to TRUE when decode
 *                      groups or base types.
 * @return             Completion status of operation:
 *                       - positive or zero value is element identifier,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpGetNextSeqElemID
(OSCTXT* pctxt, const OSXMLElemIDRec* tab, const OSXMLGroupDesc* pGroup,
 int curID, int lastMandatoryID, OSBOOL groupMode);

/**
 * This function parses the next start tag and finds index of element name in
 * descriptor table. It used for decode "all" content model in strict mode.
 *
 * @param pctxt        Pointer to context block structure.
 * @param tab          Elements descriptor table.
 * @param nrows        Number of descriptors in table.
 * @param pOrder       Pointer to array to receive elements order.
 * @param nOrder       Last element's index in order array.
 * @param maxOrder     Size of order array.
 * @param anyID        Identifier of xsd:any element.
 * @return             Completion status of operation:
 *                       - positive or zero value is element identifier,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpGetNextAllElemID
(OSCTXT* pctxt, const OSXMLElemIDRec* tab, size_t nrows,
 const OSUINT8* pOrder, OSUINT32 nOrder, OSUINT32 maxOrder, int anyID);

/**
 * This function parses the next start tag and finds index of element name in
 * descriptor table. It used for decode "all" content model in strict mode.
 * This variant used when xsd:all has above 256 elements.
 *
 * @param pctxt        Pointer to context block structure.
 * @param tab          Elements descriptor table.
 * @param nrows        Number of descriptors in table.
 * @param pOrder       Pointer to array to receive elements order.
 * @param nOrder       Last element's index in order array.
 * @param maxOrder     Size of order array.
 * @param anyID        Identifier of xsd:any element.
 * @return             Completion status of operation:
 *                       - positive or zero value is element identifier,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpGetNextAllElemID16
(OSCTXT* pctxt, const OSXMLElemIDRec* tab, size_t nrows,
 const OSUINT16* pOrder, OSUINT32 nOrder, OSUINT32 maxOrder, int anyID);


/**
 * This function parses the next start tag and finds index of element name in
 * descriptor table. It used for decode "all" content model in strict mode.
 *
 * @param pctxt        Pointer to context block structure.
 * @param tab          Elements descriptor table.
 * @param nrows        Number of descriptors in table.
 * @param pOrder       Pointer to array to receive elements order.
 * @param nOrder       Last element's index in order array.
 * @param maxOrder     Size of order array.
 * @param anyID        Identifier of xsd:any element.
 * @return             Completion status of operation:
 *                       - positive or zero value is element identifier,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpGetNextAllElemID32
(OSCTXT* pctxt, const OSXMLElemIDRec* tab, size_t nrows,
 const OSUINT32* pOrder, OSUINT32 nOrder, OSUINT32 maxOrder, int anyID);


/**
 * Sets user namespace table.
 *
 * @param pctxt        Pointer to context block structure.
 * @param namespaceTable Array of namespace URI strings.
 * @param nmNamespaces Number of namespaces in table.
 */
EXTERNXML void rtXmlpSetNamespaceTable
(OSCTXT* pctxt, const OSUTF8CHAR* namespaceTable[], size_t nmNamespaces);

/**
 * Creates pull parser reader structure within the context.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpCreateReader (OSCTXT* pctxt);

/**
 * Disable access to attributes. Function used in derived types to disable
 * repeated decode in base type.
 *
 * @param pctxt        Pointer to context block structure.
 */
EXTERNXML void rtXmlpHideAttributes (OSCTXT* pctxt);

/**
 * This function checks if attributes were previously decoded.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             State of attributes:
 *                       - TRUE = attributes was not decoded,
 *                       - FALSE = attributes had been decoded.
 */
EXTERNXML OSBOOL rtXmlpNeedDecodeAttributes (OSCTXT* pctxt);

/**
 * Save current decode position.
 *
 * @param pctxt        Pointer to context block structure.
 */
EXTERNXML void rtXmlpMarkPos (OSCTXT* pctxt);

/**
 * Rewind to saved decode position.
 *
 * @param pctxt        Pointer to context block structure.
 */
EXTERNXML void rtXmlpRewindToMarkedPos (OSCTXT* pctxt);

/**
 * Reset saved decode position.
 *
 * @param pctxt        Pointer to context block structure.
 */
EXTERNXML void rtXmlpResetMarkedPos (OSCTXT* pctxt);

/**
 * This function decodes the contents of an XSI (XML Schema Instance) type
 * attribute (xsi:type).
 *
 * @param pctxt        Pointer to context block structure.
 * @param ppAttrValue  A pointer to a pointer to a UTF8 character string
 *                       to received the decoded XSI type QName.
 * @param nsidx        A pointer to OSINT16 value to received the decoded XSI
 *                       type namespace index.
 * @param pLocalOffs   A pointer to size_t value to received the local name
 *                       offset in ppAttrValue. When pLocalOffs value equal
 *                       NULL function return in ppAttrValue local name only.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpGetXSITypeAttr
(OSCTXT* pctxt, const OSUTF8CHAR** ppAttrValue, OSINT16* nsidx,
 size_t* pLocalOffs);

/**
 * This function decodes namespace attributes from start tag and adds them to
 * the given list.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pNSAttrs     A pointer to a linked list of OSXMLNamespace structures.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpGetXmlnsAttrs (OSCTXT* pctxt, OSRTDList* pNSAttrs);

/**
 * This function decodes XSI (XML Schema Instance) that may be present in
 * any arbitrary XML element within a document.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpDecXSIAttrs (OSCTXT* pctxt);

/**
 * Check element content: empty or not.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             Status of element content:
 *                       - TRUE = element is empty,
 *                       - FALSE = element has content.
 */
EXTERNXML OSBOOL rtXmlpIsEmptyElement (OSCTXT* pctxt);

/**
 * This function used only in C14 mode. It provide atributes sorting.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlEncAttrC14N (OSCTXT* pctxt);

/**
 * This function fetches the XML reader structure from the context for
 * use in low-level pull parser calls.  If the reader structure does not
 * exist, it is created and initialized.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             Pointer to XML reader structure or NULL if an
 *                       error occurred.
 */
EXTERNXML struct OSXMLReader* rtXmlpGetReader (OSCTXT* pctxt);

/**
 * Check processing status of current tag.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             Status of element content:
 *                       - TRUE = tag marked as processed,
 *                       - FALSE = tag will be processed again.
 */
EXTERNXML OSBOOL rtXmlpIsLastEventDone (OSCTXT* pctxt);

/**
 * This function decodes the contents of an XSI (XML Schema Instance) type
 * attribute (xsi:type) and find type index in descriptor table.
 *
 * @param pctxt        Pointer to context block structure.
 * @param typetab      XSI types descriptor table.
 * @param typetabsiz   Number of descriptors in table.
 * @return             Completion status of operation:
 *                       - positive or zero value is type index,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpGetXSITypeIndex
(OSCTXT* pctxt, const OSXMLItemDescr typetab[], size_t typetabsiz);

/**
 * This function find index of XSI (XML Schema Instance) type
 * in descriptor table.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pXsiType     A pointer to XSI type name.
 * @param xsiTypeIdx   A XSI type namespace index.
 * @param typetab      XSI types descriptor table.
 * @param typetabsiz   Number of descriptors in table.
 * @return             Completion status of operation:
 *                       - positive or zero value is type index,
 *                       - negative return value is error.
 */
EXTERNXML int rtXmlpLookupXSITypeIndex
(OSCTXT* pctxt, const OSUTF8CHAR* pXsiType, OSINT16 xsiTypeIdx,
 const OSXMLItemDescr typetab[], size_t typetabsiz);

/**
 * Disable skipping of unknown elements in optional sequence tail.
 * Function used in outer types to break decode on first unknown element
 * after decoding mandatory sequence part.
 *
 * @param pctxt        Pointer to context block structure.
 */
EXTERNXML void rtXmlpForceDecodeAsGroup (OSCTXT* pctxt);

/**
 * This function checks if "decode as group" mode was forced.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             State of mode:
 *                       - TRUE = need decode as group,
 *                       - FALSE = normal sequence decoding.
 */
EXTERNXML OSBOOL rtXmlpIsDecodeAsGroup (OSCTXT* pctxt);

/**
 * This function checks if the encoding specified in XML header
 * is UTF-8.
 *
 * @param pctxt        Pointer to context block structure.
 * @return             State of mode:
 *                       - TRUE = is in UTF-8 encoding,
 *                       - FALSE = is not in UTF-8 encoding.
 */
EXTERNXML OSBOOL rtXmlpIsUTF8Encoding (OSCTXT* pctxt);

/**
 * This function reads the specified number of bytes directly from the
 * underlying XML parser stream.  It has no effect on any of the parser
 * state variables.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pbuf         Pointer to static buffer (byte array) to receive
 *                       data.  The buffer must be at least large enough to
 *                       hold the requested number of bytes.
 * @param nbytes       Number of bytes to read.
 * @return             State of mode:
 *                       - TRUE = is in UTF-8 encoding,
 *                       - FALSE = is not in UTF-8 encoding.
 */
EXTERNXML int rtXmlpReadBytes (OSCTXT* pctxt, OSOCTET* pbuf, size_t nbytes);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
