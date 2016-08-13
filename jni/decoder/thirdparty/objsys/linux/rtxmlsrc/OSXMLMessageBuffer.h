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
 * @file OSXMLMessageBuffer.h
 * XML encode/decode buffer and stream base class.
 */
#ifndef _OSXMLMESSAGEBUFFER_H_
#define _OSXMLMESSAGEBUFFER_H_

#include "rtxsrc/OSRTMsgBuf.h"
#include "rtxmlsrc/osrtxml.h"

/**
 * The XML message buffer class is derived from the OSMessageBuffer
 * base class. It is the base class for the OSXMLEncodeBuffer
 * and OSXMLDecodeBuffer classes. It contains variables and methods
 * specific to encoding or decoding XML messages.  It is used to manage
 * the buffer into which a message is to be encoded or decoded.
 *
 */
class EXTXMLCLASS OSXMLMessageBuffer : public OSRTMessageBuffer {
 protected:
   /**
    * The protected constructor creates a new context and sets the buffer class
    * type.
    *
    * @param bufferType   Type of message buffer that is being created (for
    *                       example, XMLEncode or XMLDecode).
    * @param pContext     Pointer to a context to use. If NULL, new context
    *                       will be allocated.
    */
   EXTXMLMETHOD OSXMLMessageBuffer (Type bufferType, OSRTContext* pContext = 0);

 public:
   /**
    * The getAppInfo method returns the pointer to application context
    * information.
    */
   EXTXMLMETHOD virtual void* getAppInfo ();

   /**
    * This method returns current XML output indent value.
    *
    * @return              Current indent value (>= 0) if OK, negative status
    *                      code if error.
    */
   EXTXMLMETHOD int getIndent ();

   /**
    * This method returns current XML output indent character value
    * (default is space).
    *
    * @return              Current indent character (> 0) if OK,
    *                      negative status code if error.
    */
   EXTXMLMETHOD int getIndentChar ();

   /**
    * This function returns whether writing the Unicode BOM is currently
    * enabled or disabled.
    *
    * @return              TRUE if writing BOM is enabled, FALSE otherwise.
    */
   EXTXMLMETHOD OSBOOL getWriteBOM ();

   /**
    * This method sets a namespace in the context namespace list.  If the
    * given namespace URI does not exist in the list, the namespace is added.
    * If the URI is found, the value of the namespace prefix will be changed
    * to the given prefix.
    *
    * @param prefix        Namespace prefix
    * @param uri           Namespace URI
    * @param pNSAttrs      Namespace list to which namespace is to be added
    */
   EXTXMLMETHOD virtual void setNamespace
      (const OSUTF8CHAR* prefix, const OSUTF8CHAR* uri,
       OSRTDList* pNSAttrs = 0);

   /**
    * This method sets application specific context information within
    * the common context structure.  For XML encoding/decoding, this is
    * a structure of type <i>OSXMLCtxtInfo</i>.
    *
    * @param pXMLInfo      Pointer to context information.
    */
   EXTXMLMETHOD virtual void setAppInfo (void* pXMLInfo);

   /**
    * This method sets XML output formatting to the given value.
    * If TRUE (the default), the XML document is formatted with indentation
    * and newlines.  If FALSE, all whitespace between elements is suppressed.
    * Turning formatting off can provide more compressed documents and also
    * a more canonical representation which is important for security
    * applications.
    *
    * @param doFormatting  Boolean value indicating if formatting is to be done
    * @return              Status of operation: 0 if OK, negative status
    *                      code if error.
    */
   EXTXMLMETHOD void setFormatting (OSBOOL doFormatting);

   /**
    * This method sets XML output indent to the given value.
    *
    * @param indent        Number of spaces per indent. Default is 3.
    */
   EXTXMLMETHOD void setIndent (OSUINT8 indent);

   /**
    * This method sets XML output indent character to the given value.
    *
    * @param indentChar    Indent character. Default is space.
    */
   EXTXMLMETHOD void setIndentChar (char indentChar);

   /**
    * This method sets whether to write the Unicode byte order mark
    * before the XML header.
    *
    * @param write          TRUE if BOM should be written, FALSE otherwise.
    */
   EXTXMLMETHOD void setWriteBOM (OSBOOL write);

} ;

#endif

