#include "types/BitSet.h"

namespace perfectpixel {
	namespace types{


		BitSet::BitSet(std::size_t initialSize, bool initToOne /*= false*/)
			: m_bitSize(initialSize)
		{
			m_data = std::vector<uint8_t>((m_bitSize + 7) / 8, initToOne ? 0xff : 0x00);
			if (initToOne)
			{
				trim();
			}
		}

		BitSet::BitSet()
			: BitSet(0)
		{

		}

		void BitSet::resize(size_t size, bool val /*= false*/)
		{
			m_data.resize((size + 7) / 8, val ? 0xff : 0x00);
			m_bitSize = size;
			trim();
		}

		bool BitSet::get(uint32_t index) const
		{
			return (m_data[index / 8] & (1 << (index % 8))) != 0;
		}

		void BitSet::set(uint32_t index, bool val)
		{
			if (val)
			{
				m_data[index / 8] |= (1 << (index % 8));
			}
			else
			{
				m_data[index / 8] &= ~(1 << (index % 8));
			}
		}

		BitSet::BitReference BitSet::operator[](uint32_t index)
		{
			if (index >= m_bitSize)
			{
				throw "Index out of bounds";
			}

			return BitReference(m_data[index / 8], index % 8);
		}

		BitSet & BitSet::operator&=(const BitSet &other)
		{
			for (uint32_t i = 0; i < m_data.size(); ++i)
			{
				if (i < other.m_data.size())
				{
					m_data[i] &= other.m_data[i];
				}
				else
				{
					m_data[i] = 0u;
				}
			}

			return *this;
		}

		BitSet BitSet::operator&(const BitSet &other) const
		{
			BitSet result = *this;
			result &= other;
			return result;
		}

		BitSet & BitSet::negate()
		{
			for (uint32_t i = 0; i < m_data.size(); ++i)
			{
				m_data[i] = ~m_data[i];
			}

			trim(); // Unincluded bits may have been set to 1

			return *this;
		}

		BitSet BitSet::operator~() const
		{
			BitSet result = *this;
			result.negate();
			return result;
		}

		void BitSet::trim()
		{
			if (!m_data.empty())
			{
				m_data[m_data.size() - 1] &= (uint8_t)((1 << (m_bitSize % 8)) - 1);
			}
		}

		BitSet::BitReference::BitReference(uint8_t &byteRef, uint8_t bitIndex)
			: m_byteRef(byteRef)
			, m_bitIndex(bitIndex)
		{}

		BitSet::BitReference::operator bool() const
		{
			return (m_byteRef & (1 << m_bitIndex)) > 0;
		}

		BitSet::BitReference & BitSet::BitReference::operator=(bool val)
		{
			if (val)
			{
				m_byteRef |= 1 << m_bitIndex;
			}
			else
			{
				m_byteRef &= ~(1 << m_bitIndex);
			}

			return *this;
		}

	}
}