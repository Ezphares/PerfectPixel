#include "stdafx.h"
#include "CppUnitTest.h"

#include "testutil.h"

#include <types/Quaternion.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace perfectpixel::types;


namespace tests {

	TEST_CLASS(test_Quaternion)
	{
	public:

		/// Multiplying a quaternion by it's inverse must result in the identity
		TEST_METHOD(test_invertibility)
		{
			Quaternion quat = Quaternion::rotate(Vector3::FORWARD, Angle::degrees(90));
			Quaternion inv = quat.normalInverse();

			Quaternion res = quat * inv;
			AssertFloatApprox(Quaternion::IDENTITY, res);
		}

		TEST_METHOD(test_invert_identity)
		{
			Quaternion id = Quaternion::IDENTITY;
			Quaternion inv = id.normalInverse();

			AssertFloatApprox(id, inv);
		}

		TEST_METHOD(test_rotate_identity)
		{
			Quaternion qid = Quaternion::IDENTITY;

			Vector3 v_up = Vector3::UP * 2.0f;
			Vector3 v_diag = Vector3::DOWN + Vector3::RIGHT;

			Vector3 v_res_up = qid * v_up;
			Assert::AreEqual(v_up, v_res_up);

			Vector3 v_res_diag = qid * v_diag;
			Assert::AreEqual(v_diag, v_res_diag);

		}

		TEST_METHOD(test_rotate_aligned)
		{
			// Clockwise 90 degrees around the Z-axis
			Quaternion qrot = Quaternion::rotate(Vector3::FORWARD, Angle::degrees(90));

			Vector3 v_up = Vector3::UP * 2.0f;
			Vector3 v_diag = Vector3::DOWN + Vector3::RIGHT;

			Vector3 v_res_up = qrot * v_up;
			AssertFloatApprox(Vector3(Vector3::LEFT * 2.0f), v_res_up);

			Vector3 v_res_diag = qrot * v_diag;
			AssertFloatApprox(Vector3::RIGHT + Vector3::UP, v_res_diag);
		}
	};
}

