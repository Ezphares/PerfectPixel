#pragma once

#include <bedrock/matrices.h>

namespace perfectpixel { namespace bedrock {

template <typename T>
class TransformedView2D
{
public:
    TransformedView2D(T *target, Matrix3x3 transform, Matrix3x3 inverse)
        : m_target(target)
        , m_transform(transform)
        , m_inverse(inverse)
    {}

    TransformedView2D(T *target, Matrix3x3 transform)
        : TransformedView2D(target, transform, transform.inverse())
    {}

    template <typename U>
    TransformedView2D<U> apply(U *target) const
    {
        return TransformedView2D<U>(target, m_transform, m_inverse);
    }

    operator T() const
    {
        return m_transform * (*m_target);
    }

    void operator=(const T &value)
    {
        (*m_target) = m_inverse * value;
    }

private:
    T *m_target;
    const Matrix3x3 m_transform;
    const Matrix3x3 m_inverse;
};

}} // namespace perfectpixel::bedrock
