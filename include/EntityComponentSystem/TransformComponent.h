#pragma once

#include <EntityComponentSystem/Component.h>
#include <EntityComponentSystem/CoreComponentStorage.h>
#include <EntityComponentSystem/EcsReflection.h>

#include <types/vectors.h>

namespace perfectpixel { namespace ecs {

	class TransformComponent 
		: public Component<TransformComponent>
		, public FlatComponentStorage<TransformComponent>
	{
	public:
		_Field(TransformComponent, types::Vector3, Position);
		_Field(TransformComponent, types::Vector3, Velocity);
		//inline static Field<TransformComponent, types::Vector3> Position;
		//inline static Field<TransformComponent, types::Vector3> Velocity;
	};

} }
