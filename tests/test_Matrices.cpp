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

    Matrix3x3 result = m1to9 * mid3;

    AssertMatrixEq(m1to9, result);
}
