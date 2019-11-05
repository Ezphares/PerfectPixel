#include <gtest/gtest.h>

#include "testutil.h"

#include <Bedrock/vectors.h>

using namespace perfectpixel::bedrock;

GTEST_TEST(test_Vector, Cross)
{
    Vector3 a = Vector3(3, 0, 0);
    Vector3 b = Vector3(0, 5, 0);

    Vector3 c_ab = Vector3::cross(a, b);

    AssertVectorEq(c_ab, Vector3(0, 0, 15));
}

GTEST_TEST(test_Vector, Swizzle)
{
    Vector4 base{1, 2, 3, 4};

    Vector4 identity = Vector4::swizzle(base, {X, Y, Z, W});
    AssertVectorEq(Vector4{1, 2, 3, 4}, identity);

    Vector4 rotated = Vector4::swizzle(base, {W, X, Y, Z});
    AssertVectorEq(Vector4{4, 1, 2, 3}, rotated);

    Vector4 negated = Vector4::swizzle(base, {N_X, N_Y, N_Z, N_W});
    AssertVectorEq(Vector4{-1, -2, -3, -4}, negated);

    Vector4 single = Vector4::swizzle(base, {Y, Y, Y, Y});
    AssertVectorEq(Vector4{2, 2, 2, 2}, single);
}
