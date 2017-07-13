#include "sha1.h"
#include <gtest/gtest.h>
#include <memory>

using namespace testing;

class TestSha2 : public Test
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

TEST_F(TestSha2, empty)
{
    hasher_->hash_data(nullptr, 0);
    EXPECT_EQ("d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f", hasher_->get_hash());
}

TEST_F(TestSha2, simple_string1)
{
    const auto str = std::string("The quick brown fox jumps over the lazy dog");
    hasher_->hash_data(std::vector<uint8_t>(str.begin(), str.end()));
    EXPECT_EQ("730e109bd7a8a32b1cb9d9a09aa2325d2430587ddbc0c38bad911525", hasher_->get_hash());
}

TEST_F(TestSha2, simple_string2)
{
    const auto str = std::string("The quick brown fox jumps over the lazy dog.");
    hasher_->hash_data(std::vector<uint8_t>(str.begin(), str.end()));
    EXPECT_EQ("619cba8e8e05826e9b8c519c0a5c68f4fb653e8a3d8aa04bb2c8cd4c", hasher_->get_hash());
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
