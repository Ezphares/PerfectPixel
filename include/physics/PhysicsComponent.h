#pragma once

#include <physics/Force.h>

#include <EntityComponentSystem/Component.h>
#include <EntityComponentSystem/Entity.h>
#include <EntityComponentSystem/CoreComponentStorage.h>

namespace perfectpixel { namespace physics {

	class PhysicsComponent
		: public ecs::Component<PhysicsComponent>
		, public ecs::LinearScanComponentStorage
	{
	public:
		enum PhysicsSimulationType
		{
			TRIGGER,
			KINEMATIC,
			FULL
		};

		_Field(PhysicsComponent, types::PpFloat, Mass);
		_Field(PhysicsComponent, types::PpFloat, Bounciness);
		_Field(PhysicsComponent, PhysicsSimulationType, SimulationType);
		//_Field(PhysicsComponent, physics::Force, 8u ActiveForces;

		static void MakeStaticCollider(ecs::Entity entity);
		static void ActivateForce(ecs::Entity entity, const Force &force);
		static void DeactivateForce(ecs::Entity entity, types::PpInt forceId);
	};

} }
