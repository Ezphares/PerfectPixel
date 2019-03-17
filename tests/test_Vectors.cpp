#include "stdafx.h"
#include "CppUnitTest.h"

#include "testutil.h"

#include <types/vectors.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace perfectpixel::types;

namespace tests {

	TEST_CLASS(test_Vectors)
	{
	public:

		TEST_METHOD(test_cross)
		{
			Vector3 a = Vector3(3, 0, 0);
			Vector3 b = Vector3(0, 5, 0);

			Vector3 c_ab = Vector3::cross(a, b);

			Assert::AreEqual(c_ab, Vector3(0, 0, 15));
		}

	};

}

