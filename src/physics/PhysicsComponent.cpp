#include <physics/PhysicsComponent.h>

#include <limits>

namespace perfectpixel { namespace physics {

		void PhysicsComponent::MakeStaticCollider(ecs::Entity entity)
		{
			Bounciness(entity) = 0.0f;
			Mass(entity) = types::Infinity;
			SimulationType(entity) = PhysicsComponent::KINEMATIC;
		}

} }
