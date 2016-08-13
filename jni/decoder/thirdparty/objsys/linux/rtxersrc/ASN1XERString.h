/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache\@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

#ifndef _ASN1XERString_H_
#define _ASN1XERString_H_

#include "rtxersrc/rtSAXDefs.h"
#include "rtxersrc/asn1xer.h"

/* A very simple emulation of XERCES(c) XMLString class */

class EXTERNXER ASN1XERString {
 public:
   static unsigned int stringLen (const XMLCHAR* const str);
   static unsigned int stringLen (const XMLCHAR16* const str);

   static XMLCHAR* replicate (const XMLCHAR* const str);

   static XMLCHAR* transcode (const char* const str);

   static char* transcode (const XMLCHAR16* const toTranscode);

   static int compareString (const XMLCHAR* const str1,
                             const XMLCHAR* const str2);

   static XMLCHAR* catString (XMLCHAR* destStr, int destStrSize,
                              const XMLCHAR* const str);

#ifdef XML_UNICODE
   static XMLCHAR* catString (XMLCHAR* destStr, int destStrSize,
                              const char* const str);
#else
   static XMLCHAR* catString (XMLCHAR* destStr, int destStrSize,
                              const XMLCHAR16* const str);
#endif

   static int stringUTF8Len (const unsigned char* utf8Str, int strLen);

   static XMLCHAR16* transUTF8 (const unsigned char* utf8Str,
      int *strSize, XMLCHAR16* destBuf, int destBufSize);

   static XMLCHAR* safeTranscode (const XMLCHAR16* str, int* strSize,
                                  XMLCHAR* staticBuf, int bufSize);

#ifndef XML_UNICODE
   static XMLCHAR16* safeTranscode (const XMLCHAR* str, int* strSize,
                                    XMLCHAR16* staticBuf, int bufSize);
#endif
   static const XMLCHAR* zeroLenString ();
};

#endif // _ASN1XERString_H_
