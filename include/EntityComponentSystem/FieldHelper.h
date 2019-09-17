#pragma once

#include <EntityComponentSystem/Field.h>

#define PPFIELDTYPE(Owner, T)                                                  \
    ::perfectpixel::ecs::FieldImpl<Owner, Owner::Reference, T>
#define PPARRAYFIELDTYPE(Owner, T, C)                                          \
    ::perfectpixel::ecs::ArrayField<Owner, T, C>

#if PP_FULL_REFLECTION_ENABLED

#define PPFieldImpl(Owner, T, Name, Default)                                   \
    inline static PPFIELDTYPE(Owner, T) Name = PPFIELDTYPE(Owner, T)(          \
        PP_DEQUALIFY(Owner),                                                   \
        PP_DQID(Owner),                                                        \
        #Name,                                                                 \
        PP_ID(Name),                                                           \
        bedrock::typeName<T>(),                                                \
        bedrock::typeID<T>(),                                                  \
        Default);

#define PPArrayField(Owner, T, Capacity, Name)                                 \
    inline static PPARRAYFIELDTYPE(Owner, T, Capacity) Name                    \
        = PPARRAYFIELDTYPE(Owner, T, Capacity)(                                \
            PP_DEQUALIFY(Owner),                                               \
            PP_DQID(Owner),                                                    \
            #Name,                                                             \
            PP_ID(Name),                                                       \
            bedrock::typeName<T>(),                                            \
            bedrock::typeID<T>())

#else /* PP_FULL_REFLECTION_ENABLED */

#define PPFieldImpl(Owner, T, Name, Default)                                   \
    inline static PPFIELDTYPE(Owner, T) Name = PPFIELDTYPE(Owner, T)(          \
        PP_DQID(Owner), PP_ID(Name), bedrock::typeID<T>(), Default);

#define PPArrayField(Owner, T, Capacity, Name)                                 \
    inline static PPFIELDTYPE(Owner, T, Capacity) Name = PPFIELDTYPE(          \
        Owner, T, Capacity)(PP_DQID(Owner), bedrock::typeID<T>(), PP_DQID(T));

#endif

#define PPField(Owner, T, Name) PPFieldImpl(Owner, T, Name, T())

#define PPResourceField(Owner, ResourceType, Name)                             \
    PPFieldImpl(                                                               \
        Owner,                                                                 \
        ::perfectpixel::resources::Resource,                                   \
        Name,                                                                  \
        ::perfectpixel::resources::Resource(ResourceType))

#define PPRefField(Owner, ComponentT, Name)                                    \
    PPField(Owner, ComponentT::Reference, Name)

#define PPTransientField(Owner, T, Name)                                         \
    inline static PPFIELDTYPE(Owner, T) Name = PPFIELDTYPE(Owner, T)(          \
        ::perfectpixel::ecs::FieldTable::NoReflection);
