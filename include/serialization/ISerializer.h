#pragma once

#include <string>

namespace perfectpixel {
	namespace serialization {

		class ISerializer
		{
		public:
			virtual void writeFloat(float val);
			virtual void writeInt32(int32_t val);
			virtual void writeUInt32(uint32_t val);
			virtual void writeMappedUInt32(uint32_t val);
			virtual void writeText(const std::string &val);
			virtual void writeBinary(const void *p, uint32_t size);
			virtual void writeArrayStart();
			virtual void writeArrayEnd();
			virtual void writeMapStart();
			virtual void writeMapEnd();

			virtual void readFloat(float *val);
			virtual void readInt32(int32_t *val);
			virtual void readUInt32(uint32_t *val);
			virtual void readMappedUInt32(uint32_t *val);
			virtual void readText(std::string *val);
			virtual uint32_t readBinary(void *p, uint32_t maxSize);
			virtual bool readMapKey(int32_t *val);
			virtual uint32_t readArrayStart();
			virtual void readArrayEnd();

			virtual void mapUInt32(uint32_t memory, uint32_t serialized = 0.0f);
		};
} }

// TODO: Move these
perfectpixel::serialization::ISerializer &operator<<(perfectpixel::serialization::ISerializer &ostream, const std::string &str);
perfectpixel::serialization::ISerializer &operator>>(perfectpixel::serialization::ISerializer &istream, std::string &str);

perfectpixel::serialization::ISerializer &operator<<(perfectpixel::serialization::ISerializer &ostream, const float &num);
perfectpixel::serialization::ISerializer &operator>>(perfectpixel::serialization::ISerializer &istream, float &num);

perfectpixel::serialization::ISerializer &operator<<(perfectpixel::serialization::ISerializer &ostream, const uint32_t &uint);
perfectpixel::serialization::ISerializer &operator>>(perfectpixel::serialization::ISerializer &istream, uint32_t &uint);

perfectpixel::serialization::ISerializer &operator<<(perfectpixel::serialization::ISerializer &ostream, const int32_t &num);
perfectpixel::serialization::ISerializer &operator>>(perfectpixel::serialization::ISerializer &istream, int32_t &num);