#include "core/Resource.h"

#include "core/ResourceManager.h"

#include "ecs/ReflectionTable.h"

#include "bedrock/Hash.h"
#include "bedrock/defines.h"

namespace perfectpixel::core {

Resource::Resource(bedrock::TypeID type)
    : m_type(type)
    , m_valid(false)
{}

Resource::Resource(bedrock::TypeID type, const std::string &ID)
    : Resource(type)
{
    set(ID);
}

Resource::Resource(bedrock::TypeID type, bedrock::ID ID)
    : Resource(type)
{
    set(ID);
}

Resource::Resource(const Resource &toCopy)
    : m_type(toCopy.m_type)
    , m_valid(false)
{
    set();
    if (toCopy.isValid())
    {
        set(toCopy.getId());
    }
}

Resource::Resource(Resource &&toMove)
    : m_type(toMove.m_type)
    , m_valid(false)
{
    set();
    if (toMove.isValid())
    {
        set(toMove.getId());
    }
    toMove.set();
}

Resource::~Resource()
{
    set();
}

Resource &Resource::operator=(const Resource &toCopy)
{
    set();
    if (toCopy.isValid())
    {
        set(toCopy.getId());
    }
    return *this;
}

Resource &Resource::operator=(Resource &&toMove)
{
    set();
    if (toMove.isValid())
    {
        set(toMove.getId());
    }
    toMove.set();
    return *this;
}

bool Resource::isValid() const
{
    return m_valid;
}

perfectpixel::bedrock::ID Resource::getId() const
{
    return m_id;
}

void Resource::set()
{
    m_cache     = nullptr;
    m_cacheHint = ~0u;

    if (m_valid)
    {
        ResourceManager::Release(m_type, m_id, &m_cacheHint);
    }
    m_valid = false;
}

void Resource::set(const std::string &id)
{
    ecs::ReflectionTable::Touch(id);

    set(bedrock::ID(bedrock::crc32(id)));
}

void Resource::set(bedrock::ID id)
{
    set();

    ResourceManager::Take(m_type, id, &m_cacheHint);
    m_valid = true;
    m_id    = id;
}

void *Resource::_get()
{
    if (!m_valid)
    {
        return nullptr;
    }
    if (m_cache)
    {
        return m_cache;
    }
    bool shouldCache;
    void *result
        = ResourceManager::GetData(m_type, m_id, &shouldCache, &m_cacheHint);

    if (result && shouldCache)
    {
        m_cache = result;
    }
    return result;
}

const bedrock::Opaque &Resource::getUserData() const
{
    return ResourceManager::GetUserData(m_type, m_id, &m_cacheHint);
}

} // namespace perfectpixel::core

namespace perfectpixel { namespace serialization {
ISerializer &operator<<(
    ISerializer &serializer, const perfectpixel::core::Resource &resource)
{
    if (resource.isValid())
    {
        serializer.writeIdentifier(resource.getId().m_hash);
    }
    else
    {
        serializer.writeNull();
    }

    return serializer;
}

ISerializer &
operator>>(ISerializer &serializer, perfectpixel::core::Resource &resource)
{
    resource.set();

    if (!serializer.isValueNull())
    {
        int32_t id;
        serializer.readIdentifier(&id);
        resource.set(bedrock::ID{id});
    }

    return serializer;
}
}} // namespace perfectpixel::serialization
