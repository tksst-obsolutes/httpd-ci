/* ====================================================================
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights
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
 * 4. The names "Apache" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache@apache.org.
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
 * individuals on behalf of the Apache Software Foundation.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 *
 * NIST Secure Hash Algorithm
 * 	heavily modified by Uwe Hollerbach uh@alumni.caltech edu
 * 	from Peter C. Gutmann's implementation as found in
 * 	Applied Cryptography by Bruce Schneier
 * 	This code is hereby placed in the public domain
 */

#ifndef APACHE_SHA1_H
#define APACHE_SHA1_H

#include "ap_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @package SHA1 library
 */

#define SHA_DIGESTSIZE 20

/**
 * Define the Magic String prefix that identifies a password as being
 * hashed using our algorithm.
 * @defvar AP_SHA1PW_ID "{SHA}"
 */
#define AP_SHA1PW_ID "{SHA}"
#define AP_SHA1PW_IDLEN 5

typedef struct AP_SHA1_CTX AP_SHA1_CTX;

/**
 * SHA1 context structure
 */
struct AP_SHA1_CTX {
    /** 
     * message digest
     */
    apr_uint32_t digest[5];
    /** 64-bit bit counts */
    apr_uint32_t count_lo, count_hi;
    /** SHA data buffer */
    apr_uint32_t data[16];
    /** unprocessed amount in data */
    int local;
};

/**
 * Provide a means to SHA1 crypt/encode a plaintext password in a way which
 * makes password file compatible with those commonly use in netscape web
 * and ldap installations.
 * @param clear The plaintext password
 * @param len The length of the plaintext password
 * @param out The encrypted/encoded password
 * @tip SHA1 support is useful for migration purposes, but is less
 *     secure than Apache's password format, since Apache's (MD5)
 *     password format uses a random eight character salt to generate
 *     one of many possible hashes for the same password.  Netscape
 *     uses plain SHA1 without a salt, so the same password
 *     will always generate the same hash, making it easier
 *     to break since the search space is smaller.
 * @deffunc void ap_sha1_base64(const char *clear, int len, char *out)
 */
API_EXPORT(void) ap_sha1_base64(const char *clear, int len, char *out);

/**
 * Initialize the SHA digest
 * @param context The SHA context to initialize
 * @deffunc void ap_SHA1Init(AP_SHA1_CTX *context);
 */
API_EXPORT(void) ap_SHA1Init(AP_SHA1_CTX *context);

/**
 * Update the SHA digest
 * @param context The SHA1 context to update
 * @param input The buffer to add to the SHA digest
 * @param inputLen The length of the input buffer
 * @deffunc void ap_SHA1Update(AP_SHA1_CTX *context, const char *input, unsigned int inputLen)
 */
API_EXPORT(void) ap_SHA1Update(AP_SHA1_CTX *context, const char *input,
                              unsigned int inputLen);

/**
 * Update the SHA digest with binary data
 * @param context The SHA1 context to update
 * @param input The buffer to add to the SHA digest
 * @param inputLen The length of the input buffer
 * @deffunc void ap_SHA1Update_binary(AP_SHA1_CTX *context, const unsigned char *input, unsigned int inputLen)
 */
API_EXPORT(void) ap_SHA1Update_binary(AP_SHA1_CTX *context,
                                     const unsigned char *input,
                                     unsigned int inputLen);

/**
 * Finish computing the SHA digest
 * @param digest the output buffer in which to store the digest
 * @param context The context to finalize
 * @deffunc void ap_SHA1Final(unsigned char digest[SHA_DIGESTSIZE], AP_SHA1_CTX *context)
 */
API_EXPORT(void) ap_SHA1Final(unsigned char digest[SHA_DIGESTSIZE],
                             AP_SHA1_CTX *context);

#ifdef __cplusplus
}
#endif

#endif	/* !APACHE_SHA1_H */
