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
 * @file OSJSONMessageBuffer.h
 * JSON encode/decode buffer and stream base class.
 */
#ifndef _OSJSONMESSAGEBUFFER_H_
#define _OSJSONMESSAGEBUFFER_H_

#include "rtxsrc/OSRTMsgBuf.h"
#include "rtjsonsrc/osrtjson.h"

/**
 * The JSON message buffer class is derived from the OSMessageBuffer
 * base class. It is the base class for the OSJSONEncodeBuffer
 * and OSJSONDecodeBuffer classes. It contains variables and methods
 * specific to encoding or decoding JSON messages.  It is used to manage
 * the buffer into which a message is to be encoded or decoded.
 *
 */
class EXTJSONCLASS OSJSONMessageBuffer : public OSRTMessageBuffer {
 protected:
   /**
    * The protected constructor creates a new context and sets the buffer class
    * type.
    *
    * @param bufferType   Type of message buffer that is being created (for
    *                       example, JSONEncode or JSONDecode).
    * @param pContext     Pointer to a context to use. If NULL, new context
    *                       will be allocated.
    */
   EXTJSONMETHOD OSJSONMessageBuffer (Type bufferType, OSRTContext* pContext = 0);

 public:
   /**
    * This method returns current JSON output indent value.
    *
    * @return              Current indent value (>= 0) if OK, negative status
    *                      code if error.
    */
   EXTJSONMETHOD int getIndent ();

   /**
    * This method returns current JSON output indent character value
    * (default is space).
    *
    * @return              Current indent character (> 0) if OK,
    *                      negative status code if error.
    */
   EXTJSONMETHOD int getIndentChar ();

   /**
    * This method sets JSON output indent to the given value.
    *
    * @param indent        Number of spaces per indent. Default is 3.
    */
   EXTJSONMETHOD void setIndent (OSUINT8 indent);

} ;

#endif

