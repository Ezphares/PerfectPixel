#pragma once

#include <types/numbers.h>

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

		inline static ecs::Field<PhysicsComponent, types::PpFloat> Mass;
		inline static ecs::Field<PhysicsComponent, types::PpFloat> Bounciness;
		inline static ecs::Field<PhysicsComponent, PhysicsSimulationType> SimulationType;

		static void MakeStaticCollider(ecs::Entity entity);
	};

} }
