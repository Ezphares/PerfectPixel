#pragma once

#include "stdafx.h"
#include "CppUnitTest.h"

#include <types/vectors.h>
#include <types/matrices.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace perfectpixel::types;

namespace {
	const PpFloat EPSILON = 0.0001f;
}

template <>
inline static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<Vector3>(const Vector3 &v)
{
	std::wstringstream stream;
	stream << "[" << v.x() << ", " << v.y() << ", " << v.z() << "]";
	return stream.str();
}

template <>
inline static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<Matrix3x3>(const Matrix3x3 &m)
{

	std::wstringstream stream;
	stream << "[" << ToString(Vector3(m.column(0))) << " , " << ToString(Vector3(m.column(1))) << " , " << ToString(Vector3(m.column(2))) << "]";;
	return stream.str();
}
namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {

			template <unsigned D>
			inline static std::wstring ToString(const Vector<D> &v)
			{
				std::wstringstream stream;
				stream.precision(4);
				for (unsigned i = 0; i < D; i++)
				{
					stream << std::fixed << ((i == 0) ? L"[" : L" , ") << (float)v.m_data[i];
				}
				stream << "]";
				return stream.str();
			}
		}
	}
}

inline static void AssertFloatApprox(PpFloat expected, PpFloat actual, const wchar_t *message = 0, const Microsoft::VisualStudio::CppUnitTestFramework::__LineInfo *lineinfo = 0)
{	
	std::wstring defaultMsg = (L"Expected:<" + ToString(expected) + L">\nActual:<" + ToString(actual) + L">\n(Epsilon: " + ToString(EPSILON) + L")");

	Assert::IsTrue(std::abs(expected - actual) < EPSILON, message ? message : defaultMsg.c_str(), lineinfo);
}

template<unsigned N>
inline static void AssertFloatApprox(Vector<N> expected, Vector<N> actual, const wchar_t *message = 0, const Microsoft::VisualStudio::CppUnitTestFramework::__LineInfo *lineinfo = 0)
{
	std::wstring defaultMsg = (L"Expected:<" + ToString(expected) + L">\nActual:<" + ToString(actual) + L">\n(Epsilon: " + ToString(EPSILON) + L")");

	for (unsigned i = 0; i < N; i++)
	{
		AssertFloatApprox(expected.m_data[i], actual.m_data[i], message ? message : defaultMsg.c_str());
	}
}