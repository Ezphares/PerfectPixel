#pragma once

#include <worldgraph/Entity.h>

#include <types/vectors.h>

#include <functional>

namespace perfectpixel {
	namespace world {

		typedef std::function<types::Vector3(world::Entity)> PositionCallback;


	}
}
