#pragma once

#include "bedrock/Reflect.h"
#include "bedrock/numbers.h"

#include "serialization/ISerializer.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <type_traits>

#include <cassert>

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

    float radians() const;
    float degrees() const;

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
struct VectorData
{
    std::array<float, D> m_data;

    inline float &operator[](unsigned i)
    {
        return m_data[i];
    }
    inline const float &operator[](unsigned i) const
    {
        return m_data[i];
    }
};

template <>
struct VectorData<2u>
{
    float x, y;

    inline float &operator[](unsigned i)
    {
        assert(i < 2);

        switch (i)
        {
        case 0:
            return x;
        default:
            return y;
        }
    }
    inline const float &operator[](unsigned i) const
    {
        assert(i < 2);

        switch (i)
        {
        case 0:
            return x;
        default:
            return y;
        }
    }
};

template <>
struct VectorData<3u>
{
    float x, y, z;

    inline float &operator[](unsigned i)
    {
        assert(i < 3);

        switch (i)
        {
        case 0:
            return x;
        case 1:
            return y;
        default:
            return z;
        }
    }
    inline const float &operator[](unsigned i) const
    {
        assert(i < 3);

        switch (i)
        {
        case 0:
            return x;
        case 1:
            return y;
        default:
            return z;
        }
    }
};

template <>
struct VectorData<4u>
{
    float x, y, z, w;

    inline float &operator[](unsigned i)
    {
        assert(i < 4);

        switch (i)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            return w;
        }
    }
    inline const float &operator[](unsigned i) const
    {
        assert(i < 4);

        switch (i)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            return w;
        }
    }
};

template <unsigned D>
struct Vector : public VectorData<D>
{
    Vector()
    {
        for (unsigned i = 0; i < D; i++)
        {
            (*this)[i] = 0.0f;
        }
    }

    Vector(std::array<float, D> values)
    {
        for (unsigned i = 0; i < D; i++)
        {
            (*this)[i] = values[i];
        }
    }

    static float dot(const Vector<D> &l, const Vector<D> &r)
    {
        float accumulator{0.0f};
        for (unsigned i = 0; i < D; i++)
        {
            accumulator += (l[i] * r[i]);
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
            l[1] * r[2] - l[2] * r[1],
            l[2] * r[0] - l[0] * r[2],
            l[0] * r[1] - l[1] * r[0],
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

        for (unsigned int i = 0u; i < D; ++i)
        {
            SwizzlePosition s = layout[i];
            bool negative     = (s & SwizzlePosition::N) > 0u;
            uint8_t src       = (s & ~SwizzlePosition::N);
            uint8_t srcIndex  = 0;
            while (src >>= 1)
                srcIndex++;
            result[i] = negative ? -vec[srcIndex] : vec[srcIndex];
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
            (*this)[i] += r[i];
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
            (*this)[i] -= r[i];
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
            (*this)[i] *= scalar;
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
            (*this)[i] /= scalar;
        }
        return *this;
    }

    Vector<D> operator-() const
    {
        return *this * -1.f;
    }

    bool operator==(const Vector<D> &other) const
    {
        for (unsigned i = 0; i < D; i++)
        {
            if ((*this)[i] != other[i])
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
    explicit Vector3(const Vector2 &expand, float z = 0.0f);
    explicit Vector3(const Vector4 &discard);

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
        ostream.writeFloat(vec[i]);
    }

    ostream.writeArrayEnd();
    return ostream;
}

template <unsigned D>
inline ISerializer &
operator>>(ISerializer &istream, perfectpixel::bedrock::Vector<D> &vec)
{
    uint32_t datasize = istream.readArrayStart();

    assert(datasize == D);

    for (unsigned i = 0; i < D; ++i)
    {
        istream.readFloat(&vec[i]);
    }

    istream.readArrayEnd();

    return istream;
}

}} // namespace perfectpixel::serialization

PP_REFLECT(::perfectpixel::bedrock::Vector2);
PP_REFLECT(::perfectpixel::bedrock::Vector3);
PP_REFLECT(::perfectpixel::bedrock::Vector4);
