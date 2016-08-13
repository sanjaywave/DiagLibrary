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
 * @file rtXmlExternDefs.h
 * XML external definitions macro.  This is used for Windows to properly
 * declare function scope within DLL's.
 */
#ifndef _RTXMLEXTERNDEFS_H_
#define _RTXMLEXTERNDEFS_H_

/* Extern definitions for Symbian, et c. */
#ifdef __SYMBIAN32__       /* For Symbian */
#define EXTXMLCLASS

#ifdef BUILDXMLDLL
#define EXTERNXML       EXPORT_C
#define EXTXMLMETHOD   EXPORT_C
#elif defined (USEXMLDLL)
#define EXTERNXML       IMPORT_C
#define EXTXMLMETHOD   IMPORT_C
#else
#define EXTERNXML
#define EXTXMLMETHOD
#endif /* BUILDXMLDLL */

#else                      /* Other O/S's */
#define EXTXMLMETHOD

#ifdef BUILDXMLDLL
#define EXTERNXML       __declspec(dllexport)
#define EXTXMLCLASS   __declspec(dllexport)
#elif defined (USEXMLDLL)
#define EXTERNXML       __declspec(dllimport)
#define EXTXMLCLASS   __declspec(dllimport)
#else
#define EXTERNXML
#define EXTXMLCLASS
#endif /* BUILDXMLDLL */

#endif /* Symbian */

#endif
