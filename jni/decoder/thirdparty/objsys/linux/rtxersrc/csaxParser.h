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

#ifndef __CSAXPARSER_H_
#define __CSAXPARSER_H_

#include "rtsrc/asn1type.h"
#include "rtxersrc/rtSAXDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSXMLREADER {
   OSCTXT* pctxt;
} OSXMLREADER;

/* atts is array of name/value pairs, terminated by 0;
   names and values are 0 terminated.
*/
typedef void (*CSAX_StartElementHandler)(void *userData,
                                         const XMLCHAR *name,
                                         const XMLCHAR **atts);

typedef void (*CSAX_EndElementHandler)(void *userData,
                                        const XMLCHAR *name);


/* s is not 0 terminated. */
typedef void (*CSAX_CharacterDataHandler)(void *userData,
                                          const XMLCHAR *s,
                                          int len);

OSXMLREADER* csaxCreateXmlReader (OSCTXT* pctxt, void* pSaxHandlerData,
                                  CSAX_StartElementHandler  pStartElementProc,
                                  CSAX_EndElementHandler    pEndElementProc,
                                  CSAX_CharacterDataHandler pCharactersProc);
int csaxParse (OSXMLREADER* pReader);
int csaxReleaseReader (OSXMLREADER* pReader);

#ifdef __cplusplus
}
#endif

#endif /* __CSAXPARSER_H_ */
