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
 * @file rtSaxCXmlStreamHandler.h
 * This file defines a C SAX handler implementation to convert SAX events
 * to an XML output stream.
 */
#ifndef _RTSAXCXMLSTREAMHANDLER_H_
#define _RTSAXCXMLSTREAMHANDLER_H_

#include "rtxmlsrc/osrtxml.h"
#include "rtxmlsrc/rtSaxCParser.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * SAX startElement callback function.  This function outputs an XML
 * start element tag using the given element name and attributes.
 * (&lt;qname attrs>) to the output stream.
 *
 * @param userData      User data structure that was specified by user
 *                        when handler was created.  For this handler,
 *                        userData is the context structure.
 * @param localname     Element local name.
 * @param qname         Element qualified name.
 * @param attrs         Array of attributes.  Form of the array is
 *                        attrs[n]=name, attrs[n+1]=value,...
 *                        Array is terminated with a NULL name.
 * @return              Status value. 0 = success, ~0 = failure.
 */
EXTERNXML int SAX2XMLStreamStartElement
   (void *userData, const OSUTF8CHAR* localname,
    const OSUTF8CHAR* qname, const OSUTF8CHAR* const* attrs);

/**
 * SAX characters callback function.  This function outputs XML
 * character data to the output stream.
 *
 * @param userData      User data structure that was specified by user
 *                        when handler was created.  For this handler,
 *                        userData is the context structure.
 * @param chars         Character data (not null-terminated).
 * @param length        Number of characters in chars.
 * @return              Status value. 0 = success, ~0 = failure.
 */
EXTERNXML int SAX2XMLStreamCharacters
   (void *userData, const OSUTF8CHAR* chars, int length);

/**
 * SAX endElement callback function.  This function outputs an XML
 * end element tag using the given element name (&lt;/qname>) to
 * the output stream.
 *
 * @param userData      User data structure that was specified by user
 *                        when handler was created.  For this handler,
 *                        userData is the context structure.
 * @param localname     Element local name.
 * @param qname         Element qualified name.
 * @return              Status value. 0 = success, ~0 = failure.
 */
EXTERNXML int SAX2XMLStreamEndElement
   (void *userData, const OSUTF8CHAR* localname, const OSUTF8CHAR* qname);

#ifdef __cplusplus
}
#endif

#endif
