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
 * @file rtSaxDefs.h
 */

#ifndef __RTSAXDefs_H_
#define __RTSAXDefs_H_

#include "rtxsrc/rtxCommon.h"

typedef struct OSSAXElemTableRec {
   const char*  name;
   OSINT32      nsIndex;
   OSINT32      minOccurs;
   OSINT32      maxOccurs;
} OSSAXElemTableRec;

typedef struct OSSAXElemInfo {
   const char* name;
   OSBOOL optional;
} OSSAXElemInfo;

struct OSSAXElemTableRec;
struct OSRTMEMBUF;

#define OS_SAX_FINAL_STATE -3

#define LOG_SAXERR(pctxt,stat) \
(rtSaxIncErrors (pctxt),\
LOG_RTERR (pctxt, stat),\
stat)

#define LOG_SAXERRNEW(pctxt,stat) \
((rtSaxIncErrors (pctxt)) ? \
(LOG_RTERRNEW (pctxt, stat),\
stat):0)

#define LOG_SAXERR_AND_SKIP(pctxt,stat) \
rtSaxSetSkipLevelToCurrent (pctxt, LOG_SAXERR(pctxt,stat))

#define LOG_SAXERRNEW_AND_SKIP(pctxt,stat) \
rtSaxSetSkipLevelToCurrent (pctxt, LOG_SAXERRNEW(pctxt,stat))


#define LOG_SAXERR1(pctxt,stat,a) \
(rtSaxIncErrors (pctxt),\
LOG_RTERR (pctxt, stat),\
a,stat)

#define LOG_SAXERRNEW1(pctxt,stat,a) \
((rtSaxIncErrors (pctxt)) ? \
(a,LOG_RTERRNEW (pctxt, stat),\
stat):0)

#define LOG_SAXERR1_AND_SKIP(pctxt,stat,a) \
rtSaxSetSkipLevelToCurrent (pctxt, LOG_SAXERR1(pctxt,stat,a))

#define LOG_SAXERRNEW1_AND_SKIP(pctxt,stat,a) \
rtSaxSetSkipLevelToCurrent (pctxt, LOG_SAXERRNEW1(pctxt,stat,a))


#define LOG_SAXERR2(pctxt,stat,a,b) \
(rtSaxIncErrors (pctxt),\
LOG_RTERR (pctxt, stat),\
a,b,stat)

#define LOG_SAXERRNEW2(pctxt,stat,a,b) \
((rtSaxIncErrors (pctxt)) ? \
(a,b,LOG_RTERRNEW (pctxt, stat),\
stat):0)

#define LOG_SAXERR2_AND_SKIP(pctxt,stat,a,b) \
rtSaxSetSkipLevelToCurrent (pctxt, LOG_SAXERR2(pctxt,stat,a,b))

#define LOG_SAXERRNEW2_AND_SKIP(pctxt,stat,a,b) \
rtSaxSetSkipLevelToCurrent (pctxt, LOG_SAXERRNEW2(pctxt,stat,a,b))

#define LOG_SAXERR3(pctxt,stat,a,b,c) \
(rtSaxIncErrors (pctxt),\
LOG_RTERR (pctxt, stat),\
a,b,c,stat)

#define LOG_SAXERRNEW3(pctxt,stat,a,b,c) \
((rtSaxIncErrors (pctxt)) ? \
(a,b,c,LOG_RTERRNEW (pctxt, stat),\
stat):0)

#define LOG_SAXERR3_AND_SKIP(pctxt,stat,a,b,c) \
rtSaxSetSkipLevelToCurrent (pctxt, LOG_SAXERR3(pctxt,stat,a,b,c))

#define LOG_SAXERRNEW3_AND_SKIP(pctxt,stat,a,b,c) \
rtSaxSetSkipLevelToCurrent (pctxt, LOG_SAXERRNEW3(pctxt,stat,a,b,c))

#endif /* __RTSAXDefs_H_ */
