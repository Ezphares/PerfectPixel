#include "core/IntegratorSystem.h"

#include "core/PhysicsComponent.h"

#include "ecs/QueryHelper.h"
#include "ecs/TransformComponent.h"

namespace perfectpixel { namespace physics {

namespace {
const int32_t EULER_STEPS = 1;
}

typedef ecs::QueryHelper<
    ecs::With<ecs::TransformComponent>,
    ecs::With<physics::PhysicsComponent>>
    IntegratorQuery;

IntegratorSystem::IntegratorSystem()
    : ecs::QuerySystem(IntegratorQuery::build())
{
    m_onUpdate = &onUpdate;
}

void IntegratorSystem::onUpdate(std::span<ecs::Entity> entities, float deltaT)
{
    for (auto entity : entities)
    {
        bedrock::Vector3 &velocity    = PhysicsComponent::Velocity(entity);
        bedrock::Vector3 acceleration = bedrock::Vector3();

        if (PhysicsComponent::Has(entity))
        {
            float velocityMagnitude = bedrock::Infinity;

            for (Force &force : PhysicsComponent::ActiveForces(entity))
            {
                bedrock::Vector3 forceVector = force.m_vector;

                if (force.m_relativeDirection)
                {
                    // FIXME
                }

                if (force.m_relativeMagnitude)
                {
                    if (velocityMagnitude == bedrock::Infinity)
                    {
                        velocityMagnitude = velocity.magnitude();
                    }

                    forceVector *= velocityMagnitude;
                }

                if (force.m_ignoreMass
                    || PhysicsComponent::Mass(entity) == 0.0f)
                {
                    forceVector /= PhysicsComponent::Mass(entity);
                }

                acceleration += forceVector;
            }
        }

        float eulerDeltaT = deltaT / static_cast<float>(EULER_STEPS);
        for (int32_t i = 0; i < EULER_STEPS; ++i)
        {
            velocity += acceleration * eulerDeltaT;
            ecs::TransformComponent::Position(entity)
                += PhysicsComponent::Velocity(entity) * eulerDeltaT;
        }
    }
}

}} // namespace perfectpixel::physics
