#include "sha1.h"
#include <gtest/gtest.h>
#include <memory>

using namespace testing;

class TestSha1 : public Test
{
protected:
    virtual void SetUp()
    {
        hasher_ = std::make_unique<Sha1>();
    }

    virtual void TearDown()
    {
        hasher_.reset();
    }

    std::unique_ptr<Sha1> hasher_;
};

TEST_F(TestSha1, empty)
{
    hasher_->hash_data(nullptr, 0);
    EXPECT_EQ("da39a3ee5e6b4b0d3255bfef95601890afd80709", hasher_->get_hash());
}

TEST_F(TestSha1, simple_string1)
{
    const auto str = std::string("The quick brown fox jumps over the lazy dog");
    hasher_->hash_data(std::vector<uint8_t>(str.begin(), str.end()));
    EXPECT_EQ("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12", hasher_->get_hash());
}

TEST_F(TestSha1, simple_string2)
{
    const auto str = std::string("The quick brown fox jumps over the lazy cog");
    hasher_->hash_data(std::vector<uint8_t>(str.begin(), str.end()));
    EXPECT_EQ("de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3", hasher_->get_hash());
}

TEST_F(TestSha1, multiple_execs_same_hash)
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
