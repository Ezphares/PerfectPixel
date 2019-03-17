#include "stdafx.h"
#include "CppUnitTest.h"

#include "testutil.h"

#include <types/matrices.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace perfectpixel::types;


namespace tests {

	TEST_CLASS(test_Matrices)
	{
	public:

		TEST_METHOD(test_determinant_2)
		{
			Matrix<2, 2> mid2 = Matrix<2, 2>::generateIdentity();
			Assert::AreEqual(1.0f, mid2.determinant());


			Matrix<2, 2> m1to4 = Matrix<2, 2>({ 1, 2, 3, 4 });
			Assert::AreEqual(-2.0f, m1to4.determinant());
		}

		TEST_METHOD(test_determinant_3)
		{
			Matrix<3, 3> mid3 = Matrix<3, 3>::generateIdentity();
			Assert::AreEqual(1.0f, mid3.determinant());


			Matrix<3, 3> m1to9 = Matrix<3, 3>({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });
			Assert::AreEqual(0.0f, m1to9.determinant());
		}

		TEST_METHOD(test_determinant_4)
		{

			Matrix<4, 4> mid4 = Matrix<4, 4>::generateIdentity();
			Assert::AreEqual(1.0f, mid4.determinant());


			Matrix<4, 4> m1to16 = Matrix<4, 4>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
			Assert::AreEqual(0.0f, m1to16.determinant());

		}

		TEST_METHOD(test_mul3x3_identity)
		{
			Matrix3x3 m1to9 = Matrix3x3({ 1,2,3,4,5,6,7,8,9 });
			Matrix3x3 mid3 = Matrix3x3::IDENTITY;

			Matrix3x3 result = m1to9 * mid3;

			Assert::AreEqual(m1to9, result);
		}
	};

	   
}

