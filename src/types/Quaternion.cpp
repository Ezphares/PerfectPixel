#include "types/Quaternion.h"

namespace perfectpixel {
namespace types {

	perfectpixel::types::Quaternion Quaternion::rotate(const Vector3 &axis, Angle angle)
	{
		PpFloat halfTheta = angle.radians() / 2.0f;
		return Quaternion(axis.normal() * std::sin(halfTheta), std::cos(halfTheta));
	}

	perfectpixel::types::Quaternion Quaternion::normalInverse() const
	{
		return Quaternion(Vector3(*this, false) * -1.0f, w());
	}

	perfectpixel::types::Vector3 operator*(const Quaternion &quaternion, const Vector3 &vec)
	{
		return Vector3(quaternion * Quaternion(vec, 0.0f) * quaternion.normalInverse(), false);
	}

	perfectpixel::types::Quaternion operator*(const Quaternion q1, const Quaternion q2)
	{
		Vector3
			v1 = Vector3(q1, false),
			v2 = Vector3(q2, false);

		return Quaternion(
			v2 * q1.w() + v1 * q2.w() + Vector3::cross(v1, v2),
			q1.w() * q2.w() - Vector3::dot(v1, v2));
	}

	const Quaternion Quaternion::IDENTITY = Quaternion(Vector3(), 1);

}
}