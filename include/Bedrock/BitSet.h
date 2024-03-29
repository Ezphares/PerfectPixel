#pragma once

#include <cstdint>
#include <vector>

namespace perfectpixel { namespace bedrock {

class BitSet
{
public:
    class BitReference
    {
    public:
        BitReference(uint8_t *byteRef, uint8_t bitIndex);
        BitReference(bool cval);

        BitReference &operator=(bool val);
        operator bool() const;

    private:
        uint8_t *m_byteRef;
        uint8_t m_bitIndex;
        bool m_cval;
    };

public:
    BitSet();
    BitSet(std::size_t initialSize, bool initToOne = false);
    BitSet(const uint8_t *raw, size_t bits);
    ~BitSet()
    {}

public:
    inline size_t size() const
    {
        return m_bitSize;
    }
    inline size_t capacity() const
    {
        return m_data.capacity() * 8u;
    }
    void resize(size_t size, bool val = false);

    void append(bool val);
    bool get(size_t index) const;
    void set(size_t index, bool val);
    BitReference operator[](size_t index);
    const BitReference operator[](size_t index) const;

    BitSet &operator&=(const BitSet &other);
    BitSet operator&(const BitSet &other) const;

    BitSet &operator|=(const BitSet &other);
    BitSet operator|(const BitSet &other) const;

    BitSet &negate();
    BitSet operator~() const;

    bool operator==(const BitSet &other) const;

    void executeNegation();

private:
    void trim();

private:
    std::vector<uint8_t> m_data;
    std::size_t m_bitSize;
    bool m_negated;
};

}} // namespace perfectpixel::bedrock
