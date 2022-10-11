#pragma once

#include <gtest/gtest.h>

#include <Bedrock/matrices.h>
#include <Bedrock/vectors.h>

using namespace perfectpixel::bedrock;

namespace {
const float EPSILON = std::numeric_limits<float>::epsilon();
}

template <unsigned N>
inline static void AssertVectorEq(Vector<N> expected, Vector<N> actual)
{
    for (unsigned i = 0; i < N; i++)
    {
        ASSERT_FLOAT_EQ(expected[i], actual[i]);
    }
}

template <unsigned W, unsigned H>
inline static void AssertMatrixEq(Matrix<W, H> expected, Matrix<W, H> actual)
{
    for (unsigned i = 0; i < W * H; i++)
    {
        ASSERT_FLOAT_EQ(expected.m_data[i], actual.m_data[i]);
    }
}
