#include <gtest/gtest.h>

#include "usqrt.h"

TEST(usqrt, test_usqrt)
{
    EXPECT_EQ(0, usqrt32(0));
    EXPECT_EQ(3, usqrt32(9));
    EXPECT_EQ(10, usqrt32(100));
    EXPECT_EQ(16, usqrt32(256));
}

TEST(usqrt, test_inverse)
{
    for(uint32_t i = 0; i <= UINT16_MAX; i++)
    {
        EXPECT_EQ(i, usqrt32(i*i));
    }
}