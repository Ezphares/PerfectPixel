#include "gtest/gtest.h"

#include "bedrock/Reflect.h"

namespace some_namespace {
struct TestDummy
{};

struct TestDummy2
{};
} // namespace some_namespace

PP_REFLECT(some_namespace::TestDummy)
PP_REFLECT(some_namespace::TestDummy2)

using namespace perfectpixel::bedrock;

// GTEST_TEST(test_TypeReflection, Null)
//{
//     ASSERT_EQ(0, typeID<std::nullptr_t>());
// #if PP_FULL_REFLECTION_ENABLED
//     ASSERT_EQ(std::string("nullptr_t"), typeName<std::nullptr_t>());
// #endif
// }

GTEST_TEST(test_TypeReflection, Uniqueness)
{
    ASSERT_NE(0, Reflect<some_namespace::TestDummy>::id());
    ASSERT_NE(0, Reflect<some_namespace::TestDummy2>::id());
    ASSERT_NE(
        Reflect<some_namespace::TestDummy2>::id(),
        Reflect<some_namespace::TestDummy>::id());
}

#if PP_FULL_REFLECTION_ENABLED
GTEST_TEST(test_TypeReflection, Names)
{
    ASSERT_EQ(
        std::string("TestDummy"),
        std::string(Reflect<some_namespace::TestDummy>::name()));
    ASSERT_EQ(
        std::string("TestDummy2"),
        std::string(Reflect<some_namespace::TestDummy2>::name()));
}
#endif
