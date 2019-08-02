#include <Resources/Template.h>

#include <EntityComponentSystem/EcsReflection.h>
#include <EntityComponentSystem/LifecycleComponents.h>

namespace perfectpixel { namespace resources {


	Template::Template(ecs::Entity entity)
		: m_entity(ecs::NO_ENTITY)
		, m_variantOf(bedrock::typeID<Template>())
	{
	}

	void Template::applyTo(ecs::Entity target)
	{
		// Apply parent first, so we overwrite parent values
		if (m_variantOf.isValid())
		{
			Template *parent = m_variantOf.get<Template>();
			parent->applyTo(target);
		}

		ecs::FieldTable::getInstance()->copy(target, m_entity);
	}

	perfectpixel::resources::ResourceManager::ResourceLoaderFunction Template::CreateTemplateLoader(serialization::ISerializer &serializer)
	{
		return [&serializer](char *data, size_t dataSize, void **target, const bedrock::Opaque &) {
			serializer.loadBuffer(data, dataSize);
			
			Template *tpl = new Template(ecs::EntityManager::getInstance()->create());
			
			ecs::InactiveComponent::Register(tpl->m_entity);
			ecs::FieldTable::getInstance()->deserialize(serializer, tpl->m_entity);

			*target = tpl;
		};
	}

	void Template::TemplateUnloader(void **data)
	{
		Template *tpl = static_cast<Template *>(*data);

		ecs::EntityManager::getInstance()->kill(tpl->m_entity);

		delete tpl;
	}

} }
