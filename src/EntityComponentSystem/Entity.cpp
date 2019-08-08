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
			Entity entity;
			entity.generation = generation;
			entity.index = index;
			return entity;
			//return (generation << INDEX_BITS | index & INDEX_MASK);
		}

	}
}

std::ostream & operator<<(std::ostream &stream, const perfectpixel::ecs::Entity &entity)
{
	stream << *((uint32_t*)&entity);
	return stream;
}

namespace perfectpixel { namespace serialization {

ISerializer & operator<<(ISerializer &serializer, const perfectpixel::ecs::Entity &entity)
{
	serializer.writeMappedUInt32(*((uint32_t*)&entity));
	return serializer;
}

ISerializer & operator>>(ISerializer &serializer, perfectpixel::ecs::Entity &entity)
{
	serializer.readMappedUInt32((uint32_t*)&entity);
	return serializer;
}

}}
