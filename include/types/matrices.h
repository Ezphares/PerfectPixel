#pragma once

#include <types/vectors.h>

namespace perfectpixel {
	namespace types {

struct Matrix4x4
{
	Matrix4x4(Vector4 c1 = Vector4(), Vector4 c2 = Vector4(), Vector4 c3 = Vector4(), Vector4 c4 = Vector4());

	const static Matrix4x4 IDENTITY;

	PpFloat m(unsigned index) const;
	Vector4 row(unsigned index) const;

	Vector4 m_c1, m_c2, m_c3, m_c4;
};

Matrix4x4 operator* (const Matrix4x4 &l, const Matrix4x4 &r);
Matrix4x4 &operator*= (Matrix4x4 &l, const Matrix4x4 &r);
Vector4 operator* (const Matrix4x4 &mat, const Vector4 &vec);

	}
}