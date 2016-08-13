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
/*
 * This file is deprecated and maintained for backward compatibility only.
 * Definitions in rtxsrc/rtxStream.h should be used for any new
 * development.
 */
#ifndef _RTSTREAM_H_
#define _RTSTREAM_H_

#include "rtxsrc/rtxStreamFile.h"
#include "rtxsrc/rtxStreamMemory.h"
#include "rtxsrc/rtxStreamSocket.h"
#include "rtsrc/rtExternDefs.h"
#include "rtsrc/asn1compat.h"

#ifdef __cplusplus
extern "C" {
#endif

#define rtStreamRelease rtxStreamRelease
#define rtStreamClose rtxStreamClose
#define rtStreamFlush rtxStreamFlush
#define rtStreamInit  rtxStreamInit
#define rtStreamRead  rxtStreamRead
#define rtStreamBlockingRead  rxtStreamBlockingRead
#define rtStreamSkip  rtxStreamSkip
#define rtStreamWrite rxtStreamWrite
#define rtStreamGetIOBytes rtxStreamGetIOBytes
#define rtStreamMark  rtxStreamMark
#define rtStreamReset rtxStreamReset
#define rtStreamMarkSupported rtxStreamMarkSupported
#define rtStreamIsOpened rtxStreamIsOpened
#define rtStreamIsReadable rtxStreamIsReadable
#define rtStreamIsWritable rtxStreamIsWritable

#define rtStreamBufClose rtxStreamClose
#define rtStreamBufFlush rtxStreamFlush
#define rtStreamBufInit  rtxStreamInit
#define rtStreamBufMark  rtxStreamMark
#define rtStreamBufSkip  rtxStreamSkip
#define rtStreamBufReset rtxStreamReset
#define rtStreamBufWrite rtxStreamWrite

/* special case: return code was changed in rtx to return number of
   bytes read whereas rt version returned 0 if success */
EXTERNRT int rtStreamBufRead (ASN1CTXT* pctxt, ASN1OCTET* pdata, size_t size);

#define rtStreamFileAttach rtxStreamFileAttach
#define rtStreamFileOpen rtxStreamFileOpen
#define rtStreamFileCreateReader rtxStreamFileCreateReader
#define rtStreamFileCreateWriter rtxStreamFileCreateWriter

#define rtStreamSocketAttach rtxStreamSocketAttach
#define rtStreamSocketCreateReader rtxStreamSocketCreateReader
#define rtStreamSocketCreateWriter rtxStreamSocketCreateWriter

#define rtStreamMemoryCreate rtxStreamMemoryCreate
#define rtStreamMemoryAttach rtxStreamMemoryAttach
#define rtStreamMemoryGetBuffer rxtStreamMemoryGetBuffer
#define rtStreamMemoryCreateReader rtxStreamMemoryCreateReader
#define rtStreamMemoryCreateWriter rtxStreamMemoryCreateWriter

#ifdef __cplusplus
}
#endif

#endif /* _RTSTREAM_H_ */

