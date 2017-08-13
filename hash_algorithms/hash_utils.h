#ifndef HASH_UTILS_H
#define HASH_UTILS_H

namespace hash_utils
{

constexpr auto left_rotate(uint32_t a, uint8_t b)
{
    return (a << b) | (a >> (32u - b));
}

constexpr auto right_rotate(uint32_t a, uint8_t b)
{
    return (a << (32u - b)) | (a >> b);
}

inline uint32_t swap_endianness_uint32( uint32_t val )
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | (val >> 16);
}

inline uint64_t swap_endianness_uint64( uint64_t val )
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | (val >> 32);
}
}
#endif // HASH_UTILS_H
