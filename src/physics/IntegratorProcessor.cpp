#pragma once

#include <physics/IntegratorProcessor.h>
#include <physics/PhysicsComponent.h>

#include <EntityComponentSystem/QueryHelper.h>
#include <EntityComponentSystem/TransformComponent.h>

namespace perfectpixel { namespace physics {

	typedef ecs::QueryHelper<ecs::With<ecs::TransformComponent>> IntegratorQuery;

	IntegratorProcessor::IntegratorProcessor(ecs::EntityManager *entityManager)
		: ecs::Processor(IntegratorQuery::build(entityManager))
	{
	}

	void perfectpixel::physics::IntegratorProcessor::onProcess(const std::vector<ecs::Entity> &entities, types::PpFloat deltaT)
	{
		for (ecs::Entity entity : entities)
		{
			types::Vector3 &velocity = ecs::TransformComponent::Velocity(entity);

			if (PhysicsComponent::Has(entity))
			{
				types::PpFloat velocityMagnitude = types::Infinity;
				types::Vector3 totalForce = types::Vector3();

				for (Force &force : PhysicsComponent::ActiveForces(entity))
				{
					types::Vector3 forceVector = force.m_vector;

					if (force.m_relativeDirection)
					{
						// FIXME
					}
					
					if (force.m_relativeMagnitude)
					{
						if (velocityMagnitude == types::Infinity)
						{
							velocityMagnitude = velocity.magnitude();
						}

						forceVector *= velocityMagnitude;
					}

					if (force.m_ignoreMass || PhysicsComponent::Mass(entity) == 0.0f)
					{
						forceVector /= PhysicsComponent::Mass(entity);
					}

					totalForce += forceVector;
				}

				velocity += totalForce * deltaT;
			}

			ecs::TransformComponent::Position(entity) += ecs::TransformComponent::Velocity(entity) * deltaT;
		}
	}



} }
