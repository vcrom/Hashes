#include "sha2.h"
#include <gtest/gtest.h>
#include <memory>

using namespace testing;

class TestSha2 : public Test
{
protected:
    virtual void SetUp()
    {
        hasher_ = std::make_unique<Sha2>();
    }

    virtual void TearDown()
    {
        hasher_.reset();
    }

    std::unique_ptr<Sha2> hasher_;
};

TEST_F(TestSha2, empty)
{
    hasher_->hash_data(nullptr, 0);
    EXPECT_EQ("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855", hasher_->get_hash());
}

TEST_F(TestSha2, simple_string1)
{
    const auto str = std::string("The quick brown fox jumps over the lazy dog");
    hasher_->hash_data(std::vector<uint8_t>(str.begin(), str.end()));
    EXPECT_EQ("26b54a596255364bf3b1d9b4df2fd33d61974d1f000df23b6f7de4313a6c72e5", hasher_->get_hash());
}

TEST_F(TestSha2, simple_string2)
{
    const auto str = std::string("The quick brown fox jumps over the lazy dog.");
    hasher_->hash_data(std::vector<uint8_t>(str.begin(), str.end()));
    EXPECT_EQ("47398c0ae3a363a1c8df74a5ff0f8a64f739b95335991095996be6e4e20b7525", hasher_->get_hash());
}

TEST_F(TestSha2, multiple_execs_same_hash)
{
    const auto str = std::string("The fairy tale");
    const auto data = std::vector<uint8_t>(str.begin(), str.end());
    hasher_->hash_data(data);
    const auto first = hasher_->get_hash();
    for(size_t i = 0; i < 10; ++i)
    {
        hasher_->hash_data(data);
        EXPECT_EQ(first, hasher_->get_hash());
    }
}
