#pragma once

#include <types/numbers.h>

namespace perfectpixel {
namespace types {

struct Angle;
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

struct Vector3 {
	PpFloat m_x, m_y, m_z;

	Vector3();
	Vector3(PpFloat x, PpFloat y, PpFloat z);
	Vector3(const Vector2 &vec2);

	const static Vector3 DOWN;
	const static Vector3 UP;
	const static Vector3 LEFT;
	const static Vector3 RIGHT;
	const static Vector3 FORWARD;
	const static Vector3 BACK;

	PpFloat magnitude();
	Vector3 normal();
};


struct Vector2 {
	PpFloat m_x, m_y;

	Vector2();
	Vector2(Angle direction, PpFloat magnitude);
	Vector2(PpFloat x, PpFloat y);
	Vector2(const Vector3 &vec3);

	PpFloat magnitude();
	Vector2 normal();
};

struct Point3 {
	PpInt m_x, m_y, m_z;

	Point3();
	Point3(PpInt x, PpInt y, PpInt z);
	Point3(const Point2 &point2);
};

struct Point2 {
	PpInt m_x, m_y;

	Point2();
	Point2(PpInt x, PpInt y);
	Point2(const Point3 &point3);
};

bool operator==(const Vector3 &l, const Vector3 &r);
bool operator==(const Vector2 &l, const Vector2 &r);

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

Vector2 operator*(const Vector2 &vec, PpFloat scalar);
Vector2 &operator*=(Vector2 &vec, PpFloat scalar);

}
}



