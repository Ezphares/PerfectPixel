#pragma once

#include <physics/Force.h>

#include <EntityComponentSystem/Component.h>
#include <EntityComponentSystem/CoreComponentStorage.h>

#include <EntityComponentSystem/Entity.h>

namespace perfectpixel { namespace physics {

	class PhysicsComponent
		: public ecs::Component<PhysicsComponent>
		, public ecs::LinearScanComponentStorage
	{
	public:
		enum PhysicsSimulationType : uint32_t
		{
			TRIGGER,
			KINEMATIC,
			FULL
		};

		_Field(PhysicsComponent, bedrock::PpFloat, Mass);
		_Field(PhysicsComponent, bedrock::PpFloat, Bounciness);
		_Field(PhysicsComponent, PhysicsSimulationType, SimulationType);
		_ArrayField(PhysicsComponent, physics::Force, 8u, ActiveForces);

		static void MakeStaticCollider(ecs::Entity entity);
		static void ActivateForce(ecs::Entity entity, const Force &force);
		static void DeactivateForce(ecs::Entity entity, bedrock::PpInt forceId);
	};

} }
