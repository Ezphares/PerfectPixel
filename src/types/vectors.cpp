#include <types/vectors.h>

#include <cmath>

namespace perfectpixel {
namespace types {

	namespace
	{
		const static PpFloat PI = 3.14159265358979323846f;
		const static PpFloat DEG2RAD = PI / 180;
	}

	Vector2::Vector2(const Vector3 &discard)
		: Vector<2>(std::array<PpFloat, 2>{discard.x(), discard.y()})
	{}

	const Vector2 Vector2::DOWN = Vector2(0, -1);
	const Vector2 Vector2::UP = Vector2(0, 1);
	const Vector2 Vector2::LEFT = Vector2(1, 0);
	const Vector2 Vector2::RIGHT = Vector2(-1, 0);

	Vector3::Vector3(const Vector2 &expand)
		: Vector<3>(std::array<PpFloat, 3>({expand.x(), expand.y(), 0.0f}))
	{}


	Vector3::Vector3(const Vector4 &discard, bool wDivide /*= true*/)
		: Vector<3>(std::array<PpFloat, 3>({ discard.x(), discard.y(), discard.z() }))
	{
		if (wDivide && discard.w() > 0.0f)
		{
			*this /= discard.w();
		}
	}

	const Vector3 Vector3::DOWN = Vector3(0, -1, 0);
	const Vector3 Vector3::UP = Vector3(0, 1, 0);
	const Vector3 Vector3::LEFT = Vector3(1, 0, 0);
	const Vector3 Vector3::RIGHT = Vector3(-1, 0, 0);
	const Vector3 Vector3::FORWARD = Vector3(0, 0, 1);
	const Vector3 Vector3::BACK = Vector3(0, 0, -1);

	const Vector4 Vector4::IDENTITY{ 0.0f, 0.0f, 0.0f, 1.0f };

	Point2::Point2(PpInt x, PpInt y)
		: m_x(x)
		, m_y(y)
	{
	}

	Angle Angle::radians(PpFloat radians)
	{
		Angle res;
		res.m_rad = radians;
		return res;
	}

	PpFloat Angle::radians()
	{
		return m_rad;
	}

	Angle Angle::degrees(PpFloat degrees)
	{
		Angle res;
		res.m_rad = degrees * DEG2RAD;
		return res;
	}

	PpFloat Angle::degrees()
	{
		return m_rad / DEG2RAD;
	}

}
}


