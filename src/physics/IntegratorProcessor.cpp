#pragma once

#include <physics/IntegratorProcessor.h>
#include <physics/PhysicsComponent.h>

#include <EntityComponentSystem/QueryHelper.h>
#include <EntityComponentSystem/TransformComponent.h>

namespace perfectpixel { namespace physics {

	namespace {
		const types::PpInt EULER_STEPS = 1;
	}

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
			types::Vector3 acceleration = types::Vector3();

			if (PhysicsComponent::Has(entity))
			{
				types::PpFloat velocityMagnitude = types::Infinity;

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

					acceleration += forceVector;
				}
			}

			types::PpFloat eulerDeltaT = deltaT / static_cast<types::PpFloat>(EULER_STEPS);
			for (types::PpInt i = 0; i < EULER_STEPS; ++i)
			{
				velocity += acceleration * eulerDeltaT;
				ecs::TransformComponent::Position(entity) += ecs::TransformComponent::Velocity(entity) * eulerDeltaT;
			}
		}
	}



} }
