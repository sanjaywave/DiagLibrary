/*
 * Copyright (C) 2008 by Objective Systems, Inc.
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
 * @file rtXmlKeyArray.h - Implementation of a dynamic pointer sorted array.
 */
#ifndef _RTXMLKEYARRAY_H_
#define _RTXMLKEYARRAY_H_

#include "rtxsrc/rtxContext.h"
#include "rtxmlsrc/rtXmlExternDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
   OSXSDString, OSXSDDecimal,
   OSXSDInt, OSXSDUInt
} OSXSDFieldDataType;

/* OSXSDKeyRecord represents the values for the fields of a key */
typedef struct {
  /* tag: indicates the value space in which the value falls.  For example,
  xsd:integer derives from xsd:decimal, so they fall in the same value space.
  Thus, tag will never be OSXSDInt.
  Represented using a OSXSDFieldDataType value. */
  OSINT8 tag;
  
  /* subTag: indicates the representation used for the value in u.  
  When tag == OSXSDDecimal, this can be OSXSDDecimal, OSXSDInt, OSXSDUint. */
  OSINT8 subTag;
  OSUINT16 dummy; /* alignment */
  
  union {
     OSINT32 int32;
     OSUINT32 uint32;
     const OSOCTET* pValue;
  } u;
} OSXSDKeyRecord;

typedef struct {
   /* capacity: Current size of the array buffer.  The actual size is always
     1 larger to allow for the first slot being dedicated to the new key.
     Thus, capacity really represents the "non dedicated" capacity. */      
   OSUINT32        capacity;
   OSUINT32        count;          /* Number of keys in array          */
   OSUINT32        nmFields;       /* Number of fields in each key     */
   
   /* curField: Next value expected for fldNum parameter to rtXmlKeyArraySet*
      methods.  First field is field 0. */
   OSUINT32        curField;
   
   /* data: An array of OSXSDKeyRecords.  Each field value is represented as an
      OSXSDKeyRecord.  A block of nmFields of these values forms an 
      nmFields-tuple representing a single key.  There are count many such
      blocks, representing count keys.
      The first key is reserved as the location for new keys being added (
      all of the set methods update the fields for the first key).
      The keys from 1..count are kept in sorted order. */
   OSXSDKeyRecord* data;
   
   const char* name;               /* Constrain name                   */
   OSBOOL          keyConstraint;  /* Key or unique constraint         */
   OSINT8          duplicateField; /* Index of duplicated field; -1 = none */
   OSINT8          absentField;    /* Index of absent field; -1 = none     */
} OSXSDKeyArray;


/**
 * Initialize the array by allocating memory for it.
 * @param pctxt
 * @param pArray The array whose data should be initialized.
 * @param nmFields The number of fields in the key whose data will be held
 *    in pArray->data.
 * @param key TRUE if the data is for a key; FALSE if for a unique identity 
 *    constraint.
 * @param name of the identity constraint
 */
EXTERNXML int rtXmlKeyArrayInit 
(OSCTXT* pctxt, OSXSDKeyArray* pArray, OSUINT32 nmFields, OSBOOL key,
 const char* name);

EXTERNXML void rtXmlKeyArrayFree 
(OSCTXT* pctxt, OSXSDKeyArray* pArray);


/* rtXmlKeyArraySet* methods.
 * The values for each field of the key are expected to be set in order.
 * The implementation detects missing fields or repeated fields by watching
 * the sequence of fldNum values passed to each of the rtXmlKeyArraySet*
 * methods.
 */
 
/**
 * Set the given field's value to the given string value.
 * see comment on set* methods above.
 */
EXTERNXML void rtXmlKeyArraySetString 
(OSXSDKeyArray* pArray, const OSUTF8CHAR* pValue, OSUINT32 fldNum);

/**
 * Set the given field's value to the given integer value.
 * see comment on set* methods above.
 */
EXTERNXML void rtXmlKeyArraySetInt 
(OSXSDKeyArray* pArray, OSINT32 value, OSUINT32 fldNum);

/**
 * Set the given field's value to the given unsigned integer value.
 * see comment on set* methods above.
 */
EXTERNXML void rtXmlKeyArraySetUInt 
(OSXSDKeyArray* pArray, OSUINT32 value, OSUINT32 fldNum);

/**
 * Set the given field's value to the given decimal value.
 * see comment on set* methods above.
 */
EXTERNXML void rtXmlKeyArraySetDecimal 
(OSXSDKeyArray* pArray, const OSREAL* value, OSUINT32 fldNum);


/**
 * Once all the fields for a key are set, invoke this method to add the key.
 * It will report an error if any of the following are true:
 *    - a field was skipped and the constraint is a key constraint
 *    - a field was set more than once
 * This method adds the "new" key (pointed to by pArray) into the correct
 * place within the array of keys (and their fields), also pointed to by
 * pArray.
 */
EXTERNXML int rtXmlKeyArrayAdd 
(OSCTXT* pctxt, OSXSDKeyArray* pArray);

/**
 * Once all the fields for a key are set, this method is used to check whether
 * the key is already present in the array of keys.  Thus, to check a key ref,
 * you set all the fields, then invoke this method.  You do NOT invoke 
 * rtXmlKeyArrayAdd.
 * @returns 0 if key is found as expected
 */
EXTERNXML int rtXmlKeyArrayContains 
(OSCTXT* pctxt, OSXSDKeyArray* pArray);

#ifdef __cplusplus
}
#endif

#endif
