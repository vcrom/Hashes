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
    EXPECT_EQ("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855", hasher_->get_digest());
}

TEST_F(TestSha2, simple_abc)
{
    const auto str = std::string("abc");
    hasher_->hash_data(std::vector<uint8_t>(str.begin(), str.end()));
    EXPECT_EQ("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad", hasher_->get_digest());
}

TEST_F(TestSha2, simple_string1)
{
    const auto str = std::string("The quick brown fox jumps over the lazy dog");
    hasher_->hash_data(std::vector<uint8_t>(str.begin(), str.end()));
    EXPECT_EQ("d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592", hasher_->get_digest());
}

TEST_F(TestSha2, simple_string2)
{
    const auto str = std::string("The quick brown fox jumps over the lazy dog.");
    hasher_->hash_data(std::vector<uint8_t>(str.begin(), str.end()));
    EXPECT_EQ("ef537f25c895bfa782526529a9b63d97aa631564d5d789c2b765448c8635fb6c", hasher_->get_digest());
}

TEST_F(TestSha2, multiple_execs_same_hash)
{
    const auto str = std::string("The fairy tale");
    const auto data = std::vector<uint8_t>(str.begin(), str.end());
    hasher_->hash_data(data);
    const auto first = hasher_->get_digest();
    for(size_t i = 0; i < 10; ++i)
    {
        hasher_->hash_data(data);
        EXPECT_EQ(first, hasher_->get_digest());
    }
}
