#include <Resources/ResourceManager.h>

namespace perfectpixel { namespace resources {

	void ResourceManager::Take(int32_t type, int32_t id)
	{
		ResourceManager *instance = getInstance();
		ResourceMetadata metadata = instance->getMetadata(type, id);
		if (metadata.m_refs == 0 && metadata.m_loadingStrategy == RLS_AUTO_REF)
		{
			instance->load(type, id);
		}
		metadata.m_refs++;
	}

	void ResourceManager::Release(int32_t type, int32_t id)
	{
		ResourceManager *instance = getInstance();
		ResourceMetadata metadata = instance->getMetadata(type, id);
		if (metadata.m_refs == 0)
		{
			throw "Ref corruption";
		}
		metadata.m_refs--;

		if (metadata.m_refs == 0 && metadata.m_loadingStrategy == RLS_AUTO_REF)
		{
			instance->unload(type, id);
		}
	}

	void ResourceManager::RegisterResource(
		const std::string &locator,
		ResourceLoadingStategy loadingStrategy,
		int32_t id,
		int32_t type,
		int32_t variant,
		const bedrock::Opaque &userData)
	{
		ResourceMetadata metadata;
		metadata.m_id = id;
		metadata.m_type = type;
		metadata.m_variant = variant;
		metadata.m_data = nullptr;
		metadata.m_refs = 0;
		metadata.m_loadingStrategy = loadingStrategy;
		metadata.m_userData = userData;

		getInstance()->m_locator->insert(type, id, locator);

		if (loadingStrategy == RLS_NONE)
		{
			getInstance()->load(type, id);
		}
	}

	void * ResourceManager::GetData(int32_t type, int32_t id, bool *out_cache)
	{
		ResourceManager *instance = getInstance();
		ResourceMetadata &metadata = instance->getMetadata(type, id);

		if (!metadata.m_data && metadata.m_loadingStrategy == RLS_AUTO_USE)
		{
			instance->load(type, id);
		}

		if (out_cache)
		{
			*out_cache = metadata.m_loadingStrategy != RLS_AUTO_USE;
		}

		return metadata.m_data;
	}

	void ResourceManager::insert(const ResourceMetadata &metadata)
	{
		auto it = m_metadata.begin();
		std::advance(it, offset(metadata.m_type));

		while (it != m_metadata.end() && it->m_type == metadata.m_type && it->m_id < metadata.m_id)
		{
			++it;
		}

		if (it != m_metadata.end() && it->m_id == metadata.m_id)
		{
			throw "Error: Resource inserted twice";
		}

		m_metadata.insert(it, metadata);
		pushOffset(metadata.m_type);
	}

	void ResourceManager::setResourceLocator(IResourceLocator *locator)
	{
		m_locator = locator;
	}

	size_t ResourceManager::offset(int32_t type)
	{
		for (auto it = m_offsets.begin();; ++it)
		{
			if (it == m_offsets.end() || it->first > type)
			{
				if (it == m_offsets.begin())
				{
					m_offsets.emplace(it, type, 0ul);
					return 0;
				}
				else
				{
					m_offsets.emplace(it, type, (it - 1)->second);
					return it->second;
				}
			}
			else if (it->first == type)
			{
				return it->second;
			} 
		}
	}

	void ResourceManager::pushOffset(int32_t type, size_t count /*= 1*/)
	{
		for (auto it = m_offsets.rbegin(); it->first != type; ++it)
		{
			it->second += count;
		}
	}

	void ResourceManager::load(int32_t type, int32_t id)
	{
		char *buffer;
		size_t bufferSize;

		ResourceMetadata &metadata = getMetadata(type, id);
		ResourceLoader &loader = getLoader(metadata);

		if (metadata.m_data)
		{
			loader.m_unload(&metadata.m_data);
		}

		m_locator->locate(type, id, &buffer, &bufferSize);
		loader.m_load(buffer, bufferSize, &metadata.m_data, metadata.m_userData);

		delete buffer;
	}

	void ResourceManager::unload(int32_t type, int32_t id, bool now)
	{
		if (now)
		{
			ResourceMetadata &metadata = getMetadata(type, id);
			ResourceLoader &loader = getLoader(metadata);
			if (metadata.m_data)
			{
				loader.m_unload(&metadata.m_data);
			}
			metadata.m_data = nullptr;
		}
		else
		{
			m_unloadQueue.emplace_back(type, id);
		}
	}

	void ResourceManager::processUnloads()
	{
		for (auto it : m_unloadQueue)
		{
			ResourceMetadata &metadata = getMetadata(it.first, it.second);

			// A new ref appeared, abort
			if (metadata.m_loadingStrategy == RLS_AUTO_REF && metadata.m_refs != 0)
			{
				continue;
			}

			unload(metadata.m_type, metadata.m_id, true);
		}
		m_unloadQueue.clear();
	}

	ResourceManager::ResourceMetadata & ResourceManager::getMetadata(int32_t type, int32_t id)
	{
		auto it = m_metadata.begin();
		std::advance(it, offset(type));

		for (; it != m_metadata.end(); ++it)
		{
			if (it->m_type != type)
			{
				break;
			}

			if (it->m_id == id)
			{
				return *it;
			}
		}

		throw "Metadata not found";
	}

	ResourceManager::ResourceLoader ResourceManager::getLoader(const ResourceMetadata &metadata)
	{
		for (auto &loader : m_loaderLUT)
		{
			if (std::get<0>(loader) == metadata.m_type && std::get<1>(loader) == metadata.m_variant)
			{
				return std::get<2>(loader);
			}
		}

		throw "Loader not found";
	}

} }
