#pragma once

#include <Resources/Resource.h>
#include <Resources/ResourceManager.h>

#include <EntityComponentSystem/Entity.h>

namespace perfectpixel { namespace resources {

	class Template
	{
	public:
		Template(ecs::Entity entity);

		void applyTo(ecs::Entity target);

		static ResourceManager::ResourceLoaderFunction CreateTemplateLoader(serialization::ISerializer &serializer);
		static void TemplateUnloader(void **data);

	private:
		ecs::Entity m_entity;
		Resource m_variantOf;
	};

} }

PP_TYPE_REFLECTION(::perfectpixel::resources::Template, Template);
