#include <serialization/ISerializer.h>

namespace perfectpixel { namespace serialization {

ISerializer & operator<<(ISerializer &ostream, const std::string &str)
{
	ostream.writeText(str);
	return ostream;
}

ISerializer & operator<<(ISerializer &ostream, const uint32_t &uint)
{
	ostream.writeUInt32(uint);
	return ostream;
}

ISerializer & operator<<(ISerializer &ostream, const float &num)
{
	ostream.writeFloat(num);
	return ostream;
}

ISerializer & operator<<(ISerializer &ostream, const int32_t &num)
{
	ostream.writeInt32(num);
	return ostream;
}

ISerializer & operator>>(ISerializer &istream, std::string &str)
{
	istream.readText(&str);
	return istream;
}

ISerializer & operator>>(ISerializer &istream, uint32_t &uint)
{
	istream.readUInt32(&uint);
	return istream;
}

ISerializer & operator>>(ISerializer &istream, float &num)
{
	istream.readFloat(&num);
	return istream;
}

ISerializer & operator>>(ISerializer &istream, int32_t &num)
{
	istream.readInt32(&num);
	return istream;
}

}}
