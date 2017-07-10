#ifndef MD5_H
#define MD5_H


#include<array>
#include <string>
#include <vector>

class Md5
{
public:
    Md5();
    void reset();
    std::string get_hash();

    void hash_data(const std::vector<uint8_t> &data);
    void hash_data(const uint8_t* data, size_t len);
protected:

private:
    union
    {
        std::array<uint8_t, 16> hash_;
        struct
        {
            uint32_t a_, b_, c_, d_;
        };

    };

    std::vector<uint8_t> buffer_;
    uint64_t data_bytes_processed_;

    void run_round(const std::array<uint32_t, 16> &data);
    void add_padding();
    static void md5_operation_round(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d,
                                 uint32_t data, uint32_t f, uint32_t k, uint8_t shift);

    std::string compute_digest();
};

#endif // MD5_H
