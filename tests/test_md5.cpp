#include "md5.h"
#include <gtest/gtest.h>
#include <memory>

using namespace testing;

class TestMd5 : public Test
{
protected:
    virtual void SetUp()
    {
        md5_ = std::make_unique<Md5>();
    }
    virtual void TearDown()
    {
        md5_.reset();
    }

    std::unique_ptr<Md5> md5_;
};

TEST_F(TestMd5, empty)
{
    md5_->hash_data(nullptr, 0);
    EXPECT_EQ("d41d8cd98f00b204e9800998ecf8427e", md5_->get_hash());
}

TEST_F(TestMd5, simple_string1)
{
    const auto str = std::string("The quick brown fox jumps over the lazy dog");
    md5_->hash_data(std::vector<uint8_t>(str.begin(), str.end()));
    EXPECT_EQ("9e107d9d372bb6826bd81d3542a419d6", md5_->get_hash());
}

TEST_F(TestMd5, simple_string2)
{
    const auto str = std::string("The quick brown fox jumps over the lazy dog.");
    md5_->hash_data(std::vector<uint8_t>(str.begin(), str.end()));
    EXPECT_EQ("e4d909c290d0fb1ca068ffaddf22cbd0", md5_->get_hash());
}

TEST_F(TestMd5, multiple_execs_same_hash)
{
    const auto str = std::string("The fairy tale");
    const auto data = std::vector<uint8_t>(str.begin(), str.end());
    md5_->hash_data(data);
    const auto first = md5_->get_hash();
    for(size_t i = 0; i < 10; ++i)
    {
        md5_->hash_data(data);
        EXPECT_EQ(first, md5_->get_hash());
    }
}
