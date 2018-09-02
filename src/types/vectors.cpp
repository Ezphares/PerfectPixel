#include <types/vectors.h>

#include <cmath>

namespace perfectpixel {
namespace types {

	Vector3::Vector3()
		: m_x(0)
		, m_y(0)
		, m_z(0)
	{}

	Vector3::Vector3(PpFloat x, PpFloat y, PpFloat z)
		: m_x(x)
		, m_y(y)
		, m_z(z)
	{}

	Vector3::Vector3(const Vector2 &vec2)
		: m_x(vec2.m_x)
		, m_y(vec2.m_y)
		, m_z(0)
	{}

	PpFloat Vector3::magnitude()
	{
		return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z);
	}

	const Vector3 Vector3::DOWN = Vector3(0, -1, 0);
	const Vector3 Vector3::UP = Vector3(0, 1, 0);
	const Vector3 Vector3::LEFT = Vector3(1, 0, 0);
	const Vector3 Vector3::RIGHT = Vector3(-1, 0, 0);
	const Vector3 Vector3::FORWARD = Vector3(0, 0, 1);
	const Vector3 Vector3::BACK = Vector3(0, 0, -1);

	Vector2::Vector2()
		: m_x(0)
		, m_y(0)
	{}

	Vector2::Vector2(PpFloat x, PpFloat y)
		: m_x(x)
		, m_y(y)
	{}

	Vector2::Vector2(const Vector3 &vec3)
		: m_x(vec3.m_x)
		, m_y(vec3.m_y)
	{}

	bool operator==(const Vector3 &l, const Vector3 &r)
	{
		return l.m_x == r.m_x && l.m_y == r.m_y && l.m_z == r.m_z;
	}

	bool operator==(const Vector2 &l, const Vector2 &r)
	{
		return l.m_x == r.m_x && l.m_y == r.m_y;
	}

	perfectpixel::types::Vector3 operator+(const Vector3 &l, const Vector3 &r)
	{
		Vector3 res = l;
		res += r;
		return res;
	}

	perfectpixel::types::Vector3 & operator+=(Vector3 &l, const Vector3 &r)
	{
		l.m_x += r.m_x;
		l.m_y += r.m_y;
		l.m_z += r.m_z;
		return l;
	}

	perfectpixel::types::Vector2 operator+(const Vector2 &l, const Vector2 &r)
	{
		Vector2 res = l;
		res += r;
		return res;
	}

	perfectpixel::types::Vector3 operator*(const Vector3 &vec, PpFloat scalar)
	{
		Vector3 res = vec;
		res *= scalar;
		return res;
	}

	perfectpixel::types::Vector3 & operator*=(Vector3 &vec, PpFloat scalar)
	{
		vec.m_x *= scalar;
		vec.m_y *= scalar;
		vec.m_z *= scalar;
		return vec;
	}

	perfectpixel::types::Vector2 & operator+=(Vector2 &l, const Vector2 &r)
	{
		l.m_x += r.m_x;
		l.m_y += r.m_y;

		return l;
	}

	perfectpixel::types::Vector2 & operator*=(Vector2 &vec, PpFloat scalar)
	{
		vec.m_x *= scalar;
		vec.m_y *= scalar;
		return vec;
	}

	Point2::Point2()
		: m_x(0)
		, m_y(0)
	{
	}

	Point2::Point2(PpInt x, PpInt y)
		: m_x(x)
		, m_y(y)
	{
	}

}
}


