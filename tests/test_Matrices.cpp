#include "testutil.h"

#include <Bedrock/matrices.h>

using namespace perfectpixel::bedrock;

GTEST_TEST(test_Matrix, Determinant2)
{
    Matrix<2, 2> mid2 = Matrix<2, 2>::generateIdentity();
    ASSERT_FLOAT_EQ(1.0f, mid2.determinant());

    Matrix<2, 2> m1to4 = Matrix<2, 2>({1, 2, 3, 4});
    ASSERT_FLOAT_EQ(-2.0f, m1to4.determinant());
}

GTEST_TEST(test_Matrix, Determinant3)
{
    Matrix<3, 3> mid3 = Matrix<3, 3>::generateIdentity();
    ASSERT_FLOAT_EQ(1.0f, mid3.determinant());

    Matrix<3, 3> m1to9 = Matrix<3, 3>({1, 2, 3, 4, 5, 6, 7, 8, 9});
    ASSERT_FLOAT_EQ(0.0f, m1to9.determinant());
}

GTEST_TEST(test_Matrix, Determinant4)
{

    Matrix<4, 4> mid4 = Matrix<4, 4>::generateIdentity();
    ASSERT_FLOAT_EQ(1.0f, mid4.determinant());

    Matrix<4, 4> m1to16
        = Matrix<4, 4>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    ASSERT_FLOAT_EQ(0.0f, m1to16.determinant());
}

GTEST_TEST(test_Matrix, Mul3x3Identity)
{
    Matrix3x3 m1to9 = Matrix3x3({1, 2, 3, 4, 5, 6, 7, 8, 9});
    Matrix3x3 mid3  = Matrix3x3::IDENTITY;

    Matrix3x3 result  = m1to9 * mid3;
    Matrix3x3 result2 = mid3 * m1to9;

    AssertMatrixEq(m1to9, result);
}

GTEST_TEST(test_Matrix, Mul3x3)
{
    Matrix3x3 m1 = Matrix3x3({1, 3, -4, 2, 2, 0, -1, 0, 2});
    Matrix3x3 m2 = Matrix3x3({3, 0, -2, 4, 1, 0, 2, 0, 1});

    Matrix3x3 r = Matrix3x3({5, 9, -16, 6, 14, -16, 1, 6, -6});

    Matrix3x3 result = m1 * m2;

    AssertMatrixEq(r, result);
}

GTEST_TEST(test_Matrix, Transform3x3)
{
    Vector2 point{4, 3};

    Matrix3x3 translate = Matrix3x3::translate2D(Vector2{2, -2});
    Vector2 translated  = translate * point;
    AssertVectorEq(Vector2{6, 1}, translated);

    Matrix3x3 scale = Matrix3x3::scale2D(Vector2{0.5f, -2.0f});
    Vector2 scaled  = scale * point;
    AssertVectorEq(Vector2{2, -6}, scaled);

    Matrix3x3 rotate = Matrix3x3::rotate2D(Angle::degrees(90.0f));
    Vector2 rotated  = rotate * point;
    AssertVectorEq(Vector2{-3, 4}, rotated);

    Matrix3x3 rotateAround
        = Matrix3x3::rotateAround2D(Vector2{5, 3}, Angle::degrees(90.0f));
    Vector2 rotatedAround = rotateAround * point;
    AssertVectorEq(Vector2{5, 2}, rotatedAround);
}
