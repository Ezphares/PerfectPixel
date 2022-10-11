#include <Bedrock/Quaternion.h>

namespace perfectpixel { namespace bedrock {

Quaternion Quaternion::rotate(const Vector3 &axis, Angle angle)
{
    float halfTheta = angle.radians() / 2.0f;
    return Quaternion(axis.normal() * std::sin(halfTheta), std::cos(halfTheta));
}

Quaternion Quaternion::normalInverse() const
{
    return Quaternion(Vector3(*this) * -1.0f, w);
}

Vector3 operator*(const Quaternion &quaternion, const Vector3 &vec)
{
    return Vector3(
        quaternion * Quaternion(vec, 0.0f) * quaternion.normalInverse());
}

Quaternion operator*(const Quaternion q1, const Quaternion q2)
{
    Vector3 v1 = Vector3(q1), v2 = Vector3(q2);

    return Quaternion(
        v2 * q1.w + v1 * q2.w + Vector3::cross(v1, v2),
        q1.w * q2.w - Vector3::dot(v1, v2));
}

const Quaternion Quaternion::IDENTITY = Quaternion(Vector3(), 1);

}} // namespace perfectpixel::bedrock
