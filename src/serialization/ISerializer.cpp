#include <serialization/ISerializer.h>

perfectpixel::serialization::ISerializer & operator<<(perfectpixel::serialization::ISerializer &ostream, const std::string &str)
{
	ostream.writeText(str);
	return ostream;
}

perfectpixel::serialization::ISerializer & operator<<(perfectpixel::serialization::ISerializer &ostream, const uint32_t &uint)
{
	ostream.writeUInt32(uint);
	return ostream;
}

perfectpixel::serialization::ISerializer & operator<<(perfectpixel::serialization::ISerializer &ostream, const float &num)
{
	ostream.writeFloat(num);
	return ostream;
}

perfectpixel::serialization::ISerializer & operator<<(perfectpixel::serialization::ISerializer &ostream, const int32_t &num)
{
	ostream.writeInt32(num);
	return ostream;
}

perfectpixel::serialization::ISerializer & operator>>(perfectpixel::serialization::ISerializer &istream, std::string &str)
{
	istream.readText(&str);
	return istream;
}

perfectpixel::serialization::ISerializer & operator>>(perfectpixel::serialization::ISerializer &istream, uint32_t &uint)
{
	istream.readUInt32(&uint);
	return istream;
}

perfectpixel::serialization::ISerializer & operator>>(perfectpixel::serialization::ISerializer &istream, float &num)
{
	istream.readFloat(&num);
	return istream;
}

perfectpixel::serialization::ISerializer & operator>>(perfectpixel::serialization::ISerializer &istream, int32_t &num)
{
	istream.readInt32(&num);
	return istream;
}
