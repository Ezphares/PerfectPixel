#include <EntityComponentSystem/ReflectionTable.h>

#include <EntityComponentSystem/EntityManager.h>

#include <serialization/ISerializer.h>

#include <Bedrock/Hash.h>

namespace perfectpixel { namespace ecs {

void ReflectionTable::add(
    int32_t componentId,
    int32_t fieldId,
    int32_t typeId,
    const ComponentLUTEntry &lutEntry)
{
    m_componentLUT[componentId]                  = lutEntry;
    m_typeLUT[combinedKey(componentId, fieldId)] = typeId;
}

void ReflectionTable::add(
    const std::string &componentName,
    int32_t componentId,
    const std::string &fieldName,
    int32_t fieldId,
    const std::string &typeName,
    int32_t typeId,
    const ComponentLUTEntry &lutEntry)
{
    add(componentId, fieldId, typeId, lutEntry);
#if PP_FULL_REFLECTION_ENABLED
    m_reverseHash[componentId] = componentName;
    m_reverseHash[fieldId]     = fieldName;
    m_reverseHash[typeId]      = typeName;
#endif /* PP_FULL_REFLECTION_ENABLED */
}

int32_t
ReflectionTable::componentFieldTypeID(int32_t componentId, int32_t fieldId)
{
    return m_typeLUT[combinedKey(componentId, fieldId)];
}

perfectpixel::ecs::IField *
ReflectionTable::getComponentFieldByID(int32_t componentId, int32_t fieldId)
{
    return componentFieldLookupByID(componentId)(fieldId);
}

perfectpixel::ecs::FieldLookup
ReflectionTable::componentFieldLookupByID(int32_t componentId)
{
    return m_componentLUT[componentId].m_fields;
}

perfectpixel::ecs::HasLookup
ReflectionTable::hasComponentByID(int32_t componentId)
{
    return m_componentLUT[componentId].m_has;
}

std::string ReflectionTable::dequalify(const std::string &typeName)
{
    size_t it = typeName.rfind(':');
    if (it == typeName.npos)
    {
        return typeName;
    }
    return typeName.substr(it + 1);
}

std::string ReflectionTable::reverse(int32_t id) const
{
    auto it = m_reverseHash.find(id);
    if (it != m_reverseHash.end())
    {
        return it->second;
    }

    return "";
}

void ReflectionTable::Touch(const std::string &str)
{
#if PP_FULL_REFLECTION_ENABLED
    getInstance()->m_reverseHash[bedrock::crc32(str)] = str;
#endif
}

std::string ReflectionTable::Reverse(int32_t id)
{
    return getInstance()->reverse(id);
}

void ReflectionTable::deserialize(
    serialization::ISerializer &serializer, Entity entity)
{
    serializer.readMapBegin();

    int32_t key;
    while (serializer.readMapKey(&key))
    {
        serializer.readMapBegin();

        auto &component = m_componentLUT[key];

        component.m_register(entity);
        component.m_deserialize(serializer, entity);
    }
}

void ReflectionTable::copy(Entity destination, Entity source)
{
    if (EntityManager::getInstance()->isAlive(destination)
        && EntityManager::getInstance()->isAlive(source))
    {
        for (auto it = m_componentLUT.begin(); it != m_componentLUT.end(); ++it)
        {
            if (it->second.m_has(source))
            {
                it->second.m_copy(destination, source);
            }
        }
    }
}

void ReflectionTable::serialize(
    serialization::ISerializer &serializer, Entity entity)
{
    if (EntityManager::getInstance()->isAlive(entity))
    {
        serializer.writeMapStart();

        for (auto it = m_componentLUT.begin(); it != m_componentLUT.end(); ++it)
        {
            if (it->second.m_has(entity))
            {
#if PP_FULL_REFLECTION_ENABLED
                serializer.writeMapKey(m_reverseHash[it->first]);
#else
                serializer.writeMapKey(it->first);
#endif
                it->second.m_serialize(serializer, entity);
            }
        }
    }
}

uint64_t ReflectionTable::combinedKey(int32_t p1, int32_t p2)
{
    return (static_cast<uint64_t>(p1) << 32) | static_cast<uint64_t>(p2);
}

}} // namespace perfectpixel::ecs