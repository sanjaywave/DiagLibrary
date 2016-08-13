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
 * @file ASN1CXerOpenType.h  Holds class that defines standard SAX event
 *                           handlers for decoding XER open data type
 */

#ifndef _ASN1CXEROPENTYPE_H_
#define _ASN1CXEROPENTYPE_H_

#ifdef __cplusplus
#include <stdlib.h>
#include "asn1XerCppTypes.h"

/** This class defines standard SAX event handlers for decoding XER open
* type data.  In this case, the entire structure must be preserved for
* processing by other XER encode or decode functions.
*/

class EXTERNXER ASN1CXerOpenType :
public ASN1CType, public ASN1XERSAXDecodeHandler
{
 protected:
   ASN1TOpenType& msgData;
   ASN1XERShdMemHpEncBuf mEncodeBuffer;
   OSCTXT* mpEncCtxt;

   OSBOOL isEmptyElement (const OSUTF8CHAR* const qname);

 public:
   ASN1CXerOpenType (ASN1TOpenType& data);
   ASN1CXerOpenType (OSRTMessageBufferIF& msgBuf, ASN1TOpenType& data);
   ASN1CXerOpenType (ASN1TObject& data);
   ASN1CXerOpenType (OSRTMessageBufferIF& msgBuf, ASN1TObject& data);
   ~ASN1CXerOpenType ();

   // This is needed to suppress a VC++ level 4 warning.  It has
   // no implementation.
   ASN1CXerOpenType& operator= (const ASN1CXerOpenType&);

   // SAX ContentHandler interface

   virtual int startElement
      (const OSUTF8CHAR* const uri,
       const OSUTF8CHAR* const localname,
       const OSUTF8CHAR* const qname,
       const OSUTF8CHAR* const* attrs);

   virtual int characters
      (const OSUTF8CHAR* const chars, unsigned int length);

   virtual int endElement
      (const OSUTF8CHAR* const uri,
       const OSUTF8CHAR* const localname,
       const OSUTF8CHAR* const qname);
} ;

#endif /* __cplusplus */

#endif /* _ASN1CXEROPENTYPE_H_ */
