#pragma once

#include <EntityComponentSystem/Entity.h>

namespace perfectpixel {

namespace serialization {
class ISerializer;
}

namespace ecs {

class IField;

typedef IField *(*FieldLookup)(int32_t);
typedef bool (*HasLookup)(Entity);
typedef void (*SerializeLookup)(serialization::ISerializer &, Entity);
typedef void (*DeserializeLookup)(serialization::ISerializer &, Entity);
typedef void (*RegisterLookup)(Entity);
typedef void (*DeleteLookup)(Entity);
typedef void (*CopyLookup)(Entity, Entity);

struct ComponentLUTEntry
{
    HasLookup m_has;
    SerializeLookup m_serialize;
    DeserializeLookup m_deserialize;
    RegisterLookup m_register;
    DeleteLookup m_delete;
    FieldLookup m_fields;
    CopyLookup m_copy;
};

} // namespace ecs
} // namespace perfectpixel