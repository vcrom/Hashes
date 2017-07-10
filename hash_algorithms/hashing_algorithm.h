#ifndef HASHING_ALGORITHM_H
#define HASHING_ALGORITHM_H

#include <string>
#include <vector>

class HashingAlgorithm
{
public:
    HashingAlgorithm(const size_t bytes_per_round);

    virtual std::string get_hash() = 0;
    virtual void reset();

    void hash_data(const std::vector<uint8_t> &data);
    void hash_data(const uint8_t* data, size_t len);

protected:
    virtual void run_round(const uint32_t *data) = 0;
    virtual void add_padding();

    size_t bytes_consumed_per_round_;
    uint64_t data_bytes_processed_;
    std::vector<uint8_t> buffer_;
};

#endif // HASHING_ALGORITHM_H
