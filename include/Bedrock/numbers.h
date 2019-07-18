#pragma once

#include <cstdint>
#include <limits>

namespace perfectpixel {

	namespace serialization
	{
		class BinarySerializer;
	}

	namespace bedrock {


	typedef float PpFloat;
	typedef std::int32_t PpInt;

	const static PpFloat Infinity = std::numeric_limits<PpFloat>::infinity();

	}
}

perfectpixel::serialization::BinarySerializer &operator<<(perfectpixel::serialization::BinarySerializer &ostream, const perfectpixel::bedrock::PpFloat &num);
perfectpixel::serialization::BinarySerializer &operator>>(perfectpixel::serialization::BinarySerializer &istream, perfectpixel::bedrock::PpFloat &num);

perfectpixel::serialization::BinarySerializer &operator<<(perfectpixel::serialization::BinarySerializer &ostream, const perfectpixel::bedrock::PpInt &num);
perfectpixel::serialization::BinarySerializer &operator>>(perfectpixel::serialization::BinarySerializer &istream, perfectpixel::bedrock::PpInt &num);