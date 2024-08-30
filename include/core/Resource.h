#pragma once

#include "serialization/ISerializer.h"

#include "bedrock/TypeReflection.h"
#include "bedrock/UniqueVoidPtr.h"

#include <string>

namespace perfectpixel::core {

class Resource
{
public:
    Resource(bedrock::TypeID type);
    Resource(bedrock::TypeID type, const std::string &id);
    Resource(bedrock::TypeID type, bedrock::ID id);

    ~Resource();
    Resource(const Resource &toCopy);
    Resource(Resource &&toMove);
    Resource &operator=(const Resource &toCopy);
    Resource &operator=(Resource &&toMove);

    bool isValid() const;
    bedrock::ID getId() const;

    void set(const std::string &id);
    void set(bedrock::ID id);
    void set();

    void *_get();
    template <typename T>
    T *get()
    {
        return reinterpret_cast<T *>(_get());
    }

    const bedrock::UniqueVoidPtr &getUserData() const;

private:
    const bedrock::TypeID m_type;
    bedrock::ID m_id;
    void *m_cache;
    bool m_valid;
};
} // namespace perfectpixel::core

PP_TYPE_REFLECTION(::perfectpixel::core::Resource, Resource);

namespace perfectpixel { namespace serialization {
ISerializer &operator<<(
    ISerializer &serializer, const perfectpixel::core::Resource &resource);
ISerializer &
operator>>(ISerializer &serializer, perfectpixel::core::Resource &resource);
}} // namespace perfectpixel::serialization
