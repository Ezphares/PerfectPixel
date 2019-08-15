#include <Resources/Template.h>

#include <EntityComponentSystem/EcsReflection.h>
#include <EntityComponentSystem/LifecycleComponents.h>

namespace perfectpixel { namespace resources {

	namespace {
		const auto KEY_ENTITIES = PP_KEY(Entities);
	}


	Template::Template()
		: m_entities()
		, m_variantOf(bedrock::typeID<Template>())
	{
	}

	perfectpixel::ecs::Entity Template::spawn()
	{
		// TODO: Parent
		ecs::Entity rval = ecs::NO_ENTITY;

		ecs::EntityManager *manager = ecs::EntityManager::getInstance();
		ecs::FieldTable *table = ecs::FieldTable::getInstance();

		std::vector<ecs::Entity> spawns = std::vector<ecs::Entity>(m_entities.size(), ecs::NO_ENTITY);

		for (uint32_t i = 0; i < m_entities.size(); ++i)
		{
			spawns[i] = manager->create();
			table->copy(spawns[i], m_entities[i]);
		}

		return spawns[0];
	}

	void Template::applyTo(std::vector<ecs::Entity> &target)
	{
		ecs::FieldTable *table = ecs::FieldTable::getInstance();

		// TODO: Parent
		for (uint32_t i = 0; i < m_entities.size(); ++i)
		{
			table->copy(target[i], m_entities[i]);
		}
	}

	perfectpixel::resources::ResourceManager::ResourceLoaderFunction Template::CreateTemplateLoader(std::function<serialization::ISerializer*()> provider)
	{
		return [provider](char *data, size_t dataSize, void **target, const bedrock::Opaque &) {
			serialization::ISerializer *serializer = provider();
			serializer->loadBuffer(data, dataSize);
			
			Template *tpl = new Template();

			serializer->readMapBegin();
			int32_t key;
			while (serializer->readMapKey(&key))
			{
				if (PP_KEY_EQUAL(KEY_ENTITIES, key))
				{
					uint32_t entityCount = serializer->readArrayStart();
					tpl->m_entities.reserve(entityCount);

					for (uint32_t i = 0; i < entityCount; ++i)
					{
						ecs::Entity entity = ecs::EntityManager::getInstance()->create();
						ecs::InactiveComponent::Register(entity);

						ecs::FieldTable::getInstance()->deserialize(*serializer, entity);

						tpl->m_entities.push_back(entity);
					}

					serializer->readArrayEnd();
				}
			}

			*target = tpl;
			delete serializer;
		};
	}

	void Template::TemplateUnloader(void **data)
	{
		Template *tpl = static_cast<Template *>(*data);
		ecs::EntityManager *manager = ecs::EntityManager::getInstance();

		for (ecs::Entity entity : tpl->m_entities)
		{
			manager->kill(entity);
		}

		delete tpl;
	}

} }
