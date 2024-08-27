#pragma once

#include "serialization/ISerializer.h"

#include <vector>
#include <string>

namespace perfectpixel {
	namespace serialization {

		class BinarySerializer : public ISerializer
		{
		public:
			virtual void writeText(const std::string &val);
			virtual void writeUInt32(uint32_t val);

			virtual void readText(std::string *val);
			virtual void readUInt32(uint32_t *val);

		public:
			char *require(size_t bytes);
			char *get(size_t bytes);

		private:
			std::size_t m_dataPointer;
			std::vector<char> m_buffer;
		};
	}
}
