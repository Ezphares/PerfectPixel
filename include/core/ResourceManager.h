#pragma once

#include "ResourceKeys.h"

#include "core/IResourceLocator.h"

#include "bedrock/Singleton.h"
#include "bedrock/TypeReflection.h"
#include "bedrock/UniqueVoidPtr.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace perfectpixel::serialization {
class ISerializer;
}

namespace perfectpixel::core {

class ResourceManager : public bedrock::Singleton<ResourceManager>
{
public:
    typedef void (*ResourceLoaderFunction)(
        char *, size_t, void **, const bedrock::UniqueVoidPtr &, void *);
    typedef void (*ResourceUnloaderFunction)(void **);
    struct ResourceLoader
    {
        ResourceLoaderFunction m_load;
        ResourceUnloaderFunction m_unload;
        void *m_loaderUserData;

        template <typename T>
        static ResourceUnloaderFunction GetDefaultUnloader()
        {
            return [](void **data) {
                delete reinterpret_cast<T *>(*data);
                *data = nullptr;
            };
        }
    };

    typedef void (*UserdataSerializerFunction)(
        const bedrock::UniqueVoidPtr &, serialization::ISerializer &, void *);
    typedef void (*UserdataDeserializerFunction)(
        bedrock::UniqueVoidPtr *, serialization::ISerializer &, void *);
    struct UserdataSerializer
    {
        UserdataSerializerFunction serialize;
        UserdataDeserializerFunction deserialize;
        void *serializerUserData;
    };

    enum ResourceLoadingStategy : uint8_t
    {
        RLS_NONE,
        RLS_AUTO_REF,
        RLS_AUTO_USE,
        RLS_MANUAL,
    };

private:
    struct ResourceMetadata
    {
        bedrock::ID m_id;
        bedrock::TypeID m_type;
        bedrock::TypeID m_variant;
        int32_t m_refs;
        void *m_data;
        ResourceLoadingStategy m_loadingStrategy;
        bedrock::UniqueVoidPtr m_userData;
    };

public:
    template <typename T, typename Variant = std::nullptr_t>
    static void AddLoader(
        ResourceLoaderFunction loader,
        ResourceUnloaderFunction unloader = nullptr,
        void *loaderUserData              = nullptr)
    {
        getInstance()->m_loaders.emplace(
            ResourceLoaderKey{bedrock::typeID<T>(), bedrock::typeID<Variant>()},
            ResourceLoader{
                loader,
                unloader ? unloader : ResourceLoader::GetDefaultUnloader<T>(),
                loaderUserData});
    }

    template <typename T, typename Variant = std::nullptr_t>
    static void addUserDataSerizlizer(
        UserdataSerializerFunction serializer,
        UserdataDeserializerFunction deserializer,
        void *serializerUserData = nullptr)
    {
        getInstance()->m_userdataSerializers.emplace(
            ResourceLoaderKey{bedrock::typeID<T>(), bedrock::typeID<Variant>()},
            UserdataSerializer{serializer, deserializer, serializerUserData});
    }

    static void Take(bedrock::TypeID type, bedrock::ID id);
    static void Release(bedrock::TypeID type, bedrock::ID id);
    static void RegisterResource(
        const std::string &locator,
        ResourceLoadingStategy loadingStrategy,
        bedrock::ID id,
        bedrock::TypeID type,
        bedrock::TypeID variant           = 0,
        bedrock::UniqueVoidPtr &&userData = bedrock::UniqueVoidPtr());
    static void *GetData(bedrock::TypeID type, bedrock::ID id, bool *out_cache);
    static const bedrock::UniqueVoidPtr &
    GetUserData(bedrock::TypeID type, bedrock::ID id);

    static void Shutdown();
    static void UnloadAll();

    void setResourceLocator(IResourceLocator *locator);
    void processUnloads();

private:
    ResourceMetadata &insert(ResourceMetadata &&metadata);

    void load(ResourceMetadata &metadata);
    void unload(ResourceMetadata &metadata, bool now = false);
    ResourceMetadata &getMetadata(bedrock::TypeID type, bedrock::ID id);
    ResourceLoader &getLoader(const ResourceMetadata &metadata);

private:
    std::unordered_map<ResourceKey, ResourceMetadata> m_metadata;
    std::unordered_map<ResourceLoaderKey, ResourceLoader> m_loaders;
    std::unordered_map<ResourceLoaderKey, UserdataSerializer>
        m_userdataSerializers;
    IResourceLocator *m_locator;
    std::vector<ResourceMetadata *> m_unloadQueue;
};

} // namespace perfectpixel::core
