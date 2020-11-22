#include <Bedrock/vectors.h>

#include <cmath>

namespace perfectpixel { namespace bedrock {

namespace {
const static float PI      = 3.14159265358979323846f;
const static float DEG2RAD = PI / 180;
} // namespace

Vector2::Vector2(const Vector3 &discard)
    : Vector<2>(std::array<float, 2>{discard.x(), discard.y()})
{}

const Vector2 Vector2::DOWN  = Vector2(0, -1);
const Vector2 Vector2::UP    = Vector2(0, 1);
const Vector2 Vector2::LEFT  = Vector2(-1, 0);
const Vector2 Vector2::RIGHT = Vector2(1, 0);

Vector3::Vector3(const Vector2 &expand)
    : Vector<3>(std::array<float, 3>({expand.x(), expand.y(), 0.0f}))
{}

Vector3::Vector3(const Vector4 &discard)
    : Vector<3>(std::array<float, 3>({discard.x(), discard.y(), discard.z()}))
{}

const Vector3 Vector3::DOWN    = Vector3(0, -1, 0);
const Vector3 Vector3::UP      = Vector3(0, 1, 0);
const Vector3 Vector3::LEFT    = Vector3(-1, 0, 0);
const Vector3 Vector3::RIGHT   = Vector3(1, 0, 0);
const Vector3 Vector3::FORWARD = Vector3(0, 0, 1);
const Vector3 Vector3::BACK    = Vector3(0, 0, -1);

Point2::Point2(int32_t x, int32_t y)
    : m_x(x)
    , m_y(y)
{}

Angle Angle::radians(float radians)
{
    Angle res;
    res.m_rad = radians;
    return res;
}

float Angle::radians()
{
    return m_rad;
}

Angle Angle::degrees(float degrees)
{
    Angle res;
    res.m_rad = degrees * DEG2RAD;
    return res;
}

float Angle::degrees()
{
    return m_rad / DEG2RAD;
}

}} // namespace perfectpixel::bedrock
