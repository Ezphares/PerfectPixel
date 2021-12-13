#pragma once

#include <Resources/IResourceLocator.h>

#include <Bedrock/Opaque.h>
#include <Bedrock/Singleton.h>
#include <Bedrock/TypeReflection.h>

#include <string>
#include <vector>

namespace perfectpixel { namespace resources {

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
    typedef std::tuple<int32_t, int32_t, ResourceLoader> ResourceLoaderLookup;

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
        int32_t m_id;
        int32_t m_type;
        int32_t m_variant;
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

    static void Take(int32_t type, int32_t id, uint32_t *ref_cacheHint);
    static void Release(int32_t type, int32_t id, uint32_t *ref_cacheHint);
    static void RegisterResource(
        const std::string &locator,
        ResourceLoadingStategy loadingStrategy,
        int32_t id,
        int32_t type,
        int32_t variant                 = 0,
        const bedrock::Opaque &userData = bedrock::Opaque());
    static void *
    GetData(int32_t type, int32_t id, bool *out_cache, uint32_t *ref_cacheHint);

    static void Shutdown();
    static void UnloadAll();

    void setResourceLocator(IResourceLocator *locator);
    void processUnloads();

private:
    void insert(const ResourceMetadata &metadata);
    size_t offset(int32_t type);
    void pushOffset(int32_t type, size_t count = 1);

    void load(ResourceMetadata &metadata);
    void unload(ResourceMetadata &metadata, bool now = false);
    ResourceMetadata &
    getMetadata(int32_t type, int32_t id, uint32_t *ref_cacheHint);
    ResourceLoader &getLoader(const ResourceMetadata &metadata);

private:
    std::vector<std::pair<int32_t, size_t>> m_offsets;
    std::vector<ResourceMetadata> m_metadata;
    std::vector<ResourceLoaderLookup> m_loaderLUT;
    IResourceLocator *m_locator;
    std::vector<ResourceMetadata *> m_unloadQueue;
};

}} // namespace perfectpixel::resources
