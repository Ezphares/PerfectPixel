#include "bedrock/Opaque.h"

namespace perfectpixel { namespace bedrock {

Opaque::~Opaque()
{
    destroy();
}

Opaque::Opaque()
    : m_data(nullptr)
    , m_deleter(nullptr)
    , m_inlineData{0}
{}

Opaque::Opaque(void *data, void (*deleter)(void *))
    : m_data(data)
    , m_deleter(deleter)
    , m_inlineData{0}
{}

Opaque::Opaque(Opaque &&rhs)
    : m_data(rhs.m_data)
    , m_deleter(rhs.m_deleter)
{
    if (rhs.m_data == rhs.m_inlineData)
    {
        memcpy(m_inlineData, rhs.m_inlineData, opaqueInlineBytes);
        m_data = m_inlineData;
    }
    rhs.m_data    = nullptr;
    rhs.m_deleter = nullptr;
}

perfectpixel::bedrock::Opaque &Opaque::operator=(Opaque &&rhs)
{
    if (this != &rhs)
    {
        destroy();
        if (rhs.m_data == rhs.m_inlineData)
        {
            memcpy(m_inlineData, rhs.m_inlineData, opaqueInlineBytes);
            m_data = m_inlineData;
        }
        else
        {
            m_data = rhs.m_data;
        }
        m_deleter     = rhs.m_deleter;
        rhs.m_data    = nullptr;
        rhs.m_deleter = nullptr;
    }

    return *this;
}

Opaque::operator bool() const
{
    return m_data != nullptr;
}

void Opaque::destroy()
{
    if (m_deleter)
    {
        m_deleter(m_data);
    }
}

}} // namespace perfectpixel::bedrock
