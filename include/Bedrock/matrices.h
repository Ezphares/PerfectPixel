#pragma once

#include "bedrock/Quaternion.h"
#include "bedrock/vectors.h"

#include <array>
#include <type_traits>

namespace perfectpixel { namespace bedrock {

template <unsigned W, unsigned H>
struct Matrix
{
    static_assert(
        W > 1, "Matrix should have at least 2 columns, use a vector otherwise");
    static_assert(
        H > 1, "Matrix should have at least 2 rows, use a vector otherwise");

    template <typename T>
    using EnableIfSquare = typename std::enable_if<W == H, T>::type;

    Matrix(std::array<float, W * H> data)
        : m_data(data)
    {}

    Matrix()
        : m_data()
    {
        for (unsigned i = 0; i < W * H; i++)
        {
            m_data[i] = 0.0f;
        }
    }

    float &m(unsigned x, unsigned y)
    {
        return m_data[x * H + y];
    }

    const float &m(unsigned x, unsigned y) const
    {
        return m_data[x * H + y];
    }

    std::array<float, W * H> m_data;

    Vector<H> column(unsigned index) const
    {
        std::array<float, H> result;
        for (unsigned i = 0; i < H; i++)
        {
            result[i] = m_data[index * H + i];
        }
        return result;
    }

    Vector<W> row(unsigned index) const
    {
        std::array<float, W> result;
        for (unsigned i = 0; i < W; i++)
        {
            result[i] = m_data[i * H + index];
        }
        return result;
    }

    Vector<H> operator*(const Vector<W> &r) const
    {
        Vector<H> result;
        for (unsigned i = 0; i < H; i++)
        {
            result[i] = Vector<W>::dot(row(i), r);
        }
        return result;
    }

    Matrix<H, H> operator*(const Matrix<H, W> &r) const
    {
        Matrix<H, H> result;

        for (unsigned i = 0; i < H; i++)
        {
            for (unsigned j = 0; j < H; j++)
            {
                result.m(i, j) = Vector<H>::dot(row(j), r.column(i));
            }
        }

        return result;
    }

    template <typename T = Matrix<H, W>>
    EnableIfSquare<T> &operator*=(const Matrix<H, W> &r)
    {
        return *this = *this * r;
    }

    bool operator==(const Matrix<W, H> &r) const
    {
        return m_data == r.m_data;
    }

    template <typename T = Matrix<H, H>>
    static EnableIfSquare<T> generateIdentity()
    {
        T result;

        for (unsigned i = 0; i < H; i++)
        {
            result.m(i, i) = 1;
        }

        return result;
    }

    template <typename T = Matrix<H - 1, H - 1>>
    EnableIfSquare<T> getMinor(unsigned x, unsigned y) const
    {
        T result;

        for (unsigned i = 0; i < H - 1; i++)
        {
            for (unsigned j = 0; j < H - 1; j++)
            {
                unsigned sourceX = i >= x ? i + 1 : i;
                unsigned sourceY = j >= y ? j + 1 : j;

                result.m(i, j) = m(sourceX, sourceY);
            }
        }

        return result;
    }

    template <unsigned D>
    static float getDeterminant(const Matrix<D, D> &mat)
    {
        float accumulator{0.0f};
        bool add = true;

        for (unsigned i = 0; i < D; i++)
        {
            float recurse = mat.m(i, 0) * getDeterminant(mat.getMinor(i, 0));

            accumulator += add ? recurse : -recurse;

            add = !add;
        }

        return accumulator;
    }

    static float getDeterminant(const Matrix<2, 2> &mat)
    {
        return mat.m_data[0] * mat.m_data[3] - mat.m_data[1] * mat.m_data[2];
    }

    template <typename T = float>
    EnableIfSquare<T> determinant() const
    {
        return Matrix<H, H>::getDeterminant(*this);
    }

    template <typename T = Matrix<H, H>>
    EnableIfSquare<T> transposed() const
    {
        Matrix<H, H> result;

        for (int i = 0; i < H; ++i)
        {
            for (int j = 0; j < H; ++j)
            {
                result.m(j, i) = m(i, j);
            }
        }

        return result;
    }

    template <typename T = Matrix<H, H>>
    EnableIfSquare<T> inverse() const
    {
        Matrix<H, H> cofactors;

        float det = 0;
        for (int j = 0; j < H; ++j)
        {
            for (int i = 0; i < H; ++i)
            {
                bool add = (i + j) % 2 == 0;

                float v = getDeterminant(getMinor(i, j)) * (add ? 1.0f : -1.0f);
                cofactors.m(i, j) = v;

                if (j == 0)
                {
                    det += m(i, j) * v;
                }
            }
        }

        const float inv     = 1.0f / det;
        Matrix<H, H> result = cofactors.transposed();

        for (int i = 0; i < H * H; ++i)
        {
            result.m_data[i] *= inv;
        }

        return result;
    }
};

struct Matrix2x2 : public Matrix<2, 2>
{
    /*implicit*/ Matrix2x2(const Matrix<2, 2> &convert)
        : Matrix<2, 2>(convert)
    {}
    explicit Matrix2x2(const std::array<float, 4> data)
        : Matrix<2, 2>(data)
    {}

    static const Matrix2x2 IDENTITY;

    static Matrix2x2 rotate(Angle angle);
};

struct Matrix3x3 : public Matrix<3, 3>
{
    /*implicit*/ Matrix3x3(const Matrix<3, 3> &convert)
        : Matrix<3, 3>(convert)
    {}
    explicit Matrix3x3(const std::array<float, 9> data)
        : Matrix<3, 3>(data)
    {}

    static const Matrix3x3 IDENTITY;

    static Matrix3x3 translate2D(const Vector2 &coordinates);
    static Matrix3x3 scale2D(const Vector2 &axes);
    static Matrix3x3 rotate2D(const Angle &angle);

    static Matrix3x3 rotateAround2D(const Vector2 &point, const Angle &angle);

    static Matrix3x3 transform2D(
        Vector2 &translation, const Vector2 scaling, const Angle &rotation);
};

Vector2 operator*(const Matrix3x3 &mat, const Vector2 v2);

struct Matrix4x4 : public Matrix<4, 4>
{
    /*implicit*/ Matrix4x4(const Matrix<4, 4> &convert)
        : Matrix<4, 4>(convert)
    {}
    explicit Matrix4x4(const std::array<float, 16> data)
        : Matrix<4, 4>(data)
    {}

    static const Matrix4x4 IDENTITY;

    static Matrix4x4 translate(const Vector3 &coordinates);
    static Matrix4x4 scale(const Vector3 &axes);
    static Matrix4x4 rotate(const Quaternion &quat);

    static Matrix4x4 transform(
        const Vector3 &translation,
        const Vector3 &scaling,
        const Quaternion &rotation);
};

}} // namespace perfectpixel::bedrock
