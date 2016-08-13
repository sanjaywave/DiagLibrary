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
 * @file rtxIntStack.h
 * Simple FIFO stack for storing integer values.
 */
#ifndef _RTXINTSTACK_
#define _RTXINTSTACK_

#include "rtxsrc/rtxContext.h"

/**
 * @defgroup ccfIntStack Integer Stack Utility Functions
 *
 * This is a simple stack structure with supporting push, pop, and
 * peek functions.
 *
 *@{
 */
/**
 * This is the default capacity that is used if zero is passed as the
 * capacity argument to rtxIntStackInit.
 */
#define OSRTISTK_DEFAULT_CAPACITY 100

/**
 * @struct _OSRTIntStack
 * This is the main stack structure.  It uses an expandable array for
 * storing integer values.
 */
typedef struct _OSRTIntStack {
   OSCTXT*      pctxt;      /* context pointer */
   OSINT32*     pdata;      /* stack data */
   size_t       index;      /* index to top element */
   size_t       size;       /* current size of the stack */
} OSRTIntStack;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function initializes a stack structure. It allocates the initial
 * amount of memory required to store data and sets all working variables
 * to their initil state.
 *
 * @param pctxt        A pointer to the context with which the stack is
 *                      associated.
 * @param pstack       A pointer to a stack structure to be initialized.
 * @param capacity     Initial capacity of the stack.  This is the number
 *                       of integer values that can be stored before the
 *                       stack is expanded.  Each expansion doubles the
 *                       initial capacity value.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNRT int rtxIntStackInit
(OSCTXT* pctxt, OSRTIntStack* pstack, size_t capacity);

/**
 * This function pushes an item onto the stack.
 *
 * @param pstack       A pointer to the stack structure.
 * @param value        A pointer to the data item to be pushed on the stack.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNRT int rtxIntStackPush (OSRTIntStack* pstack, OSINT32 value);

/**
 * This functions returns the data item on the top of the stack.
 *
 * @param pstack       A pointer to the stack structure.
 * @param pvalue       A pointer to a variable to store the integer value
 *                       of the item at the top of the stack.
 * @return             Status of peek operation:
 *                       - 0 (0) = success,
 *                       - RTERR_ENDOFBUF if stack is empty
 */
EXTERNRT int rtxIntStackPeek (OSRTIntStack* pstack, OSINT32* pvalue);

/**
 * This functions pops the data item on the top of the stack.
 *
 * @param pstack       A pointer to the stack structure.
 * @param pvalue       A pointer to a variable to store the integer value
 *                       of the item at the top of the stack.
 * @return             Status of pop operation:
 *                       - 0 (0) = success,
 *                       - RTERR_ENDOFBUF if stack is empty
 */
EXTERNRT int rtxIntStackPop (OSRTIntStack* pstack, OSINT32* pvalue);

/**
 * This macro tests if the stack is empty.
 *
 * @param stack        Stack structure variable to be tested.
 */
#define rtxIntStackIsEmpty(stack) (OSBOOL)((stack).index == 0)

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif
