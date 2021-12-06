#pragma once

#include <EntityComponentSystem/ComponentLUTEntry.h>
#include <EntityComponentSystem/EntityManager.h>

#include <Bedrock/Singleton.h>
#include <Bedrock/numbers.h>

#include <map>

namespace perfectpixel {
namespace serialization {

class ISerializer;
}

namespace ecs {

struct ComponentLUTEntry;
class IField;

class ReflectionTable : public bedrock::Singleton<ReflectionTable>
{
public:
    struct ReflectionHint
    {};
    inline static ReflectionHint NoReflection;

    void
    add(const std::string &componentName,
        int32_t componentId,
        const std::string &fieldName,
        int32_t fieldId,
        const std::string &typeName,
        int32_t typeId,
        const ComponentLUTEntry &lutEntry);

    void
    add(int32_t componentId,
        int32_t fieldId,
        int32_t typeId,
        const ComponentLUTEntry &lutEntry);

    int32_t componentFieldTypeID(int32_t componentId, int32_t fieldId);
    IField *getComponentFieldByID(int32_t componentId, int32_t fieldId);

    FieldLookup componentFieldLookupByID(int32_t componentId);
    HasLookup hasComponentByID(int32_t componentId);

    std::string dequalify(const std::string &typeName);
    std::string reverse(int32_t id) const;

    static void Touch(const std::string &str);

    static std::string Reverse(int32_t id);

    void deserialize(serialization::ISerializer &serializer, Entity entity);
    void copy(Entity destination, Entity source);
    void serialize(serialization::ISerializer &serializer, Entity entity);

private:
    std::map<int32_t, ComponentLUTEntry> m_componentLUT;
    std::map<std::pair<int32_t, int32_t>, int32_t> m_typeLUT;
#if PP_FULL_REFLECTION_ENABLED
    std::map<int32_t, std::string> m_reverseHash;
#endif /* PP_FULL_REFLECTION_ENABLED */
};

} // namespace ecs
} // namespace perfectpixel
