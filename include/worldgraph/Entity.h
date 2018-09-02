#pragma once

#include <cstdint>

namespace perfectpixel {
namespace world {

	typedef std::uint32_t Entity;

	const static uint32_t GENERATION_BITS = 8;
	const static uint32_t INDEX_BITS = sizeof(Entity) * 8 - GENERATION_BITS;
	const static uint32_t GENERATION_MASK = ((1 << GENERATION_BITS) - 1) << INDEX_BITS;
	const static uint32_t INDEX_MASK = (1 << INDEX_BITS) - 1;

	uint32_t entityGeneration(Entity entity);
	uint32_t entityIndex(Entity entity);
	Entity entityCreate(std::uint32_t generation, std::uint32_t index);
	
}
}
