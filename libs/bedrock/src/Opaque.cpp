#include "bedrock/Opaque.h"

namespace perfectpixel { namespace bedrock {

Opaque::~Opaque()
{
    destroy();
}

Opaque::Opaque()
    : m_data(nullptr)
    , m_deleter(nullptr)
{}

Opaque::Opaque(void *data, void (*deleter)(void *))
    : m_data(data)
    , m_deleter(deleter)
{}

Opaque::Opaque(Opaque &&rhs)
    : m_data(rhs.m_data)
    , m_deleter(rhs.m_deleter)
{
    rhs.m_data    = nullptr;
    rhs.m_deleter = nullptr;
}

perfectpixel::bedrock::Opaque &Opaque::operator=(Opaque &&rhs)
{
    if (this != &rhs)
    {
        destroy();
        m_data        = rhs.m_data;
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
