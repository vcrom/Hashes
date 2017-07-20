#include "sha2.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <algorithm>

//#define PRINT_Sha2_CONSUMED_DATA
//#define PRINT_Sha2_PROCESSED_DATA

namespace
{
constexpr size_t bytes_per_round = 64;
constexpr std::array<uint32_t, 64> k_values = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

constexpr auto left_rotate(uint32_t a, uint8_t b)
{
    return (a << b) | (a >> (32u - b));
}

constexpr auto right_rotate(uint32_t a, uint8_t b)
{
    return (a << (32u - b)) | (a >> b);
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
Sha2::Sha2() :
    HashingAlgorithm(bytes_per_round)
{
    reset();
}

void Sha2::reset()
{
    HashingAlgorithm::reset();
    h0_ = 0x6a09e667;
    h1_ = 0xbb67ae85;
    h2_ = 0x3c6ef372;
    h3_ = 0xa54ff53a;
    h4_ = 0x510e527f;
    h5_ = 0x9b05688c;
    h6_ = 0x1f83d9ab;
    h7_ = 0x5be0cd19;
}

std::string Sha2::get_hash()
{
    add_padding();
    const auto digest = compute_digest();
    reset();
    return digest;
}

std::string Sha2::compute_digest()
{
    std::stringbuf buffer;
    std::ostream out (&buffer);
    for(const auto& value : hash_)
        out << std::setfill('0') << std::setw(8) << std::hex << value;
    return buffer.str();
}

void Sha2::run_round(const uint32_t *data)
{
    run_round(expand_data(*reinterpret_cast<const std::array<uint32_t, 16>*>(data)));
}

std::array<uint32_t, 64> Sha2::expand_data(const std::array<uint32_t, 16> &data)
{
#ifdef PRINT_Sha2_CONSUMED_DATA
    for(const auto elem : data)
        std::cout << std:: setfill('0') << std::setw(8) << std::hex << swap_uint32(elem) << " ";
    std::cout << std::endl;
#endif // PRINT_Sha2_CONSUMED_DATA
    std::array<uint32_t, 64> expanded_data;
    for(size_t i = 0; i < data.size(); ++i)
        expanded_data[i] = swap_uint32(data[i]);
    for(size_t i = data.size(); i < expanded_data.size(); ++i)
    {
        const auto v0 = expanded_data[i - 15];
        const auto s0 = right_rotate(v0, 7) ^ right_rotate(v0, 18) ^ (v0 >> 3);
        const auto v1 = expanded_data[i - 2];
        const auto s1 = right_rotate(v1, 17) ^ right_rotate(v1, 19) ^ (v1 >> 10);
        expanded_data[i] = expanded_data[i - 16] + s0 + expanded_data[i - 7] + s1;
    }
    return expanded_data;
}

void Sha2::run_round(const std::array<uint32_t, 64> &data)
{
#ifdef PRINT_Sha2_PROCESSED_DATA
    for(const auto elem : data)
        std::cout << std:: setfill('0') << std::setw(8) << std::hex << elem << " ";
    std::cout << std::endl;
#endif // PRINT_Sha2_PROCESSED_DATA

    auto a = h0_;
    auto b = h1_;
    auto c = h2_;
    auto d = h3_;
    auto e = h4_;
    auto f = h5_;
    auto g = h6_;
    auto h = h7_;

   for(auto i = 0; i < 64; ++i)
        sha2_operation_round(a, b, c, d, e, f, g, h, data[i], k_values[i]);

    h0_ += a;
    h1_ += b;
    h2_ += c;
    h3_ += d;
    h4_ += e;
    h5_ += f;
    h6_ += g;
    h7_ += h;
}

void Sha2::sha2_operation_round(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e,
                                uint32_t &f, uint32_t &g, uint32_t &h,
                                uint32_t data, uint32_t k)
{
    const auto s1   = right_rotate(e, 8) ^ right_rotate(e, 11) ^ right_rotate(e, 25);
    const auto ch   = (e & f) ^ ((~e) & g);
    const auto tmp1 = h + s1 + ch + k +data;
    const auto s0   = right_rotate(a, 2) ^ right_rotate(a, 13) ^ right_rotate(a, 22);
    const auto maj  = (a & b) ^ (a & c) ^ (b & c);
    const auto tmp2 = s0 + maj;

    h = g;
    g = f;
    f = e;
    e = d + tmp1;
    d = c;
    c = b;
    b = a;
    a = tmp1 + tmp2;

}

void Sha2::add_padding()
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

