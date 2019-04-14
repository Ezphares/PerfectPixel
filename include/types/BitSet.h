#pragma once

#include <vector>

namespace perfectpixel {
	namespace types {

		class BitSet
		{
		public:
			class BitReference
			{
			public:
				BitReference(uint8_t &byteRef, uint8_t bitIndex);

				BitReference &operator=(bool val);
				operator bool() const;

			private:
				uint8_t &m_byteRef;
				uint8_t m_bitIndex;
			};

		public:
			BitSet();
			BitSet(std::size_t initialSize, bool initToOne = false);
			~BitSet() {}

		public:
			inline std::size_t size() const { return m_bitSize; } 
			inline std::size_t capacity() const { return m_data.capacity() * 8u; }
			inline void resize(size_t size, bool val = false) { m_data.resize((size + 7) / 8, val ? 0xff : 0x00); }

			void append(bool val);
			bool get(uint32_t index) const;
			void set(uint32_t index, bool val);
			BitReference operator[](uint32_t index);
		
			BitSet &operator&=(const BitSet &other);
			BitSet operator&(const BitSet &other) const;

			BitSet &negate();
			BitSet operator~() const;

		private:
			void trim();

		private:
			std::vector<uint8_t> m_data;
			std::size_t m_bitSize;
		};

	}
}
