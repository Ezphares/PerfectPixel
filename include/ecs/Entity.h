#pragma once

#include "bedrock/Reflect.h"

#include <cstdint>
#include <ostream>

namespace perfectpixel {

namespace serialization {
class ISerializer;
}
namespace ecs {

typedef std::uint32_t _Entity;

#pragma pack(push, 1)
struct Entity
{
    uint32_t generation : 8, index : 24;

    bool operator==(const Entity &other) const
    {
        return (memcmp((void *)this, (void *)&other, sizeof(Entity)) == 0);
    }

    bool operator<(const Entity &other) const
    {
        return (memcmp((void *)this, (void *)&other, sizeof(Entity)) < 0);
    }
};
#pragma pack(pop)

static_assert(sizeof(Entity) == sizeof(uint32_t));

const static uint32_t GENERATION_BITS = 8;
const static uint32_t INDEX_BITS      = sizeof(Entity) * 8 - GENERATION_BITS;
const static uint32_t GENERATION_MASK = ((1u << GENERATION_BITS) - 1u)
                                        << INDEX_BITS;
const static uint32_t INDEX_MASK = (1u << INDEX_BITS) - 1u;

// uint32_t entityGeneration(Entity entity);
// uint32_t entityIndex(Entity entity);
Entity entityCreate(std::uint32_t generation, std::uint32_t index);

static const Entity NO_ENTITY = entityCreate(~0u, ~0u);
} // namespace ecs
} // namespace perfectpixel

PP_REFLECT(::perfectpixel::ecs::Entity);

std::ostream &
operator<<(std::ostream &stream, const perfectpixel::ecs::Entity &entity);

namespace std {
template <>
struct hash<perfectpixel::ecs::Entity>
{
    std::size_t operator()(const perfectpixel::ecs::Entity &e) const
    {
        return hash<uint32_t>{}(*((uint32_t *)&e));
    }
};
} // namespace std

namespace perfectpixel { namespace serialization {
ISerializer &
operator<<(ISerializer &serializer, const perfectpixel::ecs::Entity &entity);
ISerializer &
operator>>(ISerializer &serializer, perfectpixel::ecs::Entity &entity);
}} // namespace perfectpixel::serialization
