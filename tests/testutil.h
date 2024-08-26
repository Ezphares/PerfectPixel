#pragma once

#include <gtest/gtest.h>

#include <Bedrock/matrices.h>
#include <Bedrock/vectors.h>

using namespace perfectpixel::bedrock;

namespace {
const float EPSILON = 1e-6f;
}

template <unsigned N>
inline static void AssertVectorEq(Vector<N> expected, Vector<N> actual)
{
    for (unsigned i = 0; i < N; i++)
    {
        ASSERT_NEAR(expected[i], actual[i], EPSILON);
    }
}

template <unsigned W, unsigned H>
inline static void AssertMatrixEq(Matrix<W, H> expected, Matrix<W, H> actual)
{
    for (unsigned i = 0; i < W * H; i++)
    {
        ASSERT_NEAR(expected.m_data[i], actual.m_data[i], EPSILON);
    }
}
