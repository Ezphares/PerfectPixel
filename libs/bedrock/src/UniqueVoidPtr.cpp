#include "bedrock/UniqueVoidPtr.h"

namespace perfectpixel { namespace bedrock {

UniqueVoidPtr::~UniqueVoidPtr()
{
    destroy();
}

UniqueVoidPtr::UniqueVoidPtr()
    : m_data(nullptr)
    , m_deleter(nullptr)
    , m_inlineData{0}
{}

UniqueVoidPtr::UniqueVoidPtr(void *data, void (*deleter)(void *))
    : m_data(data)
    , m_deleter(deleter)
    , m_inlineData{0}
{}

UniqueVoidPtr::UniqueVoidPtr(UniqueVoidPtr &&rhs)
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

perfectpixel::bedrock::UniqueVoidPtr &
UniqueVoidPtr::operator=(UniqueVoidPtr &&rhs)
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

UniqueVoidPtr::operator bool() const
{
    return m_data != nullptr;
}

void UniqueVoidPtr::destroy()
{
    if (m_deleter)
    {
        m_deleter(m_data);
    }
}

}} // namespace perfectpixel::bedrock
