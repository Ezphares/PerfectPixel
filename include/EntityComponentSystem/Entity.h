#pragma once

#include <Bedrock/TypeReflection.h>

#include <cstdint>
#include <ostream>

namespace perfectpixel {

	namespace serialization 
	{
		class ISerializer;
	}
namespace ecs {

	typedef std::uint32_t _Entity;

#pragma pack(push, 1)
	struct Entity
	{
		uint32_t
			generation : 8,
			index : 24;

		bool operator==(const Entity &other) const
		{
			return *((uint32_t*)this) == *((uint32_t*)&other);
		}

		bool operator<(const Entity &other) const
		{
			return *((uint32_t*)this) < *((uint32_t*)&other);
		}
	};
#pragma pack(pop)

	static_assert(sizeof(Entity) == sizeof(uint32_t));

	const static uint32_t GENERATION_BITS = 8;
	const static uint32_t INDEX_BITS = sizeof(Entity) * 8 - GENERATION_BITS;
	const static uint32_t GENERATION_MASK = ((1u << GENERATION_BITS) - 1u) << INDEX_BITS;
	const static uint32_t INDEX_MASK = (1u << INDEX_BITS) - 1u;

	//uint32_t entityGeneration(Entity entity);
	//uint32_t entityIndex(Entity entity);
	Entity entityCreate(std::uint32_t generation, std::uint32_t index);

	static const Entity NO_ENTITY = entityCreate(~0u, ~0u);
}
}

PP_TYPE_REFLECTION(::perfectpixel::ecs::Entity, Entity);

std::ostream &operator<<(std::ostream &stream, const perfectpixel::ecs::Entity &entity);

namespace perfectpixel { namespace serialization {
ISerializer &operator<<(ISerializer &serializer, const perfectpixel::ecs::Entity &entity);
ISerializer &operator>>(ISerializer &serializer, perfectpixel::ecs::Entity &entity);
}}