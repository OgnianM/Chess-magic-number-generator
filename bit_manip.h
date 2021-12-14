#pragma once

enum square_bit : uint64_t
{
    a1 = 1,
    b1 = 2,
    c1 = 4,
    d1 = 8,
    e1 = 16,
    f1 = 32,
    g1 = 64,
    h1 = 128,
    a2 = 256,
    b2 = 512,
    c2 = 1024,
    d2 = 2048,
    e2 = 4096,
    f2 = 8192,
    g2 = 16384,
    h2 = 32768,
    a3 = 65536,
    b3 = 131072,
    c3 = 262144,
    d3 = 524288,
    e3 = 1048576,
    f3 = 2097152,
    g3 = 4194304,
    h3 = 8388608,
    a4 = 16777216,
    b4 = 33554432,
    c4 = 67108864,
    d4 = 134217728,
    e4 = 268435456,
    f4 = 536870912,
    g4 = 1073741824,
    h4 = 2147483648,
    a5 = 4294967296,
    b5 = 8589934592,
    c5 = 17179869184,
    d5 = 34359738368,
    e5 = 68719476736,
    f5 = 137438953472,
    g5 = 274877906944,
    h5 = 549755813888,
    a6 = 1099511627776,
    b6 = 2199023255552,
    c6 = 4398046511104,
    d6 = 8796093022208,
    e6 = 17592186044416,
    f6 = 35184372088832,
    g6 = 70368744177664,
    h6 = 140737488355328,
    a7 = 281474976710656,
    b7 = 562949953421312,
    c7 = 1125899906842624,
    d7 = 2251799813685248,
    e7 = 4503599627370496,
    f7 = 9007199254740992,
    g7 = 18014398509481984,
    h7 = 36028797018963968,
    a8 = 72057594037927936,
    b8 = 144115188075855872,
    c8 = 288230376151711744,
    d8 = 576460752303423488,
    e8 = 1152921504606846976,
    f8 = 2305843009213693952,
    g8 = 4611686018427387904,
    h8 = 9223372036854775808ULL
};


#include <cstdint>

typedef uint64_t bitmask;
typedef uint8_t bit_index;


inline uint8_t get_index(int8_t x, int8_t y)
{
    return x + (y<<3);
}

inline bitmask get_bit(bit_index idx)
{
    return bitmask(1) << idx;
}

inline bitmask get_bit(int8_t x, int8_t y)
{
    return get_bit(get_index(x,y));
    //return (bitmask(1) << x) << (y << 3);
}


inline void flip_bit(bitmask& m, int8_t x, int8_t y)
{
    m ^= get_bit(x,y);
}

inline void flip_bit(bitmask& m,  bit_index idx)
{
    m ^= get_bit(idx);
}

inline void flip_bit(bitmask& m, square_bit square)
{
    m ^= square;
}

inline void set_bit(bitmask& m, square_bit square)
{
    m |= square;
}

inline void set_bit(bitmask& m, int8_t x, int8_t y)
{
    m |= get_bit(x,y);
}

inline void set_bit(bitmask& m, uint8_t index)
{
    m |= get_bit(index);
}

inline void clear_bit(bitmask& m, uint8_t index)
{
    m &= ~(get_bit(index));
}

inline void clear_bit(bitmask& m, int8_t x, int8_t y)
{
    clear_bit(m, get_index(x,y));
}

inline void clear_bit(bitmask& m, square_bit square)
{
    m &= ~square;
}


inline int fallback_popcntll(uint64_t bitboard)
{
    int count = 0;
    while (bitboard)
    {
        count += bitboard & 1;
        bitboard >>= 1;
    }
    return count;
}


inline uint64_t reverse_bytes(uint64_t x)
{
    x = (x >> 32) | (x << 32);
    x = ((x & 0xffff0000ffff0000) >> 16) | ((x & 0x0000ffff0000ffff) << 16);
    x = ((x & 0xff00ff00ff00ff00) >> 8)  | ((x & 0x00ff00ff00ff00ff) << 8);
    return x;
}

inline uint64_t poplsb(uint64_t& x)
{
    auto lsb = x&~(x-1);
    x &= ~lsb;
    return lsb;
}

inline uint64_t set_difference(uint64_t const& a, uint64_t const& b)
{
    // return a ^ b;
    // return a - b;
    return a&~b;
}

#ifdef USE_POPCNT
#define popcountll(x) __builtin_popcountll(x)
#else
#define popcountll(x) fallback_popcntll(x)
#endif






