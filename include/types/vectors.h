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
	static Angle ofVector(Vector2 vec);

	PpFloat radians();
	PpFloat degrees();

private:
	PpFloat m_rad;
};

struct Vector4 {
	PpFloat m_x, m_y, m_z, m_w;

	Vector4();
	Vector4(PpFloat x, PpFloat y, PpFloat z, PpFloat w);
	explicit Vector4(Vector3 vec, PpFloat w = 1.0f);

	static PpFloat dot(const Vector4 &l, const Vector4 &r);

	PpFloat m(unsigned index) const;

	const static Vector4 IDENTITY;
};
typedef Vector4 Quaternion;

struct Vector3 {
	PpFloat m_x, m_y, m_z;

	Vector3();
	Vector3(PpFloat x, PpFloat y, PpFloat z);
	explicit Vector3(Vector4 vec4, bool w_divide = true);
	explicit Vector3(const Vector2 &vec2, PpFloat z = 0.0f);

	const static Vector3 DOWN;
	const static Vector3 UP;
	const static Vector3 LEFT;
	const static Vector3 RIGHT;
	const static Vector3 FORWARD;
	const static Vector3 BACK;

	static PpFloat dot(const Vector3 &l, const Vector3 &r);

	PpFloat magnitude();
	Vector3 normal();
};


struct Vector2 {
	PpFloat m_x, m_y;

	Vector2();
	Vector2(Angle direction, PpFloat magnitude);
	Vector2(PpFloat x, PpFloat y);
	explicit Vector2(const Vector3 &vec3);

	static PpFloat dot(const Vector2 &l, const Vector2 &r);
	PpFloat magnitude();
	Vector2 normal();
};

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

bool operator==(const Vector3 &l, const Vector3 &r);
bool operator==(const Vector2 &l, const Vector2 &r);

bool operator==(const Point2 &l, const Point2 &r);

Vector3 operator+(const Vector3 &l, const Vector3 &r);
Vector3 &operator+=(Vector3 &l, const Vector3 &r);
Vector3 operator-(const Vector3 &l, const Vector3 &r);
Vector3 &operator-=(Vector3 &l, const Vector3 &r);

Vector3 operator*(const Vector3 &vec, PpFloat scalar);
Vector3 &operator*=(Vector3 &vec, PpFloat scalar);
Vector3 operator/(const Vector3 &vec, PpFloat scalar);
Vector3 &operator/=( Vector3 &vec, PpFloat scalar);

Vector2 operator+(const Vector2 &l, const Vector2 &r);
Vector2 &operator+=(Vector2 &l, const Vector2 &r);
Vector2 operator-(const Vector2 &l, const Vector2 &r);
Vector2 &operator-=(Vector2 &l, const Vector2 &r);

Vector2 operator*(const Vector2 &vec, PpFloat scalar);
Vector2 &operator*=(Vector2 &vec, PpFloat scalar);
Vector2 operator/(const Vector2 &vec, PpFloat scalar);
Vector2 &operator/=(Vector2 &vec, PpFloat scalar);


}
}



