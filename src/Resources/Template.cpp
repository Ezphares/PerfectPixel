#include <Resources/Template.h>

#include <EntityComponentSystem/LifecycleComponents.h>
#include <EntityComponentSystem/ReflectionTable.h>

namespace perfectpixel { namespace resources {

namespace {
const auto KEY_ENTITIES     = PP_KEY(Entities);
const auto KEY_SUBTEMPLATES = PP_KEY(SubTemplates);
} // namespace

Template::Template()
    : m_entities()
    , m_subTemplates()
{}

perfectpixel::ecs::Entity Template::spawn()
{
    ecs::EntityManager *manager = ecs::EntityManager::getInstance();
    ecs::FieldTable *table      = ecs::FieldTable::getInstance();

    std::vector<ecs::Entity> spawns
        = std::vector<ecs::Entity>(m_entities.size(), ecs::NO_ENTITY);

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

    for (uint32_t i = 0; i < m_entities.size(); ++i)
    {
        table->copy(target[i], m_entities[i]);
    }
}

perfectpixel::resources::ResourceManager::ResourceLoaderFunction
Template::CreateTemplateLoader(
    std::function<serialization::ISerializer *()> provider)
{
    return [provider](
               char *data,
               size_t dataSize,
               void **target,
               const bedrock::Opaque &) {
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
                    ecs::Entity entity
                        = ecs::EntityManager::getInstance()->create();
                    ecs::InactiveComponent::Register(entity);

                    ecs::FieldTable::getInstance()->deserialize(
                        *serializer, entity);

                    tpl->m_entities.push_back(entity);
                }

                serializer->readArrayEnd();
            }

            if (PP_KEY_EQUAL(KEY_SUBTEMPLATES, key))
            {
                uint32_t subCount = serializer->readArrayStart();
                tpl->m_subTemplates.reserve(subCount);

                for (uint32_t i = 0; i < subCount; ++i)
                {
                    int32_t tplId;
                    serializer->readIdentifier(&tplId);
                    tpl->m_subTemplates.emplace_back(
                        bedrock::typeID<Template>(), tplId);
                }
            }
        }

        *target = tpl;
        delete serializer;
    };
}

void Template::TemplateUnloader(void **data)
{
    Template *tpl               = static_cast<Template *>(*data);
    ecs::EntityManager *manager = ecs::EntityManager::getInstance();

    for (ecs::Entity entity : tpl->m_entities)
    {
        manager->kill(entity);
    }

    delete tpl;
}

}} // namespace perfectpixel::resources
