#pragma once

#include "types/vectors.h"

namespace perfectpixel {
	namespace bedrock {

		struct Quaternion : public Vector4
		{
			Quaternion(const Vector3 &vec, PpFloat w) : Vector4({ vec.x(), vec.y(), vec.z(), w }) {};

			static Quaternion rotate(const Vector3 &axis, Angle angle);

			/// Caller should check for norm = 1
			Quaternion normalInverse() const;

			const static Quaternion IDENTITY;
		};


		Quaternion operator*(const Quaternion q1, const Quaternion q2);

		/// Rotate a vector3 by this quaternion
		Vector3 operator*(const Quaternion &quaternion, const Vector3 &vec);


	}
}