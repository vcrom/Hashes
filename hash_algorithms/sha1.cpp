#include "sha1.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <algorithm>

//#define PRINT_SHA1_CONSUMED_DATA
//#define PRINT_SHA1_PROCESSED_DATA

namespace
{
constexpr size_t bytes_per_round = 64;
constexpr auto left_rotate(uint32_t a, uint8_t b)
{
    return (a << b) | (a >> (32u - b));
}

inline uint32_t swap_uint32( uint32_t val )
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | (val >> 16);
}
uint64_t swap_uint64( uint64_t val )
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | (val >> 32);
}

constexpr auto f1(const uint32_t &b, const uint32_t &c, const uint32_t &d)
{
    return (b & c) | ((~b) & d);
}
constexpr auto f2(const uint32_t &b, const uint32_t &c, const uint32_t &d)
{
    return b ^ c ^ d;
}
constexpr auto f3(const uint32_t &b, const uint32_t &c, const uint32_t &d)
{
    return (b & c) | (b & d) | (c & d);
}
constexpr auto f4(const uint32_t &b, const uint32_t &c, const uint32_t &d)
{
    return f2(b, c, d);
}
}
Sha1::Sha1() :
    HashingAlgorithm(bytes_per_round)
{
    reset();
}

void Sha1::reset()
{
    HashingAlgorithm::reset();
    a_ = 0x67452301;
    b_ = 0xEFCDAB89;
    c_ = 0x98BADCFE;
    d_ = 0x10325476;
    e_ = 0xC3D2E1F0;
}

std::string Sha1::get_hash()
{
    add_padding();
    const auto digest = compute_digest();
    reset();
    return digest;
}

std::string Sha1::compute_digest()
{
    std::stringbuf buffer;
    std::ostream out (&buffer);
    for(const auto& value : hash_)
        out << std::setfill('0') << std::setw(8) << std::hex << value;
    return buffer.str();
}

void Sha1::run_round(const uint32_t *data)
{
    run_round(expand_data(*reinterpret_cast<const std::array<uint32_t, 16>*>(data)));
}

std::array<uint32_t, 80> Sha1::expand_data(const std::array<uint32_t, 16> &data)
{
#ifdef PRINT_SHA1_CONSUMED_DATA
    for(const auto elem : data)
        std::cout << std:: setfill('0') << std::setw(8) << std::hex << swap_uint32(elem) << " ";
    std::cout << std::endl;
#endif // PRINT_SHA1_CONSUMED_DATA
    std::array<uint32_t, 80> expanded_data;
    for(size_t i = 0; i < data.size(); ++i)
        expanded_data[i] = swap_uint32(data[i]);
    for(size_t i = data.size(); i < expanded_data.size(); ++i)
    {
        expanded_data[i] = left_rotate(expanded_data[i-3]^expanded_data[i-8]
                                       ^expanded_data[i-14]^expanded_data[i-16], 1);
    }
    return expanded_data;
}

void Sha1::run_round(const std::array<uint32_t, 80> &data)
{
#ifdef PRINT_SHA1_PROCESSED_DATA
    for(const auto elem : data)
        std::cout << std:: setfill('0') << std::setw(8) << std::hex << elem << " ";
    std::cout << std::endl;
#endif // PRINT_SHA1_PROCESSED_DATA

    auto a = a_;
    auto b = b_;
    auto c = c_;
    auto d = d_;
    auto e = e_;

    auto i = 0u;
    for(; i < 20; ++i)
        sha1_operation_round(a, b, c, d, e, data[i], f1(b, c, d), 0x5A827999);
    for(; i < 40; ++i)
        sha1_operation_round(a, b, c, d, e, data[i], f2(b, c, d), 0x6ED9EBA1);
    for(; i < 60; ++i)
        sha1_operation_round(a, b, c, d, e, data[i], f3(b, c, d), 0x8F1BBCDC);
    for(; i < 80; ++i)
        sha1_operation_round(a, b, c, d, e, data[i], f4(b, c, d), 0xCA62C1D6);

    a_ += a;
    b_ += b;
    c_ += c;
    d_ += d;
    e_ += e;
}

void Sha1::sha1_operation_round(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e,
                           uint32_t data, uint32_t f, uint32_t k)
{
    const auto temp = left_rotate(a, 5) + f + e + data + k;
    e = d;
    d = c;
    c = left_rotate(b, 30);
    b = a;
    a = temp;
}

void Sha1::add_padding()
{
    if(buffer_.size() > 0)
    {
        buffer_.push_back(1 << 7);
        const auto buffer_size = buffer_.size();
        buffer_.resize(bytes_consumed_per_round_, 0);
        // Check if there are enough bytes to embed the 64 bit message size
        if(bytes_consumed_per_round_ - buffer_size < 8)
        {
            run_round(reinterpret_cast<const uint32_t*>(&buffer_[0]));
            std::fill(buffer_.begin(), buffer_.begin()+buffer_size, 0);
        }
    }
    else
    {
        buffer_.resize(bytes_consumed_per_round_, 0);
        buffer_[0] = 1 << 7;
    }
    auto offest_to_size_bits = buffer_.size() - 8;
    *reinterpret_cast<uint64_t*>(&buffer_[offest_to_size_bits]) = swap_uint64(data_bytes_processed_*8);
    run_round(reinterpret_cast<const uint32_t*>(&buffer_[0]));
    buffer_.clear();
}

