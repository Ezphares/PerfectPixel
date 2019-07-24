#pragma once

#include <serialization/ISerializer.h>

#include <yaml-cpp/emitter.h>
#include <yaml-cpp/node/iterator.h>

#include <stack>

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
		virtual void writeIdentifier(int32_t val);
		virtual void writeMapKey(int32_t val);
		virtual void writeIdentifier(const std::string &val);
		virtual void writeMapKey(const std::string &val);
		virtual void writeBinary(const void *p, uint32_t size);
		virtual void writeArrayStart(bool simple);
		virtual void writeArrayEnd();
		virtual void writeMapStart();
		virtual void writeMapEnd();
		virtual void writeNull();


		virtual void readFloat(float *val);
		virtual void readInt32(int32_t *val);
		virtual void readUInt32(uint32_t *val);
		virtual void readMappedUInt32(uint32_t *val);
		virtual void readText(std::string *val);
		virtual void readIdentifier(int32_t *val);
#if PP_FULL_REFLECTION_ENABLED
		virtual void readIdentifier(std::string *val);
#endif // PP_FULL_REFLECTION_ENABLED
		virtual uint32_t readBinary(void *p, uint32_t maxSize);
		virtual void readMapBegin();
		virtual bool readMapKey(int32_t *val);
		virtual uint32_t readArrayStart();
		virtual void readArrayEnd();
		virtual bool isValueNull();
		virtual void mapUInt32(uint32_t memory, uint32_t serialized);

		virtual void load(const char *data);

		void push(const YAML::Node &node);
		void pop();
		const YAML::Node readVal();

		std::string dump();

		int32_t(*m_hash)(const std::string &);
#if PP_FULL_REFLECTION_ENABLED
		std::string(*m_reverse)(int32_t);
#endif

	private:
		YAML::Emitter *m_emitter;
		YAML::Node *m_root;
		std::stack<std::pair<const YAML::Node, YAML::const_iterator*>> m_stack;
	};

} }
