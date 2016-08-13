/*
 * Copyright (c) 1997-2009 Objective Systems, Inc.
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
 * @file rtsrc/ASN1Context.h
 * Common C++ type and class definitions.
 */
#ifndef _ASN1CONTEXT_H_
#define _ASN1CONTEXT_H_

#include "rtxsrc/rtxDiag.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/OSRTContext.h"

/**
 * @defgroup asn1context Context Management Classes
 * This group of classes manages an OSCTXT structure. This is the C structure
 * use to keep track of all of the working variables required to encode or
 * decode an ASN.1 message. @{
 */
/**
 * Reference counted ASN.1 context class. This keeps track of all encode/decode
 * function variables between function invocations. It is reference counted to
 * allow a message buffer and type class to share access to it.
 */
class EXTRTCLASS ASN1Context : public OSRTContext {
 public:
   /**
    * The default constructor initializes the mCtxt member variable for
    * ASN.1 encoding/decoding.
    */
   EXTRTMETHOD ASN1Context ();

   /**
    * This method sets run-time key in the context.  When using an unlimited
    * runtime, this method will still set the key, but the runtime
    * does not actually check it.
    *
    * @param key - array of octets with the key
    * @param keylen - number of octets in key array.
    * @return            Completion status of operation:
    *                      - 0 = success,
    *                      - negative return value is error.
    */
   virtual EXTRTMETHOD int setRunTimeKey (const OSOCTET* key, size_t keylen);

   // deprecated methods: all have been changed to begin with lowercase
   // character (AB, 7/14/04)..
   inline OSCTXT* GetPtr () { return &mCtxt; }
   inline void PrintErrorInfo () { printErrorInfo (); }
};

/** @} */

#endif /* ASN1CONTEXT */
