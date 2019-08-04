
#include <serialization/BinarySerializer.h>

#include <numeric>
#include <cstring>

namespace perfectpixel { namespace serialization {

	namespace {
		const unsigned char ARRAY_START = 0xf0;
		const unsigned char ARRAY_END = 0xf1;
		const unsigned char MAP_START = 0xf2;
		const unsigned char MAP_END = 0xf3;
	}

	void BinarySerializer::writeText(const std::string &val)
	{
		uint32_t size = val.size();
		memcpy(require(4), &size, 4);
		memcpy(require(size), val.data(), size);
	}

	void BinarySerializer::writeUInt32(uint32_t val)
	{
		memcpy(require(4), &val, 4);
	}

	void BinarySerializer::readText(std::string *val)
	{
		uint32_t size;
		memcpy(&size, get(4), 4);
		val->assign(get(size), size);
	}

	void BinarySerializer::readUInt32(uint32_t *val)
	{
		memcpy(&val, get(4), 4);
	}

char * BinarySerializer::require(size_t bytes)
{
	m_buffer.reserve(m_dataPointer + bytes);
	char *loc = m_buffer.data() + m_dataPointer;
	m_dataPointer += bytes;
	return loc;


}

char * BinarySerializer::get(size_t bytes)
{
	if (m_buffer.size() < m_dataPointer + bytes)
	{
		throw "Deserialization error";
	}
	char *loc = m_buffer.data() + m_dataPointer;
	m_dataPointer += bytes;
	return loc;
}

}
}
