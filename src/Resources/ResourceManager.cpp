#include <Resources/ResourceManager.h>

namespace perfectpixel { namespace resources {

	void ResourceManager::Take(int32_t ID)
	{
	}

	void ResourceManager::Release(int32_t ID)
	{
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
		loader.m_load(buffer, bufferSize, &metadata.m_data);

		delete buffer;
	}

	perfectpixel::resources::ResourceManager::ResourceMetadata & ResourceManager::getMetadata(int32_t type, int32_t id)
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
