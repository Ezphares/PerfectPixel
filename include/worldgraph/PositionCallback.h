#pragma once

#include <worldgraph/Entity.h>

#include <types/vectors.h>

#include <boost/function.hpp>

namespace perfectpixel {
	namespace world {

		typedef boost::function<types::Vector3(world::Entity)> PositionCallback;


	}
}
