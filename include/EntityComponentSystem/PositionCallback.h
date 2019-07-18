#pragma once

#include <EntityComponentSystem/Entity.h>
#include <Bedrock/vectors.h>

#include <functional>

namespace perfectpixel {
	namespace ecs {

		typedef std::function<bedrock::Vector3(Entity)> PositionCallback;


	}
}
