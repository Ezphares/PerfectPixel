#pragma once

#include <Resources/IResourceLocator.h>

#include <Bedrock/Singleton.h>
#include <Bedrock/TypeReflection.h>
#include <Bedrock/Opaque.h>

#include <string>
#include <vector>
#include <functional>

namespace perfectpixel { namespace resources {

	class ResourceManager : public bedrock::Singleton<ResourceManager>
	{
	public:
		typedef std::function<void(char *, size_t, void **, const bedrock::Opaque &)> ResourceLoaderFunction;
		typedef std::function<void(void **)> ResourceUnloaderFunction;
		struct ResourceLoader
		{
			ResourceLoaderFunction m_load;
			ResourceUnloaderFunction m_unload;

			template<typename T>
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
		template<typename T, typename Variant = nullptr_t>
		static void AddLoader(ResourceLoaderFunction loader, ResourceUnloaderFunction unloader = nullptr)
		{
			ResourceLoaderLookup entry;
			std::get<0>(entry) = bedrock::typeID<T>();
			std::get<1>(entry) = bedrock::typeID<Variant>();
			std::get<2>(entry).m_load = loader;
			std::get<2>(entry).m_unload = unloader ? unloader : ResourceLoader::GetDefaultUnloader<T>();

			getInstance()->m_loaderLUT.push_back(entry);
		}


		static void Take(int32_t type, int32_t id);
		static void Release(int32_t type, int32_t id);
		static void RegisterResource(
			const std::string &locator,
			ResourceLoadingStategy loadingStrategy,
			int32_t id,
			int32_t type,
			int32_t variant = 0,
			const bedrock::Opaque &userData = bedrock::Opaque()
		);

		void setResourceLocator(IResourceLocator *locator);

	private:

		void insert(const ResourceMetadata &metadata);
		size_t offset(int32_t type);
		void pushOffset(int32_t type, size_t count = 1);

		void load(int32_t type, int32_t id);
		void unload(int32_t type, int32_t id);
		ResourceMetadata &getMetadata(int32_t type, int32_t id);
		ResourceLoader getLoader(const ResourceMetadata &metadata);
		

	private:
		std::vector<std::pair<int32_t, size_t>> m_offsets;
		std::vector<ResourceMetadata> m_metadata;
		std::vector<ResourceLoaderLookup> m_loaderLUT;
		IResourceLocator *m_locator;
	};

} }

