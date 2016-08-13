/*
 * Summary: interface for the encoding conversion functions
 * Description: interface for the encoding conversion functions needed for
 *              XML basic encoding and iconv() support.
 *
 * Related specs are
 * rfc2044        (UTF-8 and UTF-16) F. Yergeau Alis Technologies
 * [ISO-10646]    UTF-8 and UTF-16 in Annexes
 * [ISO-8859-1]   ISO Latin-1 characters codes.
 * [UNICODE]      The Unicode Consortium, "The Unicode Standard --
 *                Worldwide Character Encoding -- Version 1.0", Addison-
 *                Wesley, Volume 1, 1991, Volume 2, 1992.  UTF-8 is
 *                described in Unicode Technical Report #4.
 * [US-ASCII]     Coded Character Set--7-bit American Standard Code for
 *                Information Interchange, ANSI X3.4-1986.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */
/*
NOTE: the copyright notice below applies only to source and header files
that include this notice.  It does not apply to other Objective Systems
software with different attached notices.

Except where otherwise noted in the source code (e.g. the files hash.c,
list.c and the trio files, which are covered by a similar licence but
with different Copyright notices) all the files are:

 Copyright (C) 1998-2003 Daniel Veillard.  All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is fur-
nished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FIT-
NESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
DANIEL VEILLARD BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CON-
NECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Daniel Veillard shall not
be used in advertising or otherwise to promote the sale, use or other deal-
ings in this Software without prior written authorization from him.
*/
/** 
 * @file rtxUTF16.h 
 * Utility functions for converting UTF-16(LE|BE) strings to and from UTF-8.
 */
#ifndef _RTXUTF16_H_
#define _RTXUTF16_H_

#include "rtxsrc/rtxContext.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * This function converts a UTF-16LE string into a UTF-8 string.  A buffer
 * large enough to hold the converted UTF-8 characters must be provided.
 * A buffer providing 4 bytes-per-character should be large enough to
 * hold the largest possible UTF-8 conversion. This function assumes 
 * the endian property is the same between the native type of this machine 
 * and the inputed one.
 *
 * @param out           Buffer to hold converted string.
 * @param outlen        Size of output buffer.
 * @param inb           A pointer to an array of UTF-16LE passed as 
 *                         a byte array.
 * @param inlenb        The length of @inb in UTF-16LE characters.
 * @return              Total number of bytes in converted string or
 *                        a negative status code if error:
 *                          -1 if lack of space, or 
 *                          -2 if the transcoding fails (if *inb is not 
 *                             a valid utf16 string)
 */
EXTERNRT int
rtxUTF16LEToUTF8(unsigned char* out, int outlen,
            const unsigned char* inb, int inlenb);

/**
 * This function converts a UTF-8 string into a UTF-16LE string.  A buffer
 * large enough to hold the converted UTF-16 characters must be provided.
 *
 * @param outb          Buffer to hold converted string.
 * @param outlen        Size of output buffer.
 * @param in            A pointer to an array of UTF-8 chars
 * @param inlen         The length of @in
 * @return              Total number of bytes in converted string or
 *                        a negative status code if error:
 *                          -1 if lack of space, or 
 *                          -2 if the transcoding fails
 */
EXTERNRT int
rtxUTF8ToUTF16LE(unsigned char* outb, int outlen,
            const unsigned char* in, int inlen);

/**
 * This function converts a UTF-8 string into a UTF-16 string.  A buffer
 * large enough to hold the converted UTF-16 characters must be provided.
 *
 * @param outb          Buffer to hold converted string.
 * @param outlen        Size of output buffer.
 * @param in            A pointer to an array of UTF-8 chars
 * @param inlen         The length of @in
 * @return              Total number of bytes in converted string or
 *                        a negative status code if error:
 *                          -1 if lack of space, or 
 *                          -2 if the transcoding fails
 */
EXTERNRT int
rtxUTF8ToUTF16(unsigned char* outb, int outlen,
            const unsigned char* in, int inlen);

/**
 * This function converts a UTF-16BE string into a UTF-8 string.  A buffer
 * large enough to hold the converted UTF-8 characters must be provided.
 * A buffer providing 4 bytes-per-character should be large enough to
 * hold the largest possible UTF-8 conversion. This function assumes 
 * the endian property is the same between the native type of this machine 
 * and the inputed one.
 *
 * @param out           Buffer to hold converted string.
 * @param outlen        Size of output buffer.
 * @param inb           A pointer to an array of UTF-16LE passed as 
 *                         a byte array.
 * @param inlenb        The length of @inb in UTF-16LE characters.
 * @return              Total number of bytes in converted string or
 *                        a negative status code if error:
 *                          -1 if lack of space, or 
 *                          -2 if the transcoding fails (if *inb is not 
 *                             a valid utf16 string)
 */
EXTERNRT int
rtxUTF16BEToUTF8(unsigned char* out, int outlen,
            const unsigned char* inb, int inlenb);

/**
 * This function converts a UTF-8 string into a UTF-16BE string.  A buffer
 * large enough to hold the converted UTF-16 characters must be provided.
 *
 * @param outb          Buffer to hold converted string.
 * @param outlen        Size of output buffer.
 * @param in            A pointer to an array of UTF-8 chars
 * @param inlen         The length of @in
 * @return              Total number of bytes in converted string or
 *                        a negative status code if error:
 *                          -1 if lack of space, or 
 *                          -2 if the transcoding fails
 */
EXTERNRT int
rtxUTF8ToUTF16BE(unsigned char* outb, int outlen,
            const unsigned char* in, int inlen);

/**
 * This function takes a block of UTF-8 chars in and try to convert it 
 * to an UTF-16 block of chars, and write the converted chars to stream.
 *
 * @param pctxt         Pointer to context block structure.
 * @param in            A pointer to an array of UTF-8 chars.
 * @param inlen         The length of @in.
 * @return              Total number of bytes in converted string or
 *                        a negative status code if error:
 *                          -1 if lack of space, or 
 *                          -2 if the transcoding fails
 */
EXTERNRT int
rtxStreamUTF8ToUTF16 (OSCTXT* pctxt, const unsigned char* in, size_t inlen);

/**
 * This function takes a block of UTF-8 chars in and try to convert it 
 * to an UTF-16LE block of chars, and write the converted chars to stream.
 *
 * @param pctxt         Pointer to context block structure.
 * @param in            A pointer to an array of UTF-8 chars.
 * @param inlen         The length of @in.
 * @return              Total number of bytes in converted string or
 *                        a negative status code if error:
 *                          -1 if lack of space, or 
 *                          -2 if the transcoding fails
 */
EXTERNRT int
rtxStreamUTF8ToUTF16LE (OSCTXT* pctxt, const unsigned char* in, size_t inlen);

/**
 * This function takes a block of UTF-8 chars in and try to convert it 
 * to an UTF-16BE block of chars, and write the converted chars to stream.
 *
 * @param pctxt         Pointer to context block structure.
 * @param in            A pointer to an array of UTF-8 chars.
 * @param inlen         The length of @in.
 * @return              Total number of bytes in converted string or
 *                        a negative status code if error:
 *                          -1 if lack of space, or 
 *                          -2 if the transcoding fails
 */
EXTERNRT int
rtxStreamUTF8ToUTF16BE (OSCTXT* pctxt, const unsigned char* in, size_t inlen);

#ifdef __cplusplus
}
#endif

#endif
