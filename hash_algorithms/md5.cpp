#include "md5.h"

#include <iomanip>
#include <iostream>
#include <sstream>

//#define PRINT_MD5_PROCESSED_DATA

namespace
{
constexpr std::array<uint8_t, 64> shifts = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};
constexpr std::array<uint32_t, 64> k_values = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf,
    0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af,
    0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e,
    0x49b40821, 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6,
    0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
    0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122,
    0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039,
    0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244, 0x432aff97,
    0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d,
    0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

constexpr auto left_rotate(uint32_t a, uint8_t b)
{
    return (a << b) | (a >> (32u - b));
}
constexpr auto f1(const uint32_t &b, const uint32_t &c, const uint32_t &d)
{
    return (b & c) | ((~b) & d);
}
constexpr auto f2(const uint32_t &b, const uint32_t &c, const uint32_t &d)
{
    return (b & d) | (c & (~d));
}
constexpr auto f3(const uint32_t &b, const uint32_t &c, const uint32_t &d)
{
    return b ^ c ^ d;
}
constexpr auto f4(const uint32_t &b, const uint32_t &c, const uint32_t &d)
{
    return c ^ (b | (~d));
}
constexpr auto g1(const uint32_t &idx)
{
    return idx;
}
constexpr auto g2(const uint32_t &idx)
{
    return (5*idx + 1)%16;
}
constexpr auto g3(const uint32_t &idx)
{
    return (3*idx + 5)%16;
}
constexpr auto g4(const uint32_t &idx)
{
    return (7*idx)%16;
}
constexpr size_t bytes_per_round = 64;
}

Md5::Md5() :
    HashingAlgorithm(bytes_per_round)
{
    reset();
}

void Md5::reset()
{
    HashingAlgorithm::reset();
    h0_ = 0x67452301;
    h1_ = 0xefcdab89;
    h2_ = 0x98badcfe;
    h3_ = 0x10325476;
}


std::string Md5::get_digest()
{
    flush_end_of_message();
    const auto digest = compute_digest();
    reset();
    return digest;
}

std::string Md5::compute_digest()
{
    std::stringbuf buffer;
    std::ostream out (&buffer);
    for (const auto& val : hash_)
        out << std:: setfill('0') << std::setw(2) << std::hex << static_cast<uint32_t>(val);
    return buffer.str();
}

void Md5::run_round(const uint32_t *data)
{
    run_round(*reinterpret_cast<const std::array<uint32_t, 16>*>(data));
}

void Md5::run_round(const std::array<uint32_t, 16> &data)
{
#ifdef PRINT_MD5_PROCESSED_DATA
    uint8_t *bytes = (uint8_t *)&data[0];
    for(size_t i =0; i < 64; ++i)
        std::cout << std::hex << static_cast<uint32_t>(bytes[i]) << " ";
    std::cout << std::endl;
#endif // PRINT_MD5_PROCESSED_DATA

    auto a = h0_;
    auto b = h1_;
    auto c = h2_;
    auto d = h3_;

    auto i = 0u;
    for(; i < 16; ++i)
        md5_operation_round(a, b, c, d, data[g1(i)], f1(b, c, d), k_values[i], shifts[i]);
    for(; i < 32; ++i)
        md5_operation_round(a, b, c, d, data[g2(i)], f2(b, c, d), k_values[i], shifts[i]);
    for(; i < 48; ++i)
        md5_operation_round(a, b, c, d, data[g3(i)], f3(b, c, d), k_values[i], shifts[i]);
    for(; i < 64; ++i)
        md5_operation_round(a, b, c, d, data[g4(i)], f4(b, c, d), k_values[i], shifts[i]);

    h0_ += a;
    h1_ += b;
    h2_ += c;
    h3_ += d;
}

inline void Md5::md5_operation_round(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d,
                           uint32_t data, uint32_t f, uint32_t k, uint8_t shift)
{
    auto tmp = d;
    d = c;
    c = b;
    b = b + left_rotate(a + f + k + data, shift);
    a = tmp;
}
