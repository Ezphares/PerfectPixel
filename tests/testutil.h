#pragma once

#include "stdafx.h"
#include "CppUnitTest.h"

#include <types/vectors.h>
#include <types/matrices.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace perfectpixel::types;

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
	stream << "[" << ToString(Vector3(m.column(0))) << ", " << ToString(Vector3(m.column(1))) << ", " << ToString(Vector3(m.column(2))) << "]";;
	return stream.str();
}
