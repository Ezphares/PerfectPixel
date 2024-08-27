#include "gtest/gtest.h"

#include "bedrock/TypeReflection.h"

namespace some_namespace {
	struct TestDummy {};

	struct TestDummy2 {};
}

PP_TYPE_REFLECTION(some_namespace::TestDummy, TestDummy)
PP_TYPE_REFLECTION(some_namespace::TestDummy2, TestDummy2)

using namespace perfectpixel::bedrock;

GTEST_TEST(test_TypeReflection, Null)
{
	ASSERT_EQ(0, typeID<std::nullptr_t>());
#if PP_FULL_REFLECTION_ENABLED
	ASSERT_EQ(std::string("nullptr_t"), typeName<std::nullptr_t>());
#endif
}

GTEST_TEST(test_TypeReflection, Uniqueness)
{
	ASSERT_NE(0, typeID<some_namespace::TestDummy>());
	ASSERT_NE(0, typeID<some_namespace::TestDummy2>());
	ASSERT_NE(typeID<some_namespace::TestDummy2>(), typeID<some_namespace::TestDummy>());
}

#if PP_FULL_REFLECTION_ENABLED
GTEST_TEST(test_TypeReflection, Names)
{
	ASSERT_EQ(std::string("TestDummy"), typeName<some_namespace::TestDummy>());
	ASSERT_EQ(std::string("TestDummy2"), typeName<some_namespace::TestDummy2>());
}
#endif
