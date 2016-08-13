/*
 * Copyright (c) 2003-2013 Objective Systems, Inc.
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
 * @file rtXmlNamespace.h
 * XML namespace handling structures and function definitions.
 */
#ifndef _RTXMLNAMESPACE_H_
#define _RTXMLNAMESPACE_H_

#include "rtxsrc/rtxContext.h"
#include "rtxsrc/rtxDynPtrArray.h"
#include "rtxsrc/rtxXmlQName.h"
#include "rtxmlsrc/rtXmlExternDefs.h"

/* Well-known namespace URI's */
#define OSXMLNSURI "http://www.w3.org/XML/1998/namespace"
#define OSXSINSURI "http://www.w3.org/2001/XMLSchema-instance"
#define OSXSINSURI_LEN 41
#define OSXSDNSURI "http://www.w3.org/2001/XMLSchema"

typedef struct OSXMLNamespace {
   const OSUTF8CHAR* prefix;
   const OSUTF8CHAR* uri;
} OSXMLNamespace;

/* This structure links a namespace prefix to an entry in a generated
   namespace table. */
typedef struct OSXMLNSPfxLink {
   OSINT32 nsidx;
   OSXMLNamespace ns;  /* primary namespace prefix is in this record. It may be empty. */
   OSRTDynPtrArray extraPrefixes; /* holds additional prefixes for this URI, all of which must be non-empty */
   struct OSXMLNSPfxLink* next;
} OSXMLNSPfxLink;

/* Node in namespace-prefix link stack */
typedef struct OSXMLNSPfxLinkStackNode {
   struct OSXMLNSPfxLink* link;
   struct OSXMLNSPfxLinkStackNode* next;
} OSXMLNSPfxLinkStackNode;

/* This structure is used to maintain a stack of namespace prefix links
   within the context.  An entry is added to the stack at each level where
   an xmlns attribute is detected. */
typedef struct OSXMLNSPfxLinkStack {
   OSINT32      count;  /* count of elements in stack */
   OSXMLNSPfxLinkStackNode* top;  /* top of stack     */
} OSXMLNSPfxLinkStack;

/* This structure describes a namespace URI table */
typedef struct OSXMLNSURITable {
   OSUINT32     nrows;
   const OSUTF8CHAR** data;
} OSXMLNSURITable;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function adds a namespace to the context namespace list.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param pNSAttrs      Namespace attribute list to which namespace info
 *                        is to be added.
 * @param prefix        Namespace prefix to be added
 * @param uri           Namespace URI to be added
 * @return              Pointer to namespace structure or NULL if not added.
 */
EXTERNXML OSXMLNamespace* rtXmlNSAddNamespace
(OSCTXT* pctxt, OSRTDList* pNSAttrs, const OSUTF8CHAR* prefix,
 const OSUTF8CHAR* uri);

/**
 * This function checks if two namespace records are equal.  This does a
 * a deep compare in that it will first check if the pointers are equal
 * and then it will check if the contents are equal (same prefix and URI).
 *
 * @param pNS1          Pointer to first namespace records to check.
 * @param pNS2          Pointer to second record.
 * @return              True if records are equal, false otherwise.
 */
EXTERNXML OSBOOL rtXmlNSEqual (OSXMLNamespace* pNS1, OSXMLNamespace* pNS2);

/**
 * This function frees dynamic memory used to hold namespace attribute
 * values.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param pNSAttrs      Pointer to namespace attribute list to be freed.
 */
EXTERNXML void rtXmlNSFreeAttrList (OSCTXT* pctxt, OSRTDList* pNSAttrs);

/**
 * This function gets a namespace prefix assigned to the given URI.
 * This gives preference to empty prefixes.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param uri           Namespace URI to be searched for
 * @return              Pointer to namespace prefix string. If a NULL or empty
 *                      prefix was assigned to the URI, an empty sring is
 *                      returned. Otherwise, any assigned prefix is returned.
 *                      If no prefix was assigned, null is returned.
 */
EXTERNXML const OSUTF8CHAR* rtXmlNSGetPrefix
(OSCTXT* pctxt, const OSUTF8CHAR* uri);

/**
 * This function gets a namespace prefix assigned to the given URI
 * using the given index to select a specific prefix from the URI/prefix
 * map.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param uri           Namespace URI to be searched for
 * @param idx           Index to prefix in map.  This only has meaning
 *                        when multiple prefixes have been assigned to
 *                        the given URI.
 * @return              Pointer to namespace prefix string
 */
EXTERNXML const OSUTF8CHAR* rtXmlNSGetPrefixUsingIndex
(OSCTXT* pctxt, const OSUTF8CHAR* uri, OSUINT32 idx);

/**
 * This function returns the total number of prefixes currently assigned
 * to the given URI.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param uri           Namespace URI to be searched for
 * @return              Count of prefixes assigned to the URI.
 */
EXTERNXML OSUINT32 rtXmlNSGetPrefixCount
(OSCTXT* pctxt, const OSUTF8CHAR* uri);

/**
 * This function gets the index of a given prefix in the internal list of
 * prefixes maintained for a given URI in teh namespace stack.  It also
 * may return the total number of prefixes currently assigned to the URI.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param uri           Namespace URI to be searched for
 * @param prefix        Namespace prefix for which to get index.
 * @param pcount        Optional pointer to an integer count variable.
 *                        If provided, the total number of prefixes
 *                        currently assigned to the URI will be returned.
 * @return              Index to namespace prefix or -1 if the prefix is
 *                        not assigned to the given URI.
 */
EXTERNXML int rtXmlNSGetPrefixIndex
(OSCTXT* pctxt, const OSUTF8CHAR* uri, const OSUTF8CHAR* prefix,
 OSUINT32* pcount);

/**
 * This function creates a QName in the given fixed-site buffer.  If
 * the name will not fit in the buffer, it is truncated.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param buf           Buffer into which qname will be written.
 * @param bufsiz        Size of the buffer.
 * @param uri           Namespace URI.
 * @param localName     Local name of the item.
 * @return              Pointer to QName buffer (buf).
 */
EXTERNXML const OSUTF8CHAR* rtXmlNSGetQName
(OSCTXT* pctxt, OSUTF8CHAR* buf, size_t bufsiz, const OSUTF8CHAR* uri,
 const OSUTF8CHAR* localName);

/**
 * This function returns a namespace prefix for use with attributes in the
 * given namespace.  If a prefix is not found for the namespace, a new namespace
 * entry is created with a generated prefix.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param namespaceURI  Pointer to namespace URI
 * @param pNSAttrs      List of namespace records. If null, the namespace list
 *                      in the context will be used.
 * @return              The prefix.
 */
EXTXMLMETHOD const OSUTF8CHAR* rtXmlNSGetAttrPrefix (OSCTXT* pctxt,
   const OSUTF8CHAR* namespaceURI, OSRTDList* pNSAttrs);

/**
 * This function creates a QName for a qualified attribute.  If a prefix
 * is not found for the name, a new namespace entry is created with a
 * generated prefix.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param buf           Buffer into which qname will be written.
 * @param bufsiz        Size of the buffer.
 * @param pNS           Pointer to namespace URI and prefix structure.
 * @param localName     Local name of the item.
 * @param pNSAttrs      List of namespace records. If null, the namespace list
 *                      in the context will be used.
 * @return              Pointer to QName buffer (buf).
 */
EXTERNXML const OSUTF8CHAR* rtXmlNSGetAttrQName
(OSCTXT* pctxt, OSUTF8CHAR* buf, size_t bufsiz, OSXMLNamespace* pNS,
 const OSUTF8CHAR* localName, OSRTDList* pNSAttrs);

/**
 * This function looks up a namespace in the context namespace stack using
 * URI as the key value.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param uri           Namespace URI to be found.
 * @return              Pointer to namespace structure or NULL if not found.
 */
EXTERNXML OSXMLNamespace* rtXmlNSLookupURI
(OSCTXT* pctxt, const OSUTF8CHAR* uri);

/**
 * This function looks up a namespace in the given list using
 * URI as the key value.
 *
 * @param pNSAttrs      List of namespace records.
 * @param uri           Namespace URI to be found.
 * @return              Pointer to namespace structure or NULL if not found.
 */
EXTERNXML OSXMLNamespace* rtXmlNSLookupURIInList
(OSRTDList* pNSAttrs, const OSUTF8CHAR* uri);


/**
 * This function looks up a namespace in the context namespace stack using
 * URI as the key value and returns a non-empty prefix, if one has been
 * defined.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param uri           Namespace URI to be found.
 * @return              Pointer to non-empty prefix.  NULL if URI is not found
 *                      or URI has no associated non-empty prefix.
 */
EXTERNXML const OSUTF8CHAR* rtXmlNSLookupPrefixForURI
(OSCTXT* pctxt, const OSUTF8CHAR* uri);


/**
 * This function looks up a namespace in the context namespace list using
 * the prefix as the key value.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param prefix        Namespace Prefix to be found.
 * @return              Pointer to namespace URI or NULL if not found.
 */
EXTERNXML const OSUTF8CHAR* rtXmlNSLookupPrefix
(OSCTXT* pctxt, const OSUTF8CHAR* prefix);

/**
 * This function looks up a namespace in the context namespace list using
 * the prefix as the key value.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param prefix        Namespace Prefix to be found.
 * @param prefixLen     Namespace Prefix length.
 * @return              Pointer to namespace URI or NULL if not found.
 */
EXTERNXML const OSUTF8CHAR* rtXmlNSLookupPrefixFrag
(OSCTXT* pctxt, const OSUTF8CHAR* prefix, size_t prefixLen);

/**
 * This function removes all namespaces from the context namespace list and
 * frees the dynamic memory used to hold the names.
 *
 * @param pctxt         Pointer to OSCTXT structure
 */
EXTERNXML void rtXmlNSRemoveAll (OSCTXT* pctxt);

/**
 * This function sets a namespace in the context namespace list.  If the
 * given namespace URI does not exist in the list, the namespace is added.
 * If the URI is found, the action depends on the value of the override
 * flag.  If true, the value of the namespace prefix will be changed to
 * the given prefix.  If false, the existing namespace specification is
 * not altered.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param pNSAttrs      Namespace attribute list to which namespace info
 *                        is to be added.
 * @param prefix        Namespace prefix
 * @param uri           Namespace URI
 * @param override      Should existing definition be changed?
 * @return              Pointer to namespace structure or NULL if not set.
 */
EXTERNXML OSXMLNamespace* rtXmlNSSetNamespace
(OSCTXT* pctxt, OSRTDList* pNSAttrs, const OSUTF8CHAR* prefix,
 const OSUTF8CHAR* uri, OSBOOL override);

/**
 * This function returns the next unused prefix of the form "nsX" where X
 * is a number. The new namespace declaration is added to the list provided
 * or the context list if a NULL pointer is passed for pNSAttrs.
 *
 * @param pctxt         Pointer to OSCTXT structure
 * @param uri           Namespace URI.  Must not be NULL or empty string.
 * @param pNSAttrs      Pointer to list of namespace attributes. If null, the
 *                      namespace list in the context will be used.
 * @return              New namespace prefix.
 */
EXTERNXML const OSUTF8CHAR* rtXmlNSNewPrefix
(OSCTXT* pctxt, const OSUTF8CHAR* uri, OSRTDList* pNSAttrs);

/**
 * This macro populates the given QName structure with information from
 * the given namespace structure (namespace URI and prefix).
 *
 * @param qname         Reference to QName structure to be populated.
 *                        Pointers to items in pNS are assigned directly
 *                        to fields in qname.  No copies of data are made.
 * @param pNS           Pointer to namespace structure.
 */
#define RTXMLNSSETQNAME(qname,pNS) \
if (0 != pNS) { qname.nsPrefix = pNS->prefix; qname.nsURI = pNS->uri; } \
else { qname.nsPrefix = qname.nsURI = 0; }

/**
 * Add a prefix link at the current stack level.
 */
EXTERNXML int rtXmlNSAddPrefixLink
(OSCTXT* pctxt, const OSUTF8CHAR* prefix, const OSUTF8CHAR* uri,
 const OSUTF8CHAR* nsTable[], OSUINT32 nsTableRowCount);

/**
 * Free all prefixs links in the given namespace stack entry.
 */
EXTERNXML int rtXmlNSFreeAllPrefixLinks
(OSCTXT* pctxt, OSXMLNSPfxLinkStackNode* pStackNode);

/**
 * Free all data within a given namespace prefix link structure.
 */
EXTERNXML int rtXmlNSFreePrefixLink (OSCTXT* pctxt, OSXMLNSPfxLink* plink);

/**
 * Get namespace index for a given namespace prefix based on current
 * namespace stack in context.
 */
EXTERNXML int rtXmlNSGetIndex (OSCTXT* pctxt, const OSUTF8CHAR* prefix);

/**
 * Push new namespace prefix mapping level onto stack.
 */
EXTERNXML int rtXmlNSPush (OSCTXT* pctxt);

/**
 * Remove top namespace prefix mapping level from stack.
 */
EXTERNXML int rtXmlNSPop (OSCTXT* pctxt);

/**
 * Set namespace URI table in context
 */
EXTERNXML void rtXmlNSSetURITable
(OSCTXT* pctxt, const OSUTF8CHAR* data[], OSUINT32 nrows);

#ifdef __cplusplus
}
#endif

#endif
