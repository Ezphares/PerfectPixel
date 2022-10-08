#pragma once
#include <Bedrock/vectors.h>

namespace perfectpixel { namespace bedrock {

class Line2D
{
public:
    Line2D(Vector2 origin, Angle theta)
        : m_origin(origin)
        , m_theta(theta)
        , m_normal(std::cosf(theta.radians()), std::sinf(theta.radians()))
    {}

    Line2D(Vector2 origin, Vector2 normal)
        : m_origin(origin)
        , m_theta(Angle::radians(std::atan2f(normal.x(), normal.y())))
        , m_normal(normal)
    {}

    Vector2 getProjection(Vector2 point) const
    {
        const Vector2 delta = point - m_origin;
        return point - (m_normal * Vector2::dot(delta, m_normal));
    }

    Vector2 getDistance(Vector2 point) const
    {}

public:
    const Vector2 m_origin;
    const Angle m_theta;
    const Vector2 m_normal;
};

class LineSegment2D : public Line2D
{
public:
    LineSegment2D(Vector2 origin, Angle theta, float halfLength)
        : Line2D(origin, theta)
        , m_halfLength(halfLength)
    {}

public:
    const float m_halfLength;
};

}} // namespace perfectpixel::bedrock
