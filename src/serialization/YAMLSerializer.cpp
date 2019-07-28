#include <serialization/YAMLSerializer.h>

#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>

#if PP_FULL_REFLECTION_ENABLED

namespace perfectpixel { namespace serialization {

	YAMLSerializer::YAMLSerializer()
		: m_emitter(new YAML::Emitter())
		, m_hash()
		, m_reverse()
	{
	}

	void YAMLSerializer::loadBuffer(const char *buffer, size_t bufferSize)
	{
		loadBuffer(std::string(buffer, bufferSize));
	}

	void YAMLSerializer::loadBuffer(const std::string &buffer)
	{
		m_root = YAML::Load(buffer);
		while (!m_stack.empty())
		{
			m_stack.pop();
		}
		push(m_root);
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

	void YAMLSerializer::writeIdentifier(int32_t val)
	{
#if PP_FULL_REFLECTION_ENABLED
		if (m_reverse)
		{
			std::string text = m_reverse(val);
			if (text != "")
			{
				writeIdentifier(m_reverse(val));
				return;
			}
		}
#endif
		*m_emitter << val;
	}

	void YAMLSerializer::writeIdentifier(const std::string &val)
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

	void YAMLSerializer::writeNull()
	{
		*m_emitter << YAML::Null;
	}

	void YAMLSerializer::push(const YAML::Node &node)
	{
		m_stack.emplace(node, nullptr);
	}

	void YAMLSerializer::pop()
	{
		delete m_stack.top().second;
		m_stack.pop();
		if (m_stack.empty())
		{
			m_root = YAML::Node();
		}
	}

	const YAML::Node YAMLSerializer::readVal()
	{
		if (m_stack.top().first.IsMap())
		{
			return (*m_stack.top().second)->second;
		}
		else if (m_stack.top().first.IsSequence())
		{
			YAML::const_iterator &it = *m_stack.top().second;
			return *(it++);
		}
		else
		{
			return m_stack.top().first;
		}
	}

	void YAMLSerializer::readFloat(float *val)
	{
		*val = readVal().as<float>();
	}

	void YAMLSerializer::readInt32(int32_t *val)
	{
		*val = readVal().as<int32_t>();
	}

	void YAMLSerializer::readUInt32(uint32_t *val)
	{
		*val = readVal().as<uint32_t>();
	}

	void YAMLSerializer::readMappedUInt32(uint32_t *val)
	{
		throw "Todo";
	}

	void YAMLSerializer::readText(std::string *val)
	{
		*val = readVal().as<std::string>();
	}

	void YAMLSerializer::readIdentifier(int32_t *val)
	{
		const YAML::Node node = readVal();

		try
		{
			*val = node.as<int32_t>();
		}
		catch (YAML::BadConversion &)
		{
			*val = m_hash(node.as<std::string>());
		}
	}

	void YAMLSerializer::readIdentifier(std::string *val)
	{
		readText(val);
	}

	uint32_t YAMLSerializer::readBinary(void *p, uint32_t maxSize)
	{
		std::string text = readVal().as<std::string>();
		memcpy(p, text.c_str(), std::min(maxSize, text.size()));
		return std::min(maxSize, text.size());
	}

	void YAMLSerializer::readMapBegin()
	{
		const YAML::Node node = readVal();

		if (!node.IsMap())
		{
			throw "YAML Decoding error";
		}

		push(node);
	}

	bool YAMLSerializer::readMapKey(int32_t *val)
	{
		if (!m_stack.top().first.IsMap())
		{
			throw "YAML Decoding error";
		}

		if (m_stack.top().second)
		{
			++(*m_stack.top().second);
		}
		else
		{
			m_stack.top().second = new YAML::const_iterator(m_stack.top().first.begin());
		}

		YAML::const_iterator &it = *m_stack.top().second;
		if (it == m_stack.top().first.end())
		{
			pop();
			return false;
		}

		try
		{
			*val = it->first.as<int32_t>();
		}
		catch (YAML::BadConversion &)
		{
			*val = m_hash(it->first.as<std::string>());
		}
		return true;
	}

	uint32_t YAMLSerializer::readArrayStart()
	{
		const YAML::Node node = readVal();
		if (!node.IsSequence())
		{
			throw "YAML decode error";
		}

		push(node);
		m_stack.top().second = new YAML::const_iterator(node.begin());
		return node.size();
	}

	void YAMLSerializer::readArrayEnd()
	{
		if (!m_stack.top().first.IsSequence())
		{
			throw "YAML decode error";
		}

		pop();
	}

	bool YAMLSerializer::isValueNull()
	{
		return readVal().IsNull();
	}

	void YAMLSerializer::mapUInt32(uint32_t memory, uint32_t serialized)
	{
		throw "Todo";
	}

	std::string YAMLSerializer::dump()
	{
		std::string result;
		result.assign(m_emitter->c_str());
		return result;
	}

}}

#endif // PP_FULL_REFLECTION_ENABLED