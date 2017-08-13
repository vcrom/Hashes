#ifndef SHA1_H
#define SHA1_H

#include "hashing_algorithm.h"

#include <array>
#include <string>

class Sha1 : public HashingAlgorithm
{
public:
    Sha1();
    void reset() final;
    std::string get_digest() final;
protected:
    union
    {
        std::array<uint32_t, 5> hash_;
        struct
        {
            uint32_t h0_, h1_, h2_, h3_, h4_;
        };

    };

    void run_round(const uint32_t *data) final;
    void flush_end_of_message() final;

private:
    void run_round(const std::array<uint32_t, 80> &data);
    static std::array<uint32_t, 80> pre_process_data(const std::array<uint32_t, 16> &data);
    void sha1_operation_round(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e,
                               uint32_t data, uint32_t f, uint32_t k);

    std::string Sha1::compute_digest();
};

#endif // SHA1_H
