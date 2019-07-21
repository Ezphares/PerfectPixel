#include <serialization/YAMLSerializer.h>

#if PP_FULL_REFLECTION_ENABLED

namespace perfectpixel { namespace serialization {

	YAMLSerializer::YAMLSerializer()
	{
		m_emitter = new YAML::Emitter();
	}

	void YAMLSerializer::writeFloat(float val)
	{
		*m_emitter << val;
	}

	void YAMLSerializer::writeInt32(int32_t val)
	{
		*m_emitter << val;
	}

	void YAMLSerializer::writeUInt32(uint32_t val)
	{
		*m_emitter << val;
	}

	void YAMLSerializer::writeMappedUInt32(uint32_t val)
	{
		// TODO!
		*m_emitter << val;
	}

	void YAMLSerializer::writeText(const std::string &val)
	{
		*m_emitter << val;
	}

	void YAMLSerializer::writeName(int32_t val)
	{
#if PP_FULL_REFLECTION_ENABLED
		// TODO: Reverse?
		*m_emitter << val;
#else
		*m_emitter << val;
#endif
	}

	void YAMLSerializer::writeName(const std::string &val)
	{
		*m_emitter << val;
	}

	void YAMLSerializer::writeMapKey(int32_t val)
	{
		*m_emitter << YAML::Key << val << YAML::Value;
	}

	void YAMLSerializer::writeMapKey(const std::string &val)
	{
		*m_emitter << YAML::Key << val << YAML::Value;
	}

	void YAMLSerializer::writeBinary(const void *p, uint32_t size)
	{
		std::string b;
		b.assign((const char*)p, size);
		*m_emitter << b;
	}

	void YAMLSerializer::writeArrayStart(bool simple)
	{
		if (simple)
		{
			*m_emitter << YAML::Flow;
		}
		*m_emitter << YAML::BeginSeq;
	}

	void YAMLSerializer::writeArrayEnd()
	{
		*m_emitter << YAML::EndSeq;
	}

	void YAMLSerializer::writeMapStart()
	{
		*m_emitter << YAML::BeginMap;
	}

	void YAMLSerializer::writeMapEnd()
	{
		*m_emitter << YAML::EndMap;
	}

	void YAMLSerializer::readFloat(float *val)
	{
		throw "Todo";
	}

	void YAMLSerializer::readInt32(int32_t *val)
	{
		throw "Todo";
	}

	void YAMLSerializer::readUInt32(uint32_t *val)
	{
		throw "Todo";
	}

	void YAMLSerializer::readMappedUInt32(uint32_t *val)
	{
		throw "Todo";
	}

	void YAMLSerializer::readText(std::string *val)
	{
		throw "Todo";
	}

	void YAMLSerializer::readName(int32_t &val)
	{
		throw "Todo";
	}

	void YAMLSerializer::readName(std::string *val)
	{
		throw "Todo";
	}

	uint32_t YAMLSerializer::readBinary(void *p, uint32_t maxSize)
	{
		throw "Todo";
	}

	bool YAMLSerializer::readMapKey(int32_t *val)
	{
		throw "Todo";
	}

	uint32_t YAMLSerializer::readArrayStart()
	{
		throw "Todo";
	}

	void YAMLSerializer::readArrayEnd()
	{
		throw "Todo";
	}

	void YAMLSerializer::mapUInt32(uint32_t memory, uint32_t serialized)
	{
		throw "Todo";
	}

	std::string YAMLSerializer::dump()
	{
		std::string r;

		r.assign(m_emitter->c_str());

		return r;
	}

}}

#endif // PP_FULL_REFLECTION_ENABLED