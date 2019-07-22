#pragma once

#include <serialization/ISerializer.h>

#include <yaml-cpp/emitter.h>
#include <yaml-cpp/node/iterator.h>

#include <stack>

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
		virtual void readName(int32_t *val);
#if PP_FULL_REFLECTION_ENABLED
		virtual void readName(std::string *val);
#endif // PP_FULL_REFLECTION_ENABLED
		virtual uint32_t readBinary(void *p, uint32_t maxSize);
		virtual void readMapBegin();
		virtual bool readMapKey(int32_t *val);
		virtual uint32_t readArrayStart();
		virtual void readArrayEnd();
		virtual void mapUInt32(uint32_t memory, uint32_t serialized);

		virtual void load(const char *data);

		void push(const YAML::Node &node);
		void pop();
		const YAML::Node readVal();

		std::string dump();

		int32_t(*m_hash)(const std::string &);

	private:
		enum Iterating
		{
			YAML_IT_NONE,
			YAML_IT_MAP,
			YAML_IT_ARRAY
		};

		YAML::Emitter *m_emitter;
		YAML::Node *m_root;
		std::stack<std::pair<const YAML::Node, YAML::const_iterator*>> m_stack;
		Iterating m_iterating;
	};

} }

#endif PP_FULL_REFLECTION_ENABLED