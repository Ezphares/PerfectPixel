#include "testutil.h"

#include "bedrock/TransformedView2D.h"

using namespace perfectpixel::bedrock;

GTEST_TEST(test_TransformedView2D, TransformedView2D_identity)
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
GTEST_TEST(test_TransformedView2D, TransformedView2D_positions)
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

GTEST_TEST(test_TransformedView2D, TransformedView2D_rotations)
{
    Vector3 point{4, 3, 1};

    TransformedView2D<Vector3> rotate
        = TransformedView2D(&point, Matrix3x3::rotate2D(Angle::degrees(90)));

    // Check that the viewed vector is translated
    Vector3 translated = rotate;
    AssertVectorEq(Vector3{-3, 4, 1}, translated);

    // Check that writebacks work
    rotate = translated + Vector3{0, -4, 0};
    AssertVectorEq(Vector3{0, 3, 1}, point);
}