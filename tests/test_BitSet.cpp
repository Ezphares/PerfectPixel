#include <gtest/gtest.h>

#include <Bedrock/BitSet.h>

using namespace perfectpixel::bedrock;

GTEST_TEST(test_BitSet, Create)
{
	BitSet allZero{ 4 };
	for (int i = 0; i < 4; ++i)
	{
		ASSERT_FALSE(allZero[i]);
	}

	BitSet allOne{ 4 , true };
	for (int i = 0; i < 4; ++i)
	{
		ASSERT_TRUE(allOne[i]);
	}

	BitSet alternate{ (uint8_t*)"\x0a", 4 };
	ASSERT_EQ(false, alternate[0]);
	ASSERT_EQ(true,  alternate[1]);
	ASSERT_EQ(false, alternate[2]);
	ASSERT_EQ(true,  alternate[3]);
}

GTEST_TEST(test_BitSet, Equality)
{
	BitSet a{ (uint8_t*)"\xaa", 8 };
	BitSet b{ (uint8_t*)"\xaa", 8 };

	ASSERT_TRUE(a == b);

	BitSet c{ (uint8_t*)"\x55", 8 };

	ASSERT_FALSE(a == c);
	ASSERT_TRUE(a == ~c);

	BitSet d{ (uint8_t*)"\xaa\xaa", 16 };

	ASSERT_FALSE(a == d);
}

GTEST_TEST(test_BitSet, Negate)
{
	BitSet alternate{ (uint8_t*)"\x0a", 4 };
	alternate.negate();
	ASSERT_EQ(true, alternate[0]);
	ASSERT_EQ(false, alternate[1]);
	ASSERT_EQ(true, alternate[2]);
	ASSERT_EQ(false, alternate[3]);

	BitSet large{ 500 };
	large.negate();
	for (int i = 0; i < 500; ++i)
	{
		ASSERT_TRUE(large[i]);
	}
}

GTEST_TEST(test_BitSet, And)
{
	BitSet a{ (uint8_t*)"\xbe\xff", 16 };
	BitSet b{ (uint8_t*)"\xff\xaf", 16 };

	ASSERT_TRUE((BitSet{ (uint8_t*)"\xbe\xaf", 16 }) == (a & b));
}

GTEST_TEST(test_BitSet, AndDifferentSizes)
{
	BitSet a{ (uint8_t*)"\xff\xff", 16 };
	BitSet b{ (uint8_t*)"\xff", 8 };

	ASSERT_TRUE((BitSet{ (uint8_t*)"\xff\x00", 16 }) == (a & b));
	ASSERT_TRUE((BitSet{ (uint8_t*)"\xff\x00", 16 }) == (b & a));
}

GTEST_TEST(test_BitSet, Or)
{
	BitSet a{ (uint8_t*)"\xbe\x00", 16 };
	BitSet b{ (uint8_t*)"\x00\xaf", 16 };

	ASSERT_TRUE((BitSet{ (uint8_t*)"\xbe\xaf", 16 }) == (a | b));
}

GTEST_TEST(test_BitSet, OrDifferentSizes)
{
	BitSet a{ (uint8_t*)"\x00\xff", 16 };
	BitSet b{ (uint8_t*)"\xff", 8 };

	ASSERT_TRUE((BitSet{ (uint8_t*)"\xff\xff", 16 }) == (a | b));
	ASSERT_TRUE((BitSet{ (uint8_t*)"\xff\xff", 16 }) == (b | a));
}
