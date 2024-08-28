#pragma once

#include "ecs/Field.h"

#define PPFIELDTYPE(Owner, T) ::perfectpixel::ecs::FieldImpl<T>
#define PPARRAYFIELDTYPE(Owner, T, C)                                          \
    ::perfectpixel::ecs::ArrayField<Owner, T, C>
#define PPFIELDDESCRIPTORTYPE(Owner, T, C)                                     \
    ::perfectpixel::ecs::FieldDescriptor<T, T &, C>

#if PP_FULL_REFLECTION_ENABLED

#define PPFieldImpl(Owner, T, Name, Default)                                   \
    inline static PPFIELDTYPE(Owner, T) Name = PPFIELDTYPE(Owner, T)(          \
        PP_DEQUALIFY(Owner),                                                   \
        PP_DQID(Owner),                                                        \
        #Name,                                                                 \
        PP_ID(Name).m_hash,                                                    \
        bedrock::typeName<T>(),                                                \
        bedrock::typeID<T>(),                                                  \
        Default,                                                               \
        Owner::AddField,                                                       \
        Owner::FillLUTEntry,                                                   \
        Owner::Index);

#define PPArrayField(Owner, T, Capacity, Name)                                 \
    inline static PPARRAYFIELDTYPE(Owner, T, Capacity) Name                    \
        = PPARRAYFIELDTYPE(Owner, T, Capacity)(                                \
            PP_DEQUALIFY(Owner),                                               \
            PP_DQID(Owner),                                                    \
            #Name,                                                             \
            PP_ID(Name).m_hash,                                                \
            bedrock::typeName<T>(),                                            \
            bedrock::typeID<T>(),                                              \
            Owner::AddField,                                                   \
            Owner::FillLUTEntry,                                               \
            Owner::Index)

#else /* PP_FULL_REFLECTION_ENABLED */

#define PPFieldImpl(Owner, T, Name, Default)                                   \
    inline static PPFIELDTYPE(Owner, T) Name = PPFIELDTYPE(Owner, T)(          \
        PP_DQID(Owner),                                                        \
        PP_ID(Name).m_hash,                                                    \
        bedrock::typeID<T>(),                                                  \
        Default,                                                               \
        Owner::AddField,                                                       \
        Owner::FillLUTEntry,                                                   \
        Owner::Index);

#define PPArrayField(Owner, T, Capacity, Name)                                 \
    inline static PPFIELDTYPE(Owner, T, Capacity) Name                         \
        = PPFIELDTYPE(Owner, T, Capacity)(                                     \
            PP_DQID(Owner),                                                    \
            bedrock::typeID<T>(),                                              \
            PP_DQID(T),                                                        \
            Owner::AddField,                                                   \
            Owner::FillLUTEntry,                                               \
            Owner::Index);

#endif

#define PPField(Owner, T, Name) PPFieldImpl(Owner, T, Name, T())

#define PPResourceField(Owner, ResourceType, Name)                             \
    PPFieldImpl(                                                               \
        Owner,                                                                 \
        ::perfectpixel::core::Resource,                                        \
        Name,                                                                  \
        ::perfectpixel::core::Resource(ResourceType))

#define PPRefField(Owner, ComponentT, Name)                                    \
    PPField(Owner, ComponentT::Reference, Name)

#define PPTransientField(Owner, T, Name)                                       \
    inline static PPFIELDTYPE(Owner, T) Name = PPFIELDTYPE(Owner, T)(          \
        ::perfectpixel::ecs::FieldTable::NoReflection);

#define PPRuleType(Owner) ::perfectpixel::ecs::SerializationRule<Owner>
#define PPRuleName(Owner, Field) PPInternal__Rule_##Owner##Field
#define PPDoNotSerialize(Owner, Field)                                         \
    inline static PPRuleType(Owner) PPRuleName(Owner, Field) = PPRuleType(     \
        Owner)(PP_ID(Field).m_hash, &::perfectpixel::ecs::doNotSerialize);
#define PPSerializeIf(Owner, Field, Condition)                                 \
    inline static PPRuleType(Owner) PPRuleName(Owner, Field)                   \
        = PPRuleType(Owner)(PP_ID(Field), Condition);
