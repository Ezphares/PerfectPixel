#pragma once
#include <Bedrock/vectors.h>

namespace perfectpixel { namespace bedrock {

class Line2D
{
public:
    Line2D(Vector2 origin, Angle theta)
        : m_origin(origin)
        , m_normal(std::cosf(theta.radians()), std::sinf(theta.radians()))
    {}

    Line2D(Vector2 origin, Vector2 normal)
        : m_origin(origin)
        , m_normal(normal)
    {}

    Vector2 getProjection(Vector2 point) const
    {
        const Vector2 delta = point - m_origin;
        return point - (m_normal * Vector2::dot(delta, m_normal));
    }

    float getDistance(Vector2 point) const
    {
        const Vector2 delta = point - m_origin;
        return Vector2::dot(delta, m_normal);
    }

    float getOffsetFromOrigin(Vector2 point) const
    {
        const Vector2 delta = point - m_origin;
        return Vector2::dot(delta, Vector2{-m_normal.y, m_normal.x});
    }

public:
    const Vector2 m_origin;
    const Vector2 m_normal;
};

class LineSegment2D : public Line2D
{
public:
    LineSegment2D(Vector2 origin, Vector2 normal, float halfLength)
        : Line2D(origin, normal)
        , m_halfLength(halfLength)
    {}

    LineSegment2D(Vector2 origin, Angle theta, float halfLength)
        : Line2D(origin, theta)
        , m_halfLength(halfLength)
    {}

    static LineSegment2D fromStartAndEnd(Vector2 start, Vector2 end)
    {
        const Vector2 halfDelta = (end - start) * 0.5f;

        assert(
            Vector2::dot(halfDelta, halfDelta)
            > std::numeric_limits<float>::epsilon());

        const Vector2 origin    = start + halfDelta;
        const float halfLength  = halfDelta.magnitude();
        const Vector2 deltaNorm = halfDelta / halfLength;
        const Vector2 normal{-deltaNorm.y, deltaNorm.x};

        return LineSegment2D(origin, normal, halfLength);
    }

    Vector2 getStart() const
    {
        return m_origin + Vector2{-m_normal.y, m_normal.x} * m_halfLength;
    }

    Vector2 getEnd() const
    {
        return m_origin + Vector2{m_normal.y, -m_normal.x} * m_halfLength;
    }

    bool isInside(Vector2 point) const
    {
        return std::fabs(getOffsetFromOrigin(point)) < m_halfLength;
    }

    float getCapsuleDistance(Vector2 point) const
    {
        const float offset = getOffsetFromOrigin(point);

        if (offset > m_halfLength)
        {
            return (point - getEnd()).magnitude();
        }
        else if (offset < -m_halfLength)
        {
            return (point - getStart()).magnitude();
        }
        else
        {
            return getDistance(point);
        }
    }

public:
    const float m_halfLength;
};

}} // namespace perfectpixel::bedrock
