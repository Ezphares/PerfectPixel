#pragma once

#include <gtest/gtest.h>

#include <Bedrock/vectors.h>
#include <Bedrock/matrices.h>

using namespace perfectpixel::bedrock;

namespace {
	const float EPSILON = std::numeric_limits<float>::epsilon();
}

template<unsigned N>
inline static void ExpectVectorEq(Vector<N> expected, Vector<N> actual)
{
	for (unsigned i = 0; i < N; i++)
	{
		EXPECT_FLOAT_EQ(expected.m_data[i], actual.m_data[i]);
	}
}

template<unsigned W, unsigned H>
inline static void ExpectMatrixEq(Matrix<W, H> expected, Matrix<W, H> actual)
{
	for (unsigned i = 0; i < W * H; i++)
	{
		EXPECT_FLOAT_EQ(expected.m_data[i], actual.m_data[i]);
	}
}