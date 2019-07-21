#pragma once

#include <serialization/ISerializer.h>
#include <yaml-cpp/emitter.h>

#if PP_FULL_REFLECTION_ENABLED

namespace perfectpixel { namespace serialization {

	class YAMLSerializer : public ISerializer
	{
	public:
		YAMLSerializer();

		virtual void writeFloat(float val);
		virtual void writeInt32(int32_t val);
		virtual void writeUInt32(uint32_t val);
		virtual void writeMappedUInt32(uint32_t val);
		virtual void writeText(const std::string &val);
		virtual void writeName(int32_t val);
		virtual void writeMapKey(int32_t val);
		virtual void writeName(const std::string &val);
		virtual void writeMapKey(const std::string &val);
		virtual void writeBinary(const void *p, uint32_t size);
		virtual void writeArrayStart(bool simple);
		virtual void writeArrayEnd();
		virtual void writeMapStart();
		virtual void writeMapEnd();

		virtual void readFloat(float *val);
		virtual void readInt32(int32_t *val);
		virtual void readUInt32(uint32_t *val);
		virtual void readMappedUInt32(uint32_t *val);
		virtual void readText(std::string *val);
		virtual void readName(int32_t &val);
#if PP_FULL_REFLECTION_ENABLED
		virtual void readName(std::string *val);
#endif // PP_FULL_REFLECTION_ENABLED
		virtual uint32_t readBinary(void *p, uint32_t maxSize);
		virtual bool readMapKey(int32_t *val);
		virtual uint32_t readArrayStart();
		virtual void readArrayEnd();
		virtual void mapUInt32(uint32_t memory, uint32_t serialized);

		std::string dump();

	private:
		YAML::Emitter *m_emitter;
	};

} }

#endif PP_FULL_REFLECTION_ENABLED