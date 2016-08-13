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
 * @file OSRTBase64TextInputStream.h
 * C++ hexadecimal text input stream filter class.
 */
#ifndef _OSRTBASE64TEXTINPUTSTREAM_H_
#define _OSRTBASE64TEXTINPUTSTREAM_H_

#include "rtxsrc/OSRTInputStream.h"
#include "rtxsrc/rtxStreamBase64Text.h"

/**
 * Hexadecimal text input stream filter class.  This class is created on
 * top of an existing stream class to provide conversion of hexadecimal
 * text input into binary form.
 */
class EXTRTCLASS OSRTBase64TextInputStream : public OSRTInputStream {
 protected:
   OSRTInputStream* mpUnderStream;

   OSBOOL mbOwnUnderStream;

 public:
   /**
    * Initializes the input stream using the existing standard input
    * stream.  Only file and memory underlying stream types are
    * supported.
    *
    * @param pstream  The underlying input stream object.  Note that this
    *                   class will take control of the underlying stream
    *                   object and delete it upon destruction.
    *
    * @see ::rtxStreamHexTextAttach
    */
   EXTRTMETHOD OSRTBase64TextInputStream (OSRTInputStream* pstream);

   /**
    * The destructor deletes the underlying stream object.  That object
    * should be used as nothing more to a surrogate to this object.
    */
   EXTRTMETHOD ~OSRTBase64TextInputStream ();

   /**
    * This method is used to query a stream object in order to determine
    * its actual type.
    *
    * @param id  Enumerated stream identifier
    * @return    True if the stream matches the identifier
    */
   virtual OSBOOL isA (StreamID id) const {
      return mpUnderStream->isA (id);
   }

   /**
   * This method is used to transfer ownership of the underlying stream
   * to the class.
   */
   inline void setOwnUnderStream (OSBOOL value = TRUE) {
      mbOwnUnderStream = value;
   }

   /**
   * This method is used to determine if a certificate was parsed.
   */
   OSBOOL isCertificate () {
      return ((Base64TextStrmDesc*)
              (getCtxtPtr()->pStream->extra))->mbCertificate;
   }

} ;

#endif /* _OSRTBASE64TEXTINPUTSTREAM_H_ */
