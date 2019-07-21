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
	const static uint32_t GENERATION_MASK = ((1 << GENERATION_BITS) - 1) << INDEX_BITS;
	const static uint32_t INDEX_MASK = (1 << INDEX_BITS) - 1;

	//uint32_t entityGeneration(Entity entity);
	//uint32_t entityIndex(Entity entity);
	Entity entityCreate(std::uint32_t generation, std::uint32_t index);
}
}

PP_TYPE_REFLECTION(::perfectpixel::ecs::Entity, Entity);

std::ostream &operator<<(std::ostream &stream, const perfectpixel::ecs::Entity &entity);

perfectpixel::serialization::ISerializer &operator<<(
	perfectpixel::serialization::ISerializer &serializer,
	const perfectpixel::ecs::Entity &entity);