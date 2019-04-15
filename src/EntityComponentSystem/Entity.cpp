#include <EntityComponentSystem/Entity.h>

namespace perfectpixel {
	namespace world {

		uint32_t entityGeneration(Entity entity)
		{
			return entity >> INDEX_BITS;
		}

		uint32_t entityIndex(Entity entity)
		{
			return entity & ((1 << INDEX_BITS) - 1);
		}

		perfectpixel::world::Entity entityCreate(std::uint32_t generation, std::uint32_t index)
		{
			return (generation << INDEX_BITS | index & INDEX_MASK);
		}

	}
}


