#ifndef SHA2_H
#define SHA2_H

#include "hashing_algorithm.h"

#include <array>
#include <string>

class Sha2 : public HashingAlgorithm
{
public:
    Sha2();
    void reset() final;
    std::string get_hash() final;
protected:
    union
    {
        std::array<uint32_t, 8> hash_;
        struct
        {
            uint32_t h0_, h1_, h2_, h3_, h4_, h5_, h6_, h7_;
        };

    };

    void run_round(const uint32_t *data) final;
    void add_padding() final;

private:
    void run_round(const std::array<uint32_t, 64> &data);
    static std::array<uint32_t, 64> expand_data(const std::array<uint32_t, 16> &data);
    void sha2_operation_round(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e,
                              uint32_t &f, uint32_t &g, uint32_t &h,
                              uint32_t data, uint32_t k);

    std::string compute_digest();
};

#endif // SHA2_H
