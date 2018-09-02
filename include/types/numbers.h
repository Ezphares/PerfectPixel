#pragma once

#include <cstdint>
#include <limits>

namespace perfectpixel {
	namespace types {


	typedef float PpFloat;
	typedef std::int32_t PpInt;

	const static PpFloat Infinity = std::numeric_limits<PpFloat>::infinity();

	}
}
