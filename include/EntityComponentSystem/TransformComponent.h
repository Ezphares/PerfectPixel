#pragma once

#include <EntityComponentSystem/Component.h>
#include <EntityComponentSystem/CoreComponentStorage.h>

#include <types/vectors.h>

namespace perfectpixel { namespace ecs {

	class TransformComponent 
		: public Component<TransformComponent>
		, public FlatComponentStorage<TransformComponent>
	{
	public:
		inline static Field<TransformComponent, types::Vector3> Position;
		inline static Field<TransformComponent, types::Vector3> Velocity;
	};

} }
