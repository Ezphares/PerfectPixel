#include "testutil.h"

#include <Bedrock/Quaternion.h>

using namespace perfectpixel::bedrock;

/// Multiplying a quaternion by it's inverse must result in the identity
GTEST_TEST(test_Quaternion, Invertibility)
{
    Quaternion quat = Quaternion::rotate(Vector3::FORWARD, Angle::degrees(90));
    Quaternion inv  = quat.normalInverse();

    Quaternion res = quat * inv;
    AssertVectorEq(Quaternion::IDENTITY, res);
}

GTEST_TEST(test_Quaternion, CreateIsNormal)
{
    Quaternion qid = Quaternion::IDENTITY;
    ASSERT_FLOAT_EQ(1.0f, qid.magnitude());

    Quaternion qrot
        = Quaternion::rotate(Vector3(2.0f, -3.0f, 0.5f), Angle::radians(1.5f));
    ASSERT_FLOAT_EQ(1.0f, qrot.magnitude());
}

GTEST_TEST(test_Quaternion, InvertIdentity)
{
    Quaternion id  = Quaternion::IDENTITY;
    Quaternion inv = id.normalInverse();

    AssertVectorEq(id, inv);
}

GTEST_TEST(test_Quaternion, RotateIdentity)
{
    Quaternion qid = Quaternion::IDENTITY;

    Vector3 v_up   = Vector3::UP * 2.0f;
    Vector3 v_diag = Vector3::DOWN + Vector3::RIGHT;

    Vector3 v_res_up = qid * v_up;
    AssertVectorEq(v_up, v_res_up);

    Vector3 v_res_diag = qid * v_diag;
    AssertVectorEq(v_diag, v_res_diag);
}

GTEST_TEST(test_Quaternion, RotateAligned)
{
    // Clockwise 90 degrees around the Z-axis
    Quaternion qrot = Quaternion::rotate(Vector3::FORWARD, Angle::degrees(90));

    Vector3 v_up   = Vector3::UP * 2.0f;
    Vector3 v_diag = Vector3::DOWN + Vector3::RIGHT;

    Vector3 v_res_up = qrot * v_up;
    AssertVectorEq(Vector3(Vector3::LEFT * 2.0f), v_res_up);

    Vector3 v_res_diag = qrot * v_diag;
    AssertVectorEq(Vector3::RIGHT + Vector3::UP, v_res_diag);
}

/// Rotations of vectors along their own axis should not change them
GTEST_TEST(test_Quaternion, RotateAlongSelf)
{
    Vector3 axis = Vector3::UP * 2.0f + Vector3::BACK;

    Quaternion qrot = Quaternion::rotate(axis.normal(), Angle::degrees(45));

    Vector3 result = qrot * axis;

    AssertVectorEq(axis, result);
}
