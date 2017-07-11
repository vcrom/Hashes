#include "hashing_algorithm.h"

HashingAlgorithm::HashingAlgorithm(const size_t bytes_per_round) :
    bytes_consumed_per_round_(bytes_per_round)
{
    buffer_.reserve(bytes_per_round);
}

void HashingAlgorithm::reset()
{
    buffer_.clear();
    data_bytes_processed_ = 0;
}

void HashingAlgorithm::hash_data(const std::vector<uint8_t> &data)
{
    if(data.size() > 0)
        hash_data(&data[0], data.size());
}

void HashingAlgorithm::hash_data(const uint8_t* data, size_t len)
{
    auto processed_bytes = size_t{0};
    // handle previous buffered data if any
    if(buffer_.size() > 0)
    {
        for(size_t i = buffer_.size();
            i < bytes_consumed_per_round_ && processed_bytes < len;
            ++i, ++processed_bytes)
        {
            buffer_.emplace_back(data[processed_bytes]);
        }
        if(buffer_.size() == bytes_consumed_per_round_)
        {
            run_round(reinterpret_cast<uint32_t*>(&buffer_[0]));
            buffer_.clear();
        }
    }
    //Process buffer
    while(len - processed_bytes >= bytes_consumed_per_round_)
    {
        run_round(reinterpret_cast<const uint32_t*>(&data[processed_bytes]));
        processed_bytes += bytes_consumed_per_round_;
    }
    //Buffer remainder of message in case the message has not been fully processed
    while(processed_bytes < len)
    {
        buffer_.emplace_back(data[processed_bytes]);
        ++processed_bytes;
    }
    data_bytes_processed_ += len;
}

void HashingAlgorithm::add_padding()
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
    *reinterpret_cast<uint64_t*>(&buffer_[offest_to_size_bits]) = data_bytes_processed_*8;
    run_round(reinterpret_cast<const uint32_t*>(&buffer_[0]));
    buffer_.clear();
}
