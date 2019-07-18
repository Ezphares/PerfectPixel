#include <EntityComponentSystem/Entity.h>

#include <serialization/BinarySerializer.h>

namespace perfectpixel {
	namespace ecs {

		uint32_t entityGeneration(Entity entity)
		{
			//return entity >> INDEX_BITS;
			return entity.generation;
		}

		uint32_t entityIndex(Entity entity)
		{
			//return entity & ((1 << INDEX_BITS) - 1);
			return entity.index;
		}

		Entity entityCreate(std::uint32_t generation, std::uint32_t index)
		{
			return Entity{ generation = generation, index = index };
			//return (generation << INDEX_BITS | index & INDEX_MASK);
		}

	}
}

perfectpixel::serialization::BinarySerializer & operator<<(perfectpixel::serialization::BinarySerializer &serializer, const perfectpixel::ecs::Entity &entity)
{
	serializer << *((uint32_t*)&entity);
	return serializer;
}

std::ostream & operator<<(std::ostream &stream, const perfectpixel::ecs::Entity &entity)
{
	stream << *((uint32_t*)&entity);
	return stream;
}
