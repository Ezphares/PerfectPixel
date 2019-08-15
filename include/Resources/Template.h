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
		std::vector<ecs::Entity> m_entities;
		Resource m_variantOf;
	};

} }

PP_TYPE_REFLECTION(::perfectpixel::resources::Template, Template);
