#pragma once

#include <Resources/IResourceLocator.h>

#include <Bedrock/Singleton.h>

#include <string>
#include <vector>
#include <functional>

namespace perfectpixel { namespace resources {

	class ResourceManager : public bedrock::Singleton<ResourceManager>
	{
	public:
		typedef std::function<void(char *, size_t, void **)> ResourceLoaderFunction;
		typedef std::function<void(void **)> ResourceUnloaderFunction;
		struct ResourceLoader
		{
			ResourceLoaderFunction m_load;
			ResourceUnloaderFunction m_unload;

			template<typename T>
			static ResourceLoaderFunction GetDefaultUnloader()
			{
				return [](void **data) {
					delete reinterpret_cast<T *>(*data);
					*data = nullptr;
				}
			}
		};
		typedef std::tuple<int32_t, int32_t, ResourceLoader> ResourceLoaderLookup;

		struct ResourceMetadata
		{

			enum ResourceLoadingStategy : uint8_t
			{
				RLS_NONE,
				RLS_AUTO_REF,
				RLS_AUTO_USE,
				RLS_MANUAL,
			};

			int32_t m_id;
			int32_t m_type;
			int32_t m_variant;
			void *m_data;
			ResourceLoadingStategy m_loadingStrategy;
		};


	public:
		static void Take(int32_t id);
		static void Release(int32_t id);
		static void RegisterResource(
			const ResourceMetadata &resource,
			const std::string &locator
		);

	private:
		void insert(const ResourceMetadata &metadata);
		size_t offset(int32_t type);
		void pushOffset(int32_t type, size_t count = 1);

		void load(int32_t type, int32_t id);
		ResourceMetadata &getMetadata(int32_t type, int32_t id);
		ResourceLoader getLoader(const ResourceMetadata &metadata);
		

	private:
		std::vector<std::pair<int32_t, size_t>> m_offsets;
		std::vector<ResourceMetadata> m_metadata;
		std::vector<ResourceLoaderLookup> m_loaderLUT;
		IResourceLocator *m_locator;
	};

} }

