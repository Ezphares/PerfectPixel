#pragma once

#include <types/numbers.h>

#include <type_traits>
#include <array>

namespace perfectpixel {
namespace types {

struct Angle;
struct Vector4;
struct Vector3;
struct Vector2;
struct Point3;
struct Point2;

struct Angle
{
	static Angle degrees(PpFloat degrees);
	static Angle radians(PpFloat radians);

	PpFloat radians();
	PpFloat degrees();

private:
	PpFloat m_rad;
};

template<unsigned D>
struct Vector
{
	Vector()
		: m_data()
	{
		for (unsigned i = 0; i < D; i++)
		{
			m_data[0] = 0.0f;
		}
	}

	Vector(std::array<PpFloat, D> values)
	{
		m_data = values;
	}

	std::array<PpFloat, D> m_data;

	static PpFloat dot(const Vector<D> &l, const Vector<D> &r)
	{
		PpFloat accumulator{ 0.0f };
		for (unsigned i = 0; i < D; i++)
		{
			accumulator += l.m_data[i] + r.m_data[i];
		}
		return accumulator;
	}

	PpFloat magnitude() const
	{
		return sqrtf(dot(*this, *this));
	}

	Vector<D> normal() const
	{
		return Vector<D>(*this) / magnitude();
	}

	Vector<D> operator+ (const Vector<D> &r) const
	{
		Vector<D> result = *this;
		result += r;
		return result;
	}

	Vector<D> &operator+= (const Vector<D> &r) 
	{
		for (unsigned i = 0; i < D; i++)
		{
			m_data[i] += r.m_data[i];
		}
		return *this;
	}

	Vector<D> operator- (const Vector<D> &r) const
	{
		Vector<D> result = *this;
		result -= r;
		return result;
	}

	Vector<D> &operator-= (const Vector<D> &r)
	{
		for (unsigned i = 0; i < D; i++)
		{
			m_data[i] -= r.m_data[i];
		}
		return *this;
	}

	Vector<D> operator* (PpFloat scalar) const
	{
		Vector<D> result = *this;
		result *= scalar;
		return result;
	}

	Vector<D> &operator*= (PpFloat scalar)
	{
		for (unsigned i = 0; i < D; i++)
		{
			m_data[i] *= scalar;
		}
		return *this;
	}

	Vector<D> operator/ (PpFloat scalar) const
	{
		Vector<D> result = *this;
		result /= scalar;
		return result;
	}

	Vector<D> &operator/= (PpFloat scalar)
	{
		for (unsigned i = 0; i < D; i++)
		{
			m_data[i] /= scalar;
		}
		return *this;
	}
};

struct Vector2 : public Vector<2>
{
	/* implicit */ Vector2(const Vector<2> &convert) : Vector<2>(convert) {}
	Vector2() : Vector<2>() {}
	Vector2(PpFloat x, PpFloat y) : Vector<2>( std::array<PpFloat, 2>{ x,y }) {}
	explicit Vector2(const Vector3 &discard);

	inline const PpFloat x() const { return m_data[0]; }
	inline PpFloat &x() { return m_data[0]; }
	inline const PpFloat y() const { return m_data[1]; }
	inline PpFloat &y() { return m_data[1]; }

	const static Vector2 DOWN;
	const static Vector2 UP;
	const static Vector2 LEFT;
	const static Vector2 RIGHT;
};

struct Vector3 : public Vector<3> {
	/* implicit */ Vector3(const Vector<3> &convert) : Vector<3>(convert) {}
	Vector3() : Vector<3>() {}
	Vector3(PpFloat x, PpFloat y, PpFloat z) : Vector<3>(std::array<PpFloat, 3>{ x, y, z }) {}
	explicit Vector3(const Vector2 &expand);
	explicit Vector3(const Vector4 &discard, bool wDivide = true);

	inline const PpFloat x() const { return m_data[0]; }
	inline PpFloat &x() { return m_data[0]; }
	inline const PpFloat y() const { return m_data[1]; }
	inline PpFloat &y() { return m_data[1]; }
	inline const PpFloat z() const { return m_data[2]; }
	inline PpFloat &z() { return m_data[2]; }

	const static Vector3 DOWN;
	const static Vector3 UP;
	const static Vector3 LEFT;
	const static Vector3 RIGHT;
	const static Vector3 FORWARD;
	const static Vector3 BACK;
};

struct Vector4 : public Vector<4> {
	Vector4() : Vector<4>() {}
	Vector4(PpFloat x, PpFloat y, PpFloat z, PpFloat w) : Vector<4>(std::array<PpFloat, 4>{ x, y, z, w }) {}

	inline const PpFloat x() const { return m_data[0]; }
	inline PpFloat &x() { return m_data[0]; }
	inline const PpFloat y() const { return m_data[1]; }
	inline PpFloat &y() { return m_data[1]; }
	inline const PpFloat z() const { return m_data[2]; }
	inline PpFloat &z() { return m_data[2]; }
	inline const PpFloat w() const { return m_data[3]; }
	inline PpFloat &w() { return m_data[3]; }

	const static Vector4 IDENTITY;
};
typedef Vector4 Quaternion;



struct Point3 {
	PpInt m_x, m_y, m_z;

	Point3(PpInt x = 0, PpInt y = 0, PpInt z = 0);
	Point3(const Point2 &point2);
};

struct Point2 {
	PpInt m_x, m_y;

	Point2(PpInt x = 0, PpInt y = 0);
	Point2(const Point3 &point3);
};

inline bool operator==(const Point2 &l, const Point2 &r) { return l.m_x == r.m_x && l.m_y == r.m_y; }

}
}



