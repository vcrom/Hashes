#ifndef MD5_H
#define MD5_H

#include "hashing_algorithm.h"

#include <array>
#include <string>
#include <vector>

class Md5 : public HashingAlgorithm
{
public:
    Md5();
    void reset() final;
    std::string get_hash() final;

protected:
    union
    {
        std::array<uint8_t, 16> hash_;
        struct
        {
            uint32_t a_, b_, c_, d_;
        };

    };

    void run_round(const uint32_t *data) final;

private:
    void run_round(const std::array<uint32_t, 16> &data);
    static void md5_operation_round(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d,
                                 uint32_t data, uint32_t f, uint32_t k, uint8_t shift);

    std::string compute_digest();
};

#endif // MD5_H
