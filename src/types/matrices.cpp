#include <types/matrices.h>

namespace perfectpixel {
namespace types {



	Matrix4x4::Matrix4x4(Vector4 c1 /*= Vector4()*/, Vector4 c2 /*= Vector4()*/, Vector4 c3 /*= Vector4()*/, Vector4 c4 /*= Vector4()*/)
		: m_c1(c1)
		, m_c2(c2)
		, m_c3(c3)
		, m_c4(c4)
	{
	}

	const Matrix4x4 Matrix4x4::IDENTITY{
		Vector4(1.0f, 0.0f, 0.0f, 0.0f),
		Vector4(0.0f, 1.0f, 0.0f, 0.0f),
		Vector4(0.0f, 0.0f, 1.0f, 0.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f) };

	perfectpixel::types::PpFloat Matrix4x4::m(unsigned index) const
	{

		unsigned column = index / 4;

		switch (index)
		{
		case 0:
			return m_c1.m(index % 4);
		case 1:
			return m_c2.m(index % 4);
		case 2:
			return m_c3.m(index % 4);
		case 3:
			return m_c4.m(index % 4);
		default:
			throw "Matrix index out of bounds";

			break;
		}
	}

	perfectpixel::types::Vector4 Matrix4x4::row(unsigned index) const
	{
		if (index >= 4)
		{
			throw "Matrix row index out of bounds";
		}

		return Vector4(m_c1.m(index), m_c2.m(index), m_c3.m(index), m_c4.m(index));
	}

	perfectpixel::types::Vector4 operator*(const Matrix4x4 &mat, const Vector4 &vec)
	{
		return Vector4{
			Vector4::dot(mat.row(0), vec),
			Vector4::dot(mat.row(1), vec),
			Vector4::dot(mat.row(2), vec),
			Vector4::dot(mat.row(3), vec)
		};
	}


	perfectpixel::types::Matrix4x4 operator*(const Matrix4x4 &l, const Matrix4x4 &r)
	{
		Matrix4x4 result = l;
		result *= r;
		return result;
	}

	perfectpixel::types::Matrix4x4 &operator*=(Matrix4x4 &l, const Matrix4x4 &r)
	{
		Vector4 c1 = l * r.m_c1;
		Vector4 c2 = l * r.m_c2;
		Vector4 c3 = l * r.m_c3;
		l.m_c4 = l * r.m_c4; // It's ok to pollute l here, as we are done calculating rows
		l.m_c3 = c3;
		l.m_c2 = c2;
		l.m_c1 = c1;

		return l;
	}

}
}