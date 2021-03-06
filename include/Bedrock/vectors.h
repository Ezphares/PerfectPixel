#pragma once

#include <Bedrock/TypeReflection.h>
#include <Bedrock/numbers.h>

#include <serialization/ISerializer.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <type_traits>

namespace perfectpixel {

namespace serialization {
class ISerializer;
}

namespace bedrock {

struct Angle;
struct Vector4;
struct Vector3;
struct Vector2;
struct Point3;
struct Point2;

struct Angle
{
    static Angle degrees(float degrees);
    static Angle radians(float radians);

    float radians();
    float degrees();

private:
    float m_rad;
};

enum SwizzlePosition : uint8_t
{
    X = 1 << 0,
    Y = 1 << 1,
    Z = 1 << 2,
    W = 1 << 3,

    N   = 1 << 7,
    N_X = N | X,
    N_Y = N | Y,
    N_Z = N | Z,
    N_W = N | W,
};

template <unsigned D>
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

    Vector(std::array<float, D> values)
    {
        m_data = values;
    }

    std::array<float, D> m_data;

    static float dot(const Vector<D> &l, const Vector<D> &r)
    {
        float accumulator{0.0f};
        for (unsigned i = 0; i < D; i++)
        {
            accumulator += l.m_data[i] * r.m_data[i];
        }
        return accumulator;
    }

    template <unsigned _ = D>
    static Vector<D> cross(
        const Vector<D> &l,
        const Vector<D> &r,
        typename std::enable_if<_ == 3, void>::type * = nullptr)
    {
        return Vector<D>(std::array<float, 3>{
            l.m_data[1] * r.m_data[2] - l.m_data[2] * r.m_data[1],
            l.m_data[2] * r.m_data[0] - l.m_data[0] * r.m_data[2],
            l.m_data[0] * r.m_data[1] - l.m_data[1] * r.m_data[0],
        });
    }

    template <unsigned _ = D>
    static float triple(
        const Vector<D> &a,
        const Vector<D> &b,
        const Vector<D> &c,
        typename std::enable_if<_ == 3, void>::type * = nullptr)
    {
        return dot(a, cross(b, c));
    }

    static Vector<D>
    swizzle(const Vector<D> &vec, const std::array<SwizzlePosition, D> &layout)
    {
        Vector<D> result;

        for (size_t i = 0u; i < D; ++i)
        {
            SwizzlePosition s = layout[i];
            bool negative     = (s & SwizzlePosition::N) > 0u;
            uint8_t src       = (s & ~SwizzlePosition::N);
            uint8_t srcIndex  = 0;
            while (src >>= 1)
                srcIndex++;
            result.m_data[i]
                = negative ? -vec.m_data[srcIndex] : vec.m_data[srcIndex];
        }

        return result;
    }

    float magnitude() const
    {
        return std::sqrt(dot(*this, *this));
    }

    Vector<D> normal() const
    {
        return Vector<D>(*this) / magnitude();
    }

    Vector<D> operator+(const Vector<D> &r) const
    {
        Vector<D> result = *this;
        result += r;
        return result;
    }

    Vector<D> &operator+=(const Vector<D> &r)
    {
        for (unsigned i = 0; i < D; i++)
        {
            m_data[i] += r.m_data[i];
        }
        return *this;
    }

    Vector<D> operator-(const Vector<D> &r) const
    {
        Vector<D> result = *this;
        result -= r;
        return result;
    }

    Vector<D> &operator-=(const Vector<D> &r)
    {
        for (unsigned i = 0; i < D; i++)
        {
            m_data[i] -= r.m_data[i];
        }
        return *this;
    }

    Vector<D> operator*(float scalar) const
    {
        Vector<D> result = *this;
        result *= scalar;
        return result;
    }

    Vector<D> &operator*=(float scalar)
    {
        for (unsigned i = 0; i < D; i++)
        {
            m_data[i] *= scalar;
        }
        return *this;
    }

    Vector<D> operator/(float scalar) const
    {
        Vector<D> result = *this;
        result /= scalar;
        return result;
    }

    Vector<D> &operator/=(float scalar)
    {
        for (unsigned i = 0; i < D; i++)
        {
            m_data[i] /= scalar;
        }
        return *this;
    }

    bool operator==(const Vector<D> &other) const
    {
        for (unsigned i = 0; i < D; i++)
        {
            if (m_data[i] != other.m_data[i])
            {
                return false;
            }
        }
        return true;
    }
};

struct Vector2 : public Vector<2>
{
    /* implicit */ Vector2(const Vector<2> &convert)
        : Vector<2>(convert)
    {}
    Vector2()
        : Vector<2>()
    {}
    Vector2(float x, float y)
        : Vector<2>(std::array<float, 2>{x, y})
    {}
    explicit Vector2(const Vector3 &discard);

    inline float x() const
    {
        return m_data[0];
    }
    inline float &x()
    {
        return m_data[0];
    }
    inline float y() const
    {
        return m_data[1];
    }
    inline float &y()
    {
        return m_data[1];
    }

    const static Vector2 DOWN;
    const static Vector2 UP;
    const static Vector2 LEFT;
    const static Vector2 RIGHT;
};

struct Vector3 : public Vector<3>
{
    /* implicit */ Vector3(const Vector<3> &convert)
        : Vector<3>(convert)
    {}
    Vector3()
        : Vector<3>()
    {}
    Vector3(float x, float y, float z)
        : Vector<3>(std::array<float, 3>{x, y, z})
    {}
    explicit Vector3(const Vector2 &expand);
    explicit Vector3(const Vector4 &discard);

    inline float x() const
    {
        return m_data[0];
    }
    inline float &x()
    {
        return m_data[0];
    }
    inline float y() const
    {
        return m_data[1];
    }
    inline float &y()
    {
        return m_data[1];
    }
    inline float z() const
    {
        return m_data[2];
    }
    inline float &z()
    {
        return m_data[2];
    }

    const static Vector3 DOWN;
    const static Vector3 UP;
    const static Vector3 LEFT;
    const static Vector3 RIGHT;
    const static Vector3 FORWARD;
    const static Vector3 BACK;
};

struct Vector4 : public Vector<4>
{
    /* implicit */ Vector4(const Vector<4> &convert)
        : Vector<4>(convert)
    {}
    Vector4()
        : Vector<4>()
    {}
    Vector4(float x, float y, float z, float w)
        : Vector<4>(std::array<float, 4>{x, y, z, w})
    {}

    inline float x() const
    {
        return m_data[0];
    }
    inline float &x()
    {
        return m_data[0];
    }
    inline float y() const
    {
        return m_data[1];
    }
    inline float &y()
    {
        return m_data[1];
    }
    inline float z() const
    {
        return m_data[2];
    }
    inline float &z()
    {
        return m_data[2];
    }
    inline float w() const
    {
        return m_data[3];
    }
    inline float &w()
    {
        return m_data[3];
    }
};

struct Point3
{
    int32_t m_x, m_y, m_z;

    Point3(int32_t x = 0, int32_t y = 0, int32_t z = 0);
    Point3(const Point2 &point2);
};

struct Point2
{
    int32_t m_x, m_y;

    Point2(int32_t x = 0, int32_t y = 0);
    Point2(const Point3 &point3);
};

inline bool operator==(const Point2 &l, const Point2 &r)
{
    return l.m_x == r.m_x && l.m_y == r.m_y;
}

} // namespace bedrock
} // namespace perfectpixel

namespace perfectpixel { namespace serialization {

template <unsigned D>
inline ISerializer &
operator<<(ISerializer &ostream, const perfectpixel::bedrock::Vector<D> &vec)
{
    ostream.writeArrayStart(true);

    for (unsigned i = 0; i < D; ++i)
    {
        ostream.writeFloat(vec.m_data[i]);
    }

    ostream.writeArrayEnd();
    return ostream;
}

template <unsigned D>
inline ISerializer &
operator>>(ISerializer &istream, perfectpixel::bedrock::Vector<D> &vec)
{
    uint32_t datasize = istream.readArrayStart();

    for (unsigned i = 0; i < std::min(D, datasize); ++i)
    {
        istream.readFloat(&vec.m_data[i]);
    }

    istream.readArrayEnd();

    return istream;
}

}} // namespace perfectpixel::serialization

PP_TYPE_REFLECTION(::perfectpixel::bedrock::Vector2, Vector2);
PP_TYPE_REFLECTION(::perfectpixel::bedrock::Vector3, Vector3);
PP_TYPE_REFLECTION(::perfectpixel::bedrock::Vector4, Vector4);
