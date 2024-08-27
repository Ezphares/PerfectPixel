#include "core/PhysicsComponent.h"

#include <limits>

namespace perfectpixel { namespace physics {

	void PhysicsComponent::MakeStaticCollider(ecs::Entity entity)
	{
		Bounciness(entity) = 0.0f;
		Mass(entity) = bedrock::Infinity;
		SimulationType(entity) = PhysicsSimulationType::PST_KINEMATIC;
	}

	/*
	void PhysicsComponent::ActivateForce(ecs::Entity entity, const Force &force)
	{
		for (Force &currentForce : ActiveForces(entity))
		{
			if (force.m_identifier == currentForce.m_identifier)
			{
				currentForce = force;
				return;
			}
		}
		ActiveForces(entity).push_back(force);
	}

	void PhysicsComponent::DeactivateForce(ecs::Entity entity, types::int32_t forceId)
	{
		for (auto it = ActiveForces(entity).begin(); it != ActiveForces(entity).end(); ++it)
		{
			if (it->m_identifier == forceId)
			{
				ActiveForces(entity).erase(it);
				return;
			}
		}
	}
	*/
} }
