#pragma once

#include <EntityComponentSystem/Entity.h>

#include <types/vectors.h>

#include <functional>

namespace perfectpixel {
	namespace ecs {

		typedef std::function<types::Vector3(Entity)> PositionCallback;


	}
}
