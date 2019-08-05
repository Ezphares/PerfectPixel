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

		virtual void loadBuffer(const char *buffer, size_t bufferSize) override;
		virtual void loadBuffer(const std::string &buffer) override;

		virtual void writeFloat(float val) override;
		virtual void writeInt32(int32_t val) override;
		virtual void writeUInt32(uint32_t val) override;
		virtual void writeMappedUInt32(uint32_t val) override;
		virtual void writeText(const std::string &val) override;
		virtual void writeIdentifier(int32_t val) override;
		virtual void writeMapKey(int32_t val) override;
		virtual void writeIdentifier(const std::string &val) override;
		virtual void writeMapKey(const std::string &val) override;
		virtual void writeBinary(const void *p, uint32_t size) override;
		virtual void writeArrayStart(bool simple) override;
		virtual void writeArrayEnd() override;
		virtual void writeMapStart() override;
		virtual void writeMapEnd() override;
		virtual void writeNull() override;


		virtual void readFloat(float *val) override;
		virtual void readInt32(int32_t *val) override;
		virtual void readUInt32(uint32_t *val) override;
		virtual void readMappedUInt32(uint32_t *val) override;
		virtual void readText(std::string *val) override;
		virtual void readIdentifier(int32_t *val) override;
#if PP_FULL_REFLECTION_ENABLED
		virtual void readIdentifier(std::string *val) override;
#endif // PP_FULL_REFLECTION_ENABLED
		virtual uint32_t readBinary(void *p, uint32_t maxSize) override;
		virtual void readMapBegin() override;
		virtual bool readMapKey(int32_t *val) override;
		virtual uint32_t readArrayStart() override;
		virtual void readArrayEnd() override;
		virtual bool isValueNull() override;
		virtual void mapUInt32(uint32_t memory, uint32_t serialized) override;

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
		YAML::Node m_root;
		std::stack<std::pair<const YAML::Node, YAML::const_iterator*>> m_stack;
	};

} }
