#include "testutil.h"

#include <Bedrock/TransformedView2D.h>

using namespace perfectpixel::bedrock;

GTEST_TEST(TransformedView2D, TransformedView2D_identity)
{
    Vector3 pointA{4, 3, 1};
    Vector3 pointB{1, 2, -1};

    TransformedView2D<Vector3> translate
        = TransformedView2D(&pointA, Matrix3x3::IDENTITY);

    // Check that the viewed vector is translated
    Vector3 transformed = translate;
    AssertVectorEq(pointA, transformed);

    // Check that writebacks work
    translate = pointB;
    AssertVectorEq(pointB, pointA);
}
GTEST_TEST(TransformedView2D, TransformedView2D_positions)
{
    Vector3 point{4, 3, 1};

    TransformedView2D<Vector3> translate
        = TransformedView2D(&point, Matrix3x3::translate2D(Vector2{2, 1}));

    // Check that the viewed vector is translated
    Vector3 translated = translate;
    AssertVectorEq(Vector3{6, 4, 1}, translated);

    // Check that writebacks work
    translate = translated + Vector3{-1, -1, 0};
    AssertVectorEq(Vector3{3, 2, 1}, point);
}