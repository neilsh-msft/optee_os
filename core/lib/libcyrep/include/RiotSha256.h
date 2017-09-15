/*
 * Copyright (c) 2000-2001, Aaron D. Gifford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTOR(S) ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTOR(S) BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef __RIOT_CRYPTO_SHA256_H__
#define __RIOT_CRYPTO_SHA256_H__

typedef int asb;

typedef uint8_t  sha2_uint8_t;  // Exactly 1 byte
typedef uint32_t sha2_word32;   // Exactly 4 bytes
typedef uint64_t sha2_word64;   // Exactly 8 bytes

#define SHA256_BLOCK_LENGTH         64
#define SHA256_DIGEST_LENGTH        32

typedef uint64_t hashMagic_t;

#ifdef HOST_IS_LITTLE_ENDIAN
#define HASH_MAGIC_VALUE    (0x4078746368736168LL)
#else
#define HASH_MAGIC_VALUE    (0x6861736863747840LL)
#endif

#ifdef CONFIG_CYREP_OPTEE_BUILD
/* The OPTEE hash provider interface requires the context
 * structure to be dynamically allocated based on a size it provides.
 * That would require changing the RIOT interface, which we don't want to do,
 * so instead we ensure the static context structure is big enough. 
 * Size of this structure must be at least as big as sha256_state in
 * tomcrypt_hash.h. We cannot include this header directly, but there are
 * runtime checks to ensure this structure is the proper size.
 */
typedef struct _RIOT_SHA256_CONTEXT {
	uint64_t length;
    uint32_t state[8], curlen;
    uint8_t buf[64];
} RIOT_SHA256_CONTEXT;
#else
typedef struct _RIOT_SHA256_CONTEXT {
    uint32_t    state[8];
    hashMagic_t magic;
    uint64_t    bitcount;
    uint8_t     buffer[SHA256_BLOCK_LENGTH];
} RIOT_SHA256_CONTEXT;
#endif

//
// Initialize the hash context
// @param context the hash context
//
void RIOT_SHA256_Init(RIOT_SHA256_CONTEXT *context);

//
// Update the digest using the specific bytes
// @param context the hash context
// @param buf the bytes to digest
// @param bufSize the number of bytes to digest
//
void RIOT_SHA256_Update(RIOT_SHA256_CONTEXT *context,
                        const sha2_uint8_t *data, size_t len);

//
// Retrieve the final digest
// @param context the hash context
// @param digest the buffer to hold the digest.  Must be of size SHA256_DIGEST_LENGTH
//
void RIOT_SHA256_Final(RIOT_SHA256_CONTEXT *context, sha2_uint8_t *digest);

//
// Hash a block of data
// @param context the hash context
// @param buf the buffer containing the data to hash
// @param bufSize the number of bytes in the buffer
// @param digest the buffer to hold the digest.  Must be of size SHA256_DIGEST_LENGTH
//
void RIOT_SHA256_Block_ctx(RIOT_SHA256_CONTEXT *context,
                           const uint8_t *buf, size_t bufSize,
                           uint8_t *digest);

//
// Hash a block of data
// @param buf the buffer containing the data to hash
// @param bufSize the number of bytes in the buffer
// @param digest the buffer to hold the digest.  Must be of size SHA256_DIGEST_LENGTH
//
void RIOT_SHA256_Block(const uint8_t *buf, size_t bufSize,
                       uint8_t *digest);

#endif

