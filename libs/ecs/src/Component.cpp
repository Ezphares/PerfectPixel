#include "ecs/Component.h"

namespace perfectpixel { namespace ecs {

BaseComponent::BaseComponent()
    : m_objects(0)
    , m_lastIndex(0)
    , m_fields()
    , m_dirtyFrame(0)
    , m_serializationRules()
{}

BaseComponent::~BaseComponent()
{}

void BaseComponent::initialize(uint32_t idx)
{
    for (auto field : m_fields)
    {
        field.second->reset(idx);
    }
}

void BaseComponent::destroy(uint32_t idx)
{
    purge(idx);
    m_dirtyFrame = EntityManager::getInstance()->getTick();
    m_objects--;
}

void BaseComponent::prepare(uint32_t idx)
{
    if (idx >= m_lastIndex)
    {
        m_lastIndex++;
    }

    initialize(idx);
    m_objects++;

    m_dirtyFrame = EntityManager::getInstance()->getTick();
}

void BaseComponent::purge(uint32_t idx)
{
    (void)idx;
}

bool BaseComponent::addField(int32_t id, IField *field)
{
    // Inline statics are weird, we have to check for duplicates
    bool add = m_fields.find(id) == m_fields.end();
    if (add)
    {
        m_fields[id] = field;
    }

    return add;
}

void BaseComponent::addSerializationRule(
    int32_t id, IField::SerializationCondition rule)
{
    m_serializationRules[id] = rule;
}

perfectpixel::ecs::IField *BaseComponent::lookup(int32_t id)
{
    return m_fields[id];
}

void BaseComponent::serialize(
    serialization::ISerializer &serializer, Entity entity, uint32_t idx)
{
    serializer.writeMapStart();

    for (auto it = m_fields.begin(); it != m_fields.end(); ++it)
    {
        auto condition = m_serializationRules.find(it->first);
        if (condition != m_serializationRules.end()
            && !condition->second(entity))
        {
            continue;
        }

#if PP_FULL_REFLECTION_ENABLED
        serializer.writeMapKey(
            ReflectionTable::getInstance()->reverse(it->first));
#else
        serializer.writeMapKey(it->first);
#endif
        it->second->serialize(serializer, idx);
    }

    serializer.writeMapEnd();
}

void BaseComponent::deserialize(
    serialization::ISerializer &serializer, uint32_t idx)
{
    int32_t k;

    while (serializer.readMapKey(&k))
    {
        if (serializer.isValueNull())
            continue;

        m_fields[k]->deserialize(serializer, idx);
    }
}

}} // namespace perfectpixel::ecs