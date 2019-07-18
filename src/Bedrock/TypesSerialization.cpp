
#include <Bedrock/numbers.h>

#include <serialization/BinarySerializer.h>

namespace {
	uint64_t pack754(long double f, unsigned bits, unsigned expbits)
	{
		long double fnorm;
		int shift;
		long long sign, exp, significand;
		unsigned significandbits = bits - expbits - 1; // -1 for sign bit

		if (f == 0.0) return 0; // get this special case out of the way

		// check sign and begin normalization
		if (f < 0) { sign = 1; fnorm = -f; }
		else { sign = 0; fnorm = f; }

		// get the normalized form of f and track the exponent
		shift = 0;
		while (fnorm >= 2.0) { fnorm /= 2.0; shift++; }
		while (fnorm < 1.0) { fnorm *= 2.0; shift--; }
		fnorm = fnorm - 1.0;

		// calculate the binary form (non-float) of the significand data
		significand = fnorm * ((1LL << significandbits) + 0.5f);

		// get the biased exponent
		exp = shift + ((1 << (expbits - 1)) - 1); // shift + bias

		// return the final answer
		return (sign << (bits - 1)) | (exp << (bits - expbits - 1)) | significand;
	}

	long double unpack754(uint64_t i, unsigned bits, unsigned expbits)
	{
		long double result;
		long long shift;
		unsigned bias;
		unsigned significandbits = bits - expbits - 1; // -1 for sign bit

		if (i == 0) return 0.0;

		// pull the significand
		result = (i&((1LL << significandbits) - 1)); // mask
		result /= (1LL << significandbits); // convert back to float
		result += 1.0f; // add the one back on

		// deal with the exponent
		bias = (1 << (expbits - 1)) - 1;
		shift = ((i >> significandbits)&((1LL << expbits) - 1)) - bias;
		while (shift > 0) { result *= 2.0; shift--; }
		while (shift < 0) { result /= 2.0; shift++; }

		// sign it
		result *= (i >> (bits - 1)) & 1 ? -1.0 : 1.0;

		return result;
	}
}

perfectpixel::serialization::BinarySerializer & operator<<(perfectpixel::serialization::BinarySerializer &ostream, const perfectpixel::bedrock::PpFloat &num)
{
	// Assume IEEE754
	uint32_t packed = pack754(num, 32, 8);
	memcpy(ostream.require(4), &packed, 4);
	return ostream;
}

perfectpixel::serialization::BinarySerializer & operator>>(perfectpixel::serialization::BinarySerializer &istream, perfectpixel::bedrock::PpFloat &num)
{
	uint32_t packed;
	memcpy(&packed, istream.get(4), 4);
	num = unpack754(packed, 32, 8);
	return istream;
}

perfectpixel::serialization::BinarySerializer &operator<<(perfectpixel::serialization::BinarySerializer &ostream, const perfectpixel::bedrock::PpInt &num)
{
	memcpy(ostream.require(4), &num, 4);
	return ostream;
}
perfectpixel::serialization::BinarySerializer &operator>>(perfectpixel::serialization::BinarySerializer &istream, perfectpixel::bedrock::PpInt &num)
{
	memcpy(&num, istream.require(4), 4);
	return istream;
}