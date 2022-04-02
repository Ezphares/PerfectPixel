#include <Bedrock/matrices.h>

#include <cmath>

namespace perfectpixel { namespace bedrock {

const Matrix2x2 Matrix2x2::IDENTITY = Matrix2x2::generateIdentity();
const Matrix3x3 Matrix3x3::IDENTITY = Matrix3x3::generateIdentity();
const Matrix4x4 Matrix4x4::IDENTITY = Matrix4x4::generateIdentity();

perfectpixel::bedrock::Matrix2x2 Matrix2x2::rotate(Angle angle)
{
    float cosine = std::cos(angle.radians());
    float sine   = std::sin(angle.radians());
    return Matrix<2, 2>(std::array<float, 4>{
        cosine,
        sine, // C1
        -sine,
        cosine, // C2
    });
}

perfectpixel::bedrock::Matrix4x4
Matrix4x4::translate(const Vector3 &coordinates)
{
    Matrix4x4 result = IDENTITY;

    result.m(3, 0) = coordinates.x();
    result.m(3, 1) = coordinates.y();
    result.m(3, 2) = coordinates.z();

    return result;
}

perfectpixel::bedrock::Matrix4x4 Matrix4x4::scale(const Vector3 &axes)
{
    Matrix4x4 result = IDENTITY;

    result.m(0, 0) = axes.x();
    result.m(1, 1) = axes.y();
    result.m(2, 2) = axes.z();

    return result;
}

perfectpixel::bedrock::Matrix4x4 Matrix4x4::roatation(const Quaternion &quat)
{
    Matrix4x4 result = IDENTITY;

    const float xx = quat.x() * quat.x();
    const float xw = quat.x() * quat.w();
    const float xy = quat.x() * quat.y();

    const float yy = quat.y() * quat.y();
    const float yw = quat.y() * quat.w();
    const float yz = quat.y() * quat.z();

    const float zz = quat.z() * quat.z();
    const float zw = quat.z() * quat.w();
    const float zx = quat.x() * quat.z();

    result.m(0, 0) = 1.0f - 2.0f * (yy + zz);
    result.m(0, 1) = 2.0f * (xy - zw);
    result.m(0, 2) = 2.0f * (zx + yw);

    result.m(1, 0) = 2.0f * (xy + zw);
    result.m(1, 1) = 1.0f - 2.0f * (zz + xx);
    result.m(1, 2) = 2.0f * (yz - xw);

    result.m(2, 0) = 2.0f * (zx - yw);
    result.m(2, 1) = 2.0f * (yz + xw);
    result.m(2, 2) = 1.0f - 2.0f * (xx + yy);

    return result;
}

}} // namespace perfectpixel::bedrock
