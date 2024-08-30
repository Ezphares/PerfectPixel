#include "core/ResourceManager.h"

#include "bedrock/Assert.h"

namespace perfectpixel::core {

void ResourceManager::Take(bedrock::TypeID type, bedrock::ID id)
{
    ResourceManager *self      = getInstance();
    ResourceMetadata &metadata = self->getMetadata(type, id);
    if (metadata.m_refs == 0 && metadata.m_loadingStrategy == RLS_AUTO_REF)
    {
        self->load(metadata);
    }
    metadata.m_refs++;
}

void ResourceManager::Release(bedrock::TypeID type, bedrock::ID id)
{
    ResourceManager *self      = getInstance();
    ResourceMetadata &metadata = self->getMetadata(type, id);

    PP_ASSERT(
        metadata.m_refs > 0, "Releasing resource that is already at zero refs");

    metadata.m_refs--;

    if (metadata.m_refs == 0 && metadata.m_loadingStrategy == RLS_AUTO_REF)
    {
        self->unload(metadata);
    }
}

void ResourceManager::RegisterResource(
    const std::string &locator,
    ResourceLoadingStategy loadingStrategy,
    bedrock::ID id,
    bedrock::TypeID type,
    bedrock::TypeID variant /*= 0*/,
    bedrock::Opaque &&userData /*= bedrock::Opaque()*/)
{
    ResourceManager *self = getInstance();

    ResourceMetadata metadata{};
    metadata.m_id              = id;
    metadata.m_type            = type;
    metadata.m_variant         = variant;
    metadata.m_data            = nullptr;
    metadata.m_refs            = 0;
    metadata.m_loadingStrategy = loadingStrategy;
    metadata.m_userData        = std::move(userData);

    auto &inserted = self->insert(std::move(metadata));
    self->m_locator->insert(type, id, locator);

    if (loadingStrategy == RLS_NONE)
    {
        self->load(inserted);
    }
}

void *
ResourceManager::GetData(bedrock::TypeID type, bedrock::ID id, bool *out_cache)
{
    ResourceManager *self      = getInstance();
    ResourceMetadata &metadata = self->getMetadata(type, id);

    if (!metadata.m_data && metadata.m_loadingStrategy == RLS_AUTO_USE)
    {
        self->load(metadata);
    }

    if (out_cache)
    {
        *out_cache = metadata.m_loadingStrategy != RLS_AUTO_USE;
    }

    if (metadata.m_loadingStrategy == RLS_AUTO_USE)
    {
        self->unload(metadata);
    }

    return metadata.m_data;
}

const perfectpixel::bedrock::Opaque &
ResourceManager::GetUserData(bedrock::TypeID type, bedrock::ID id)
{
    ResourceManager *self      = getInstance();
    ResourceMetadata &metadata = self->getMetadata(type, id);
    return metadata.m_userData;
}

void ResourceManager::Shutdown()
{
    UnloadAll();

    ResourceManager *self = getInstance();

    self->m_metadata.clear();
    self->m_loaders.clear();
}

void ResourceManager::UnloadAll()
{
    ResourceManager *self = getInstance();
    for (auto it = self->m_metadata.begin(); it != self->m_metadata.end(); ++it)
    {
        self->unload(it->second, true);
    }
    self->m_unloadQueue.clear();
}

ResourceManager::ResourceMetadata &
ResourceManager::insert(ResourceMetadata &&metadata)
{
    // TODO: Assert not present

    return m_metadata
        .emplace(
            ResourceKey{metadata.m_type, metadata.m_id}, std::move(metadata))
        .first->second;
}

void ResourceManager::setResourceLocator(IResourceLocator *locator)
{
    m_locator = locator;
}

void ResourceManager::load(ResourceMetadata &metadata)
{
    char *buffer;
    size_t bufferSize;

    ResourceLoader &loader = getLoader(metadata);

    if (metadata.m_data)
    {
        loader.m_unload(&metadata.m_data);
    }

    m_locator->locate(metadata.m_type, metadata.m_id, &buffer, &bufferSize);
    loader.m_load(
        buffer,
        bufferSize,
        &metadata.m_data,
        metadata.m_userData,
        loader.m_loaderUserData);

    delete buffer;
}

void ResourceManager::unload(ResourceMetadata &metadata, bool now /* = false */)
{
    if (now)
    {
        ResourceLoader &loader = getLoader(metadata);
        if (metadata.m_data)
        {
            loader.m_unload(&metadata.m_data);
        }
        metadata.m_data = nullptr;
    }
    else
    {
        m_unloadQueue.push_back(&metadata);
    }
}

void ResourceManager::processUnloads()
{
    for (auto metadata : m_unloadQueue)
    {
        // A new ref appeared, abort
        if (metadata->m_loadingStrategy == RLS_AUTO_REF
            && metadata->m_refs != 0)
        {
            continue;
        }

        unload(*metadata, true);
    }
    m_unloadQueue.clear();
}

perfectpixel::core::ResourceManager::ResourceMetadata &
ResourceManager::getMetadata(bedrock::TypeID type, bedrock::ID id)
{
    auto it = m_metadata.find(ResourceKey{type, id});

    PP_ASSERT(it != m_metadata.end(), "Failed to find metadata for resource");

    return it->second;
}

ResourceManager::ResourceLoader &
ResourceManager::getLoader(const ResourceMetadata &metadata)
{
    auto it = m_loaders.find(
        ResourceLoaderKey{metadata.m_type, metadata.m_variant});

    PP_ASSERT(it != m_loaders.end(), "Failed to find loader for resource");

    return it->second;
}

} // namespace perfectpixel::core
