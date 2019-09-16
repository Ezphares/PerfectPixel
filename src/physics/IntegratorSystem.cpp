#include <physics/IntegratorSystem.h>

#include <physics/PhysicsComponent.h>

#include <EntityComponentSystem/QueryHelper.h>
#include <EntityComponentSystem/TransformComponent.h>

namespace perfectpixel { namespace physics {

namespace {
const int32_t EULER_STEPS = 1;
}

typedef ecs::QueryHelper<ecs::With<ecs::TransformComponent>> IntegratorQuery;

IntegratorSystem::IntegratorSystem()
    : ecs::System(IntegratorQuery::build())
{
    m_onUpdate = &onUpdate;
}

void IntegratorSystem::onUpdate(
    const RangeLimit &begin, const RangeLimit &end, float deltaT)
{
    for (auto it = begin; it != end; ++it)
    {
        ecs::Entity entity = *it;

        bedrock::Vector3 &velocity = ecs::TransformComponent::Velocity(entity);
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
                += ecs::TransformComponent::Velocity(entity) * eulerDeltaT;
        }
    }
}

}} // namespace perfectpixel::physics
