#include <Bedrock/BitSet.h>

#include <cstring>

namespace perfectpixel { namespace bedrock {

BitSet::BitSet(std::size_t initialSize, bool initToOne /*= false*/)
    : m_bitSize(initialSize)
    , m_negated(false)
{
    m_data = std::vector<uint8_t>((m_bitSize + 7) / 8, initToOne ? 0xff : 0x00);
    if (initToOne)
    {
        trim();
    }
}

BitSet::BitSet()
    : BitSet(0)
{}

BitSet::BitSet(const uint8_t *raw, size_t bits)
    : m_data()
    , m_bitSize(bits)
    , m_negated(false)
{
    m_data.resize((bits + 7) / 8);
    memcpy(m_data.data(), raw, m_data.size());
    trim();
}

void BitSet::resize(size_t size, bool val /*= false*/)
{
    m_data.resize((size + 7) / 8, val ? 0xff : 0x00);
    m_bitSize = size;
    trim();
}

bool BitSet::get(size_t index) const
{
    return ((m_data[index / 8] & (1 << (index % 8))) != 0) ^ m_negated;
}

void BitSet::set(size_t index, bool val)
{
    val ^= m_negated;
    if (val)
    {
        m_data[index / 8] |= (1 << (index % 8));
    }
    else
    {
        m_data[index / 8] &= ~(1 << (index % 8));
    }
}

BitSet::BitReference BitSet::operator[](size_t index)
{
    if (index >= m_bitSize)
    {
        throw "Index out of bounds";
    }

    executeNegation();

    return BitReference(&m_data[index / 8], index % 8);
}

const BitSet::BitReference BitSet::operator[](size_t index) const
{
    if (index >= m_bitSize)
    {
        throw "Index out of bounds";
    }

    return BitReference(
        ((m_data[index / 8] & (1 << (index % 8))) > 0) ^ m_negated);
}

BitSet &BitSet::operator&=(const BitSet &other)
{
    executeNegation();
    if (other.m_bitSize > m_bitSize)
    {
        resize(other.m_bitSize);
    }

    for (uint32_t i = 0; i < m_data.size(); ++i)
    {
        if (i < other.m_data.size())
        {
            m_data[i] &= other.m_data[i] ^ (other.m_negated ? 0xff : 0x00);
        }
        else
        {
            m_data[i] &= other.m_negated ? 0xff : 0x00;
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

BitSet &BitSet::operator|=(const BitSet &other)
{
    executeNegation();
    if (other.m_bitSize > m_bitSize)
    {
        resize(other.m_bitSize);
    }
    for (uint32_t i = 0; i < other.m_data.size(); ++i)
    {
        m_data[i] |= other.m_data[i] ^ (other.m_negated ? 0xff : 0x00);
    }
    return *this;
}

BitSet BitSet::operator|(const BitSet &other) const
{
    BitSet result = *this;
    result |= other;
    return result;
}

BitSet &BitSet::negate()
{
    m_negated = !m_negated;

    return *this;
}

BitSet BitSet::operator~() const
{
    BitSet result = *this;
    result.negate();
    return result;
}

bool BitSet::operator==(const BitSet &other) const
{
    if (m_bitSize != other.m_bitSize)
    {
        return false;
    }

    if (m_negated == other.m_negated)
    {
        return m_data == other.m_data;
    }

    for (uint32_t i = 0; i < m_data.size(); ++i)
    {
        if ((~m_data[i]) != other.m_data[i])
        {
            return false;
        }
    }

    return true;
}

void BitSet::executeNegation()
{
    if (m_negated)
    {
        for (uint32_t i = 0; i < m_data.size(); ++i)
        {
            m_data[i] = ~m_data[i];
        }

        trim(); // Unincluded bits may have been set to 1
    }
    m_negated = false;
}

void BitSet::trim()
{
    if (!m_data.empty())
    {
        uint32_t mod = m_bitSize % 8;
        uint8_t mask = mod == 0 ? 0xff : (1 << mod) - 1;
        m_data[m_data.size() - 1] &= mask;
    }
}

BitSet::BitReference::BitReference(uint8_t *byteRef, uint8_t bitIndex)
    : m_byteRef(byteRef)
    , m_bitIndex(bitIndex)
    , m_cval(false)
{}

BitSet::BitReference::BitReference(bool cval)
    : m_byteRef(nullptr)
    , m_bitIndex(9)
    , m_cval(cval)
{}

BitSet::BitReference::operator bool() const
{
    return m_byteRef ? ((*m_byteRef) & (1 << m_bitIndex)) > 0 : m_cval;
}

BitSet::BitReference &BitSet::BitReference::operator=(bool val)
{
    if (val)
    {
        (*m_byteRef) |= 1 << m_bitIndex;
    }
    else
    {
        (*m_byteRef) &= ~(1 << m_bitIndex);
    }

    return *this;
}

}} // namespace perfectpixel::bedrock
