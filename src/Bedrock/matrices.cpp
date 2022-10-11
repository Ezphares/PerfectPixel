#include <Bedrock/matrices.h>

#include <cmath>

namespace perfectpixel { namespace bedrock {

const Matrix2x2 Matrix2x2::IDENTITY = Matrix2x2::generateIdentity();

const Matrix3x3 Matrix3x3::IDENTITY = Matrix3x3::generateIdentity();
const Matrix4x4 Matrix4x4::IDENTITY = Matrix4x4::generateIdentity();

perfectpixel::bedrock::Matrix2x2 Matrix2x2::rotate(Angle angle)
{
    const float cosine = std::cos(angle.radians());
    const float sine   = std::sin(angle.radians());
    return Matrix<2, 2>(std::array<float, 4>{
        cosine,
        sine, // C1
        -sine,
        cosine, // C2
    });
}

perfectpixel::bedrock::Matrix3x3
Matrix3x3::translate2D(const Vector2 &coordinates)
{
    Matrix3x3 result = IDENTITY;

    result.m(2, 0) = coordinates.x;
    result.m(2, 1) = coordinates.y;

    return result;
}

perfectpixel::bedrock::Matrix3x3 Matrix3x3::scale2D(const Vector2 &axes)
{
    Matrix3x3 result = IDENTITY;

    result.m(0, 0) = axes.x;
    result.m(1, 1) = axes.y;

    return result;
}

perfectpixel::bedrock::Matrix3x3 Matrix3x3::rotate2D(const Angle &angle)
{
    Matrix3x3 result = IDENTITY;

    const float cosine = std::cos(angle.radians());
    const float sine   = std::sin(angle.radians());

    result.m(0, 0) = cosine;
    result.m(0, 1) = sine;
    result.m(1, 0) = -sine;
    result.m(1, 1) = cosine;

    return result;
}

perfectpixel::bedrock::Matrix3x3
Matrix3x3::rotateAround2D(const Vector2 &point, const Angle &angle)
{
    return translate2D(point) * rotate2D(angle) * translate2D(-point);
}

perfectpixel::bedrock::Matrix3x3 Matrix3x3::transform2D(
    Vector2 &translation, const Vector2 scaling, const Angle &rotation)
{
    return translate2D(translation) * rotate2D(rotation) * scale2D(scaling);
}

perfectpixel::bedrock::Matrix4x4
Matrix4x4::translate(const Vector3 &coordinates)
{
    Matrix4x4 result = IDENTITY;

    result.m(3, 0) = coordinates.x;
    result.m(3, 1) = coordinates.y;
    result.m(3, 2) = coordinates.z;

    return result;
}

perfectpixel::bedrock::Matrix4x4 Matrix4x4::scale(const Vector3 &axes)
{
    Matrix4x4 result = IDENTITY;

    result.m(0, 0) = axes.x;
    result.m(1, 1) = axes.y;
    result.m(2, 2) = axes.z;

    return result;
}

perfectpixel::bedrock::Matrix4x4 Matrix4x4::rotate(const Quaternion &quat)
{
    Matrix4x4 result = IDENTITY;

    const float xx = quat.x * quat.x;
    const float xw = quat.x * quat.w;
    const float xy = quat.x * quat.y;

    const float yy = quat.y * quat.y;
    const float yw = quat.y * quat.w;
    const float yz = quat.y * quat.z;

    const float zz = quat.z * quat.z;
    const float zw = quat.z * quat.w;
    const float zx = quat.x * quat.z;

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

perfectpixel::bedrock::Matrix4x4 Matrix4x4::transform(
    const Vector3 &translation,
    const Vector3 &scaling,
    const Quaternion &rotation)
{
    return translate(translation) * rotate(rotation) * scale(scaling);
}

perfectpixel::bedrock::Vector2 operator*(const Matrix3x3 &mat, const Vector2 v2)
{
    return Vector2{mat * Vector3{v2, 1.0f}};
}

}} // namespace perfectpixel::bedrock
