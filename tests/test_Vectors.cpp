#include <gtest/gtest.h>

#include "testutil.h"

#include <Bedrock/vectors.h>

using namespace perfectpixel::bedrock;

GTEST_TEST(Vector, cross)
{
	Vector3 a = Vector3(3, 0, 0);
	Vector3 b = Vector3(0, 5, 0);

	Vector3 c_ab = Vector3::cross(a, b);

	ExpectVectorEq(c_ab, Vector3(0, 0, 15));
}

GTEST_TEST(Vector, swizzle)
{
	Vector4 base{ 1, 2, 3, 4 };

	Vector4 identity = Vector4::swizzle(base, { X, Y, Z, W });
	ExpectVectorEq(Vector4{ 1, 2, 3, 4 }, identity);

	Vector4 rotated = Vector4::swizzle(base, { W, X, Y, Z });
	ExpectVectorEq(Vector4{ 4, 1, 2, 3 }, rotated);

	Vector4 negated = Vector4::swizzle(base, { N_X, N_Y, N_Z, N_W });
	ExpectVectorEq(Vector4{ -1, -2, -3, -4 }, negated);

	Vector4 single = Vector4::swizzle(base, { Y, Y, Y, Y });
	ExpectVectorEq(Vector4{ 2, 2, 2, 2 }, single);
}
