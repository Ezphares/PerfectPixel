#pragma once

#include <Resources/Resource.h>
#include <Resources/ResourceManager.h>

#include <EntityComponentSystem/Entity.h>

namespace perfectpixel { namespace resources {

	class Template
	{
	public:
		Template();

		ecs::Entity spawn();

		void applyTo(std::vector<ecs::Entity> &target);

		static ResourceManager::ResourceLoaderFunction CreateTemplateLoader(std::function<serialization::ISerializer*()> provider);
		static void TemplateUnloader(void **data);

	private:
		void prepare(std::vector<ecs::Entity> &all);
		void spawnPrepared();

		struct Preparation
		{
			uint32_t m_start;
			std::vector<uint32_t> m_idxMap;
		};

		std::vector<ecs::Entity> m_entities;
		std::vector<Resource> m_subTemplates;
	};

} }

PP_TYPE_REFLECTION(::perfectpixel::resources::Template, Template);
