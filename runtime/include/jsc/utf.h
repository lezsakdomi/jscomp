// Copyright (c) 2015 Tzvetan Mikov.
// Licensed under the Apache License v2.0. See LICENSE in the project
// root for complete license information.

#ifndef JSCOMP_UTF_H
#define JSCOMP_UTF_H

#ifndef JSCOMP_COMMON_H
#include "jsc/common.h"
#endif
#include <cstdint>

namespace js {


const uint32_t UNICODE_MAX_VALUE             = 0x10FFFF;
const uint32_t UNICODE_SURROGATE_LO          =   0xD800;
const uint32_t UNICODE_SURROGATE_HI          =   0xDFFF;
const uint32_t UNICODE_REPLACEMENT_CHARACTER =   0xFFFD;
const uint32_t UNICODE_ERROR                 = 0xFFFFFFFF;

// UTF-8 encoded UNICODE_REPLACEMENT_CHARACTER
const uint8_t UTF8_REPLACEMENT_CHAR_0 = 0xEF;
const uint8_t UTF8_REPLACEMENT_CHAR_1 = 0xBF;
const uint8_t UTF8_REPLACEMENT_CHAR_2 = 0xBD;

inline bool isValidCodePoint ( uint32_t cp )
{
    return !((cp >= UNICODE_SURROGATE_LO && cp <= UNICODE_SURROGATE_HI) || cp > UNICODE_MAX_VALUE);
}

inline unsigned utf8CodePointLength (unsigned char firstByte)
{
    if (JS_LIKELY((firstByte & 0x80) == 0)) // Ordinary ASCII?
        return 1;
    else if (JS_LIKELY((firstByte & 0xE0) == 0xC0))
        return 2;
    else if (JS_LIKELY((firstByte & 0xF0) == 0xE0))
        return 3;
    else if ((firstByte & 0xF8) == 0xF0)
        return 4;
    else
        return 1;
}

inline bool utf8IsStartByte (unsigned char firstByte)
{
    return !(firstByte & 0x80) || (firstByte & 0xC0) == 0xC0;
}

/**
 *
 * @param dst  buffer big enough to hold at least 6 bytes
 * @param codePoint
 * @return the number of characters stored
 */
unsigned utf8Encode (unsigned char * dst, uint32_t codePoint);
unsigned utf8EncodedLength (uint32_t codePoint);
unsigned utf8Length (const unsigned char * from, const unsigned char * to);

/*
 * Decode one utf-8 code point. We always require the input buffer be zero-terminated. That guarantees us safety
 * even when it is invalid (e.g. partial utf-8 sequence). The terminating zero will be an invalid character and
 * we never access more than that.
 *
 * @param from a zero-terminated buffer.
 */
const unsigned char * utf8Decode (const unsigned char * from, uint32_t * res);

/**
 * Decode from a source which is guaranteed to be valid (thus no checks are necessary)
 */
uint32_t utf8DecodeFast (const unsigned char * from);

}; // namespace js

#endif //JSCOMP_UTF_H
