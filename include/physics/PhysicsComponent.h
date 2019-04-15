#pragma once

#include <types/numbers.h>

#include <EntityComponentSystem/Entity.h>

namespace perfectpixel
{
	namespace physics
	{

		class PhysicsComponent
		{
		public:
			enum SimulationType
			{
				TRIGGER,
				KINEMATIC,
				FULL
			};

		public:
			PhysicsComponent(
				ecs::Entity entity,
				types::PpFloat mass,
				types::PpFloat bounciness,
				SimulationType simulation);
			~PhysicsComponent();

		public:
			static PhysicsComponent staticCollider(ecs::Entity entity);

			ecs::Entity getEntity() const;

			types::PpFloat getMass();
			types::PpFloat getBounciness() const;
			void setMass(types::PpFloat mass);

		private:
			ecs::Entity m_entity;
			types::PpFloat m_mass;
			types::PpFloat m_bounciness;
			SimulationType m_simulation;
		};

	}
}
