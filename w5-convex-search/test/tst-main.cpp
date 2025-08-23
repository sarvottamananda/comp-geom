#include <gtest/gtest.h>

int bar(int x) { return x; }

TEST(OneTest, Sample) { EXPECT_EQ(1, bar(1)); }

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
