#include <gtest/gtest.h>

#include "testutil.h"

#include <Bedrock/vectors.h>

using namespace perfectpixel::bedrock;

GTEST_TEST(Vector, test_cross)
{
	Vector3 a = Vector3(3, 0, 0);
	Vector3 b = Vector3(0, 5, 0);

	Vector3 c_ab = Vector3::cross(a, b);

	ExpectVectorEq(c_ab, Vector3(0, 0, 15));
}

