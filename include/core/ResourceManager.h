#pragma once

#include "core/IResourceLocator.h"

#include "bedrock/Opaque.h"
#include "bedrock/Singleton.h"
#include "bedrock/TypeReflection.h"

#include <string>
#include <vector>

namespace perfectpixel::core {

class ResourceManager : public bedrock::Singleton<ResourceManager>
{
public:
    typedef void (*ResourceLoaderFunction)(
        char *, size_t, void **, const bedrock::Opaque &, void *);
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
    typedef std::tuple<bedrock::TypeID, bedrock::TypeID, ResourceLoader>
        ResourceLoaderLookup;

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
        bedrock::Opaque m_userData;
    };

public:
    template <typename T, typename Variant = std::nullptr_t>
    static void AddLoader(
        ResourceLoaderFunction loader,
        ResourceUnloaderFunction unloader = nullptr,
        void *loaderUserData              = nullptr)
    {
        ResourceLoaderLookup entry;
        std::get<0>(entry)        = bedrock::typeID<T>();
        std::get<1>(entry)        = bedrock::typeID<Variant>();
        std::get<2>(entry).m_load = loader;
        std::get<2>(entry).m_unload
            = unloader ? unloader : ResourceLoader::GetDefaultUnloader<T>();
        std::get<2>(entry).m_loaderUserData = loaderUserData;

        getInstance()->m_loaderLUT.push_back(entry);
    }

    static void
    Take(bedrock::TypeID type, bedrock::ID id, uint32_t *ref_cacheHint);
    static void
    Release(bedrock::TypeID type, bedrock::ID id, uint32_t *ref_cacheHint);
    static void RegisterResource(
        const std::string &locator,
        ResourceLoadingStategy loadingStrategy,
        bedrock::ID id,
        bedrock::TypeID type,
        bedrock::TypeID variant    = 0,
        bedrock::Opaque &&userData = bedrock::Opaque());
    static void *GetData(
        bedrock::TypeID type,
        bedrock::ID id,
        bool *out_cache,
        uint32_t *ref_cacheHint);
    static const bedrock::Opaque &
    GetUserData(bedrock::TypeID type, bedrock::ID id, uint32_t *ref_cacheHint);

    static void Shutdown();
    static void UnloadAll();

    void setResourceLocator(IResourceLocator *locator);
    void processUnloads();

private:
    ResourceMetadata &insert(ResourceMetadata &&metadata);
    size_t offset(bedrock::TypeID type);
    void pushOffset(bedrock::TypeID type, size_t count = 1);

    void load(ResourceMetadata &metadata);
    void unload(ResourceMetadata &metadata, bool now = false);
    ResourceMetadata &
    getMetadata(bedrock::TypeID type, bedrock::ID id, uint32_t *ref_cacheHint);
    ResourceLoader &getLoader(const ResourceMetadata &metadata);

private:
    std::vector<std::pair<bedrock::TypeID, size_t>> m_offsets;
    std::vector<ResourceMetadata> m_metadata;
    std::vector<ResourceLoaderLookup> m_loaderLUT;
    IResourceLocator *m_locator;
    std::vector<ResourceMetadata *> m_unloadQueue;
};

} // namespace perfectpixel::core
